#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

#include "juno.h"
#include "bob.h"

#if BIG_TARGET
#include <time.h>
#include <errno.h>
#elif defined(_AVR_)
#endif



static const char *sources[] = {
	[SOURCE_SILENCE] = "silence",
	[SOURCE_ASPIRATION] = "aspiration",
	[SOURCE_FRICATION] = "frication",
	[SOURCE_BUZZ] = "buzz",
};









// this should be a no-op (the phoneme data should have frequencies in the
// correct unit)
static void convertFreq(FreqSet *f)
{
	int i;
	for (i = 0; i < 4; ++i) {
		//fprintf(stderr, "  [%d] %hd -> ", i, f->f[i]);
		f->f[i] = FREQ(f->f[i]);
		//fprintf(stderr, "%hd\n", f->f[i]);
	}
}


#if 1
static int parseFreqSet(FreqSet *f, const char *line)
{
	float x[4];

	if (sscanf(line, "%f%f%f%f", x,x+1,x+2,x+3) != 4) return 0;

	int i;
	for (i = 0; i < 4; ++i) f->f[i] = x[i];

	return 1;
}

// get a line, skipping over blank lines and lines beginning with #
static char *readline(char *line, int size, FILE *f)
{
	char *r;
	while ((r = fgets(line, size, f))) {
		if (line[0] != '\n' && line[0] != '#')
			break;
	}
	return r;
}

static int read_and_speak_segment(struct juno *juno)
{
	char line[80];
	char type[80];
	float dur;
	FreqSet start, end;

	// read a segment from stdin and play it
	// format:
	// type dur
	// f0 f1 f2 f3 [start]
	// f0 f1 f2 f3 [end]
	if (!readline(line, sizeof line, stdin)) goto done;
	if (sscanf(line, "%s%f", type, &dur) != 2) goto done;

	int source = -1;
	int i;
	for (i = 0; i < SOURCE_LAST; ++i) {
		if (strcmp(sources[i], type) == 0) {
			source = i;
			break;
		}
	}
	if (source < 0) goto done;

	if (!readline(line, sizeof line, stdin)) goto done;
	if (!parseFreqSet(&start, line)) goto done;
	if (!readline(line, sizeof line, stdin)) goto done;
	if (!parseFreqSet(&end, line)) goto done;

	convertFreq(&start);
	convertFreq(&end);

	juno_speak_segment(juno, &start, &end, DUR(dur), source);

	return 1;
done:
	return 0;
}

