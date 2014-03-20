#include <stdbool.h>

#include "juno.h"
#include "phonemes.h"
#include "voice.h"

// XXX we should have a routine that returns a complete "phoneme" object, including all the flags.
// The flags are split out from each phoneme to reduce memory requirements
// because they are the same for all phonemes of each type, but the code above
// us doesn't have to know that.

const PhonemeFlags phonemeFlags[PHONEME_TYPE_COUNT] PROGMEM = {
	[PHONEME_TYPE_SILENCE] = {
		//.glide = false,
		//.obstruent = false,
		.presrc = SOURCE_SILENCE,
		.nsrc = SOURCE_SILENCE,
		.postsrc = SOURCE_SILENCE,
		.ndur = D(40), // estimate (somewhat fine-tuned)
		.tdur = D(0),
	}, // silent
	[PHONEME_TYPE_PLOSIVE] = {
		.glide = true,
		.obstruent = true,
		.presrc = SOURCE_SILENCE,
		.nsrc = SOURCE_FRICATION,
		.postsrc = SOURCE_ASPIRATION,
		.ndur = D(30), // estimate (somewhat fine-tuned)
		.tdur = D(30), // estimate (somewhat fine-tuned)
	},
	[PHONEME_TYPE_VPLOSIVE] = {
		.glide = true,
		.obstruent = true,
		.presrc = SOURCE_BUZZ,
		.nsrc = SOURCE_SILENCE,
		.postsrc = SOURCE_BUZZ,
		.buzz = BUZZ_PLOSIVE,
		.ndur = D(30), // estimate (somewhat fine-tuned)
		.tdur = D(20), // estimate (somewhat fine-tuned)
	}, // voiced plosive
	[PHONEME_TYPE_FRICATIVE] = {
		//.glide = false,
		//.obstruent = false,
		.presrc = SOURCE_SILENCE,
		.nsrc = SOURCE_FRICATION,
		.postsrc = SOURCE_SILENCE,
		.ndur = D(110), // estimate (somewhat fine-tuned)
		.tdur = D(0),
	},
	[PHONEME_TYPE_ASPIRATED] = {
		//.glide = false,
		//.obstruent = false,
		.presrc = SOURCE_ASPIRATION,
		.nsrc = SOURCE_ASPIRATION,
		.postsrc = SOURCE_ASPIRATION,
		.ndur = D(0), // estimate
		.tdur = D(80), // estimate
	},
	[PHONEME_TYPE_VOWEL] = {
		.glide = true,
		//.obstruent = false,
		.presrc = SOURCE_BUZZ,
		.nsrc = SOURCE_BUZZ,
		.postsrc = SOURCE_BUZZ,
		.buzz = BUZZ_VOWEL,
		.ndur = D(50), // estimate (somewhat fine-tuned)
		.tdur = D(30), // estimate (somewhat fine-tuned)
	},
	[PHONEME_TYPE_SEMIVOWEL] = {
		.glide = true,
		//.obstruent = false,
		.presrc = SOURCE_BUZZ,
		.nsrc = SOURCE_BUZZ,
		.postsrc = SOURCE_BUZZ,
		.buzz = BUZZ_VOWEL,
		.ndur = D(30), // guessed
		.tdur = D(50), // guessed
	},
	[PHONEME_TYPE_LIQUID] = {
		.glide = true,
		//.obstruent = false,
		.presrc = SOURCE_BUZZ,
		.nsrc = SOURCE_BUZZ,
		.postsrc = SOURCE_BUZZ,
		.buzz = BUZZ_LIQUID,
		.ndur = D(100), // estimate
		.tdur = D(25), // estimate
	},
	[PHONEME_TYPE_NASAL] = {
		.glide = true,
		.obstruent = true,
		.presrc = SOURCE_BUZZ,
		.nsrc = SOURCE_BUZZ,
		.postsrc = SOURCE_BUZZ,
		.buzz = BUZZ_NASAL,
		.ndur = D(60), // estimate (somewhat fine-tuned)
		.tdur = D(30), // estimate (somewhat fine-tuned)
	},
};

