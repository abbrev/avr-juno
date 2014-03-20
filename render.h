#ifndef _RENDER_H_
#define _RENDER_H_

#include "oscillator.h"
#include "wave.h"

void render_formants(oscillator *const osc, int nsamp, void (*out)(mono8));
void render_fricative(fric_oscillator *const osc, int nsamp,
 		void (*out)(mono8));
void render_silence(int nsamp, void (*out)(mono8));

#endif
