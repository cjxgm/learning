
#pragma once
#include <stddef.h>
#include <complex.h>

typedef double complex FFTCplx;
void fft(double* in, FFTCplx* out, size_t n);
void ifft(FFTCplx* in, double* out, size_t n);
void spectrum(FFTCplx* in, double* out, size_t n);

