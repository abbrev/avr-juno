/* audio driver for AVR */

#ifndef _AUDIO_H_
#define _AUDIO_H_

typedef signed char mono8;
extern void audio_play_sample(mono8 s);
extern void audio_init(void);

#if BIG_TARGET
# define SAMPLE_RATE 16000
//#define SAMPLE_RATE_IS_POWER_OF_2 0
#elif defined(_AVR_)
// XXX ~28000 is the max for AVR :/
# define SAMPLE_RATE 24000
//#define SAMPLE_RATE_IS_POWER_OF_2 1
#else
#error
# define SAMPLE_RATE 8192
//#define SAMPLE_RATE_IS_POWER_OF_2 1
#endif


#endif
