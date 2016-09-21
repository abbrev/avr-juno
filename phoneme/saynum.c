#include <stdio.h>

#include "phoneme.h"
#include "saynum.h"

/*
**              Integer to Readable ASCII Conversion Routine.
**
** Synopsis:
**
**      say_cardinal(value)
**      	long int     value;          -- The number to output
**
**	The number is translated into a string of phonemes
**
*/

static char *Cardinals[] = 
	{
	" ZERO ",	" ONE ",	" TWO ",		" THREE ",
	" FOUR ",	" FIVE ",	" SIX ",	" SEVEN ",
	" EIGHT ",		" NINE ",
	" TEN ",	" ELEVEN ",	" TWELVE ",	" THIRTEEN ",
	" FOURTEEN ",	" FIFTEEN ", 	" SIXTEEN ",	" SEVENTEEN ",
	" EIGHTEEN ",	" NINETEEN "
	} ;

static char *Twenties[] = 
	{
	" TWENTY ",	" THIRTY ",	" FORTY ",	" FIFTY ",
	" SIXTY ",	" SEVENTY ",	" EIGHTY ",	" NINETY "
	} ;

static char *Ordinals[] = 
	{
	" ZEROTH ",	" FIRST ",	" SECOND ",	" THIRD ",
	" FOURTH ",	" FIFTH ",	" SIXTH ",	" SEVENTH ",
	" EIGHTH ",		" NINTH ",
	" TENTH ",	" ELEVENTH ",	" TWELVETH ",	" THIRTEENTH ",
	" FOURTEENTH ",	" FIFTEENTH ", 	" SIXTEENTH ",	" SEVENTEENTH ",
	" EIGHTEENTH ",	" NINETEENTH "
	} ;

static char *Ord_twenties[] = 
	{
	" TWENTIETH ",	" THIRTIETH ",	" FORTIETH ",	" FIFTIETH ",
	" SIXTIETH ",	" SEVENTIETH ",	" EIGHTIETH ",	" NINETIETH "
	} ;


/*
** Translate a number to phonemes.  This version is for CARDINAL numbers.
**	 Note: this is recursive.
*/
void say_cardinal(long svalue)
	{
	unsigned long value;
	if (svalue < 0)
		{
		xlate_word(" MINUS ");
		value = (-svalue);
		}
	else
		{
		value = svalue;
		}

	if (value >= 1000000000L)	/* Billions */
		{
		say_cardinal(value/1000000000L);
		xlate_word(" BILLION ");
		value = value % 1000000000;
		if (value == 0)
			return;		/* Even billion */
		if (value < 100)	/* as in THREE BILLION AND FIVE */
			xlate_word(" AND ");
		}

	if (value >= 1000000L)	/* Millions */
		{
		say_cardinal(value/1000000L);
		xlate_word(" MILLION ");
		value = value % 1000000L;
		if (value == 0)
			return;		/* Even million */
		if (value < 100)	/* as in THREE MILLION AND FIVE */
			xlate_word(" AND ");
		}

	/* Thousands 1000..1099 2000..99999 */
	/* 1100 to 1999 is eleven-hunderd to ninteen-hunderd */
	if ((value >= 1000L && value <= 1099L) || value >= 2000L)
		{
		say_cardinal(value/1000L);
		xlate_word(" THOUSAND ");
		value = value % 1000L;
		if (value == 0)
			return;		/* Even thousand */
		if (value < 100)	/* as in THREE THOUSAND AND FIVE */
			xlate_word(" AND ");
		}

	if (value >= 100L)
		{
		xlate_word(Cardinals[value/100]);
		xlate_word(" HUNDRED ");
		value = value % 100;
		if (value == 0)
			return;		/* Even hundred */
		}

	if (value >= 20)
		{
		xlate_word(Twenties[(value-20)/ 10]);
		value = value % 10;
		if (value == 0)
			return;		/* Even ten */
		}

	xlate_word(Cardinals[value]);
	return;
	} 


/*
** Translate a number to phonemes.  This version is for ORDINAL numbers.
**	 Note: this is recursive.
*/
void say_ordinal(long svalue)
	{
	unsigned long value;

	if (svalue < 0)
		{
		xlate_word(" MINUS ");
		value = (-svalue);
		}
	else
		{
		value = svalue;
		}

	if (value >= 1000000000L)	/* Billions */
		{
		say_cardinal(value/1000000000L);
		value = value % 1000000000;
		if (value == 0)
			{
			xlate_word(" BILLIONTH ");
			return;		/* Even billion */
			}
		xlate_word(" BILLION ");
		if (value < 100)	/* as in THREE BILLION AND FIVE */
			xlate_word(" AND ");
		}

	if (value >= 1000000L)	/* Millions */
		{
		say_cardinal(value/1000000L);
		value = value % 1000000L;
		if (value == 0)
			{
			xlate_word(" MILLIONTH ");
			return;		/* Even million */
			}
		xlate_word(" MILLION ");
		if (value < 100)	/* as in THREE MILLION AND FIVE */
			xlate_word(" AND ");
		}

	/* Thousands 1000..1099 2000..99999 */
	/* 1100 to 1999 is eleven-hunderd to ninteen-hunderd */
	if ((value >= 1000L && value <= 1099L) || value >= 2000L)
		{
		say_cardinal(value/1000L);
		value = value % 1000L;
		if (value == 0)
			{
			xlate_word(" THOUSANDTH ");
			return;		/* Even thousand */
			}
		xlate_word(" THOUSAND ");
		if (value < 100)	/* as in THREE THOUSAND AND FIVE */
			xlate_word(" AND ");
		}

	if (value >= 100L)
		{
		xlate_word(Cardinals[value/100]);
		value = value % 100;
		if (value == 0)
			{
			xlate_word(" HUNDREDTH ");
			return;		/* Even hundred */
			}
		xlate_word(" HUNDRED ");
		}

	if (value >= 20)
		{
		if ((value%10) == 0)
			{
			xlate_word(Ord_twenties[(value-20)/ 10]);
			return;		/* Even ten */
			}
		xlate_word(Twenties[(value-20)/ 10]);
		value = value % 10;
		}

	xlate_word(Ordinals[value]);
	return;
	} 
