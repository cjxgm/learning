#include <stdio.h>
#include <stdlib.h>
#include <math.h>

static void output(float* buf, size_t n);
const size_t srate = 8000;
#define OUTPUT(BUF)		output((BUF), sizeof((BUF))/sizeof(*(BUF)))



/////////// EDIT HERE ///////////
static void ks(float x[], size_t n, float gain, float alpha, size_t repeat)
{
	float out[n*repeat];
	for (int i=0; i<repeat; i++, gain*=alpha) {
		float* y = &out[i*n];
		float* p = x;
		for (int j=0; j<n; j++)
			*y++ = *p++ * gain;
	}
	OUTPUT(out);
}

// auto calc alpha
const float ks_zero = 0.1;
static void ks_auto(float x[], size_t n, float gain, size_t repeat)
{
	float alpha = powf(ks_zero, 1.0/repeat);
	ks(x, n, gain, alpha, repeat);
}

static float unirand()
{
	return (rand() & 0xFF) / 255.0 * 2 - 1;
}

// duration in seconds
static void pluck_freq(float freq, float gain, float duration)
{
	int len = roundf(srate / freq);
	float buf[len];
	// use random number
	for (int i=0; i<len; i++)
		buf[i] = unirand();
/*
	// use saw wave
	for (int i=0; i<len; i++)
		buf[i] = i * 2.0f / len - 1;
*/
	ks_auto(buf, len, gain, ceilf(duration*srate/len));
}

static float bpm = 300;
typedef unsigned char u8;
// 0x42 -> 0x4: octave;  0x2: note D (see below)
//
// 0 1 2 3 4 5 6 7 8 9 a b c
// C # D # E F # G # A # B C
//
// middle C is 0x40
// middle A is 0x49 = 440 Hz
static void pluck(u8 note, u8 velocity, float beat)
{
	note = 12*(note>>4) + (note&0x0F);
	static const u8 A = 4*12 + 9;	// middle A 0x49
	float freq = 440 * powf(2, (note-A) / 12.0f);
	float gain = velocity / 255.0f;
	float duration = beat / bpm * 60;
	pluck_freq(freq, gain, duration);
}

static void process()
{
	pluck(0x40, 0x7f, 2);
	pluck(0x42, 0x7f, 2);
	pluck(0x44, 0x7f, 2);
	pluck(0x45, 0x7f, 2);
	pluck(0x47, 0x7f, 2);
	pluck(0x49, 0x7f, 2);
	pluck(0x4b, 0x7f, 2);
	pluck(0x50, 0x7f, 2);
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

