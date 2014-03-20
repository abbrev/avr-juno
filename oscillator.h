#ifndef _OSCILLATOR_H_
#define _OSCILLATOR_H_

#include "wave.h"

typedef struct {
	unsigned short freq;
	unsigned short phase;
	const wavetable_t *waveform;
} oscillator;

typedef struct {
	unsigned short freq;
	unsigned long phase;
} fric_oscillator;

#endif
