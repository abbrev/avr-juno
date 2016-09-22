/*
**	English to Phoneme rules.
**
**	Derived from:
**
**	     AUTOMATIC TRANSLATION OF ENGLISH TEXT TO PHONETICS
**	            BY MEANS OF LETTER-TO-SOUND RULES
**
**			NRL Report 7948
**
**		      January 21st, 1976
**	    Naval Research Laboratory, Washington, D.C.
**
**
**	Published by the National Technical Information Service as
**	document "AD/A021 929".
**
**
**
**	The Phoneme codes:
**
**		i	bEEt		I	bIt
**		eI	gAte		E	gEt
**		&	fAt		A	fAther
**		O	lAWn		oU	lOne
**		U	fUll		u	fOOl
**		R	mURdER		@	About
**		V	bUt		aI	hIde
**		aU	hOW		oI	tOY
**	
**		p	Pack		b	Back
**		t	Time		d	Dime
**		k	Coat		g	Goat
**		f	Fault		v	Vault
**		T	eTHer		D	eiTHer
**		s	Sue		z	Zoo
**		S	leaSH		Z	leiSure
**		h	How		m	suM
**		n	suN		N	suNG
**		l	Laugh		w	Wear
**		j	Young		r	Rate
**		tS	CHar		dZ	Jar
**		hw	WHere
**
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
**		Separate each block of letters (apostrophes included)
**		and add a space on each side. For each unmatched
**		letter in the word, look through the rules where the
**		text to match starts with the letter in the word. If
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
**			(Found in right context only)
**		+	One of E, I or Y (a "front" vowel)
**
*/

#include "phoneme.h"

/* Context definitions */
static const char Anything[] = "";	/* No context requirement */
static const char Nothing[] = " ";	/* Context is beginning or end of word */

/* Phoneme definitions */
static const char Pause[] = " ";	/* Short silence */
static const char Silent[] = "";	/* No phonemes */

/*0 = Punctuation */
/*
**	left		match		right		out
*/
static const Rule punct_rules[] =
	{
	{Anything,	" ",		Anything,	Pause	},
	{Anything,	"-",		Anything,	Silent	},
	{".",		"'S",		Anything,	"z"	},
	{"#:.E",	"'S",		Anything,	"z"	},
	{"#",		"'S",		Anything,	"z"	},
	{Anything,	"'",		Anything,	Silent	},
	{Anything,	",",		Anything,	Pause	},
	{Anything,	".",		Anything,	Pause	},
	{Anything,	"?",		Anything,	Pause	},
	{Anything,	"!",		Anything,	Pause	},
	{Anything,	0,		Anything,	Silent	},
	};

/*
**	left		match		right		out
*/
static const Rule A_rules[] =
	{
	{Anything,	"A",		Nothing,	"@"	},
	{Nothing,	"ARE",		Nothing,	"AR"	},
	{Nothing,	"AR",		"O",		"@r"	},
	{Anything,	"AR",		"#",		"E:r"	},
	{"^",		"AS",		"#",		"eIs"	},
	{Anything,	"A",		"WA",		"@"	},
	{Anything,	"AW",		Anything,	"O:"	},
	{" :",		"ANY",		Anything,	"'E:ni:"},
	{Anything,	"A",		"^+#",		"eI"	},
	{"#:",		"ALLY",		Anything,	"@li:"	},
	{Nothing,	"AL",		"#",		"@l"	},
	{Anything,	"AGAIN",	Anything,	"@g'E:n"},
	{"#:",		"AG",		"E",		"I:dZ"	},
	{Anything,	"A",		"^+:#",		"&:"	},
	{" :",		"A",		"^+ ",		"eI"	},
	{Anything,	"A",		"^%",		"eI"	},
	{Nothing,	"ARR",		Anything,	"@r"	},
	{Anything,	"ARR",		Anything,	"'&:r"	},
	{" :",		"AR",		Nothing,	"AR"	},
	{Anything,	"AR",		Nothing,	"R:"	},
	{Anything,	"AR",		Anything,	"A:r"	},
	{Anything,	"AIR",		Anything,	"E:r"	},
	{Anything,	"AI",		Anything,	"eI"	},
	{Anything,	"AY",		Anything,	"eI"	},
	{Anything,	"AU",		Anything,	"O:"	},
	{"#:",		"AL",		Nothing,	"L"	},
	{"#:",		"ALS",		Nothing,	"Lz"	},
	{Anything,	"ALK",		Anything,	"O:k"	},
	{Anything,	"AL",		"^",		"OL"	},
	{" :",		"ABLE",		Anything,	"'eIbL:"},
	{Anything,	"ABLE",		Anything,	"@bL:"},
	{Anything,	"ANG",		"+",		"eIndZ"	},
	{Anything,	"A",		Anything,	"&:"	},
 	{Anything,	0,		Anything,	Silent	},
	};

