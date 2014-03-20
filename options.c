#include <stdbool.h>

#include "juno.h"
#include "options.h"

/*
 * Juno options
 */

struct juno_options options = {
	.voice = 0,
	.pitch = 0,
	.rate = 0,
	.fd = 1, // stdout
};

/*
 * Get/set options individually.
 */

// XXX argument types may need to be changed
// TODO these methods may need to initialize other variables

// XXX how do we adjust pitch and rate? does each voice have a base pitch and
// base rate that can be relatively adjusted?

bool juno_get_voice(int *voice)
{
	*voice = options.voice;
	return true;
}

bool juno_set_voice(int voice)
{
	if (options.voice == voice) return true;

	options.voice = voice;
	return true;
}

bool juno_get_pitch(int *pitch)
{
	*pitch = options.pitch;
	return true;
}

bool juno_set_pitch(int pitch)
{
	if (options.pitch == pitch) return true;

	options.pitch = pitch;
	return true;
}

bool juno_get_pitch_modulation(int *pitch_modulation)
{
	*pitch_modulation = options.pitch_modulation;
	return true;
}

bool juno_set_pitch_modulation(int pitch_modulation)
{
	if (options.pitch_modulation == pitch_modulation) return true;

	options.pitch_modulation = pitch_modulation;
	return true;
}

bool juno_get_rate(int *rate)
{
	*rate = options.rate;
	return true;
}

bool juno_set_rate(int rate)
{
	if (options.rate == rate) return true;

	options.rate = rate;
	return true;
}

bool juno_get_fd(int *fd)
{
	*fd = options.fd;
	return true;
}

bool juno_set_fd(int fd)
{
	if (options.fd == fd) return true;

	options.fd = fd;
	return true;
}

/*
 * Get/set all options.
 */

bool juno_getoptions(struct juno_options *o)
{
	*o = options;
	return true;
}

bool juno_setoptions(const struct juno_options *new, struct juno_options *old)
{
	bool ret = true;

	if (old) {
		ret &= juno_getoptions(old);
	}

	if (new) {
		ret &= juno_set_voice(new->voice);
		ret &= juno_set_pitch(new->pitch);
		ret &= juno_set_rate(new->rate);
		ret &= juno_set_fd(new->fd);
	}

	return ret;
}