const Voice voiceBob PROGMEM = {
	.pitch = F(130), // XXX estimate
	.rate = 120, // XXX estimate
	// TODO put buzz waveforms here?

	.phonemes = {
[P_j] = {
	.type = PHONEME_TYPE_SEMIVOWEL,
	.f = { .f = { F(140), F(235), F(2100), F(3000) }, }, // estimated values
	.obstargets = {
		[OBS_LABIAL]   = { .f = { F(140), F(180), F( 600),F(3000) }, },
		[OBS_ALVEOLAR] = { .f = { F(140), F(180), F(1700),F(3000) }, },
		[OBS_VELAR]    = { .f = { F(140), F(180), F(3500),F(3500) }, },
		[OBS_VELAR]    = { .f = { F(139), F(180), F(2700),F(3000) }, },
	},
},
[P_i] = {
	.type = PHONEME_TYPE_VOWEL,
	.f = { .f = { F(139), F(343), F(2323), F(3000) }, },
	.obstargets = {
		[OBS_LABIAL]   = { .f = { F(139), F(180), F( 600),F(3000) }, },
		[OBS_ALVEOLAR] = { .f = { F(139), F(180), F(1700),F(3000) }, },
		[OBS_VELAR]    = { .f = { F(139), F(180), F(3500),F(3800) }, },
		[OBS_VELAR]    = { .f = { F(139), F(180), F(2700),F(3000) }, },
	},
},
[P_I] = {
	.type = PHONEME_TYPE_VOWEL,
	.f = { .f = { F(136), F(429), F(2034), F(2687) }, },
	.obstargets = {
		[OBS_LABIAL]   = { .f = { F(136), F(180), F( 600),F(2687) }, },
		[OBS_ALVEOLAR] = { .f = { F(136), F(180), F(1700),F(2687) }, },
		[OBS_VELAR]    = { .f = { F(136), F(180), F(3500),F(3800) }, },
		[OBS_VELAR]    = { .f = { F(136), F(180), F(2600),F(2900) }, },
	},
},
[P_e] = {
	.type = PHONEME_TYPE_VOWEL,
	// XXX should F2 be lower than F2 of I?
	.f = { .f = { F(129), F(476), F(2090), F(2692) }, },
	.obstargets = {
		[OBS_LABIAL]   = { .f = { F(129), F(180), F( 600),F(2692) }, },
		[OBS_ALVEOLAR] = { .f = { F(129), F(180), F(1700),F(2692) }, },
		[OBS_VELAR]    = { .f = { F(129), F(180), F(3500),F(3800) }, },
		[OBS_VELAR]    = { .f = { F(129), F(180), F(2600),F(2900) }, },
	},
},

// i I e E a A O V o U u

[P_E] = {
	.type = PHONEME_TYPE_VOWEL,
	.f = { .f = { F(127), F(588), F(1803), F(2604) }, },
	.obstargets = {
		[OBS_LABIAL]   = { .f = { F(127), F(180), F( 600),F(2604) }, },
		[OBS_ALVEOLAR] = { .f = { F(127), F(180), F(1700),F(2604) }, },
		[OBS_VELAR]    = { .f = { F(127), F(180), F(3500),F(3800) }, },
		[OBS_VELAR]    = { .f = { F(127), F(180), F(2600),F(2900) }, },
	},
},
[P_ae] = {
	.type = PHONEME_TYPE_VOWEL,
	.f = { .f = { F(126), F(591), F(1930), F(2595) }, },
	.f = { .f = { F(126), F(600), F(2050), F(2595) }, }, // experiments
	.obstargets = {
		[OBS_LABIAL]   = { .f = { F(126), F(180), F( 600),F(2595) }, },
		[OBS_ALVEOLAR] = { .f = { F(126), F(180), F(1700),F(2595) }, },
		[OBS_VELAR]    = { .f = { F(126), F(180), F(3500),F(3800) }, },
		[OBS_VELAR]    = { .f = { F(126), F(180), F(2600),F(2900) }, },
	},
},
[P_a] = {
	.type = PHONEME_TYPE_VOWEL,
	// XXX should F2 be lower than F2 of E?
	.f = { .f = { F(128), F(620), F(1900), F(2600) }, }, // estimated values
	.f = { .f = { F(128), F(700), F(1800), F(2600) }, }, // experiments
	.obstargets = {
		[OBS_LABIAL]   = { .f = { F(128), F(50), F( 600), F(2600) }, },
		[OBS_ALVEOLAR] = { .f = { F(128), F(50), F(1700), F(2600) }, },
		[OBS_VELAR]    = { .f = { F(128), F(50), F(3500), F(3800) }, },
		[OBS_VELAR]    = { .f = { F(128), F(50), F(2500), F(2800) }, },
	},
},

// F1 peaks here
[P_A] = {
	.type = PHONEME_TYPE_VOWEL,
	.f = { .f = { F(127), F(756), F(1309), F(2535) }, },
	.obstargets = {
		[OBS_LABIAL]   = { .f = { F(125),F(180),F( 600),F(2535) }, },
		[OBS_ALVEOLAR] = { .f = { F(125),F(180),F(1700),F(2535) }, },
		[OBS_VELAR]    = { .f = { F(125),F(180),F(3500),F(3800) }, },
		[OBS_VELAR]    = { .f = { F(125),F(180),F(2300),F(2600) }, },
	},
},

// i I e E a A O V o U u

[P_O] = {
	.type = PHONEME_TYPE_VOWEL,
	.f = { .f = { F(125), F(656), F(1023), F(2521) }, },
	.obstargets = {
		[OBS_LABIAL]   = { .f = { F(125), F(180), F( 600),F(2521) }, },
		[OBS_ALVEOLAR] = { .f = { F(125), F(180), F(1700),F(2521) }, },
		[OBS_VELAR]    = { .f = { F(125), F(180), F(1500),F(1800) }, },
		[OBS_VELAR]    = { .f = { F(125), F(180), F(1500),F(1800) }, },
	},
},
[P_V] = {
	.type = PHONEME_TYPE_VOWEL,
	.f = { .f = { F(129), F(621), F(1191), F(2548) }, },
	.obstargets = {
		[OBS_LABIAL]   = { .f = { F(129), F(180), F( 600),F(2548) }, },
		[OBS_ALVEOLAR] = { .f = { F(129), F(180), F(1700),F(2548) }, },
		[OBS_VELAR]    = { .f = { F(129), F(180), F(1500),F(1800) }, },
	},
},
[P_o] = {
	.type = PHONEME_TYPE_VOWEL,
	.f = { .f = { F(130), F(498), F(910), F(2459) }, },
	.obstargets = {
		[OBS_LABIAL]   = { .f = { F(130), F(180), F( 600),F(2459) }, },
		[OBS_ALVEOLAR] = { .f = { F(130), F(180), F(1700),F(2459) }, },
		[OBS_VELAR]    = { .f = { F(130), F(180), F(1500),F(1800) }, },
	},
},
[P_U] = {
	.type = PHONEME_TYPE_VOWEL,
	.f = { .f = { F(133), F(469), F(1123), F(2435) }, },
	.obstargets = {
		[OBS_LABIAL]   = { .f = { F(133), F(180), F( 600),F(2435) }, },
		[OBS_ALVEOLAR] = { .f = { F(133), F(180), F(1700),F(2435) }, },
		[OBS_VELAR]    = { .f = { F(133), F(180), F(1500),F(1800) }, },
	},
},

// i I e E a A O V o U u

[P_u] = {
	.type = PHONEME_TYPE_VOWEL,
	.f = { .f = { F(130), F(380), F(992), F(2355) }, },
	.obstargets = {
		[OBS_LABIAL]   = { .f = { F(130), F(180), F( 600),F(2355) }, },
		[OBS_ALVEOLAR] = { .f = { F(130), F(180), F(1700),F(2355) }, },
		[OBS_VELAR]    = { .f = { F(130), F(180), F(1500),F(1800) }, },
	},
},

// /w/ is like /u/ but shorter in duration
[P_w] = {
	.type = PHONEME_TYPE_SEMIVOWEL,
	.f = { .f = { F(130), F(380), F(992), F(2355) }, }, // not actual values
	.f = { .f = { F(130), F(280), F(792), F(2155) }, }, // estimated
	.obstargets = {
		[OBS_LABIAL]   = { .f = { F(130), F(180), F( 600),F(2355) }, },
		[OBS_ALVEOLAR] = { .f = { F(130), F(180), F(1700),F(2355) }, },
		[OBS_VELAR]    = { .f = { F(130), F(180), F(1500),F(1800) }, },
	},
},
[P_SCHWA] = {
	.type = PHONEME_TYPE_VOWEL,
	.f = { .f = { F(130), F(500), F(1500), F(2500) }, }, // not actual values
	.obstargets = {
		[OBS_LABIAL]   = { .f = { F(130), F(180), F( 600),F(2500) }, },
		[OBS_ALVEOLAR] = { .f = { F(130), F(180), F(1700),F(2500) }, },
		[OBS_VELAR]    = { .f = { F(130), F(180), F(1500),F(1800) }, },
	},
},
[P_R] = {
	.type = PHONEME_TYPE_VOWEL,
	.f = { .f = { F(130), F(300), F(1500), F(1700) }, }, // not actual values
	.obstargets = {
		[OBS_LABIAL]   = { .f = { F(130), F(180), F( 600),F(2355) }, },
		[OBS_ALVEOLAR] = { .f = { F(130), F(180), F(1700),F(2355) }, },
		[OBS_VELAR]    = { .f = { F(130), F(180), F(1500),F(1500) }, },
	},
},
[P_r] = {
	.type = PHONEME_TYPE_LIQUID,
	.f = { .f = { F(130), F(300), F(1100), F(1900) }, }, // not actual values
}, // how is this different from p_R?

// liquids (treated like vowels)
[P_l] = {
	.type = PHONEME_TYPE_LIQUID,
	.f = { .f = { F(130), F(300), F(1500), F(2800) }, }, // estimated values
	.obstargets = {
		[OBS_LABIAL]   = { .f = { F(130), F(50), F( 600), F(2800) }, },
		[OBS_ALVEOLAR] = { .f = { F(130), F(50), F(1700), F(2800) }, },
		[OBS_VELAR]    = { .f = { F(130), F(50), F(3500), F(3500) }, },
		[OBS_VELAR]    = { .f = { F(130), F(50), F(2100), F(2100) }, },
	},
},

/*
Nasal    F2
/m/    1000
/n/    2000
/N/    3000

Nasal frequencies:
Nasal   F1    F2    F3
/m/    250  1000  2000
/n/    250  2000  2700
/N/    250  2300  ???? (what is F3?)
*/

// nasals
[P_m] = {
	.type = PHONEME_TYPE_NASAL,
	.f = { .f = { F(130), F(180), F(1000), F(2000) }, },
	.obstype = OBS_LABIAL,
	/*
	.target = {
		130), F(180), F(1000), F(2000
	},
	*/
},
[P_n] = {
	.type = PHONEME_TYPE_NASAL,
	.f = { .f = { F(130), F(180), F(1700), F(2700) }, },
	.obstype = OBS_ALVEOLAR,
	/*
	.target = {
		130), F(180), F(1700), F(2700
	},
	*/
},
[P_N] = {
	.type = PHONEME_TYPE_NASAL,
	.f = { .f = { F(130), F(180), F(2300), F(0) }, },
	.obstype = OBS_VELAR,
	/*
	.target = {
		130), F(180), F(3000), F(0
	},
	*/
},

// plosives
// TODO re-read the pi-ka-pu experiment and other papers on the subject to
// derive suitable plosive frication frequencies
// XXX are these frequencies used for the frication?
[P_b] = {
	.type = PHONEME_TYPE_VPLOSIVE,
	.f = { .f = { F(100), F(300), F(500), F(900) }, }, // not actual values
	.obstype = OBS_LABIAL,
},
[P_d] = {
	.type = PHONEME_TYPE_VPLOSIVE,
	.f = { .f = { F(100), F(300), F(500), F(900) }, }, // not actual values
	.obstype = OBS_ALVEOLAR,
},
[P_g] = {
	.type = PHONEME_TYPE_VPLOSIVE,
	.f = { .f = { F(100), F(300), F(500), F(900) }, }, // not actual values
	.obstype = OBS_VELAR,
},
[P_p] = {
	.type = PHONEME_TYPE_PLOSIVE,
	.f = { .f = { F(100), F(300), F(500), F(900) }, }, // not actual values
	.f = { .f = {   F(0), F(500), F(500), F(500) }, }, // estimates based on pi-ka-pu
	.obstype = OBS_LABIAL,
},
[P_t] = {
	.type = PHONEME_TYPE_PLOSIVE,
	.f = { .f = { F(100),  F(300),  F(500),  F(900) }, }, // not actual values
	.f = { .f = {   F(0), F(4000), F(4000), F(4000) }, }, // estimates based on pi-ka-pu
	.obstype = OBS_ALVEOLAR,
},
[P_k] = {
	.type = PHONEME_TYPE_PLOSIVE,
	.f = { .f = { F(100),  F(300),  F(500),  F(900) }, }, // not actual values
	.f = { .f = {   F(0), F(1200), F(1200), F(1200) }, }, // estimates based on pi-ka-pu
	.obstype = OBS_VELAR,
},

// fricatives
[P_f] = {
	.type = PHONEME_TYPE_FRICATIVE,
	.f = { .f = { F(0), F(100),  F(100), F(100) }, }, // estimates
	.f = { .f = { F(0), F(850), F(1200),   F(0) }, }, // estimates (somewhat fine-tuned)
},
[P_v] = {
	.type = PHONEME_TYPE_FRICATIVE,
	.f = { .f = { F(120),   F(0), F(1050), F(100) }, }, // estimates
	.f = { .f = { F(120), F(850), F(1200),   F(0) }, }, // estimates (somewhat fine-tuned)
},
[P_s] = {
	.type = PHONEME_TYPE_FRICATIVE,
	.f = { .f = { F(0), F(4000), F(6000), F(7000) }, },

},
[P_z] = {
	.type = PHONEME_TYPE_FRICATIVE,
	.f = { .f = { F(120), F(4000), F(6000), F(7000) }, },
},
[P_S] = {
	.type = PHONEME_TYPE_FRICATIVE,
	.f = { .f = { F(0), F(2500), F(4800), F(6400) }, },
},
[P_Z] = {
	.type = PHONEME_TYPE_FRICATIVE,
	.f = { .f = { F(120), F(2500), F(4800), F(6400) }, },
},
[P_T] = {
	.type = PHONEME_TYPE_FRICATIVE,
	.f = { .f = { F(0), F(5500), F(0), F(0) }, }, // estimate
},
[P_D] = {
	.type = PHONEME_TYPE_FRICATIVE,
	.f = { .f = { F(120), F(5500), F(0), F(0) }, }, // estimate
},

[P_none] = {
	.type = PHONEME_TYPE_SILENCE,
},

// aspirated
[P_h] = {
	.type = PHONEME_TYPE_ASPIRATED,
},

//		[P_none] = &p_none,
//		[P_j] = &p_j,
//		[P_w] = &p_w,
//		[P_i] = &p_i,
//		[P_u] = &p_u,
//		[P_I] = &p_I,
//		[P_U] = &p_U,
//		[P_e] = &p_e,
//		[P_o] = &p_o,
//		[P_V] = &p_V,
//		[P_SCHWA] = &p_SCHWA,
//		[P_R] = &p_R,
//		[P_E] = &p_E,
//		[P_O] = &p_O,
//		[P_ae] = &p_ae,
//		[P_a] = &p_a,
//		[P_A] = &p_A,
//
//		// liquids
//		[P_l] = &p_l,
//		[P_r] = &p_r, // how is this different from P_R?
//
//		// nasals
//		[P_m] = &p_m,
//		[P_n] = &p_n,
//		[P_N] = &p_N,
//
//		// stops
//		[P_b] = &p_b,
//		[P_p] = &p_p,
//		[P_d] = &p_d,
//		[P_t] = &p_t,
//		[P_g] = &p_g,
//		[P_k] = &p_k,
//
//		// fricatives
//		[P_f] = &p_f,
//		[P_v] = &p_v,
//		[P_s] = &p_s,
//		[P_z] = &p_z,
//		[P_S] = &p_S,
//		[P_Z] = &p_Z,
//		[P_T] = &p_T,
//		[P_D] = &p_D,
//
//		// aspirated
//		[P_h] = &p_h,
	},
};
/*
 * F2:
 * bi be ba bo bu: ~600 ?
 * di de da do du: ~1800
 * gi ge ga: ~3500 ?
 * go gu: ~1500 ?
 */

// i I e E a A O V o U u



