#ifndef _PHONEMES_H_
#define _PHONEMES_H_

#include <stdint.h>
#include <stdbool.h>

#include "formant.h"

#define N_PHONEME P_LAST //40

enum {
	// used for transitions to/from word boundaries
	P_none,

	// vowels
	P_j,          P_w,
	P_i,          P_u,
	   P_I,    P_U,
	P_e,          P_o,
	              P_V,
	     P_SCHWA,
	     P_R,
	P_E,          P_O,
	P_ae,
	P_a,          P_A,

	// liquids (treated like vowels)
	P_l,
	P_r, // how is this different from P_R?

	// nasals
	P_m,
	P_n,
	P_N,

	// stops
	P_b, P_p,
	P_d, P_t,
	P_g, P_k,

	// fricatives
	P_f, P_v,
	P_s, P_z,
	P_S, P_Z,
	P_T, P_D,

	// aspirated
	P_h,

	P_LAST,
};

typedef enum {
	PHONEME_TYPE_SILENCE, // silent
	PHONEME_TYPE_PLOSIVE,
	PHONEME_TYPE_VPLOSIVE, // voiced plosive
	PHONEME_TYPE_FRICATIVE,
	PHONEME_TYPE_ASPIRATED,

	PHONEME_TYPE_VOWEL,
	PHONEME_TYPE_SEMIVOWEL,
	PHONEME_TYPE_NASAL,
	PHONEME_TYPE_LIQUID,

	PHONEME_TYPE_COUNT
} PhonemeType;

typedef enum {
	BUZZ_VOWEL,
	BUZZ_LIQUID,
	BUZZ_PLOSIVE,
	BUZZ_NASAL,
} BuzzType;

typedef enum {
	SOURCE_SILENCE,
	SOURCE_ASPIRATION,
	SOURCE_FRICATION,
	SOURCE_BUZZ,
	SOURCE_LAST,
} SoundSource;

typedef struct {
	// following are booleans:
	uint8_t glide;
	uint8_t obstruent; // this affects surrounding vowels

	uint8_t buzz; // BuzzType
	uint8_t presrc; // SoundSource
	uint8_t nsrc; // SoundSource
	uint8_t postsrc; // SoundSource

	short ndur; // nucleus duration
	short tdur; // transition duration
} PhonemeFlags;

extern const PhonemeFlags phonemeFlags[PHONEME_TYPE_COUNT];

