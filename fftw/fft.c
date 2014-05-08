
#include "fft.h"
#include <complex.h>	// must be included before fftw3.h
#include <fftw3.h>


void fft(double* in, FFTCplx* out, size_t n)
{
	fftw_plan p = fftw_plan_dft_r2c_1d(n, in, out, FFTW_ESTIMATE);
	fftw_execute(p);
	fftw_destroy_plan(p);
	// normalize
	double invn = 1.0 / n;
	for (int i=0; i<n; i++) out[i] *= invn;
}

void ifft(FFTCplx* in, double* out, size_t n)
{
	fftw_plan p = fftw_plan_dft_c2r_1d(n, in, out, FFTW_ESTIMATE);
	fftw_execute(p);
	fftw_destroy_plan(p);
}

void spectrum(FFTCplx* in, double* out, size_t n)
{
	while (n--) {
		*out = cabs(*in++);
		*out *= *out;
		out++;
	}
}

