#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "phoneme.h"

/*
**	English to Phoneme translation.
**
**	Rules are made up of four parts:
**	
**		The left context.
**		The text to match.
**		The right context.
**		The phonemes to substitute for the matched text.
**
**	Procedure:
**
**		Seperate each block of letters (apostrophes included) 
**		and add a space on each side.  For each unmatched 
**		letter in the word, look through the rules where the 
**		text to match starts with the letter in the word.  If 
**		the text to match is found and the right and left 
**		context patterns also match, output the phonemes for 
**		that rule and skip to the next unmatched letter.
**
**
**	Special Context Symbols:
**
**		#	One or more vowels
**		:	Zero or more consonants
**		^	One consonant.
**		.	One of B, D, V, G, J, L, M, N, R, W or Z (voiced 
**			consonants)
**		%	One of ER, E, ES, ED, ING, ELY (a suffix)
**			(Right context only)
**		+	One of E, I or Y (a "front" vowel)
*/

extern const Rule *const Rules[];	/* An array of pointers to rules */

static bool isvowel(char chr)
	{
	return strchr("AEIOU", chr);
	}

static bool isconsonant(char chr)
	{
	return (isupper(chr) && !isvowel(chr));
	}

static int find_rule(const char word[], int index, const Rule *rules);

void xlate_word(const char word[])
	{
	int index;	/* Current position in word */
	int type;	/* First letter of match part */

	index = 1;	/* Skip the initial blank */
	do
		{
		if (isupper(word[index]))
			type = word[index] - 'A' + 1;
		else
			type = 0;

		index = find_rule(word, index, Rules[type]);
		}
	while (word[index] != '\0');
	}

static bool leftmatch(const char *pattern, const char *context);
static bool rightmatch(const char *pattern, const char *context);
static void outstring(const char *string);
extern void outchar(int chr);

static int find_rule(const char word[], int index, const Rule *rules)
	{
	const Rule *rule;
	const char *match;
	int remainder;

	for (;;)	/* Search for the rule */
		{
		rule = rules++;
		match = rule->match;

		if (match == 0)	/* bad symbol! */
			{
			fprintf(stderr,
"Error: Can't find rule for: '%c' in \"%s\"\n", word[index], word);
			return index+1;	/* Skip it! */
			}

		for (remainder = index; *match != '\0'; match++, remainder++)
			{
			if (*match != word[remainder])
				break;
			}

		if (*match != '\0')	/* found missmatch */
			continue;
/*
printf("\nWord: \"%s\", Index:%4d, Trying: \"%s/%s/%s\" = \"%s\"\n",
    word, index, rule->left, rule->match, rule->right, rule->output);
*/

		if (!leftmatch(rule->left, &word[index-1]))
			continue;
/*
printf("leftmatch(\"%s\",\"...%c\") succeded!\n", rule->left, word[index-1]);
*/
		if (!rightmatch(rule->right, &word[remainder]))
			continue;
/*
printf("rightmatch(\"%s\",\"%s\") succeded!\n", rule->right, &word[remainder]);
*/
/*
printf("Success: ");
*/
		outstring(rule->output);
		return remainder;
		}
	}

static void outstring(const char *string)
	{
	while (*string != '\0')
		outchar(*string++);
	}



	//char *pattern;	/* first char of pattern to match in text */
	//char *context;	/* last char of text to be matched */
static bool leftmatch(const char *pattern, const char *context)
	{
	const char *pat;
	const char *text;
	int count;

	if (*pattern == '\0')	/* null string matches any context */
		{
		return true;
		}

	/* point to last character in pattern string */
	count = strlen(pattern);
	pat = pattern + (count - 1);

	text = context;

	for (; count > 0; pat--, count--)
		{
		/* First check for simple text or space */
		if (isalpha(*pat) || strchr(" '", *pat))
			{
			if (*pat != *text)
				{
				return false;
				}
			else
				{
				text--;
				continue;
				}
			}

		switch (*pat)
			{
		case '#':	/* One or more vowels */
			if (!isvowel(*text))
				return false;

			text--;

			while (isvowel(*text))
				text--;
			break;

		case ':':	/* Zero or more consonants */
			while (isconsonant(*text))
				text--;
			break;

		case '^':	/* One consonant */
			if (!isconsonant(*text))
				return false;
			text--;
			break;

		case '.':	/* B, D, V, G, J, L, M, N, R, W, Z */
			if (!strchr("BDVGJLMNRWZ", *text))
				return false;
			text--;
			break;

		case '+':	/* E, I or Y (front vowel) */
			if (!strchr("EIY", *text))
				return false;
			text--;
			break;

		case '%':
		default:
			fprintf(stderr, "Bad char in left rule: '%c'\n", *pat);
			return false;
			}
		}

	return true;
	}


	//char *pattern;	/* first char of pattern to match in text */
	//char *context;	/* last char of text to be matched */
static bool rightmatch(const char *pattern, const char *context)
	{
	const char *pat;
	const char *text;

	if (*pattern == '\0')	/* null string matches any context */
		return true;

	pat = pattern;
	text = context;

	for (pat = pattern; *pat != '\0'; pat++)
		{
		/* First check for simple text or space */
		if (isalpha(*pat) || strchr(" '", *pat))
			{
			if (*pat != *text)
				{
				return false;
				}
			else
				{
				text++;
				continue;
				}
			}

		switch (*pat)
			{
		case '#':	/* One or more vowels */
			if (!isvowel(*text))
				return false;

			text++;

			while (isvowel(*text))
				text++;
			break;

		case ':':	/* Zero or more consonants */
			while (isconsonant(*text))
				text++;
			break;

		case '^':	/* One consonant */
			if (!isconsonant(*text))
				return false;
			text++;
			break;

		case '.':	/* B, D, V, G, J, L, M, N, R, W, Z */
			if (!strchr("BDVGJLMNRWZ", *text))
				return false;
			text++;
			break;

		case '+':	/* E, I or Y (front vowel) */
			if (!strchr("EIY", *text))
				return false;
			text++;
			break;

		case '%':	/* ER, E, ES, ED, ING, ELY (a suffix) */
			if (strncmp(text, "ING", 3) == 0 ||
			    strncmp(text, "ELY", 3) == 0)
				{
				text += 3;
				break;
				}
			if (strncmp(text, "ER", 2) == 0 ||
			    strncmp(text, "ES", 2) == 0 ||
			    strncmp(text, "ED", 2) == 0)
				{
				text += 2;
				break;
				}
			if (*text == 'E')
				{
				text += 1;
				break;
				}
			return false;

		default:
			fprintf(stderr, "Bad char in right rule:'%c'\n", *pat);
			return false;
			}
		}

	return true;
	}
