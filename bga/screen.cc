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
	};
};

