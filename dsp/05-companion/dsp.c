// vim: noet ts=4 sw=4 sts=0
#include "dsp.h"
#include "jack.h"
#include <stdio.h>

static float volume = 0.7f;
static uint8_t nloop = 4;
static uint8_t nnote = 0;

void process_wave(float* in, float* out, size_t nframe)
{
}

void chord(size_t frame, uint8_t op, int id, uint8_t vol, int lower)
{
	static const uint8_t companions[12*3] = {
		48, 52, 55,		// 1	1 3 5
		 0,  0,  0,		// 1#  TODO
		47, 53, 55,		// 2	-7 4 5
		 0,  0,  0,		// 2#  TODO
		48, 52, 55,		// 3	1 3 5
		47, 50, 55,		// 4	-7 2 5
		 0,  0,  0,		// 4#  TODO
		47, 50, 55,		// 5	-7 2 5
		 0,  0,  0,		// 5#  TODO
		48, 53, 57,		// 6	1 4 6
		 0,  0,  0,		// 6#  TODO
		50, 53, 55,		// 7	2 4 5
	};

	const uint8_t* comp = &companions[3*id];
	uint8_t ev[3] = { op, 0, vol };
	midi_write(frame, (ev[1]=*comp++ - 12*lower, ev));
	midi_write(frame, (ev[1]=*comp++ - 12*lower, ev));
	midi_write(frame, (ev[1]=*comp++ - 12*lower, ev));
}

void process_midi(size_t frame, uint8_t ev[3])
{
	printf("%2.2x %d %d\n", ev[0], ev[1], ev[2]);

	if (ev[0] == 0x90 || ev[0] == 0x80)	{ //  preseed or released
		if (ev[1] >= 48 && ev[1] <= 59) {
			if (ev[0] == 0x80 && !nnote) {}
			else {
				if (ev[0] == 0x90 && !nnote)
					chord(frame, 0x80, ev[1]-48, 0, 1);
				chord(frame, ev[0], ev[1]-48, ev[2]*volume, !nnote);
			}

			if (ev[0] == 0x80) nnote = (nnote+1) % nloop;
		}
		else midi_write(frame, ev);
		return;
	}

	if (ev[0] == 0xb0 && ev[1] == 0x7f) {	// controller no. 127
		volume = ev[2] / 127.0f;

		printf("volume = %4.4f", volume);
		if (ev[2] == 0) puts("  silent");
		else if (ev[2] == 0x7f) puts("  full");
		else putchar('\n');
		return;
	}

	if (ev[0] == 0xb0 && ev[1] == 0x7e) {	// controller no. 126
		nloop = ev[2];
		nnote = 0;
		printf("loop = %d\n", nloop);
		return;
	}
}