/*
**	left		match		right		out
*/
static const Rule B_rules[] =
	{
	{Nothing,	"BE",		"^#",		"bI:"	},
	{Anything,	"BEING",	Anything,	"'biIN"},
	{Nothing,	"BOTH",		Nothing,	"boUT"	},
	{Nothing,	"BUS",		"#",		"bI:z"	},
	{Anything,	"BUIL",		Anything,	"bIL"	},
	{Anything,	"B",		Anything,	"b"	},
	{Anything,	0,		Anything,	Silent	},
	};

/*
**	left		match		right		out
*/
static const Rule C_rules[] =
	{
	{Nothing,	"CH",		"^",		"k"	},
	{"^E",		"CH",		Anything,	"k"	},
	{Anything,	"CH",		Anything,	"tS"	},
	{" S",		"CI",		"#",		"saI"	},
	{Anything,	"CI",		"A",		"S"	},
	{Anything,	"CI",		"O",		"S"	},
	{Anything,	"CI",		"EN",		"S"	},
	{Anything,	"C",		"+",		"s"	},
	{Anything,	"CK",		Anything,	"k"	},
	{Anything,	"COM",		"%",		"kV:m"	},
	{Anything,	"C",		Anything,	"k"	},
	{Anything,	0,		Anything,	Silent	},
	};

/*
**	left		match		right		out
*/
static const Rule D_rules[] =
	{
	{"#:",		"DED",		Nothing,	"dI:d"	},
	{".E",		"D",		Nothing,	"d"	},
	{"#:^E",	"D",		Nothing,	"t"	},
	{Nothing,	"DE",		"^#",		"dI:"	},
	{Nothing,	"DO",		Nothing,	"du:"	},
	{Nothing,	"DOES",		Anything,	"dV:z"	},
	{Nothing,	"DOING",	Anything,	"'duIN"	},
	{Nothing,	"DOW",		Anything,	"daU"	},
	{Anything,	"DU",		"A",		"dZu"	},
	{Anything,	"D",		Anything,	"d"	},
	{Anything,	0,		Anything,	Silent	},
	};

