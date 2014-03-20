#include "juno.h"

//#include "fp.h"
#include "formant.h"
//#include "phones.h"
#include "wave.h"
#include "oscillator.h"
#include "render.h"
#include "phonemes.h"
#include "bob.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

#if BIG_TARGET
#include <time.h>
#include <errno.h>
#elif defined(_AVR_)
#endif

#define PITCH_MODULATION_CYCLE 16

struct juno {
        void (*write_sample)(mono8 sample);

	Voice const*voice;

	int pitch_phase;

        // TODO add voice/speech parameters here
        // TODO add pitch modulation table here
	unsigned short modulated_pitch[PITCH_MODULATION_CYCLE];
};

extern const wavetable_t sine_wavetables[BUZZ_WAVETABLE_SCALE] PROGMEM;
extern const fric_wavetable_t frication_wavetable PROGMEM;
extern const fric_wavetable_t aspiration_wavetable PROGMEM;
extern const wavetable_t vowel_buzz PROGMEM, nasal_buzz PROGMEM, liquid_buzz PROGMEM, frication_buzz PROGMEM;

/*
Hierarchy of sound fragments (smallest first):
- sample (one value)
- timeslice (one set of frequencies)
- segment (two sets of frequencies)
- diphone (two phones)

- render_* routines convert timeslices to samples
- speak_formant_segment/speak_frication_segment/speak_silence_segment convert
  segments to timeslices; call render_* for each timeslice
- juno_speak_diphone converts diphone (two phone endpoints) to segments; calls
  speak_formant_segment/speak_frication_segment/speak_silence_segment for each
  segment
- juno_speak_phones converts a string of phones into diphones; calls
  juno_speak_diphone for each diphone
- speak_english will convert a string of English text into phones; calls
  juno_speak_phones
*/

#if 0
// XXX we might get better performance by copying only a pointer to the
// waveform to an oscillator
static void copy_wavetable(oscillator *osc, const wavetable_t *from)
{
#if 0
	memcpy(to, from, sizeof *from);
#else
	osc->waveform = from;
#endif
}
#endif


/*
 * 2013-05-13
 * XXX phases of all oscillators except the fundamental oscillator seem to be
 * wrong. The generated waveforms are discontinuous.
 * This is because we are deriving the phase of Fn oscillator by multiplying
 * the F0 oscillator's phase by the harmonic number of Fn, while the phase of
 * Fn may be different if Fn oscillator represented a different harmonic
 * previously. We need to keep track of all oscillators (across calls to
 * juno_speak_segment). The phase of each oscillator is automatically updated
 * during rendering so we really don't have to do much else here or in
 * juno_speak_segment.
 *
 * 2013-05-14
 * phases seem to be correct now. The only discontinuities now are due to
 * sudden changes in amplitude of an oscillator.
 *
 * 2013-12-19
 * Phases were still not correct. The phase for each harmonic needs to be
 * maintained, rather than the phase for a small number of oscillators. When an
 * oscillator changes harmonic band, there is a discontinuity.
 *
 * Actually, this is back to where we were on 2013-05-13.... The change on
 * 2013-05-14 seems to give fewer discontinuities.
 *
 * 2014-03-05
 * Oscillator phases may be either synchronized with F0 or not with the
 * SYNC_PHASES macro.
 */
#define SYNC_PHASES 1
/*
 * For each formant we calculate the frequencies, phases, and amplitudes of one
 * harmonic above and one harmonic below the formant frequency. The frequencies
 * and phases are integer multiples of the fundamental frequency and phase, and
 * the amplitudes are the formant amplitude weighted by the distance of the
 * harmonic from the formant frequency.
 * 
 * For example, if f0=100 and ff=320, then h=3, fh[0]=300, fh[1]=400,
 * amph[0]=amp*0.8, and amph[1]=amp*0.2
 * 
 * The amplitude weights are calculated using a simple triangle window
 * function. The benefits of using a more accurate window function (such as a
 * Gaussian window) are not worth the effort nor computation time; indeed,
 * research in speech synthesis shows that formant bandwidth has little effect
 * on the quality and intelligibility of synthesized speech, and I understand
 * that to include the choice of window function as well.
 */
