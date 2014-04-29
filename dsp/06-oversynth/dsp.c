// vim: noet ts=4 sw=4 sts=0
#include "dsp.h"
#include "jack.h"
#include "synth.h"
#include <math.h>
#include <stdio.h>
#include <string.h>

static float alpha = 0.80f;
static float comp_alpha = 1.0f - 0.80;
static float note_duration = 1.00f;
static float last;

static void dsp_zero(float* out, size_t nframe);
static void dsp_mix(float* out, float* in, size_t nframe);
static void dsp_lowpass(float* out, float* in, size_t nframe);

typedef float Sound(int i, float freq, float offset);
// if id<0 then return the selected sound function
// otherwise, select the sound specified by id;
static Sound* sound_select(int id);


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
	else if (ev[0] >= 0x90 && ev[0] <= 0xa0) {		// note on in channel 0
		sound_select(ev[0] - 0x90);
		synth_add(ev[1], note_duration, ev[2] / 127.0f);
		printf("note = %2.2x    vol %2.2f    sound %x\n",
				ev[1], ev[2] / 127.0f, ev[0]-0x90);
	}
//	else printf("unknown: %2.2x: %2.2x(% 3d) %2.2x(% 3d)\n",
//			ev[0], ev[1], ev[1], ev[2], ev[2]);
}

static const float zero = 0.001;
void synthesis(float* out, size_t nframe, float freq, float velocity, float offset)
{
	// 1 + log_a(nframe+1) = zero  <=>  ln(a) = ln(nframe+1)/(zero-1)
	// and in C, log is base-e logarithm
	float loga = logf(nframe+1.0f) / (zero-1.0f);

	float func(int i, float freq, float offset)
	{
		int sfreq = srate / freq;
		i = (int)(i+offset*sfreq) % sfreq;
		if (i < sfreq/2) return 1;
		return -1;
	}

	void overtone(int i, float freq, float velocity, float offset)
	{
		if (velocity < 0.01f) return;
		*out += sound_select(-1)(i, freq, offset)*velocity;
		overtone(i, freq*2.000000f, velocity*0.10, 0.30); // +do: 2^(12/12)
		overtone(i, freq*1.498307f, velocity*0.19, 0.70); // sol: 2^( 7/12)
		overtone(i, freq*1.259921f, velocity*0.13, 0.11); // mi : 2^( 4/12)
		overtone(i, freq/1.498307f, velocity*0.11, 0.37); // -fa: 2^(-7/12)
		overtone(i, freq/2.000000f, velocity*0.07, 0.43); // -do: 2^(12/12)
	}

	for (int i=0; i<nframe; i++,out++) {
		float gain = 1 + log(i+1) / loga;
		overtone(i, freq, gain*velocity, offset);
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




static float sound_sqr(int i, float freq, float offset)
{
	int sfreq = srate / freq;
	i = (int)(i+offset*sfreq) % sfreq;
	if (i < sfreq/2) return 1;
	return -1;
}

static float sound_saw(int i, float freq, float offset)
{
	int sfreq = srate / freq;
	i = (int)(i+offset*sfreq) % sfreq;
	return 2.0f * i / sfreq - 1.0f;
}

static float sound_sin(int i, float freq, float offset)
{
	return sin(2*M_PI*(freq/srate*i + offset));
}

static Sound* sound_select(int id)
{
	static Sound* sounds[] = {
		&sound_sqr,
		&sound_saw,
		&sound_sin,
	};
	static Sound* current = &sound_sqr;

	if (id < 0) return current;

	const int nsound = sizeof(sounds)/sizeof(sounds[0]);
	current = sounds[id % nsound];
	return current;
}

