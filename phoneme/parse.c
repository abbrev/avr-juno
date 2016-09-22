#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "phoneme.h"
#include "saynum.h"
#include "spellword.h"

#define MAX_LENGTH 128

static FILE *In_file;
static FILE *Out_file;

static int Chars[4];
static void xlate_file(void);

#define Char  Chars[0]
#define Char1 Chars[1]
#define Char2 Chars[2]
#define Char3 Chars[3]

/*
** main(argc, argv)
**	int argc;
**	char *argv[];
**
**	This is the main program. It takes up to two file names (input
**	and output)  and translates the input file to phoneme codes
**	(see ENGLISH.C) on the output file.
*/
int main(int argc, char *argv[])
	{
	if (argc > 3)
		{
		fputs("Usage: PHONEME [infile [outfile]]\n", stderr);
		exit(1);
		}

	if (argc == 1)
		{
		fputs("Enter English text:\n", stderr);
		}

	if (argc > 1)
		{
		In_file = fopen(argv[1], "r");
		if (In_file == 0)
			{
			fputs("Error: Cannot open input file.\n", stderr);
			fputs("Usage: PHONEME [infile [outfile]]\n", stderr);
			exit(1);
			}
		}
	else
		In_file = stdin;

	if (argc > 2)
		{
		Out_file = fopen(argv[2], "w");
		if (Out_file == 0)
			{
			fputs("Error: Cannot create output file.\n", stderr);
			fputs("Usage: PHONEME [infile [outfile]]\n", stderr);
			exit(1);
			}
		}
	else
		Out_file = stdout;

	xlate_file();
	return 0;
	}

void outchar(int chr)
	{
	fputc(chr,Out_file);
	}


static int makeupper(int character)
	{
	if (islower(character))
		return toupper(character);
	else
		return character;
	}

static int new_char(void)
	{
	/*
	If the cache is full of newline, time to prime the look-ahead
	again. If an EOF is found, fill the remainder of the queue with
	EOF's.
	*/
	if (Char == '\n'  && Char1 == '\n' && Char2 == '\n' && Char3 == '\n')
		{	/* prime the pump again */
		Char = getc(In_file);
		if (Char == EOF)
			{
			Char1 = EOF;
			Char2 = EOF;
			Char3 = EOF;
			return Char;
			}
		if (Char == '\n')
			return Char;

		Char1 = getc(In_file);
		if (Char1 == EOF)
			{
			Char2 = EOF;
			Char3 = EOF;
			return Char;
			}
		if (Char1 == '\n')
			return Char;

		Char2 = getc(In_file);
		if (Char2 == EOF)
			{
			Char3 = EOF;
			return Char;
			}
		if (Char2 == '\n')
			return Char;

		Char3 = getc(In_file);
		}
	else
		{
		/*
		Buffer not full of newline, shuffle the characters and
		either get a new one or propagate a newline or EOF.
		*/
		Char = Char1;
		Char1 = Char2;
		Char2 = Char3;
		if (Char3 != '\n' && Char3 != EOF)
			Char3 = getc(In_file);
		}
	return Char;
	}

static void have_letter(void);
static void have_number(void);
static void have_dollars(void);
static void have_special(void);

/*
** xlate_file()
**
**	This is the input file translator. It sets up the first character
**	and uses it to determine what kind of text follows.
*/
static void xlate_file(void)
	{
	/* Prime the queue */
	Char = '\n';
	Char1 = '\n';
	Char2 = '\n';
	Char3 = '\n';
	new_char();	/* Fill Char, Char1, Char2 and Char3 */

	while (Char != EOF)	/* All of the words in the file */
		{
		if (isdigit(Char))
			have_number();
		else
		if (isalpha(Char) || Char == '\'')
			have_letter();
		else
		if (Char == '$' && isdigit(Char1))
			have_dollars();
		else
			have_special();
		}
	}

static void have_dollars(void)
	{
	unsigned long value;

	value = 0L;
	for (new_char() ; isdigit(Char) || Char == ',' ; new_char())
		{
		if (Char != ',')
			value = 10 * value + (Char-'0');
		}

	say_cardinal(value);	/* Say number of whole dollars */

	/* Found a character that is a non-digit and non-comma */

	/* Check for no decimal or no cents digits */
	if (Char != '.' || !isdigit(Char1))
		{
		if (value == 1L)
			xlate_word(" DOLLAR ");
		else
			xlate_word(" DOLLARS ");
		return;
		}

	/* We have '.' followed by a digit */

	new_char();	/* Skip the period */

	/* If it is ".dd " say as " DOLLARS AND n CENTS " */
	if (isdigit(Char1) && !isdigit(Char2))
		{
		if (value == 1L)
			xlate_word(" DOLLAR ");
		else
			xlate_word(" DOLLARS ");
		if (Char == '0' && Char1 == '0')
			{
			new_char();	/* Skip tens digit */
			new_char();	/* Skip units digit */
			return;
			}

		xlate_word(" AND ");
		value = (Char-'0')*10 + Char1-'0';
		say_cardinal(value);

		if (value == 1L)
			xlate_word(" CENT ");
		else
			xlate_word(" CENTS ");
		new_char();	/* Used Char (tens digit) */
		new_char();	/* Used Char1 (units digit) */
		return;
		}

	/* Otherwise say as "n POINT ddd DOLLARS " */

	xlate_word(" POINT ");
	for ( ; isdigit(Char) ; new_char())
		{
		say_ascii(Char);
		}

	xlate_word(" DOLLARS ");

	return;
	}

