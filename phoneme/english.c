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
	{Anything,	"AR",		"#",		"Er"	},
	{"^",		"AS",		"#",		"eIs"	},
	{Anything,	"A",		"WA",		"@"	},
	{Anything,	"AW",		Anything,	"O"	},
	{" :",		"ANY",		Anything,	"Eni"	},
	{Anything,	"A",		"^+#",		"eI"	},
	{"#:",		"ALLY",		Anything,	"@li"	},
	{Nothing,	"AL",		"#",		"@l"	},
	{Anything,	"AGAIN",	Anything,	"@gEn"},
	{"#:",		"AG",		"E",		"IdZ"	},
	{Anything,	"A",		"^+:#",		"&"	},
	{" :",		"A",		"^+ ",		"eI"	},
	{Anything,	"A",		"^%",		"eI"	},
	{Nothing,	"ARR",		Anything,	"@r"	},
	{Anything,	"ARR",		Anything,	"&r"	},
	{" :",		"AR",		Nothing,	"AR"	},
	{Anything,	"AR",		Nothing,	"R"	},
	{Anything,	"AR",		Anything,	"Ar"	},
	{Anything,	"AIR",		Anything,	"Er"	},
	{Anything,	"AI",		Anything,	"eI"	},
	{Anything,	"AY",		Anything,	"eI"	},
	{Anything,	"AU",		Anything,	"O"	},
	{"#:",		"AL",		Nothing,	"L"	},
	{"#:",		"ALS",		Nothing,	"Lz"	},
	{Anything,	"ALK",		Anything,	"Ok"	},
	{Anything,	"AL",		"^",		"OL"	},
	{" :",		"ABLE",		Anything,	"eIbL"},
	{Anything,	"ABLE",		Anything,	"@bL"},
	{Anything,	"ANG",		"+",		"eIndZ"	},
	{Anything,	"A",		Anything,	"&"	},
 	{Anything,	0,		Anything,	Silent	},
	};

/*
**	left		match		right		out
*/
static const Rule B_rules[] =
	{
	{Nothing,	"BE",		"^#",		"bI"	},
	{Anything,	"BEING",	Anything,	"biIN"},
	{Nothing,	"BOTH",		Nothing,	"boUT"	},
	{Nothing,	"BUS",		"#",		"bIz"	},
	{Anything,	"BUIL",		Anything,	"bIl"	},
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
	{Anything,	"COM",		"%",		"kVm"	},
	{Anything,	"C",		Anything,	"k"	},
	{Anything,	0,		Anything,	Silent	},
	};

