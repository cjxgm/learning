// vim: noet ts=4 sw=4 sts=0
#pragma once
#include <stdlib.h>		// for size_t

extern size_t srate;	// sample rate

void midi_write(size_t frame, uint8_t ev[3]);

