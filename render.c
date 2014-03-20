#include "juno.h"
#include "render.h"
#include "oscillator.h"
#include "wave.h"
#include "audio.h"

#include <stdio.h>

#if BIG_TARGET
#define OUT(s) out(s)
#else
#define OUT(s) audio_play_sample(s)
#endif

// shift phase accumulator right LOG2_PHASE_MODULUS-LOG2_*_WAVETABLE_PERIOD bits
// mask lowest LOG2_*_WAVETABLE_SIZE bits
// where * is BUZZ or FRIC

#define ADDOSC(o) do { \
	o.phase += o.freq; \
	o.phase &= 0xffff; \
	/* \
	 * shift and mask phase to get upper bits which is used \
	 * as the position in the waveform table \
	 */ \
	pos = (o.phase >> (LOG2_PHASE_MODULUS-LOG2_BUZZ_WAVETABLE_PERIOD)) & (BUZZ_WAVETABLE_SIZE-1); \
	/*fprintf(stderr, "%5d %d: sample=%d\n", i, j, o->waveform.periodic->samples[pos]); */\
	s += pgm_read_mono8(&o.waveform->samples[pos]); \
} while (0)

void render_formants(oscillator *const osc, int nsamp, void (*out)(mono8))
{
	int i, j;
	unsigned pos;
#define N_FORMANTS_FLATOSC 7
	for (i = 0; i < nsamp; ++i) {
		int s = 0;
		for (j = 0; j < N_FORMANTS_FLATOSC; ++j) {
			ADDOSC(osc[j]);
		}
		OUT(s);
	}
}

#define ADDFRIC(o) do { \
	o.phase += o.freq; \
	/*o.phase &= 0xffffffff; */\
	/* \
	 * shift and mask phase to get upper bits which is used \
	 * as the position in the waveform table \
	 */ \
	pos = (o.phase >> (LOG2_PHASE_MODULUS-LOG2_FRIC_WAVETABLE_PERIOD)) & (FRIC_WAVETABLE_SIZE-1); \
	/*fprintf(stderr, "pos=%u\n", pos);*/ \
	/*fprintf(stderr, "%5d %d: sample=%d\n", i, j, o.waveform.aperiodic->samples[pos]); */\
	s += pgm_read_mono8(&frication_wavetable.samples[pos]); \
} while (0)

#define AMPMOD_ADDOSC(o) do { \
	o.phase += o.freq; \
	o.phase &= 0xffffffff; \
	/* \
	 * shift and mask phase to get upper bits which is used \
	 * as the position in the waveform table \
	 */ \
	pos = (o.phase >> (LOG2_PHASE_MODULUS-LOG2_BUZZ_WAVETABLE_PERIOD)) & (BUZZ_WAVETABLE_SIZE-1); \
	/* XXX multiplication by 4 is a hack. fix build-wave.c instead! */ \
	mod = 1*(int)pgm_read_mono8(&frication_buzz.samples[pos]) + 128; \
	s = (s * mod / 256) + pgm_read_mono8(&vowel_buzz.samples[pos]); \
} while (0)

// We use only one frication wavetable and one frication buzz wavetable. The
// frication wavetable is fairly large (larger than the buzz and sine
// wavetables) to minimize its periodicity.
void render_fricative(fric_oscillator *const osc, int nsamp, void (*out)(mono8))
{
	int i, j;
	unsigned pos;
	int mod;
#define N_FRICATIVE_FLATOSC 3
	for (i = 0; i < nsamp; ++i) {
		int s = 0;
		for (j = 0; j < N_FRICATIVE_FLATOSC; ++j) {
			ADDFRIC(osc[j]);
		}
		AMPMOD_ADDOSC(osc[j]);
		OUT(s);
	}
}

void render_silence(int nsamp, void (*out)(mono8))
{
	int i;
	for (i = 0; i < nsamp; ++i) {
		OUT(0);
	}
}

