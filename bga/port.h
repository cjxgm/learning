#pragma once
#include "util.h"

#define OUTPORT(size) \
	static inline void out##size (u16 port, u##size data) \
	{ \
		asm volatile("out %[data], %[port]" \
			:: \
			[data] "a" (data), \
			[port] "Nd" (port) \
		); \
	}

#define INPORT(size) \
	static inline u##size in##size(u16 port) \
	{ \
		u##size data; \
		asm("in %[port], %[data]" \
			: [data] "=a" (data) \
			: [port] "Nd" (port) \
		); \
		return data; \
	}

OUTPORT(8);
OUTPORT(16);
OUTPORT(32);

INPORT(8);
INPORT(16);
INPORT(32);

#undef INPORT
#undef OUTPORT

