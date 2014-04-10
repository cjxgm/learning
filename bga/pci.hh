#pragma once
#include "util.hh"

namespace kernel
{
	namespace pci
	{
		u16 read(u8 bus, u8 dev, u8 func, u8 reg);
	};
};

