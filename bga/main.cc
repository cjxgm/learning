#include "main.hh"
#include "screen.hh"
#include "port.hh"
#include "pci.hh"
#include "bga.hh"

void kernel::main()
{
	screen::clear();

	screen::cursor(1, 8);
	screen::put("detected bga version: ", screen::color::green);
	int ver = driver::video::bga::version();
	if (ver < 0) screen::put("no", screen::color::hgreen);
	else screen::put(ver);

	// setup video mode
	if (driver::video::bga::mode(800, 600))
		screen::put("    mode-setting failed.", screen::color::hred);


	screen::cursor(0, 4);
	screen::put(port::in<u8>(1) + 'a', screen::color::rgbh(1, 0, 0));
	port::out<u8>(1, 2);

	screen::cursor(3, 12);
	screen::put("Hello, World!",
			screen::color::make(
				screen::color::rgbh(1, 1, 0, 1),
				screen::color::rgbh(0, 0, 1, 0)));

	screen::put<16,u32>(0x12345678);
	screen::put<16,u16>(0x9abc);
	screen::put<16,u8 >(0xde);
	screen::put<16,u8 >(0xF);	// use upper case F to test...... LOL

	driver::video::bga::enable(false);

	// scan the pci
	int yline = 5;
	screen::cursor(5, 4);
	for (u16 bus=0; bus<0x100; bus++)
		for (u8 dev=0; dev<0b100000; dev++)
			for (u8 func=0; func<0b1000; func++) {
				pci::DeviceRaw raw;
				if (pci::device(raw, bus, dev, func)) continue;
				if (!(raw[0] == 0x11111234)) continue;
				for (int i=0; i<256/4; i++) {
					screen::put<16>(raw[i]);
					screen::put(':');
				}
				u8* xxx = (u8*)raw[4];
				*xxx = 255;
				/*
				if (vendor == 0xffff) continue;
				screen::put<16,u8>(bus);
				screen::put(':');
				screen::put<16>(dev);
				screen::put(' ');
				screen::put<16>(vendor);
				screen::put("    ");
				*/
			}

	// print line numbers
	for (int y=1; y<=25; y++) {
		screen::cursor(y-1);
		screen::put(' ');
		if (y<10) screen::put(' ');
		screen::put(y, screen::color::yellow);
		screen::put(' ');
	}
}

