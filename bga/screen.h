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

#define PUTHEXC(size) void puthex##size##c(u##size hex, u8 color)
#define PUTHEX(size) void puthex##size(u##size hex)

PUTHEXC(8);
PUTHEXC(16);
PUTHEXC(32);

PUTHEX(8);
PUTHEX(16);
PUTHEX(32);

#undef PUTHEX
#undef PUTHEXC

