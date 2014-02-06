#include <stdio.h>
#include <stdlib.h>
#include <math.h>

static void output(float* buf, size_t n);
const size_t srate = 8000;
#define OUTPUT(BUF)		output((BUF), sizeof((BUF))/sizeof(*(BUF)))



/////////// EDIT HERE ///////////
static void process()
{
	float xxx[srate];
	const float f = 440;
	for (int i=0; i<srate; i++)
		xxx[i] = sin((float)i/srate * 2*M_PI*f);
	OUTPUT(xxx);
}
/////////// EDIT  END ///////////




static FILE* dspout;

int main(int argc, char* argv[])
{
	dspout = fopen("/dev/dsp", "w");
	if (!dspout) {
		if (argc == 1) { // prevent fork bomb
			char buf[128];
			// added "-" to prevent fork bomb
			sprintf(buf, "padsp %s -", argv[0]);
			return system(buf);
		}
		return 1;
	}
	process();
	fclose(dspout);
	return 0;
}

static void output(float* buf, size_t n)
{
	unsigned char outbuf[n];
	for (unsigned char* p=outbuf; n--; ) {
		float x = *buf++;
		if (x < -1) x = -1;
		if (x >  1) x =  1;
		*p++ = (x+1) / 2 * 0xFF;	// FIXME: ignored fp-error
	}
	fwrite(outbuf, sizeof(outbuf), 1, dspout);
	fflush(dspout);
}

