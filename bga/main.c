#include "screen.h"
#include "port.h"

void main()
{
	charpos(1, 1);
	putchar('a');

	charpos(0, 0);
	putcharc(inb(1) + 'a', COLOR_R);
	outb(1, 2);

	charpos(4, 0);
	putstrc("Hello, World!", COLOR_H|COLOR_R|COLOR_G | COLOR_BG(COLOR_B));
	puthexl(0x12345678);
	puthexw(0x9abc);
	puthexb(0xde);
	puthexb(0xF);
}