/*
**	left		match		right		out
*/
static const Rule D_rules[] =
	{
	{"#:",		"DED",		Nothing,	"dId"	},
	{".E",		"D",		Nothing,	"d"	},
	{"#:^E",	"D",		Nothing,	"t"	},
	{Nothing,	"DE",		"^#",		"dI"	},
	{Nothing,	"DO",		Nothing,	"du"	},
	{Nothing,	"DOES",		Anything,	"dVz"	},
	{Nothing,	"DOING",	Anything,	"duIN"},
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
	{" :",		"E",		Nothing,	"i"	},
	{"#",		"ED",		Nothing,	"d"	},
	{"DR",		"E",		"D",		"E"	},
	{"#:",		"E",		"D ",		Silent	},
	{Anything,	"EV",		"ER",		"Ev"	},
	{Anything,	"E",		"^%",		"i"	},
	{Anything,	"ERI",		"#",		"iri"	},
	{Anything,	"ERI",		Anything,	"ErI"	},
	{"#:",		"ER",		"#",		"R"	},
	{Anything,	"ER",		"#",		"Er"	},
	{Anything,	"ER",		Anything,	"R"	},
	{Nothing,	"EVEN",		Anything,	"ivEn"},
	{"#:",		"E",		"W",		Silent	},
	{"T",		"EW",		Anything,	"u"	},
	{"S",		"EW",		Anything,	"u"	},
	{"R",		"EW",		Anything,	"u"	},
	{"D",		"EW",		Anything,	"u"	},
	{"L",		"EW",		Anything,	"u"	},
	{"Z",		"EW",		Anything,	"u"	},
	{"N",		"EW",		Anything,	"u"	},
	{"J",		"EW",		Anything,	"u"	},
	{"TH",		"EW",		Anything,	"u"	},
	{"CH",		"EW",		Anything,	"u"	},
	{"SH",		"EW",		Anything,	"u"	},
	{Anything,	"EW",		Anything,	"ju"	},
	{Anything,	"E",		"O",		"i"	},
	{"#:S",		"ES",		Nothing,	"Iz"	},
	{"#:C",		"ES",		Nothing,	"Iz"	},
	{"#:G",		"ES",		Nothing,	"Iz"	},
	{"#:Z",		"ES",		Nothing,	"Iz"	},
	{"#:X",		"ES",		Nothing,	"Iz"	},
	{"#:J",		"ES",		Nothing,	"Iz"	},
	{"#:CH",	"ES",		Nothing,	"Iz"	},
	{"#:SH",	"ES",		Nothing,	"Iz"	},
	{"#:",		"E",		"S ",		Silent	},
	{"#:",		"ELY",		Nothing,	"li"	},
	{"#:",		"EMENT",	Anything,	"mEnt"	},
	{Anything,	"EFUL",		Anything,	"fUL"	},
	{Anything,	"EE",		Anything,	"i"	},
	{Anything,	"EARN",		Anything,	"Rn"	},
	{Nothing,	"EAR",		"^",		"R"	},
	{Anything,	"EAD",		Anything,	"Ed"	},
	{"#:",		"EA",		Nothing,	"i@"	},
	{Anything,	"EA",		"SU",		"E"	},
	{Anything,	"EA",		Anything,	"i"	},
	{Anything,	"EIGH",		Anything,	"eI"	},
	{Anything,	"EI",		Anything,	"i"	},
	{Nothing,	"EYE",		Anything,	"aI"	},
	{Anything,	"EY",		Anything,	"i"	},
	{Anything,	"EU",		Anything,	"ju"	},
	{Anything,	"E",		Anything,	"E"	},
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
	{Anything,	"GIV",		Anything,	"gIv"	},
	{Nothing,	"G",		"I^",		"g"	},
	{Anything,	"GE",		"T",		"gE"	},
	{"SU",		"GGES",		Anything,	"gdZEs"	},
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
	{Nothing,	"HAV",		Anything,	"h&v"	},
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
	{Nothing,	"IN",		Anything,	"In"	},
	{Nothing,	"I",		Nothing,	"aI"	},
	{Anything,	"IN",		"D",		"aIn"	},
	{Anything,	"IER",		Anything,	"iR"	},
	{"#:R",		"IED",		Anything,	"id"	},
	{Anything,	"IED",		Nothing,	"aId"	},
	{Anything,	"IEN",		Anything,	"iEn"	},
	{Anything,	"IE",		"T",		"aIE"	},
	{" :",		"I",		"%",		"aI"	},
	{Anything,	"I",		"%",		"i"	},
	{Anything,	"IE",		Anything,	"i"	},
	{Anything,	"I",		"^+:#",		"I"	},
	{Anything,	"IR",		"#",		"aIr"	},
	{Anything,	"IZ",		"%",		"aIz"	},
	{Anything,	"IS",		"%",		"aIz"	},
	{Anything,	"I",		"D%",		"aI"	},
	{"+^",		"I",		"^+",		"I"	},
	{Anything,	"I",		"T%",		"aI"	},
	{"#:^",		"I",		"^+",		"I"	},
	{Anything,	"I",		"^+",		"aI"	},
	{Anything,	"IR",		Anything,	"R"	},
	{Anything,	"IGH",		Anything,	"aI"	},
	{Anything,	"ILD",		Anything,	"aILd"	},
	{Anything,	"IGN",		Nothing,	"aIn"	},
	{Anything,	"IGN",		"^",		"aIn"	},
	{Anything,	"IGN",		"%",		"aIn"	},
	{Anything,	"IQUE",		Anything,	"ik"	},
	{Anything,	"I",		Anything,	"I"	},
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
	{Anything,	"LEAD",		Anything,	"lid"	},
	{Anything,	"L",		Anything,	"l"	},
	{Anything,	0,		Anything,	Silent	},
	};

