// vim: noet ts=4 sw=4 sts=0
#include "dsp.h"
#include "jack.h"
#include "synth.h"
#include <stdio.h>
#include <string.h>
#include <math.h>

static float alpha = 0.80f;
static float comp_alpha = 1.0f - 0.80;
static float note_duration = 1.00f;
static float last;

static void dsp_zero(float* out, size_t nframe);
static void dsp_mix(float* out, float* in, size_t nframe);
static void dsp_lowpass(float* out, float* in, size_t nframe);


void process_wave(float* out, float* in, size_t nframe)
{
	dsp_zero(out, nframe);
	synth_fill(out, nframe);
	dsp_mix(out, in, nframe);
	dsp_lowpass(out, out, nframe);
}

void process_midi(size_t frame, uint8_t ev[3])
{
	if (ev[0] == 0xb0 && ev[1] == 0x7f) {			// controller no. 127
		alpha = 1.0f - ev[2] / 127.0f;	//-make alpha goes slower when
		alpha = 1.0f - alpha * alpha;	// reaching 1
		comp_alpha = 1.0f - alpha;		//-complementary of alpha

		printf("alpha = %4.4f", alpha);
		if (ev[2] == 0) puts("  original sound");
		else if (ev[2] == 0x7f) puts("  complete silence");
		else putchar('\n');
	}
	else if (ev[0] == 0xb0 && ev[1] == 0x7e) {		// controller no. 126
		note_duration = ev[2] / 31.0f;
		printf("len = %2.2f\n", note_duration);
	}
	else if (ev[0] == 0x90) {						// note on in channel 0
		synth_add(ev[1], note_duration, ev[2] / 127.0f);
		printf("note = %2.2x    vol %2.2f\n", ev[1], ev[2] / 127.0f);
	}
}

static float unirand()
{
	return 2.0f * (rand() & 0xFFFF) / 0xFFFF - 1.0f;
}
static const float zero = 0.001;
void synthesis(float* out, size_t nframe, float freq, float velocity)
{
	// 1 + log_a(nframe+1) = zero  <=>  ln(a) = ln(nframe+1)/(zero-1)
	// and in C, log is base-e logarithm
	float loga = logf(nframe+1.0f) / (zero-1.0f);
	int sfreq = srate / freq;

	float noise[sfreq];
	for (int i=0; i<sfreq; i++) noise[i] = unirand();

	for (int i=0; i<nframe; i++) {
		float gain = 1 + log(i+1) / loga;
		int m = i % sfreq;
		//*out++ = (2.0f * m / sfreq - 1.0f) * gain*velocity;
		*out++ = noise[m] * gain*velocity;
	}
}




static void dsp_lowpass(float* out, float* in, size_t nframe)
{
	// leaky integrator:
	// 		y[n] = alpha * y[n-1] + (1-alpha) * x[n]
	for (; nframe--; out++, in++)
		last = *out = alpha * last + comp_alpha * *in;
}

static void dsp_mix(float* out, float* in, size_t nframe)
{
	while (nframe--) *out++ += *in++;
}

static void dsp_zero(float* out, size_t nframe)
{
	memset(out, 0, sizeof(*out)*nframe);
}

