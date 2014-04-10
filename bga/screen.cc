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

		template <>
		void put<10>(char ch, color::Type color)
		{
			*current++ = Glyph { u8(ch), color };
		}

		template <>
		void put<10>(const char* s, color::Type color)
		{
			while (*s) put(*s++, color);
		}

		template <>
		void put<10>(int num, color::Type color)
		{
			if (num < 0) {
				put('-', color);
				num = -num;
			}
			put(u32(num), color);
		}

		template <>
		void put<10>(u32 num, color::Type color)
		{
			if (!num) {
				put('0', color);
				return;
			}
			char buf[16];
			char* p = &buf[15];
			for (*p--=0; num; num/=10)
				*p-- = '0' + num%10;
			put((const char*)(++p), color);
		}

		template <>
		void put<16>(u32 hex, int size, color::Type color)
		{
			constexpr const char* digits = "0123456789abcdef";
			for (int i=(size<<1)-1; i>=0; i--)
				put(char(digits[(hex >> (i<<2)) & 0xF]), color);
		}
	};
};

