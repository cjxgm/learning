#pragma once
#include "util.h"

#define COLOR_BG(C)		((C) << 4)
#define COLOR_H			(1<<3)			// highlight
#define COLOR_R			(1<<2)			// red
#define COLOR_G			(1<<1)			// green
#define COLOR_B			(1<<0)			// blue
#define COLOR_WHITE		(COLOR_R | COLOR_G | COLOR_B)
#define COLOR_YELLOW	(COLOR_R | COLOR_G)

void putcharc(u8 ch, u8 color);
void putchar(u8 ch);
void charpos(int y, int x);
void putstrc(const char* s, u8 color);
void putstr(const char* s);


//////////////////// put hex series ///////////////////
#define PUTHEXC(name, size) void puthex##name##c(u##size hex, u8 color)
#define PUTHEX(name, size) void puthex##name(u##size hex)
PUTHEXC(b, 8);
PUTHEXC(w, 16);
PUTHEXC(l, 32);
PUTHEX(b, 8);
PUTHEX(w, 16);
PUTHEX(l, 32);
#undef PUTHEX
#undef PUTHEXC

