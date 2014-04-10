#include "screen.hh"

namespace kernel
{
	namespace screen
	{
		struct Glyph
		{
			u8 ch;
			u8 color;
		} PACKED;

		static constexpr Glyph* text = (Glyph*)0xb8000;
		static Glyph* current = text;

		void cursor(int y, int x)
		{
			current = &text[y*80 + x];
		}

		void put(u8 ch, color::Type color)
		{
			*current++ = Glyph { ch, color };
		}

		void put(const char* s, color::Type color)
		{
			while (*s) put(*s++, color);
		}

		void putnum(int num, color::Type color)
		{
			if (num < 0) {
				put('-', color);
				num = -num;
			}
			putnum(u32(num), color);
		}

		void putnum(u32 num, color::Type color)
		{
			if (!num) {
				put('0', color);
				return;
			}
			char buf[16];
			char* p = &buf[15];
			for (*p--=0; num; num/=10)
				*p-- = '0' + num%10;
			put(++p, color);
		}
	};
};

