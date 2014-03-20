#ifndef _VOICES_H_
#define _VOICES_H_

#include "phonemes.h"

typedef struct {
	// default pitch and rate of this voice
	int pitch;
	int rate;

	// XXX should I put buzz wavetables here? Those are probably different
	// for each voice

	const Phoneme phonemes[N_PHONEME];
} Voice;

extern const Voice *voices[];

#endif
