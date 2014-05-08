// vim: noet ts=4 sw=4 sts=0
#include "dsp.h"
#include "jack.h"
#include <stdio.h>

static float volume = 0.98f;
static float alpha = 0.98f;
static float comp_alpha = 0.02f;
static float last;

void process_wave(float* in, float* out, size_t nframe)
{
	for (; nframe--; in++, out++) {
		*out = 2*volume * -*in;
		last = *out = alpha * last + comp_alpha * *out;
	}
}

void process_midi(size_t frame, uint8_t ev[3])
{
	if (ev[0] == 0xb0 && ev[1] == 0x7f) {	// controller no. 127
		volume = 1.0f - ev[2] / 127.0f;		//-make alpha goes slower when
		volume = 1.0f - volume * volume;	// reaching 1

		printf("volume = %4.4f", volume);
		if (ev[2] == 0) puts("  no antiwave");
		else if (ev[2] == 0x7f) puts("  full antiwave");
		else putchar('\n');
	}

	if (ev[0] == 0xb0 && ev[1] == 0x7e) {	// controller no. 126
		alpha = 1.0f - ev[2] / 127.0f;	//-make alpha goes slower when
		alpha = 1.0f - alpha * alpha;	// reaching 1
		comp_alpha = 1.0f - alpha;

		printf("alpha = %4.4f", alpha);
		if (ev[2] == 0) puts("  original");
		else if (ev[2] == 0x7f) puts("  silence");
		else putchar('\n');
	}
}


