#pragma once
#include "util.hh"

namespace kernel
{
	namespace pci
	{
		u32 read(u8 bus, u8 dev, u8 func, u8 reg);
		using DeviceRaw = u32[256/4];
		bool device(DeviceRaw& raw, u8 bus, u8 dev, u8 func);
	};
};

