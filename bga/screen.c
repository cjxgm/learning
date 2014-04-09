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


