#pragma once
#include "util.hh"

namespace kernel
{
	namespace driver { namespace video { namespace bga {
		int version();
		void enable(bool yes = true);
		bool mode(u16 xres, u16 yres, u16 bpp);
	};};};
};

