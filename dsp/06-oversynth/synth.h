// vim: noet ts=4 sw=4 sts=0
#pragma once
#include <stdlib.h>		// for size_t
#include <stdint.h>		// for uint8_t

void synth_add(uint8_t midi_note, float duration, float velocity);
void synth_fill(float* out, size_t nframe);

