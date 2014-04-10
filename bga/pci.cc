#include "pci.hh"
#include "port.hh"

namespace kernel
{
	namespace pci
	{
		namespace config
		{
			constexpr static u16 addr = 0xCF8;
			constexpr static u16 data = 0xCFC;
		};

		namespace address
		{
			union Type
			{
				struct Detail {
					u8 _0   : 2;	// always 0
					u8 reg  : 6;	// register
					u8 func : 3;	// function
					u8 dev  : 5;	// device
					u8 bus  : 8;	// bus
					u8 _1   : 7;	// reserved
					u8 _2   : 1;	// enable (should be 1)
				} PACKED;
				Detail detail;
				u32 bits;
			};

			constexpr static u32 make(u8 bus, u8 dev, u8 func, u8 reg)
			{
				return (Type {
							.detail = { 0, reg, func, dev, bus, 0, 1 }
						}).bits;
			}
		};

		u16 read(u8 bus, u8 dev, u8 func, u8 reg)
		{
			port::out(config::addr, address::make(bus, dev, func, reg));
			// FIXME: why? ((reg & 0b10) << 3) will choose the correct 16-bit.
			return port::in<u32>(config::data) >> ((reg & 0b10) << 3);
		}
	};
};

