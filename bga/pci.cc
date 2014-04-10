#include "pci.hh"
#include "port.hh"

namespace kernel
{
	namespace pci
	{
		namespace config
		{
			constexpr static u16 addr = 0xcf8;
			constexpr static u16 data = 0xcfc;
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

		u32 read(u8 bus, u8 dev, u8 func, u8 reg)
		{
			port::out(config::addr, address::make(bus, dev, func, reg));
			return port::in<u32>(config::data);
		}

		bool device(DeviceRaw& raw, u8 bus, u8 dev, u8 func)
		{
			raw[0] = read(bus, dev, func, 0);
			if (raw[0] == 0xffffffff) return true;
			for (int i=1; i<256/4; i++)
				raw[i] = read(bus, dev, func, i);
			return false;
		}
	};
};

