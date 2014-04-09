#pragma once
#include "util.hh"

namespace kernel
{
	namespace pci
	{
		namespace config
		{
			constexpr u16 addr = 0xCF8;
			constexpr u16 data = 0xCFC;
		};
		u16 read(u8 bus, u8 dev, u8 func, u8 reg);
	};
};

