#pragma once
#include "util.h"

#define COLOR_BG(C)		((C) << 4)
#define COLOR_H			(1<<3)			// highlight
#define COLOR_R			(1<<2)			// red
#define COLOR_G			(1<<1)			// green
#define COLOR_B			(1<<0)			// blue
#define COLOR_WHITE		(COLOR_R | COLOR_G | COLOR_B)

inline void putcharc(u8 ch, u8 color);
inline void putchar(u8 ch);
inline void charpos(int y, int x);
inline void putstrc(const char* s, u8 color);
inline void putstr(const char* s);

