#include "main.hh"
#include "screen.hh"
#include "port.hh"

void kernel::main()
{
	screen::cursor(1, 1);
	screen::put('a');

	screen::cursor(0, 0);
	screen::put(port::in<u8>(1) + 'a', screen::color::rgbh(1, 0, 0));
	port::out<u8>(1, 2);

	screen::cursor(4);
	screen::put("Hello, World!",
			screen::color::make(
				screen::color::rgbh(1, 1, 0, 1),
				screen::color::rgbh(0, 0, 1, 0)));

	screen::puthexf<u32>(0x12345678);
	screen::puthexf<u16>(0x9abc);
	screen::puthexf<u8>(0xde);
	screen::puthexf<u8>(0xF);	// use upper case F to test...... LOL
}

