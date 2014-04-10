#include "main.hh"
#include "screen.hh"
#include "port.hh"
#include "pci.hh"
#include "bga.hh"

void kernel::main()
{
	screen::cursor(1, 8);
	screen::put("detected bga version: ", screen::color::green);
	int ver = driver::video::bga::version();
	if (ver < 0) screen::put("no", screen::color::hgreen);
	else screen::put(ver);

	screen::cursor(0, 0);
	screen::put(port::in<u8>(1) + 'a', screen::color::rgbh(1, 0, 0));
	port::out<u8>(1, 2);

	screen::cursor(4);
	screen::put("Hello, World!",
			screen::color::make(
				screen::color::rgbh(1, 1, 0, 1),
				screen::color::rgbh(0, 0, 1, 0)));

	screen::put<16,u32>(0x12345678);
	screen::put<16,u16>(0x9abc);
	screen::put<16,u8 >(0xde);
	screen::put<16,u8 >(0xF);	// use upper case F to test...... LOL

	// scan the pci
	screen::cursor(5);
	for (u16 bus=0; bus<0x100; bus++)
		for (u8 dev=0; dev<0b100000; dev++) {
			u16 vendor = pci::read(bus, dev, 0, 0);
			if (vendor == 0xffff) continue;
			screen::put<16,u8>(bus);
			screen::put(':');
			screen::put<16>(dev);
			screen::put(' ');
			screen::put<16>(vendor);
			screen::put("    ");
		}
}

