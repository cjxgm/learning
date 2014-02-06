#include <stdio.h>
#include <stdlib.h>
#include <math.h>

static void output(float* buf, size_t n);
const size_t srate = 8000;
#define OUTPUT(BUF)		output((BUF), sizeof((BUF))/sizeof(*(BUF)))



/////////// EDIT HERE ///////////
static void ks(size_t n; float x[n], size_t n, float alpha, size_t repeat)
{
	float out[n*repeat];
	float scale = 1;
	for (int i=0; i<repeat; i++, scale*=alpha) {
		float* y = &out[i*n];
		float* p = x;
		for (int j=0; j<n; j++)
			*y++ = *p++ * scale;
	}
	OUTPUT(out);
}

static float unirand()
{
	return (rand() & 0xFF) / 255.0 * 2 - 1;
}

static void process()
{
	float buf[50];
	for (int i=0; i<50; i++)
		buf[i] = unirand();
	ks(buf, 50, 0.9, 50);
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

