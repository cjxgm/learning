// vim: noet ts=4 sw=4 sts=0
#include "dsp.h"
#include "jack.h"
#include <stdio.h>

static float alpha = 0.98f;
static float comp_alpha = 0.02f;
static float last;

void process_wave(float* in, float* out, size_t nframe)
{
	// y[n] = alpha * y[n-1] + (1-alpha) * x[n]
	for (; nframe--; in++, out++)
		last = *out = alpha * last + comp_alpha * *in;
}

void process_midi(size_t frame, uint8_t ev[3])
{
	if (ev[0] == 0xb0 && ev[1] == 0x7f) {	// controller no. 127
		alpha = 1.0f - ev[2] / 127.0f;	//-make alpha goes slower when
		alpha = 1.0f - alpha * alpha;	// reaching 1
		comp_alpha = 1.0f - alpha;		//-complementary of alpha

		printf("alpha = %4.4f", alpha);
		if (ev[2] == 0) puts("  original sound");
		else if (ev[2] == 0x7f) puts("  complete silence");
		else putchar('\n');
	}
}