/*
**	left		match		right		out
*/
static const Rule E_rules[] =
	{
	{"#:",		"E",		Nothing,	Silent	},
	{"':^",		"E",		Nothing,	Silent	},
	{" :",		"E",		Nothing,	"i:"	},
	{"#",		"ED",		Nothing,	"d"	},
	{"DR",		"E",		"D",		"I:"	},
	{"#:",		"E",		"D ",		Silent	},
	{Anything,	"EV",		"ER",		"E:v"	},
	{Anything,	"E",		"^%",		"i:"	},
	{Anything,	"ERI",		"#",		"'i:ri"},
	{Anything,	"ERI",		Anything,	"E:rI"	},
	{"#:",		"ER",		"#",		"R:"	},
	{Anything,	"ER",		"#",		"E:r"	},
	{Anything,	"ER",		Anything,	"R:"	},
	{Nothing,	"EVEN",		Anything,	"i:vE:n"},
	{"#:",		"E",		"W",		Silent	},
	{"T",		"EW",		Anything,	"u:"	},
	{"S",		"EW",		Anything,	"u:"	},
	{"R",		"EW",		Anything,	"u:"	},
	{"D",		"EW",		Anything,	"u:"	},
	{"L",		"EW",		Anything,	"u:"	},
	{"Z",		"EW",		Anything,	"u:"	},
	{"N",		"EW",		Anything,	"u:"	},
	{"J",		"EW",		Anything,	"u:"	},
	{"TH",		"EW",		Anything,	"u:"	},
	{"CH",		"EW",		Anything,	"u:"	},
	{"SH",		"EW",		Anything,	"u:"	},
	{Anything,	"EW",		Anything,	"ju:"	},
	{Anything,	"E",		"O",		"i:"	},
	{"#:S",		"ES",		Nothing,	"I:z"	},
	{"#:C",		"ES",		Nothing,	"I:z"	},
	{"#:G",		"ES",		Nothing,	"I:z"	},
	{"#:Z",		"ES",		Nothing,	"I:z"	},
	{"#:X",		"ES",		Nothing,	"I:z"	},
	{"#:J",		"ES",		Nothing,	"I:z"	},
	{"#:CH",	"ES",		Nothing,	"I:z"	},
	{"#:SH",	"ES",		Nothing,	"I:z"	},
	{"#:",		"E",		"S ",		Silent	},
	{"#:",		"ELY",		Nothing,	"li:"	},
	{"#:",		"EMENT",	Anything,	"mE:nt"	},
	{Anything,	"EFUL",		Anything,	"fUL"	},
	{Anything,	"EE",		Anything,	"i:"	},
	{Anything,	"EARN",		Anything,	"R:n"	},
	{Nothing,	"EAR",		"^",		"R:"	},
	{Anything,	"EAD",		Anything,	"E:d"	},
	{"#:",		"EA",		Nothing,	"i@"	},
	{Anything,	"EA",		"SU",		"E:"	},
	{Anything,	"EA",		Anything,	"i:"	},
	{Anything,	"EIGH",		Anything,	"'eI"	},
	{Anything,	"EI",		Anything,	"i:"	},
	{Nothing,	"EYE",		Anything,	"aI"	},
	{Anything,	"EY",		Anything,	"i:"	},
	{Anything,	"EU",		Anything,	"ju:"	},
	{Anything,	"E",		Anything,	"E:"	},
	{Anything,	0,		Anything,	Silent	},
	};

/*
**	left		match		right		out
*/
static const Rule F_rules[] =
	{
	{Anything,	"FUL",		Anything,	"fUL"	},
	{Anything,	"F",		Anything,	"f"	},
	{Anything,	0,		Anything,	Silent	},
	};

/*
**	left		match		right		out
*/
static const Rule G_rules[] =
	{
	{Anything,	"GIV",		Anything,	"gI:v"	},
	{Nothing,	"G",		"I^",		"g"	},
	{Anything,	"GE",		"T",		"gE:"	},
	{"SU",		"GGES",		Anything,	"gdZ'E:s"},
	{Anything,	"GG",		Anything,	"g"	},
	{" B#",		"G",		Anything,	"g"	},
	{Anything,	"G",		"+",		"dZ"	},
	{Anything,	"GREAT",	Anything,	"greIt"	},
	{"#",		"GH",		Anything,	Silent	},
	{Anything,	"G",		Anything,	"g"	},
	{Anything,	0,		Anything,	Silent	},
	};

/*
**	left		match		right		out
*/
static const Rule H_rules[] =
	{
	{Nothing,	"HAV",		Anything,	"h&:v"	},
	{Nothing,	"HERE",		Anything,	"hiR"	},
	{Nothing,	"HOUR",		Anything,	"aUR"	},
	{Anything,	"HOW",		Anything,	"haU"	},
	{Anything,	"H",		"#",		"h"	},
	{Anything,	"H",		Anything,	Silent	},
	{Anything,	0,		Anything,	Silent	},
	};

