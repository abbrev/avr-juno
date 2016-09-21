#ifndef PHONEME_H
#define PHONEME_H

typedef struct {
	const char *left;
	const char *match;
	const char *right;
	const char *output;
} Rule;

void xlate_word(const char word[]);

#endif