// PhonemeType should be converted to a set of flags to indicate how the
// phoneme should be generated (obstruent, sonorant, etc):
// stop       obstruent, plosive (unvoiced stops only)
// nasal      obstruent, voicing
// vowel      voicing
// aspirated  aspiration
// fricative  obstruent, frication, voicing (voiced fricatives only)
//
// obstruent flag means that formant locus frequencies are used rather than the
// regular formant frequencies during transitions.
// sonorant flag means that the phoneme is voiced during its nucleus
// frication flag means that the phoneme uses a fricative noise source
// aspirated flag means that the phoneme uses an aspirative noise source
// (basically the same as fricative) and its frequencies depend on the next
// phoneme.
// closure flag indicates a small period of silence after the formant
// transition from the previous phoneme and before the release burst.
//
// these flags should be categorized:
// - sound source: none, plosive, frication, aspiration, voicing (only voicing
//   is formant-based)
// - formant transition type: obstruent (this has 3 types)
//
// generating a diphoneme from these flags:
//
// if current phoneme is obstruent
// 	get start target formants (depending on place of articulation)
// 	if current phoneme is plosive
// 		adjust the start target formants to account for closure
// else
// 	get start formants from current phoneme's formants
//
// if next phoneme is obstruent
// 	get end target formants (depending on place of articulation)
// 	if next phoneme is plosive
// 		adjust end target formants to account for closure
// else
// 	get end formants from next phoneme's formants
//
// TODO
//
// general layout of phoneme-to-phoneme transition:
// - end of current phoneme nucleus
// - transition from current phoneme to next phone
// - start of next phoneme nucleus
//
// (these assume aspiration has its own formant frequencies like a schwa)
//
// end of current phoneme nucleus:
// plosive: release burst
// voicing: short steady voicing of current phone
// frication: short steady frication of current phone
// aspiration: short steady aspiration of current phone
//
// transition:
// -- get target frequencies
// if current phoneme is an obstruent
// 	set outer start frequencies to obstruent target frequencies
// else
// 	set outer start frequencies to phoneme frequencies
// if next phoneme is an obstruent
// 	set outer end frequencies to obstruent target frequencies
// else
// 	set outer end frequencies to phoneme frequencies
//
// -- glide the frequencies only if neither phoneme is fricative
// if either phoneme is fricative
// 	set inner end to outer start
// 	set inner start to outer end
// else
// 	compute inner end and start frequencies based on duration of current
// 	  and next phonemes
//
// (the start and end frequencies are chosen based on whether each phoneme is
// an obstruent and on which type of obstruent)
// (the sound source basically switches to the next phoneme's sound source
// somewhere in the middle of the transition; the durations of each segment on
// each side of the change need to be computed based on the length of the two
// phonemes)
//
// (voiced fricatives are considered to be frication when determining frequency
// transitions):
// - none => none: silence
// - none => plosive: silence
// - none => voicing: silence
// - none => frication: silence
// - none => aspiration: silence
// - plosive => none: aspiration (or silence)
// - plosive => plosive: aspiration (or silence), silence, glide
// - plosive => voicing: aspiration (or silence), voicing, glide
// - plosive => frication: steady aspiration (or silence), frication
// - plosive => aspiration: aspiration (or silence), aspiration, glide
// - voicing => none: silence
// - voicing => plosive: voicing, silence, glide
// - voicing => voicing: voicing, voicing, glide
// - voicing => frication: steady voicing, steady frication
// - voicing => aspiration: voiced, aspirated, glide
// - frication => none: silence
// - frication => plosive: steady frication, silence
// - frication => voicing: steady voicing, steady frication
// - frication => frication: steady frication, steady frication
// - frication => aspiration: steady frication, steady aspiration
// - aspiration => none: silence
// - aspiration => plosive: aspiration, silence, glide
// - aspiration => voicing: aspirated, voiced, glide
// - aspiration => frication: steady aspiration, steady frication
// - aspiration => aspiration: aspirated, aspirated, glide
//
// NB. all transitions glide (linear interpolate) the frequencies between the
// two endpoints, unless frication is on either side
// This can probably be simplified by computing two pairs of frequency
// endpoints, the first pair for the current phoneme and the second for the
// next, and then handling each segment independently. First compute the two
// outer endpoints, then if the current phoneme is frication, set the inner
// start to the outer end; if the next phoneme is frication, set the inner end
// to the outer start.
//
// start of next phoneme nucleus:
// plosive: nothing
// voicing: short steady voicing of next phone
// frication: short steady frication of next phone
// aspiration: short steady aspiration of next phone
//
// XXX phoneme nuclei might not be needed (except for the plosive cases, which
// can be moved to the transition stage)
//
// XXX all durations are elastic except for plosive closure and release times


// these are obstruent targets
// XXX are these valid for all such obstruents? ie, do b, p, and m have the
// same target frequencies for any given vowel?
typedef enum {
	OBS_LABIAL,   // b, p, m
	OBS_ALVEOLAR, // d, t, n
	OBS_VELAR,    // g, k, N
	
	OBS_LAST,
} ObsType;
#define N_OBS OBS_LAST

typedef struct {
	PhonemeType type;

	FreqSet obstargets[N_OBS];
	// XXX we might need another set of FreqSet for each obstruent burst
	// frequency, especially for /k/ (which has a variable burst frequency
	// depending on the following vowel's F2)
	FreqSet f;
	ObsType obstype;
	// There should also be a field here to indicate which waveform data
	// table to use for this phoneme. This would be the "voice bar"
	// waveform only.
} Phoneme;

extern const Phoneme *const phonemes[];

#endif
