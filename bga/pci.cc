#include "pci.hh"
#include "port.hh"


namespace kernel
{
	namespace pci
	{
		u16 read(u8 bus, u8 dev, u8 func, u8 reg)
		{
			// -------- addr --------
			//
			// 00-01  2    (=0)
			// 02-07  6    register
			// 08-10  3    function
			// 11-15  5    device
			// 16-23  8    bus
			// 24-30  7    RESERVED
			// 31-31  1    enable (=1)
			//
			u32 addr =  ((reg  & 0b111111) << 2 ) |
						((func & 0b111   ) << 8 ) |
						((dev  & 0b11111 ) << 11) |
						( bus              << 16) |
						( 1                << 31);
			port::out<u32>(config::addr, addr);

			// ((reg & 0b10) << 3) will choose the first 16-bit.
			return port::in<u32>(config::data) >> ((reg & 0b10) << 3);
		}
	};
};