/*
**	left		match		right		out
*/
static const Rule I_rules[] =
	{
	{"AL",		"I",		"VE",		"'aI"    },
	{Anything,	"I",		"NUS",		"'aI"    },
	{Nothing,	"IN",		Anything,	"I:n"	},
	{Nothing,	"I",		Nothing,	"aI"	},
	{Anything,	"IN",		"D",		"aIn"	},
	{Anything,	"IER",		Anything,	"iR"	},
	{"#:R",		"IED",		Anything,	"i:d"	},
	{Anything,	"IED",		Nothing,	"aId"	},
	{Anything,	"IEN",		Anything,	"iEn"	},
	{Anything,	"IE",		"T",		"aIE"	},
	{" :",		"I",		"%",		"aI"	},
	{Anything,	"I",		"%",		"i:"	},
	{Anything,	"IE",		Anything,	"i:"	},
	{Anything,	"I",		"^+:#",		"I:"	},
	{Anything,	"IR",		"#",		"aIr"	},
	{Anything,	"IZ",		"%",		"aIz"	},
	{Anything,	"IS",		"%",		"aIz"	},
	{Anything,	"I",		"D%",		"aI"	},
	{"+^",		"I",		"^+",		"I:"	},
	{Anything,	"I",		"T%",		"aI"	},
	{"#:^",		"I",		"^+",		"I:"	},
	{Anything,	"I",		"^+",		"aI"	},
	{Anything,	"IR",		Anything,	"R:"	},
	{Anything,	"IGH",		Anything,	"aI"	},
	{Anything,	"ILD",		Anything,	"aILd"	},
	{Anything,	"IGN",		Nothing,	"'aIn"	},
	{Anything,	"IGN",		"^",		"'aIn"	},
	{Anything,	"IGN",		"%",		"'aIn"	},
	{Anything,	"IQUE",		Anything,	"'i:k"	},
	{Anything,	"I",		Anything,	"I:"	},
	{Anything,	0,		Anything,	Silent	},
	};

/*
**	left		match		right		out
*/
static const Rule J_rules[] =
	{
	{Anything,	"J",		Anything,	"dZ"	},
	{Anything,	0,		Anything,	Silent	},
	};

/*
**	left		match		right		out
*/
static const Rule K_rules[] =
	{
	{Nothing,	"K",		"N",		Silent	},
	{Anything,	"K",		Anything,	"k"	},
	{Anything,	0,		Anything,	Silent	},
	};

/*
**	left		match		right		out
*/
static const Rule L_rules[] =
	{
	{Anything,	"LO",		"C#",		"loU"	},
	{"L",		"L",		Anything,	Silent	},
	{"#:^",		"L",		"%",		"L"	},
	{Anything,	"LEAD",		Anything,	"li:d"	},
	{Anything,	"L",		Anything,	"l"	},
	{Anything,	0,		Anything,	Silent	},
	};

/*
**	left		match		right		out
*/
static const Rule M_rules[] =
	{
	{Anything,	"MOV",		Anything,	"mu:v"	},
	{Anything,	"M",		Anything,	"m"	},
	{Anything,	0,		Anything,	Silent	},
	};

/*
**	left		match		right		out
*/
static const Rule N_rules[] =
	{
	{"E",		"NG",		"+",		"ndZ"	},
	{Anything,	"NG",		"R",		"Ng"	},
	{Anything,	"NG",		"#",		"Ng"	},
	{Anything,	"NGL",		"%",		"NgL"},
	{Anything,	"NG",		Anything,	"N"	},
	{Anything,	"NK",		Anything,	"Nk"	},
	{Nothing,	"NOW",		Nothing,	"naU"	},
	{Anything,	"N",		Anything,	"n"	},
	{Anything,	0,		Anything,	Silent	},
	};

