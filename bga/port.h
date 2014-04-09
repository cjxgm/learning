#pragma once

#include "util.h"

#define OUTPORT(name, size) \
	static inline void out##name (u16 port, u##size data) \
	{ \
		asm volatile("out %[data], %[port]" \
			:: \
			[data] "a" (data), \
			[port] "Nd" (port) \
		); \
	}

#define INPORT(name, size) \
	static inline u##size in##name(u16 port) \
	{ \
		u##size data; \
		asm("in %[port], %[data]" \
			: [data] "=a" (data) \
			: [port] "Nd" (port) \
		); \
		return data; \
	}

OUTPORT(b, 8);
OUTPORT(w, 16);
OUTPORT(l, 32);

INPORT(b, 8);
INPORT(w, 16);
INPORT(l, 32);

#undef INPORT
#undef OUTPORT

