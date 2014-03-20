#include "voice.h"

// XXX it would be nice to put all voices in a subdirectory and be able to
// build/link only the voices desired for an application
extern Voice voiceBob;

const Voice *voices[] = {
	&voiceBob,
};