/*
**	left		match		right		out
*/
static const Rule O_rules[] =
	{
	{Anything,	"OF",		Nothing,	"@v"	},
	{Anything,	"OROUGH",	Anything,	"'RoU"	},
	{"#:",		"OR",		Nothing,	"R:"	},
	{"#:",		"ORS",		Nothing,	"R:z"	},
	{Anything,	"OR",		Anything,	"O:r"	},
	{Nothing,	"ONE",		Anything,	"wV:n"	},
	{Anything,	"OW",		Anything,	"oU"	},
	{Nothing,	"OVER",		Anything,	"oUvR:"	},
	{Anything,	"OV",		Anything,	"V:v"	},
	{Anything,	"O",		"^%",		"oU"	},
	{Anything,	"O",		"^EN",		"oU"	},
	{Anything,	"O",		"^I#",		"oU"	},
	{Anything,	"OL",		"D",		"oUL"	},
	{Anything,	"OUGHT",	Anything,	"O:t"	},
	{Anything,	"OUGH",		Anything,	"V:f"	},
	{Nothing,	"OU",		Anything,	"aU"	},
	{"H",		"OU",		"S#",		"aU"	},
	{Anything,	"OUS",		Anything,	"@s"	},
	//{Anything,	"OUR",		Anything,	"OR"	}, // XXX
	{Anything,	"OULD",		Anything,	"U:d"	},
	{"^",		"OU",		"^L",		"V:"	},
	{Anything,	"OUP",		Anything,	"u:p"	},
	{Anything,	"OU",		Anything,	"aU"	},
	{Anything,	"OY",		Anything,	"oI"	},
	{Anything,	"OING",		Anything,	"'oUIN"	},
	{Anything,	"OI",		Anything,	"oI"	},
	{Anything,	"OOR",		Anything,	"OR"	},
	{Anything,	"OOK",		Anything,	"U:k"	},
	{Anything,	"OOD",		Anything,	"U:d"	},
	{Anything,	"OO",		Anything,	"u:"	},
	{Anything,	"O",		"E",		"oU"	},
	{Anything,	"O",		Nothing,	"oU"	},
	{Anything,	"OA",		Anything,	"oU"	},
	{Nothing,	"ONLY",		Anything,	"'oUnli:"},
	{Nothing,	"ONCE",		Anything,	"wV:ns:"},
	{Anything,	"ON'T",		Anything,	"oUnt"	},
	{"C",		"O",		"N",		"A:"	},
	{Anything,	"O",		"NG",		"O:"	},
	{" :^",		"O",		"N",		"V:"	},
	{"I",		"ON",		Anything,	"@n"	},
	{"#:",		"ON",		Nothing,	"@n"	},
	{"#^",		"ON",		Anything,	"@n"	},
	{Anything,	"O",		"ST ",		"oU"	},
	{Anything,	"OF",		"^",		"O:f"	},
	{Anything,	"OTHER",	Anything,	"'V:DR:"},
	{Anything,	"OSS",		Nothing,	"O:s:"	},
	{"#:^",		"OM",		Anything,	"V:m"	},
	{Anything,	"O",		Anything,	"A:"	},
	{Anything,	0,		Anything,	Silent	},
	};

/*
**	left		match		right		out
*/
static const Rule P_rules[] =
	{
	{Anything,	"PH",		Anything,	"f"	},
	{Anything,	"PEOP",		Anything,	"p'i:p"	},
	{Anything,	"POW",		Anything,	"paU"	},
	{Anything,	"PUT",		Nothing,	"pU:t"	},
	{Anything,	"P",		Anything,	"p"	},
	{Anything,	0,		Anything,	Silent	},
	};

/*
**	left		match		right		out
*/
static const Rule Q_rules[] =
	{
	{Anything,	"QUAR",		Anything,	"kwOR"	},
	{Anything,	"QU",		Anything,	"kw"	},
	{Anything,	"Q",		Anything,	"k"	},
	{Anything,	0,		Anything,	Silent	},
	};

/*
**	left		match		right		out
*/
static const Rule R_rules[] =
	{
	{Nothing,	"RE",		"^#",		"ri:"	},
	{Anything,	"R",		Nothing,	"R:"	},
	{Anything,	"R",		Anything,	"r"	},
	{Anything,	0,		Anything,	Silent	},
	};