static void calc_envelope(const FreqSet *freqs, oscillator osc[],
                          const wavetable_t *buzz,
                          const wavetable_t *wavetables,
                          int samples)
{
#if 0
	// TODO maintain phases for all harmonics up to the maximum value for F3
	// XXX phases must be maintained even for harmonics that are not used
	// in a segment
	static short phases[4000/100]; // XXX assumes f0=100
#endif

#if 0 && BIG_TARGET
	int i;
	fprintf(stderr, "calc_envelope: freqs={ ");
	for (i = 0; i < N_FREQ; ++i) {
		fprintf(stderr, "%d, ", freqs->f[i]);
	}
	fprintf(stderr, "}\n");
#endif
	// base frequency
	long f0 = freqs->f[0];
	// amplitude of this formant (supplied as input?)
	long amp = BUZZ_WAVETABLE_SCALE - 1;

	// frequency of a formant
	long ff;
	// lower harmonic number (1 is fundamental harmonic)
	long h = 0;

	// frequencies of harmonics
	long fh[2];
	// amplitudes of harmonics
	long amph[2];

	osc->freq  = f0;
	//osc->phase = phases[1];
	osc->waveform = buzz;

#if SYNC_PHASES
	long p0 = osc->phase; // phase of first oscillator
#endif

	++osc;

	int f;
	for (f = 1; f < N_FREQ; ++f) {
		h = 0;
		if (f0 == 0) {
			fh[0] = fh[1] = 0;
			amph[0] = amph[1] = 0;
		} else {
			ff = freqs->f[f];
			h = ff / f0;

			fh[0] = h * f0;
			fh[1] = fh[0] + f0;

			amph[0] = amp * (fh[1] - ff) / f0;
			amph[1] = amp - amph[0];
		}

		// attenuate amplitude for higher formants
		amp = amp * 3 / 4;

		osc->freq  = fh[0];
#if SYNC_PHASES
		osc->phase = h * p0; // XXX
#endif
		//osc->phase = phases[h];
		osc->waveform = &wavetables[amph[0]];
		++osc;

		osc->freq  = fh[1];
#if SYNC_PHASES
		osc->phase = (h+1) * p0; // XXX
#endif
		//osc->phase = phases[h+1];
		osc->waveform = &wavetables[amph[1]];
		++osc;
	}
#if 0
	// update all phases for next time
	int n = samples;
	for (h = 1; h < sizeof phases/sizeof phases[0]; ++h) {
		phases[h] += n;
		n += samples;
	}
#endif
}


static void calc_frication(const FreqSet *freqs, fric_oscillator osc[])
{
	// XXX freqs[0] is the voice bar if it is non-zero
	// it is the last oscillator
	long f;
	for (f = 1; f < 4; ++f) {
		osc->freq = freqs->f[f];
		if (osc->freq == 0)
			osc->phase = 0;
		++osc;
	}
	// voice bar
	osc->freq = freqs->f[0];
	if (freqs->f[0] == 0) {
		osc->phase = 0;
	}
}

#define UNDUR(sl)  ((double)(sl) * 1000 / SLICES_PER_SECOND)
#define UNFREQ(au) ((double)(au) * SAMPLE_RATE / 65536)

#if BIG_TARGET
// print FreqSet, assuming in accumulator units (not Hz)
static void printFreqSet(const FreqSet *f)
{
	int i;
	for (i = 0; i < N_FREQ; ++i) {
		fprintf(stderr, " %.0f", UNFREQ(f->f[i]));
	}
	fputc('\n', stderr);
}
#endif

// number of samples in a slice
#define SLICE_SAMPLES (SAMPLE_RATE/SLICES_PER_SECOND)

