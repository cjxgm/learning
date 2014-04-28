// vim: noet ts=4 sw=4 sts=0
#include "synth.h"
#include "jack.h"
#include "dsp.h"
#include <math.h>
#include <string.h>


typedef struct Track
{
	struct Track* next;
	size_t len;		// the length of the unplayed
	size_t pos;
	float* buf;
}
Track;

static Track* tracks;
static float* track_new_for_buf(size_t len);
static int track_fill(Track* t, float* buf, size_t nframe);




void synth_add(uint8_t midi_note, float duration, float velocity)
{
	// middle A (69) = 440 Hz
	float freq = 440.0f * powf(2.0f, (midi_note - 69) / 12.0f);
	size_t len = roundf(duration * srate);
	float* buf = track_new_for_buf(len);
	float offset = (rand() & 0xFF) / 255.0f;
	synthesis(buf, len, freq, velocity, offset);
}

void synth_fill(float* out, size_t nframe)
{
	Track* now = tracks;
	Track* old = NULL;
	while (now) {
		Track* future = now->next;
		if (track_fill(now, out, nframe)) {
			if (old) old->next = future;
			else tracks = future;
			free(now);
			now = old;
		}
		old = now;
		now = future;
	}
}




static float* track_new_for_buf(size_t len)
{
	Track* track = malloc(sizeof(Track));

	track->len = len;
	track->buf = calloc(sizeof(*track->buf), len);
	track->pos = 0;

	track->next = tracks;
	tracks = track;

	return track->buf;
}

static int track_fill(Track* t, float* buf, size_t nframe)
{
	size_t nfill = (t->len < nframe ? t->len : nframe);
	float* in = t->buf + t->pos;
	t->pos += nfill;
	t->len -= nfill;
	while (nfill--) *buf++ += *in++;
	return !t->len;
}

