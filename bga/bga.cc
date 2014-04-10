#include "bga.hh"
#include "port.hh"

namespace kernel
{
	namespace driver { namespace video { namespace bga {
		namespace ioport
		{
			constexpr static u16 index = 0x1ce;
			constexpr static u16 data  = 0x1cf;
		};

		namespace index
		{
			constexpr static u16 id     = 0;
			constexpr static u16 xres   = 1;
			constexpr static u16 yres   = 2;
			constexpr static u16 bpp    = 3;
			constexpr static u16 enable = 4;
			constexpr static u16 bank   = 5;
			constexpr static u16 vw     = 6;	// virtual width
			constexpr static u16 vh     = 7;	// virtual width
			constexpr static u16 ox     = 8;	// offset x
			constexpr static u16 oy     = 9;	// offset y
		};

		static inline void out(u16 index, u16 data)
		{
			port::out(ioport::index, index);
			port::out(ioport::data , data );
		}

		static inline u16 in(u16 index)
		{
			port::out(ioport::index, index);
			return port::in<u16>(ioport::data);
		}


		int version()
		{
			u16 id = bga::in(index::id);
			if ((id & 0xfff0) != 0xb0c0) return -1;
			return (id & 0xf);
		}

		void enable(bool yes)
		{
			bga::out(index::enable, yes);
		}

		bool mode(u16 xres, u16 yres, u16 bpp)
		{
			bga::enable(false);
			bga::out(index::xres, xres);
			bga::out(index::yres, yres);
			bga::out(index::bpp , bpp );
			bga::enable();
			return (bga::in(index::xres) != xres ||
					bga::in(index::yres) != yres ||
					bga::in(index::bpp ) != bpp);
		}
	};};};
};