// test program
int main(int argc, char *argv[])
{
	struct juno *juno = NULL;

	audio_init();
	juno = juno_create();
	if (!juno) {
		fprintf(stderr, "Cannot create juno object!\n");
		exit(1);
	}

	juno_set_output(juno, audio_play_sample);

	fprintf(stderr, "# argc=%d\n", argc);
	if (argc == 2) {
		if (strcmp(argv[1], "-s") == 0) {
			while (read_and_speak_segment(juno))
				;
		} else if (strcmp(argv[1], "-i") == 0){
			int c;
			while ((c = getchar()) != EOF)
				juno_speak_phone(juno, c);
			juno_speak_phone(juno, ' ');
		} else {
			fprintf(stderr, "unknown option %s\n", argv[1]);
			return 1;
		}
		return 0;
	}

	//juno_speak_phones(juno, "AdA");
	//juno_speak_phones(juno, "ElOu");
	//juno_speak_phones(juno, "urlda");
	//juno_speak_phones(juno, "IR  ");
	//juno_speak_phones(juno, "hElOU  ");
	//juno_speak_phones(juno, "AIm kVmIN Aut");
	//juno_speak_phones(juno, "AIm sAri dEiv   AIm @frEid AI  kant  du  Dat");
	//juno_speak_phones(juno, "no dIs@sEmb@l");
	juno_speak_phones(juno, "gRitiNz pRofEsR fAlkIn");
	juno_speak_phones(juno, "   Sal wi plEi @ gEim");
	//juno_speak_phones(juno, "   wUdInt ju pRIfR @ nAIs gEim @v tSEs");
	//juno_speak_phones(juno, "go @hEd &nd dZVmp");
	//juno_speak_phones(juno, "mAIt &z wEl dZVmp");
	//juno_speak_phones(juno, "hAU @bAUt @ nAIs gem @v tSEs");
#if 0
	const char *c;
	for (c = "mnNptkbdg"; *c; ++c) {
		const char *v;
		for (v = "uo@AEIi"; *v; ++v) {
			char word[10];
			sprintf(word, "%c%c%c%c %c%c%c%c",
					*c,*v,*c,*v,*c,*v,*c,*v);
			juno_speak_phones(juno, word);
		}
		juno_speak_phones(juno, "");
	}
#endif
	//juno_speak_phones(juno, "ii AI ii AI Ou");
	return 0;
	juno_speak_diphone(juno, (&voiceBob.phonemes[P_A]), (&voiceBob.phonemes[P_i]));
	juno_speak_diphone(juno, (&voiceBob.phonemes[P_i]), (&voiceBob.phonemes[P_A]));
	juno_speak_diphone(juno, (&voiceBob.phonemes[P_A]), (&voiceBob.phonemes[P_i]));
	juno_speak_diphone(juno, (&voiceBob.phonemes[P_i]), (&voiceBob.phonemes[P_A]));
	juno_speak_diphone(juno, (&voiceBob.phonemes[P_A]), (&voiceBob.phonemes[P_i]));
	juno_speak_diphone(juno, (&voiceBob.phonemes[P_i]), (&voiceBob.phonemes[P_A]));
	return 0;
//65536L / SAMPLE_RATE
	FreqSet freqs[] = {
#if 1
		{{ FREQ(110), FREQ(800), FREQ(1300), FREQ(2500) }},
		{{ FREQ( 95), FREQ(800), FREQ(1300), FREQ(2400) }},
		{{ FREQ(100), FREQ(600), FREQ(1300), FREQ(1650) }},
		{{ FREQ(100), FREQ(300), FREQ(1300), FREQ(1500) }},
#else
		{{ FREQ(110), FREQ(600), FREQ(1200), FREQ(2500) }},
		{{ FREQ( 95), FREQ(610), FREQ(1150), FREQ(2400) }},
		{{ FREQ(100), FREQ(620), FREQ(1250), FREQ(1650) }},
		{{ FREQ(100), FREQ(400), FREQ(1300), FREQ(1500) }},
#endif

		{{ FREQ(130), FREQ(850), FREQ(1610), FREQ(0) }},
		{{ FREQ(130), FREQ(240), FREQ(2400), FREQ(0) }},
		{{ FREQ(2000), FREQ(2000), FREQ(0), FREQ(0) }},
		{{ FREQ(130), FREQ(2000./150), FREQ(0000/128), FREQ(0000/128) }},
		{{ FREQ(  0), FREQ(240/16), FREQ(2400/16), FREQ(0) }},
		//{{ FREQ(90), FREQ(850), FREQ(1610), FREQ(0) }},
	};
	//const int nsamp = DUR(250);
	//bool voiced = true;

#ifdef _AVR_
	// PD6 is output
	DDRD |= _BV(PD6);
	// 50% duty cycle
	OCR0A = 128;
	// non-inverting mode
	TCCR0A |= _BV(COM0A1);
	// fast PWM mode
	TCCR0A |= _BV(WGM01) | _BV(WGM00);
	// enable PWM with no prescaler
	// 16MHz clock => 62.5kHz PWM
	TCCR0B |= _BV(CS00);
#endif

#if 0
	juno_speak_segment(juno, &freqs[4], &freqs[5], nsamp, SOURCE_BUZZ);
	//juno_speak_segment(juno, &freqs[6], &freqs[6], nsamp, SOURCE_BUZZ);
	//juno_speak_segment(juno, &freqs[7], &freqs[7], nsamp, SOURCE_FRICATION); // XXX
	juno_speak_segment(juno,     NULL,     NULL, nsamp, SOURCE_SILENCE);
	//speak_formants(start, end, nsamp, voiced);
	return 0;
#else
	juno_speak_segment(juno, &freqs[0], &freqs[1], DUR(150), SOURCE_BUZZ);
	juno_speak_segment(juno, &freqs[1], &freqs[2], DUR(500), SOURCE_BUZZ);
	juno_speak_segment(juno, &freqs[2], &freqs[3], DUR(400), SOURCE_BUZZ);
	return 0;
#endif

}
#endif