/*
**	left		match		right		out
*/
static const Rule M_rules[] =
	{
	{Anything,	"MOV",		Anything,	"muv"	},
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
	{Anything,	"OROUGH",	Anything,	"RoU"	},
	{"#:",		"OR",		Nothing,	"R"	},
	{"#:",		"ORS",		Nothing,	"Rz"	},
	{Anything,	"OR",		Anything,	"Or"	},
	{Nothing,	"ONE",		Anything,	"wVn"	},
	{Anything,	"OW",		Anything,	"oU"	},
	{Nothing,	"OVER",		Anything,	"oUvR"	},
	{Anything,	"OV",		Anything,	"Vv"	},
	{Anything,	"O",		"^%",		"oU"	},
	{Anything,	"O",		"^EN",		"oU"	},
	{Anything,	"O",		"^I#",		"oU"	},
	{Anything,	"OL",		"D",		"oUL"	},
	{Anything,	"OUGHT",	Anything,	"Ot"	},
	{Anything,	"OUGH",		Anything,	"Vf"	},
	{Nothing,	"OU",		Anything,	"aU"	},
	{"H",		"OU",		"S#",		"aU"	},
	{Anything,	"OUS",		Anything,	"@s"	},
	{Anything,	"OUR",		Anything,	"OR"	},
	{Anything,	"OULD",		Anything,	"Ud"	},
	{"^",		"OU",		"^L",		"V"	},
	{Anything,	"OUP",		Anything,	"up"	},
	{Anything,	"OU",		Anything,	"aU"	},
	{Anything,	"OY",		Anything,	"oI"	},
	{Anything,	"OING",		Anything,	"oUIN"},
	{Anything,	"OI",		Anything,	"oI"	},
	{Anything,	"OOR",		Anything,	"OR"	},
	{Anything,	"OOK",		Anything,	"Uk"	},
	{Anything,	"OOD",		Anything,	"Ud"	},
	{Anything,	"OO",		Anything,	"u"	},
	{Anything,	"O",		"E",		"oU"	},
	{Anything,	"O",		Nothing,	"oU"	},
	{Anything,	"OA",		Anything,	"oU"	},
	{Nothing,	"ONLY",		Anything,	"oUnli"},
	{Nothing,	"ONCE",		Anything,	"wVns"	},
	{Anything,	"ON'T",		Anything,	"oUnt"	},
	{"C",		"O",		"N",		"A"	},
	{Anything,	"O",		"NG",		"O"	},
	{" :^",		"O",		"N",		"V"	},
	{"I",		"ON",		Anything,	"@n"	},
	{"#:",		"ON",		Nothing,	"@n"	},
	{"#^",		"ON",		Anything,	"@n"	},
	{Anything,	"O",		"ST ",		"oU"	},
	{Anything,	"OF",		"^",		"Of"	},
	{Anything,	"OTHER",	Anything,	"VDR"},
	{Anything,	"OSS",		Nothing,	"Os"	},
	{"#:^",		"OM",		Anything,	"Vm"	},
	{Anything,	"O",		Anything,	"A"	},
	{Anything,	0,		Anything,	Silent	},
	};

/*
**	left		match		right		out
*/
static const Rule P_rules[] =
	{
	{Anything,	"PH",		Anything,	"f"	},
	{Anything,	"PEOP",		Anything,	"pip"	},
	{Anything,	"POW",		Anything,	"paU"	},
	{Anything,	"PUT",		Nothing,	"pUt"	},
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
	{Nothing,	"RE",		"^#",		"ri"	},
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
	{Anything,	"SOME",		Anything,	"sVm"	},
	{"#",		"SUR",		"#",		"ZR"	},
	{Anything,	"SUR",		"#",		"SR"	},
	{"#",		"SU",		"#",		"Zu"	},
	{"#",		"SSU",		"#",		"Su"	},
	{"#",		"SED",		Nothing,	"zd"	},
	{"#",		"S",		"#",		"z"	},
	{Anything,	"SAID",		Anything,	"sEd"	},
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
	{"#",		"SM",		Anything,	"zm"	},
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
	{Anything,	"THAT",		Nothing,	"D&t"	},
	{Nothing,	"THIS",		Nothing,	"DIs"	},
	{Nothing,	"THEY",		Anything,	"DeI"	},
	{Nothing,	"THERE",	Anything,	"DER"	},
	{Anything,	"THER",		Anything,	"DR"	},
	{Anything,	"THEIR",	Anything,	"DER"	},
	{Nothing,	"THAN",		Nothing,	"D&n"	},
	{Nothing,	"THEM",		Nothing,	"DEm"	},
	{Anything,	"THESE",	Nothing,	"Diz"	},
	{Nothing,	"THEN",		Anything,	"DEn"	},
	{Anything,	"THROUGH",	Anything,	"Tru"	},
	{Anything,	"THOSE",	Anything,	"DoUz"	},
	{Anything,	"THOUGH",	Nothing,	"DoU"	},
	{Nothing,	"THUS",		Anything,	"DVs"	},
	{Anything,	"TH",		Anything,	"T"	},
	{"#:",		"TED",		Nothing,	"tId"	},
	{"S",		"TI",		"#N",		"tS"	},
	{Anything,	"TI",		"O",		"S"	},
	{Anything,	"TI",		"A",		"S"	},
	{Anything,	"TIEN",		Anything,	"S@n"	},
	{Anything,	"TUR",		"#",		"tSR"	},
	{Anything,	"TU",		"A",		"tSu"	},
	{Nothing,	"TWO",		Anything,	"tu"	},
	{Anything,	"T",		Anything,	"t"	},
	{Anything,	0,		Anything,	Silent	},
	};

