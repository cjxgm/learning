// vim: noet ts=4 sw=4 sts=0
#pragma once
#include <stdlib.h>		// for size_t
#include <stdint.h>		// for uint8_t

void process_wave(float* in, float* out, size_t nframe);
void process_midi(size_t frame, uint8_t ev[3]);