/*
**	left		match		right		out
*/
static const Rule S_rules[] =
	{
	{Anything,	"SH",		Anything,	"S"	},
	{"#",		"SION",		Anything,	"Z@n"	},
	{Anything,	"SOME",		Anything,	"sV:m"	},
	{"#",		"SUR",		"#",		"ZR:"	},
	{Anything,	"SUR",		"#",		"SR:"	},
	{"#",		"SU",		"#",		"Zu:"	},
	{"#",		"SSU",		"#",		"Su:"	},
	{"#",		"SED",		Nothing,	"zd"	},
	{"#",		"S",		"#",		"z"	},
	{Anything,	"SAID",		Anything,	"sE:d"	},
	{"^",		"SION",		Anything,	"S@n"	},
	{Anything,	"S",		"S",		Silent	},
	{".",		"S",		Nothing,	"z"	},
	{"#:.E",	"S",		Nothing,	"z"	},
	{"#:^##",	"S",		Nothing,	"z"	},
	{"#:^#",	"S",		Nothing,	"s"	},
	{"U",		"S",		Nothing,	"s"	},
	{" :#",		"S",		Nothing,	"z"	},
	{Nothing,	"SCH",		Anything,	"sk"	},
	{Anything,	"S",		"C+",		Silent	},
	{"#",		"SM",		Anything,	"z@m"	},
	{"#",		"SN",		"'",		"z@n"	},
	{Anything,	"S",		Anything,	"s"	},
	{Anything,	0,		Anything,	Silent	},
	};

/*
**	left		match		right		out
*/
static const Rule T_rules[] =
	{
	{Nothing,	"THE",		Nothing,	"D@"	},
	{Anything,	"TO",		Nothing,	"tu"	},
	{Anything,	"THAT",		Nothing,	"D&:t"	},
	{Nothing,	"THIS",		Nothing,	"DI:s"	},
	{Nothing,	"THEY",		Anything,	"DeI"	},
	{Nothing,	"THERE",	Anything,	"DER"	},
	{Anything,	"THER",		Anything,	"DR:"	},
	{Anything,	"THEIR",	Anything,	"DeR"	},
	{Nothing,	"THAN",		Nothing,	"D&:n"	},
	{Nothing,	"THEM",		Nothing,	"DE:m"	},
	{Anything,	"THESE",	Nothing,	"Di:z"	},
	{Nothing,	"THEN",		Anything,	"DE:n"	},
	{Anything,	"THROUGH",	Anything,	"Tru:"	},
	{Anything,	"THOSE",	Anything,	"DoUz"	},
	{Anything,	"THOUGH",	Nothing,	"DoU"	},
	{Nothing,	"THUS",		Anything,	"DV:s"	},
	{Anything,	"TH",		Anything,	"T"	},
	{"#:",		"TED",		Nothing,	"tId"	},
	{"S",		"TI",		"#N",		"tS"	},
	{Anything,	"TI",		"O",		"S"	},
	{Anything,	"TI",		"A",		"S"	},
	{Anything,	"TIEN",		Anything,	"S@n"	},
	{Anything,	"TUR",		"#",		"tSR:"	},
	{Anything,	"TU",		"A",		"tSu:"	},
	{Nothing,	"TWO",		Anything,	"tu:"	},
	{Anything,	"T",		Anything,	"t"	},
	{Anything,	0,		Anything,	Silent	},
	};