// TODO take frequencies in fixed-point so the frequency slopes can be
// calculated more precisely
// or use a set of fixed-point deltas
// XXX this doesn't seem to noticeably affect segments that will actually be
// spoken with juno because of their relatively short durations. For example,
// 50ms is only 12 slices, and with a typical delta on the order of 100Hz (409
// phase units) the maximum slope error is only about 3%, which is nothing to
// worry about.
void juno_speak_segment(struct juno *juno,
		const FreqSet *start, const FreqSet *end,
                int nslices, SoundSource source)
{
	int i;

#if BIG_TARGET
	static const char *sources[] = {
		[SOURCE_SILENCE] = "silence",
		[SOURCE_ASPIRATION] = "aspiration",
		[SOURCE_FRICATION] = "frication",
		[SOURCE_BUZZ] = "buzz",
	};

	//fprintf(stderr, "%s: %d slices (%d samples)\n", __func__, nslices, nslices * SLICE_SAMPLES);
	fprintf(stderr, "# play segment:\n%s %.2f\n", sources[source], UNDUR(nslices));
	printFreqSet(start);
	printFreqSet(end);
#endif

	if (source == SOURCE_SILENCE) {
		render_silence(nslices * SLICE_SAMPLES, juno->write_sample);
		return;
	}

	bool isFormants = source >= SOURCE_BUZZ;

	// formant oscillators are static so we can maintain phases
	// XXX we should maintain the phase for every harmonic up to the
	// maximum value of F3, rather than per-oscillator (because oscillators
	// may shift to different harmonics during their lifetimes)
	static oscillator formantosc[2*(N_FREQ-1)+1];
	static fric_oscillator fricosc[N_FREQ+1];
	FreqSet freqs;

	// XXX fslopes might need to be fixed-point
	long fslopes[N_FREQ];

	int f;
	for (f = 0; f < N_FREQ; ++f) {
		// calculate the slope of each frequency between start and end,
		// inclusive
		//
		// both endpoints are included so we can have symmetrical
		// behavior of targets on both ends
		//
		// if either endpoint frequency is 0, we don't render that
		// frequency at all
		//
		// if there is only one timeslice, set the initial frequencies
		// to halfway between the start and end (the slopes aren't used
		// in that case)
		fslopes[f] = 0;

		long a = (long)start->f[f];
		long b = (long)end->f[f];
		// if one side is 0, set it to the same as the other side
		if (a == 0) {
			a = b;
		} else if (b == 0) {
			b = a;
		}

		freqs.f[f] = a;

		if (a == 0) {
			// don't play this frequency
			//freqs.f[f] = 0;
		} else if (nslices == 1) {
			// special case: one timeslice
			// add half the difference between start and end
			freqs.f[f] += ((long)b - (long)a) / 2;
		} else {
			// normal case: calculate slope of frequencies between
			// start and end, inclusive
			// nslices - 1 forces the inclusion of the end
			// frequency in the last timeslice
			fslopes[f] = ((long)b - (long)a) /
			             ((long)nslices - 1);
		}
	}

	const wavetable_t *wavetable = NULL;
	const wavetable_t *buzz = NULL;
	switch (source) {
	default:
		break;
	case SOURCE_ASPIRATION:
		// no voice bar for aspiration
		freqs.f[0] = 0;
		fslopes[0] = 0;
		//wavetable = &aspiration_wavetable; // XXX
		//wavetable = &frication_wavetable;
		break;
	case SOURCE_FRICATION:
		//wavetable = &frication_wavetable;
		buzz = &frication_buzz; // only for voiced fricatives
		break;
	case SOURCE_BUZZ:
		wavetable = &sine_wavetables[0];
#if BIG_TARGET
		//fprintf(stderr, "using vowel_buzz\n");
#endif
		buzz = &vowel_buzz;
		break;
#if 0
	case SOURCE_NASAL:
		wavetable = &sine_wavetables[0];
		buzz = &nasal_buzz;
		break;
	case SOURCE_LIQUID:
		wavetable = &sine_wavetables[0];
		buzz = &liquid_buzz;
		break;
#endif
	}

	for (i = 0; i < nslices; ++i) {

		if (isFormants) {
			// calculate envelope for each timeslice as frequencies
			// change
			calc_envelope(&freqs, formantosc, buzz, wavetable, SLICE_SAMPLES);

			// put rubber to asphalt with the oscillators
			render_formants(formantosc, SLICE_SAMPLES, juno->write_sample);
		} else {
			calc_frication(&freqs, fricosc);

			render_fricative(fricosc, SLICE_SAMPLES, juno->write_sample);

		}

		// update the base phase
		//p0 += freqs.f[0] * SLICE_SAMPLES;

		// update frequencies for the next timeslice
		for (f = 0; f < N_FREQ; ++f) {
			freqs.f[f] += fslopes[f];
		}
	}
}









