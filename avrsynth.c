#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

#include "juno.h"
#include "bob.h"

// test program
int main()
{
	struct juno *juno = NULL;

	audio_init();
	sei();

	juno = juno_create();
	if (!juno) {
		//fprintf(stderr, "Cannot create juno object!\n");
		exit(1);
	}

	juno_set_output(juno, audio_play_sample);

#if 0
	juno_speak_phones(juno, "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA");
	juno_speak_phones(juno, "ssssssssssssssssssssssssssssssss");
	juno_speak_phones(juno, "SSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSS");
#endif


	//juno_speak_phones(juno, "AdA");
	//juno_speak_phones(juno, "ElOu");
	//juno_speak_phones(juno, "urlda");
	//juno_speak_phones(juno, "IR  ");
	//juno_speak_phones(juno, "hElOU  ");
	//juno_speak_phones(juno, "AIm kVmIN Aut");
	//juno_speak_phones(juno, "AIm sAri dEiv   AIm @frEid AI  kant  du  Dat");
	//juno_speak_phones(juno, "no dIs@sEmb@l");
	juno_speak_phones(juno, "nob@di EkspEks T@ spanIS INkwIzIS@n   ");
	juno_speak_phones(juno, "gRitiNz pRofEsR fAlkIn");
	juno_speak_phones(juno, "   Sal wi plEi @ gEim");
	//juno_speak_phones(juno, "   wUdInt ju pRIfR @ nAIs gEim @v tSEs");
	//juno_speak_phones(juno, "go @hEd &nd dZVmp");
	//juno_speak_phones(juno, "mAIt &z wEl dZVmp");
	//juno_speak_phones(juno, "hAU @bAUt @ nAIs gem @v tSEs");
	//juno_speak_phones(juno, "ii AI ii AI Ou");
	while (1);
	return 0;
}