/*
**	left		match		right		out
*/
static const Rule U_rules[] =
	{
	{Nothing,	"UN",		"I",		"ju:n"	},
	{Nothing,	"UN",		Anything,	"V:n"	},
	{Nothing,	"UPON",		Anything,	"@p'O:n"},
	{"T",		"UR",		"#",		"U:r"	},
	{"S",		"UR",		"#",		"U:r"	},
	{"R",		"UR",		"#",		"U:r"	},
	{"D",		"UR",		"#",		"U:r"	},
	{"L",		"UR",		"#",		"U:r"	},
	{"Z",		"UR",		"#",		"U:r"	},
	{"N",		"UR",		"#",		"U:r"	},
	{"J",		"UR",		"#",		"U:r"	},
	{"TH",		"UR",		"#",		"U:r"	},
	{"CH",		"UR",		"#",		"U:r"	},
	{"SH",		"UR",		"#",		"U:r"	},
	{Anything,	"UR",		"#",		"jU:r"	},
	{Anything,	"UR",		Anything,	"R:"	},
	{Anything,	"U",		"^ ",		"V:"	},
	{Anything,	"U",		"^^",		"V:"	},
	{Anything,	"UY",		Anything,	"aI"	},
	{" G",		"U",		"#",		Silent	},
	{"G",		"U",		"%",		Silent	},
	{"G",		"U",		"#",		"w"	},
	{"#N",		"U",		Anything,	"ju:"	},
	{"T",		"U",		Anything,	"u:"	},
	{"S",		"U",		Anything,	"u:"	},
	{"R",		"U",		Anything,	"u:"	},
	{"D",		"U",		Anything,	"u:"	},
	{"L",		"U",		Anything,	"u:"	},
	{"Z",		"U",		Anything,	"u:"	},
	{"N",		"U",		Anything,	"u:"	},
	{"J",		"U",		Anything,	"u:"	},
	{"TH",		"U",		Anything,	"u:"	},
	{"CH",		"U",		Anything,	"u:"	},
	{"SH",		"U",		Anything,	"u:"	},
	{Anything,	"U",		Anything,	"ju:"	},
	{Anything,	0,		Anything,	Silent	},
	};

/*
**	left		match		right		out
*/
static const Rule V_rules[] =
	{
	{Anything,	"VIEW",		Anything,	"vju:"	},
	{Anything,	"V",		Anything,	"v"	},
	{Anything,	0,		Anything,	Silent	},
	};

/*
**	left		match		right		out
*/
static const Rule W_rules[] =
	{
	{Nothing,	"WERE",		Anything,	"wR:"	},
	{Anything,	"WA",		"S",		"wA:"	},
	{Anything,	"WA",		"T",		"wA:"	},
	{Anything,	"WHERE",	Anything,	"hwER"	},
	{Anything,	"WHAT",		Anything,	"hwA:t"	},
	{Anything,	"WHOL",		Anything,	"hoUL"	},
	{Anything,	"WHO",		Anything,	"hu:"	},
	{Anything,	"WH",		Anything,	"hw"	},
	{Anything,	"WAR",		Anything,	"wOR"	},
	{Anything,	"WOR",		"^",		"wR:"	},
	{Anything,	"WR",		Anything,	"r"	},
	{Anything,	"W",		Anything,	"w"	},
	{Anything,	0,		Anything,	Silent	},
	};

/*
**	left		match		right		out
*/
static const Rule X_rules[] =
	{
	{Anything,	"X",		Anything,	"ks"	},
	{Anything,	0,		Anything,	Silent	},
	};

/*
**	left		match		right		out
*/
static const Rule Y_rules[] =
	{
	{Anything,	"YOUNG",	Anything,	"jV:N"	},
	{Nothing,	"YOU",		Anything,	"ju:"	},
	{Nothing,	"YES",		Anything,	"jE:s"	},
	{Nothing,	"Y",		Anything,	"j"	},
	{"#:^",		"Y",		Nothing,	"i:"	},
	{"#:^",		"Y",		"I",		"i:"	},
	{" :",		"Y",		Nothing,	"aI"	},
	{" :",		"Y",		"#",		"aI"	},
	{" :",		"Y",		"^+:#",		"I:"	},
	{" :",		"Y",		"^#",		"aI"	},
	{Anything,	"Y",		Anything,	"I:"	},
	{Anything,	0,		Anything,	Silent	},
	};

/*
**	left		match		right		out
*/
static const Rule Z_rules[] =
	{
	{Anything,	"Z",		Anything,	"z"	},
	{Anything,	0,		Anything,	Silent	},
	};

const Rule *const Rules[] =
	{
	punct_rules,
	A_rules, B_rules, C_rules, D_rules, E_rules, F_rules, G_rules,
	H_rules, I_rules, J_rules, K_rules, L_rules, M_rules, N_rules,
	O_rules, P_rules, Q_rules, R_rules, S_rules, T_rules, U_rules,
	V_rules, W_rules, X_rules, Y_rules, Z_rules
	};