static bool glideFromPhonemeFlags(const PhonemeFlags *f)
{
	return pgm_read_byte(&f->glide);
}

static uint8_t obstruentFromPhonemeFlags(const PhonemeFlags *f)
{
	return pgm_read_byte(&f->obstruent);
}

static uint8_t presrcFromPhonemeFlags(const PhonemeFlags *f)
{
	return pgm_read_byte(&f->presrc);
}

static uint8_t postsrcFromPhonemeFlags(const PhonemeFlags *f)
{
	return pgm_read_byte(&f->postsrc);
}

static short nsrcFromPhonemeFlags(const PhonemeFlags *f)
{
	return pgm_read_byte(&f->nsrc);
}

static short ndurFromPhonemeFlags(const PhonemeFlags *f)
{
	return (pgm_read_short(&f->ndur));
}

static short tdurFromPhonemeFlags(const PhonemeFlags *f)
{
	return (pgm_read_short(&f->tdur));
}

// get frequency set from read-only memory and convert to proper frequency units
static FreqSet getFreqSet_PF(const FreqSet *f1)
{
	FreqSet f2;
	memcpy_PF(&f2, f1, sizeof f2);
	return f2;
}

static FreqSet freqSetFromPhoneme(const Phoneme *p)
{
	return getFreqSet_PF(&p->f);
}

static PhonemeType typeFromPhoneme(const Phoneme *p)
{
	return pgm_read_int(&p->type);
}

static ObsType obstypeFromPhoneme(const Phoneme *p)
{
	return pgm_read_int(&p->obstype);
}

static FreqSet obstargetsFromPhoneme(const Phoneme *p, ObsType type)
{
	return getFreqSet_PF(&p->obstargets[type]);
}

/*
 * A diphone has three major stages:
 * - nucleus of first phoneme
 * - first half of transition (use sound source of first phoneme)
 * - second half of transition (use sound source of second phoneme)
 *
 * The nucleus is generated from the nucleus sound source of the first phoneme.
 */
