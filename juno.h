#ifndef _JUNO_H_
#define _JUNO_H_

#include <stdbool.h>

#ifdef unix
# define BIG_TARGET 1
#else
# define BIG_TARGET 0
#endif

#include "formant.h"
#include "phonemes.h"
#include "voice.h"
#include "audio.h"

/*
 * External interface for the Juno speech synthesizer
 */

#ifndef JUNO_MAX_OBJECTS
#define JUNO_MAX_OBJECTS 1
#endif

struct juno;

#if 0
 {
	void (*write_sample)(mono8 sample);

	// TODO add voice/speech parameters here
};
#endif

struct juno *juno_create(void);

#if 0
/*
 * Initialize Juno. This must be called before any other Juno routine.
 * XXX is this necessary?
 */
void juno_init(struct juno *juno);
#endif

void juno_speak_phone(struct juno *juno, char);
void juno_speak_diphone(struct juno *juno, const Phoneme *p1, const Phoneme *p2);

/*
 * Speak a string of text (phones or English).
 */
void juno_speak_phones(struct juno *juno, const char *);
void juno_speak_english(struct juno *juno, const char *);

// XXX this shouldn't be public but for testing
void juno_speak_segment(struct juno *juno,
                const FreqSet *start, const FreqSet *end,
                int nslices, SoundSource source);

/*
 * Juno options
 */
struct juno_options {
	// which voice (probably only one voice will be supported on most
	// targets)
	int voice;
	// pitch (adjusts the base frequency of the voice)
	// measured in half-tones in the same scale as MIDI
	int pitch;
	// pitch modulation (adjusts the maximum deviation of pitch)
	// measured as percentage 0%..100%
	int pitch_modulation;
	// playback rate (adjusts the base playback rate)
	// measured in (approximate) words per minute
	int rate;
};

/*
 * Get/set all options.
 */
bool juno_getoptions(struct juno const *juno, struct juno_options *new);
// attempt to set all options
// return false if any options could not be set
bool juno_setoptions(struct juno *juno, struct juno_options const *new, struct juno_options *old);

/*
 * Get/set options individually.
 */

// XXX argument types may need to be changed
//bool juno_get_voice(struct juno const *juno, int *voice);
//bool juno_set_voice(struct juno *juno, int voice);
void juno_set_voice(struct juno *juno, Voice const*voice);

bool juno_get_pitch(struct juno const *juno, int *pitch);
bool juno_set_pitch(struct juno *juno, int pitch);

bool juno_get_pitch_modulation(struct juno const *juno, int *pitch);
bool juno_set_pitch_modulation(struct juno *juno, int pitch);

bool juno_get_rate(struct juno const *juno, int *rate);
bool juno_set_rate(struct juno *juno, int rate);

void juno_set_output(struct juno *juno, void (*out)(mono8 sample));

#if defined(__AVR_ATmega328P__)
# include <avr/io.h>
# include <avr/interrupt.h>
# include <avr/pgmspace.h>
# define AUDIO_DDR  DDRB
# define AUDIO_PORT PORTB
# define AUDIO_BIT  _BV(PB4)
// WGM13 = PWM, phase and frequency corrected; top = ICR1
// CS10 = no prescaling
# define PWM_FLAGS (_BV(WGM13) | _BV(CS10))
# define PWM_TOP (1200/2)
# define pgm_read_mono8 (mono8)pgm_read_byte
# define pgm_read_char (char)pgm_read_byte
# define pgm_read_short (short)pgm_read_word
# define pgm_read_unsigned_short (unsigned short)pgm_read_word
# define pgm_read_int (int)pgm_read_word
# define pgm_read_long (long)pgm_read_dword
# define pgm_read_ptr (void *)pgm_read_word
# define pgm_read(v) ((typeof(*v)) \
         (__builtin_choose_expr(sizeof(*v) == 1, pgm_read_byte(v), \
         __builtin_choose_expr(sizeof(*v) == 2, pgm_read_word(v), \
         __builtin_choose_expr(sizeof(*v) == 4, pgm_read_dword(v), \
         ({ char error[((sizeof(*v) == 1) || (sizeof(*v) == 2) || \
                         (sizeof(*v) == 4)) ? 1 : -1]; error[0]; }) \
         )))))
# define memcpy_PF(a, b, c) memcpy_PF(a, (short)(b), c)
#else
# define PROGMEM
# define pgm_read_mono8(x) (*(x))
# define pgm_read_byte(x) (*(x))
# define pgm_read_word(x) (*(x))
# define pgm_read_dword(x) (*(x))
# define pgm_read_char(x) (*(x))
# define pgm_read_short(x) (*(x))
# define pgm_read_unsigned_short(x) (*(x))
# define pgm_read_int(x) (*(x))
# define pgm_read_long(x) (*(x))
# define pgm_read_ptr(x) (*(x))
# define memcpy_PF(a, b, c) memcpy(a, b, c)
#endif

#define SLICES_PER_SECOND 250

// convert duration in milliseconds to samples
// these are to be used only for compile-time constants!
#define DUR(ms)  ((double)(ms) * SLICES_PER_SECOND / 1000)
//#define FREQC(hz) ((double)((int)hz/32*32) * 65536 / SAMPLE_RATE)
#define FREQU(hz) ((double)(hz) * 65536 / SAMPLE_RATE)
#define FREQ(hz) FREQU(hz)

#define F(hz) FREQU(hz)
#define D(ms) DUR(ms)


#endif

