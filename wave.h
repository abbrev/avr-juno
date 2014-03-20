#ifndef _WAVE_H_
#define _WAVE_H_

#include "audio.h"

// how many scaled versions of each wavetable we have
// this need not be a power of 2, but it might help
// TODO scale the wavetable exponentially, ie, such that it's linear on a dB
// scale. For example, with 8 steps of 3 dB each, the top waveform would be 0
// dB and the bottom waveform would be -18 dB, which would be an amplitude of
// 0.125 of the full scale wave (there is one waveform reserved with zero
// amplitude).
#define BUZZ_WAVETABLE_SCALE 4
//10 (1024) for frication
//#define LOG2_WAVEFORM_SIZE 10
// 5 is a good size for periodic waveforms
// 8 or higher is needed for aperiodic waveforms (but see below)
#define LOG2_BUZZ_WAVETABLE_SIZE 5 // total size of buzz wavetable
#define LOG2_BUZZ_WAVETABLE_PERIOD 5 // length of 1 cycle
#define LOG2_FRIC_WAVETABLE_SIZE 11 // total size of frication wavetable
#define LOG2_FRIC_WAVETABLE_PERIOD 3 // length of 1 cycle

// shift phase accumulator right by LOG2_PHASE_MODULUS-*_PERIOD bits
// mask lowest *_SIZE bits


/* TODO fricative waveform needs to have a divisor (right-shift) like the
 * periodic waveforms so the frequency value can be in a good range for
 * fricatives (say, 40-4000Hz) and still have decent granularity
 *
 * The computed frequency already depends on sample rate.
 *
 * The formula is f' = f * 65536 / SAMPLE_RATE
 *
 * The fricative waveform is filtered so that a noise sound at N Hz requires
 * playing waveform samples at M*N Hz, where the value of M needs to be
 * determined (M>=2). The aperiodic waveform is basically many M-cycle noise
 * waveforms. Assume for this discussion that M=4 (a 100Hz noise requires 400
 * waveform samples per second). When N is 2000, 8000 waveform samples are
 * played per second, so that at 16000Hz, one waveform sample occupies the
 * space of 2 audio samples.
 * N => audio samples per waveform sample
 * 62.5 => 64 (out of range when M=4, S=16000, b=11)
 * 125 => 32
 * 250 => 16
 * 500 => 8
 * 1000 => 4
 * 1500 => 2.6...
 * 2000 => 2
 * 4000 => 1
 *
 *
 * The fricative waveform must be large enough to prevent periodicity above a
 * given frequency (say, 8Hz, aka 1/(125ms)) at the highest supported fricative
 * frequency (4000Hz). That would be 2000 samples when M=4, or 2048 as the next
 * power of two. That is a LOG2 value of 11, which leaves 5 bits out of 16 (the
 * width of the phase accumulator). This means we can shift right by up to 5
 * bits and still have enough bits to cover the entire fricative wavetable. The
 * number of bits we need to shift for an aperiodic waveform is not dependent
 * on the number of remaining bits, as is the case with periodic waveforms.
 *
 * Let's see what the limits (min/max frequency N) of M=4, b=11 are for
 * different values of S:
 * S         min      max
 * 8000      62.5     2000
 * 16000     125      4000
 * 48000     375      12000
 *
 * Let's see what the limits of M=4, S=16000 are for different values of b:
 * b         min      max
 * 10        62.5     4000
 * 11        125      4000
 * 12        250      4000
 * 13        500      4000
 *
 * So increasing b reduces the range by raising the min. Increasing S raises
 * the min and max by the same factor. Increasing M (I believe) reduces the
 * range by lowering the max.
 *
 * The range could be raised or lowered (but not increased) independent of the
 * value of S (up to a limit) by expanding the width of the phase accumulator
 * to 24 or 32 bits. This shouldn't add any overhead to rendering periodic
 * waveforms because those use only the lower 16 bits; the upper 8 or 16 can be
 * ignored.
 *
 * S and M set the absolute highest max frequency to S/M (actually, that's not
 * true. It's the max frequency that can be played *without skipping any
 * aperiodic waveform samples*, and which has no risk of aliasing effects).
 * S=16000 and M=4 are good starting values to work with. With b=10, the
 * aperiodic waveform has a period of 0.256 second at N=4000Hz, which should be
 * large enough. The minimum frequency is S/2^(w-b)/M (is this formula
 * correct?) = 62.5Hz. There are 2^(w-b)=64 discrete frequencies between 62.5Hz
 * and 125Hz.
 *
 * The minimum waveform sample rate (waveform samples played per second) is
 * S/(2^(w-b)). (w is width of phase accumulator)
 *
 * Note: The phase accumulator wraps around at the given real frequency. 
 *
 * Now for the real question: how do we convert a "computed" frequency to an
 * aperiodic frequency value?
 *
 * Given S=16000, b=10, M=4, f=1000Hz,
 * f' = f*65536/S = 4096
 * ff = S/(f*M) = 4 // XXX this seems backwards
 * ff = f*M*65536/S = 16384 // XXX this seems almost correct but doesn't take b and w into consideration
 * ff = f*M*65536/2^(w-b)/S = 256 // XXX is this correct now?
 * ff = f'*M/2^(w-b) // should be correct now
 *    = f' / (2^(w-b)/M) // divisor can be computed at compile-time, and if M is a power of two will result in a right-shift
 */

//#define BUZZ_WAVETABLE_PERIOD (1<<LOG2_BUZZ_WAVETABLE_PERIOD)
#define BUZZ_WAVETABLE_SIZE (1<<LOG2_BUZZ_WAVETABLE_SIZE)
#define FRIC_WAVETABLE_PERIOD (1<<LOG2_FRIC_WAVETABLE_PERIOD)
#define FRIC_WAVETABLE_SIZE (1<<LOG2_FRIC_WAVETABLE_SIZE)

// LOG2_PHASE_MODULUS is the width of the integer part of the phase accumulator
#define LOG2_PHASE_MODULUS 16 // width of phase accumulator, in bits

typedef struct {
	mono8 samples[BUZZ_WAVETABLE_SIZE];
} wavetable_t;

typedef struct {
	mono8 samples[FRIC_WAVETABLE_SIZE];
} fric_wavetable_t;

// TODO make a different wavetable type for the frication waveform so it can be
// bigger than the periodic waveforms
// aperiodic waves (noise) depend on the absolute phase rather than phase
// divided by a constant factor as periodic waves are.

extern const wavetable_t sine_wavetables[BUZZ_WAVETABLE_SCALE];
extern const fric_wavetable_t frication_wavetable;
extern const fric_wavetable_t aspiration_wavetable;
extern const wavetable_t vowel_buzz, nasal_buzz, liquid_buzz, frication_buzz;

#endif