// XXX this function should also take speech modifiers, eg, rate and pitch
void juno_speak_diphone(struct juno *juno, const Phoneme *p1, const Phoneme *p2)
{
	// modulate F0

	unsigned short f0a = juno->modulated_pitch[juno->pitch_phase];
	if (++juno->pitch_phase >= PITCH_MODULATION_CYCLE)
		juno->pitch_phase = 0;
	unsigned short f0b = juno->modulated_pitch[juno->pitch_phase];

	//fprintf(stderr, "%s (%d) p1=%p p2=%p\n", __func__, __LINE__, p1, p2);
	
	const PhonemeFlags *flags1 = &phonemeFlags[typeFromPhoneme(p1)];
	const PhonemeFlags *flags2 = &phonemeFlags[typeFromPhoneme(p2)];
	//fprintf(stderr, "%s (%d)\n", __func__, __LINE__);
	if (ndurFromPhonemeFlags(flags1)) {
#if BIG_TARGET
		fprintf(stderr, "# -- playing nucleus\n");
	//fprintf(stderr, "%s (%d)\n", __func__, __LINE__);
#endif
		FreqSet f = freqSetFromPhoneme(p1);
		if (f.f[0] != 0)
			f.f[0] = f0a;
		juno_speak_segment(juno, &f, &f, ndurFromPhonemeFlags(flags1), nsrcFromPhonemeFlags(flags1));
	}
	if (tdurFromPhonemeFlags(flags1) == 0 && tdurFromPhonemeFlags(flags2) == 0) return;

	// XXX this does not handle aspirated phoneme (this should be aspirated
	// but take the frequencies of the following sonorant).
#if 1
	FreqSet fsets[4]; // 4 endpoints: 2 for post- and 2 for pre-phoneme
	if (obstruentFromPhonemeFlags(flags1)) {
#if BIG_TARGET
		fprintf(stderr, "# %s: p1 is obstruent\n", __func__);
#endif
		fsets[0] = obstargetsFromPhoneme(p2, obstypeFromPhoneme(p1));
	} else {
#if BIG_TARGET
		fprintf(stderr, "# %s: p1 is not obstruent\n", __func__);
#endif
		fsets[0] = freqSetFromPhoneme(p1);
	}
	if (obstruentFromPhonemeFlags(flags2)) {
#if BIG_TARGET
		fprintf(stderr, "# %s: p2 is obstruent\n", __func__);
#endif
		fsets[3] = obstargetsFromPhoneme(p1, obstypeFromPhoneme(p2));
	} else {
#if BIG_TARGET
		fprintf(stderr, "# %s: p2 is not obstruent\n", __func__);
#endif
		fsets[3] = freqSetFromPhoneme(p2);
	}

	int i;
	short dur1 = tdurFromPhonemeFlags(flags1);
	short dur2 = tdurFromPhonemeFlags(flags2);
#if BIG_TARGET
	fprintf(stderr, "# %s: dur1=%d dur2=%d\n", __func__, dur1, dur2);
#endif
	if (!glideFromPhonemeFlags(flags1) || !glideFromPhonemeFlags(flags2)) {
#if BIG_TARGET
		fprintf(stderr, "# %s: no glide!\n", __func__);
#endif
		// easy: stair-step transition
		fsets[1] = fsets[0];
		fsets[2] = fsets[3];
	} else {
#if BIG_TARGET
		fprintf(stderr, "# %s: glide!\n", __func__);
	//fprintf(stderr, "# %s (%d)\n", __func__, __LINE__);
#endif
		// harder: compute center point using transition durations of
		// post and pre
		for (i = 0; i < 4; ++i) {
			short f0 = fsets[0].f[i];
			short f3 = fsets[3].f[i];
#if BIG_TARGET
			fprintf(stderr, "# %s: [%d] f0=%d f3=%d\n", __func__, i, f0, f3);
#endif
			// f1=f2 = d1/(d1+d2) * (f3-f0) + f0
			// = (d1 * (f3-f0)) / (d1+d2) + f0
			fsets[1].f[i] = fsets[2].f[i] = ((long)dur1 * (f3-f0)) /
			                        ((long)dur1 + dur2) + f0;
		}
	}
	// TODO we want voiced phonemes to play during their post segments even
	// if the next phoneme is silent or aspirated. as it is now, nothing is
	// voiced because the next phoneme has formants of 0
	if (dur1) {
#if BIG_TARGET
		fprintf(stderr, "# -- playing post\n");
#endif
		//if (fsets[0].f[0] != 0)
			fsets[0].f[0] = f0a;
		//if (fsets[1].f[0] != 0)
			fsets[1].f[0] = f0a;
		// XXX hack for aspirated consonant
		if (nsrcFromPhonemeFlags(flags1) == SOURCE_ASPIRATION) {
			fsets[0].f[1] = fsets[3].f[1];
			fsets[0].f[2] = fsets[3].f[2];
			fsets[0].f[3] = fsets[3].f[3];
		}
		juno_speak_segment(juno, &fsets[0], &fsets[1], dur1, postsrcFromPhonemeFlags(flags1));
	}
	int presrc = presrcFromPhonemeFlags(flags2);
	if (presrc == SOURCE_ASPIRATION) {
		// XXX hack!
		// don't play segment before aspiration
		dur2 = 0;
	}
	if (dur2) {
#if BIG_TARGET
		fprintf(stderr, "# -- playing pre\n");
#endif
		//if (fsets[2].f[0] != 0)
			fsets[2].f[0] = f0a;
		//if (fsets[3].f[0] != 0)
			fsets[3].f[0] = f0b;
#if 0
		if (presrc == SOURCE_ASPIRATION) {
			// XXX hack
			// extend previous phoneme
			//fsets[3].f[0] = fsets[2].f[0];
			fsets[3].f[1] = fsets[2].f[1];
			fsets[3].f[2] = fsets[2].f[2];
			fsets[3].f[3] = fsets[2].f[3];
			presrc = postsrcFromPhonemeFlags(flags1);
		}
#endif
		juno_speak_segment(juno, &fsets[2], &fsets[3], dur2, presrc);
	}
#else
	//
	// XXX quick hack without gliding during the transition
	if (tdurFromPhonemeFlags(flags1)) {
		juno_speak_segment(juno, &p1->f, &p1->f, tdurFromPhonemeFlags(flags1), postsrcFromPhonemeFlags(flags1));
	}
	if (tdurFromPhonemeFlags(flags2)) {
		juno_speak_segment(juno, &p2->f, &p2->f, tdurFromPhonemeFlags(flags2), presrcFromPhonemeFlags(flags2));
	}
#endif
	// TODO
}

