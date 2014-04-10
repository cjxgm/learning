#include "main.hh"
#include "screen.hh"
#include "port.hh"
#include "pci.hh"
#include "bga.hh"

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

	// scan the pci
	screen::cursor(5);
	for (u16 bus=0; bus<0x100; bus++)
		for (u8 dev=0; dev<0b100000; dev++) {
			u16 vendor = pci::read(bus, dev, 0, 0);
			if (vendor == 0xffff) continue;
			screen::puthexf<u8>(bus);
			screen::put(':');
			screen::puthexf(dev);
			screen::put(' ');
			screen::puthexf(vendor);
			screen::put("    ");
		}

	// check for bga
	screen::cursor(2);
	screen::puthexf<u32>(driver::video::bga::version());
}

