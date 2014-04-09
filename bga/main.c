#include "screen.h"
#include "port.h"

void main()
{
	charpos(1, 1);
	putchar('a');

	charpos(0, 0);
	putcharc(in8(1) + 'a', COLOR_R);
	out8(1, 2);

	charpos(4, 0);
	putstrc("Hello, World!", COLOR_H|COLOR_R|COLOR_G | COLOR_BG(COLOR_B));
	puthex32(0x12345678);
	puthex16(0x9abc);
	puthex8(0xde);
	puthex8(0xF);
}

