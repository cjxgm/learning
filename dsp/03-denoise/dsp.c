// vim: noet ts=4 sw=4 sts=0
#include "dsp.h"
#include "jack.h"
#include <stdio.h>

void process_wave(float* in, float* out, size_t nframe)
{
}

void process_midi(size_t frame, uint8_t ev[3])
{
	printf("%lu: %2.2hhx %2.2hhx %2.2hhx\n", frame, ev[0], ev[1], ev[2]);
}


