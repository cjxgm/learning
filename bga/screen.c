#include "screen.h"

typedef struct Glyph
{
	u8 ch;
	u8 color;
}
PACKED
Glyph;

static Glyph* const text = ANY 0xb8000;
static Glyph* current = ANY 0xb8000;


static inline void set(Glyph* g, u8 ch, u8 color)
{
	g->ch    = ch;
	g->color = color;
}

inline void putcharc(u8 ch, u8 color)
{
	set(current++, ch, color);
}

inline void putchar(u8 ch)
{
	putcharc(ch, COLOR_WHITE);
}

inline void charpos(int y, int x)
{
	current = &text[y*80 + x];
}

inline void putstrc(const char* s, u8 color)
{
	while (*s) putcharc(*s++, color);
}

inline void putstr(const char* s)
{
	putstrc(s, COLOR_WHITE);
}




//////////////////// put hex series ///////////////////

static const char hexdigits[] = "0123456789abcdef";

#define PUTHEXC(name, size) \
	inline void puthex##name##c(u##size hex, u8 color) \
	{ \
		for (int i=(size>>2)-1; i>=0; i--) \
			putcharc(hexdigits[(hex >> (i*4)) & 0xF], color); \
	}

#define PUTHEX(name, size) \
	inline void puthex##name(u##size hex) \
	{ \
		putstrc("0x", COLOR_YELLOW); \
		puthex##name##c(hex, COLOR_YELLOW | COLOR_H); \
	}

PUTHEXC(b, 8);
PUTHEXC(w, 16);
PUTHEXC(l, 32);

PUTHEX(b, 8);
PUTHEX(w, 16);
PUTHEX(l, 32);

#undef PUTHEX
#undef PUTHEXC