/*
**	left		match		right		out
*/
static const Rule U_rules[] =
	{
	{Nothing,	"UN",		"I",		"jun"	},
	{Nothing,	"UN",		Anything,	"Vn"	},
	{Nothing,	"UPON",		Anything,	"@pOn"},
	{"T",		"UR",		"#",		"Ur"	},
	{"S",		"UR",		"#",		"Ur"	},
	{"R",		"UR",		"#",		"Ur"	},
	{"D",		"UR",		"#",		"Ur"	},
	{"L",		"UR",		"#",		"Ur"	},
	{"Z",		"UR",		"#",		"Ur"	},
	{"N",		"UR",		"#",		"Ur"	},
	{"J",		"UR",		"#",		"Ur"	},
	{"TH",		"UR",		"#",		"Ur"	},
	{"CH",		"UR",		"#",		"Ur"	},
	{"SH",		"UR",		"#",		"Ur"	},
	{Anything,	"UR",		"#",		"jUr"	},
	{Anything,	"UR",		Anything,	"R"	},
	{Anything,	"U",		"^ ",		"V"	},
	{Anything,	"U",		"^^",		"V"	},
	{Anything,	"UY",		Anything,	"aI"	},
	{" G",		"U",		"#",		Silent	},
	{"G",		"U",		"%",		Silent	},
	{"G",		"U",		"#",		"w"	},
	{"#N",		"U",		Anything,	"ju"	},
	{"T",		"U",		Anything,	"u"	},
	{"S",		"U",		Anything,	"u"	},
	{"R",		"U",		Anything,	"u"	},
	{"D",		"U",		Anything,	"u"	},
	{"L",		"U",		Anything,	"u"	},
	{"Z",		"U",		Anything,	"u"	},
	{"N",		"U",		Anything,	"u"	},
	{"J",		"U",		Anything,	"u"	},
	{"TH",		"U",		Anything,	"u"	},
	{"CH",		"U",		Anything,	"u"	},
	{"SH",		"U",		Anything,	"u"	},
	{Anything,	"U",		Anything,	"ju"	},
	{Anything,	0,		Anything,	Silent	},
	};

/*
**	left		match		right		out
*/
static const Rule V_rules[] =
	{
	{Anything,	"VIEW",		Anything,	"vju"	},
	{Anything,	"V",		Anything,	"v"	},
	{Anything,	0,		Anything,	Silent	},
	};

/*
**	left		match		right		out
*/
static const Rule W_rules[] =
	{
	{Nothing,	"WERE",		Anything,	"wR"	},
	{Anything,	"WA",		"S",		"wA"	},
	{Anything,	"WA",		"T",		"wA"	},
	{Anything,	"WHERE",	Anything,	"hwER"	},
	{Anything,	"WHAT",		Anything,	"hwAt"	},
	{Anything,	"WHOL",		Anything,	"hoUL"	},
	{Anything,	"WHO",		Anything,	"hu"	},
	{Anything,	"WH",		Anything,	"hw"	},
	{Anything,	"WAR",		Anything,	"wOR"	},
	{Anything,	"WOR",		"^",		"wR"	},
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
	{Anything,	"YOUNG",	Anything,	"jVN"	},
	{Nothing,	"YOU",		Anything,	"ju"	},
	{Nothing,	"YES",		Anything,	"jEs"	},
	{Nothing,	"Y",		Anything,	"j"	},
	{"#:^",		"Y",		Nothing,	"i"	},
	{"#:^",		"Y",		"I",		"i"	},
	{" :",		"Y",		Nothing,	"aI"	},
	{" :",		"Y",		"#",		"aI"	},
	{" :",		"Y",		"^+:#",		"I"	},
	{" :",		"Y",		"^#",		"aI"	},
	{Anything,	"Y",		Anything,	"I"	},
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
