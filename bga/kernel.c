#include "util.h"
#include "port.h"

typedef struct Glyph
{
	u8 ch;
	u8 color;
}
PACKED
Glyph;

static Glyph* text = ANY 0xb8000;

void main()
{
	text[0].color = 0b0100;
	text[0].ch = inb(1) + 'a';
	outb(1, 2);
}