static __flash const int char_phoneme[256] = {
	['j'] = P_j,
	['w'] = P_w,
	['i'] = P_i,    
	['u'] = P_u,
	['I'] = P_I,    
	['U'] = P_U,
	['e'] = P_e,          
	['o'] = P_o,
	['V'] = P_V,
	['@'] = P_SCHWA,
	['R'] = P_R,
	['E'] = P_E,          
	['O'] = P_O,
   	['&'] = P_ae,
	['a'] = P_a,          
	['A'] = P_A,

	['l'] = P_l,
	['r'] = P_r, // how is this different from P_R?

	// nasals
	['m'] = P_m,
	['n'] = P_n,
	['N'] = P_N,

	// stops
	['b'] = P_b,
	['p'] = P_p,
	['d'] = P_d,
	['t'] = P_t,
	['g'] = P_g,
	['k'] = P_k,

	// fricatives
	['f'] = P_f,
	['v'] = P_v,
	['s'] = P_s,
	['z'] = P_z,
	['S'] = P_S,
	['Z'] = P_Z,
	['T'] = P_T,
	['D'] = P_D,

	// aspirated
	['h'] = P_h,
};

static int phoneme_from_char(int c)
{
	if (0 <= c && c < 256)
		return char_phoneme[c];
	return P_none;
}

void juno_speak_phone(struct juno *juno, char c)
{
	static int lastp = P_none;
	int nextp = phoneme_from_char(c);

	const Phoneme *a = (&juno->voice->phonemes[lastp]);
	const Phoneme *b = (&juno->voice->phonemes[nextp]);

	juno_speak_diphone(juno, a, b);

	lastp = nextp;

#if BIG_TARGET
	static int lastc = ' ';
	fprintf(stderr, "# play diphone /%c%c/\n", lastc, c);
	fflush(NULL);
	lastc = c;
#endif
}

void juno_speak_phones(struct juno *juno, const char *phones)
{
	while (*phones) {
		juno_speak_phone(juno, *phones++);
	}
	juno_speak_phone(juno, ' ');
	return;
}

static void default_write_sample(mono8 unused) { /* no-op */ }

struct juno *juno_create(void)
{
	static struct juno juno[JUNO_MAX_OBJECTS];
	static int next = 0;

	if (next >= JUNO_MAX_OBJECTS) return NULL;
	
	struct juno *j = &juno[next++];

	juno_set_output(j, default_write_sample);
	juno_set_voice(j, &voiceBob);

	j->pitch_phase = 0;

	// this is an array of ratios (scaled by 256) to pitch modulate
	// the voice
	static const unsigned short pitch_modulation[] PROGMEM = {
		120*256L/120, 128*256L/120, 134*256L/120, 138*256L/120,
		140*256L/120, 138*256L/120, 134*256L/120, 128*256L/120,
		120*256L/120, 112*256L/120, 106*256L/120, 102*256L/120,
		100*256L/120, 102*256L/120, 106*256L/120, 112*256L/120,
	};

	int i;
	unsigned long pitch = pgm_read_unsigned_short(&j->voice->pitch);
	for (i = 0; i < PITCH_MODULATION_CYCLE; ++i) {
		j->modulated_pitch[i] =
			pgm_read_short(&pitch_modulation[i]) * pitch / 256;
#if BIG_TARGET
		fprintf(stderr, "%2d: %hu\n", i, j->modulated_pitch[i]);
#endif
	}

	// TODO initialize juno object

	return j;
}

void juno_set_output(struct juno *juno, void (*out)(mono8))
{
	juno->write_sample = out ?: default_write_sample;
}

void juno_set_voice(struct juno *juno, Voice const*voice)
{
	juno->voice = voice;
	// TODO modulate pitch here
}