static void have_special(void)
	{
#if 0
	if (Char == '\n')
		outchar('\n');
	else
#endif
	if (!isspace(Char))
		say_ascii(Char);

	new_char();
	return;
	}


static void have_number(void)
	{
	unsigned long value;
	int lastdigit;

	value = Char - '0';
	lastdigit = Char;

	for (new_char() ; isdigit(Char) ; new_char())
		{
		value = 10 * value + (Char-'0');
		lastdigit = Char;
		}

	/* Recognize ordinals based on last digit of number */
	switch (lastdigit)
		{
	case '1':	/* ST */
		if (makeupper(Char) == 'S' && makeupper(Char1) == 'T' &&
		    !isalpha(Char2) && !isdigit(Char2))
			{
			say_ordinal(value);
			new_char();	/* Used Char */
			new_char();	/* Used Char1 */
			return;
			}
		break;

	case '2':	/* ND */
		if (makeupper(Char) == 'N' && makeupper(Char1) == 'D' &&
		    !isalpha(Char2) && !isdigit(Char2))
			{
			say_ordinal(value);
			new_char();	/* Used Char */
			new_char();	/* Used Char1 */
			return;
			}
		break;

	case '3':	/* RD */
		if (makeupper(Char) == 'R' && makeupper(Char1) == 'D' &&
		    !isalpha(Char2) && !isdigit(Char2))
			{
			say_ordinal(value);
			new_char();	/* Used Char */
			new_char();	/* Used Char1 */
			return;
			}
		break;

	case '0':	/* TH */
	case '4':	/* TH */
	case '5':	/* TH */
	case '6':	/* TH */
	case '7':	/* TH */
	case '8':	/* TH */
	case '9':	/* TH */
		if (makeupper(Char) == 'T' && makeupper(Char1) == 'H' &&
		    !isalpha(Char2) && !isdigit(Char2))
			{
			say_ordinal(value);
			new_char();	/* Used Char */
			new_char();	/* Used Char1 */
			return;
			}
		break;
		}

	say_cardinal(value);

	/* Recognize decimal points */
	if (Char == '.' && isdigit(Char1))
		{
		xlate_word(" POINT ");
		for (new_char() ; isdigit(Char) ; new_char())
			{
			say_ascii(Char);
			}
		}

	/* Spell out trailing abbreviations */
	if (isalpha(Char))
		{
		while (isalpha(Char))
			{
			say_ascii(Char);
			new_char();
			}
		}

	return;
	}

static void abbrev(const char buff[]);

static void have_letter(void)
	{
	char buff[MAX_LENGTH];
	int count;

	count = 0;
	buff[count++] = ' ';	/* Required initial blank */

	buff[count++] = makeupper(Char);

	for (new_char() ; isalpha(Char) || Char == '\'' ; new_char())
		{
		buff[count++] = makeupper(Char);
		if (count > MAX_LENGTH-2)
			{
			buff[count++] = ' ';
			buff[count++] = '\0';
			xlate_word(buff);
			count = 1;
			}
		}

	buff[count++] = ' ';	/* Required terminating blank */
	buff[count++] = '\0';

	/* Check for AAANNN type abbreviations */
	if (isdigit(Char))
		{
		spell_word(buff);
		return;
		}
	else
	if (strlen(buff) == 3)	 /* one character, two spaces */
		say_ascii(buff[1]);
	else
	if (Char == '.')		/* Possible abbreviation */
		abbrev(buff);
	else
		xlate_word(buff);

	if (Char == '-' && isalpha(Char1))
		new_char();	/* Skip hyphens */

	}

/* Handle abbreviations. Text in buff was followed by '.' */
static void abbrev(const char buff[])
	{
	if (strcmp(buff, " DR ") == 0)
		{
		xlate_word(" DOCTOR ");
		new_char();
		}
	else
	if (strcmp(buff, " MR ") == 0)
		{
		xlate_word(" MISTER ");
		new_char();
		}
	else
	if (strcmp(buff, " MRS ") == 0)
		{
		xlate_word(" MISSUS ");
		new_char();
		}
	else
	if (strcmp(buff, " PHD ") == 0)
		{
		spell_word(" PHD ");
		new_char();
		}
	else
		xlate_word(buff);
	}
