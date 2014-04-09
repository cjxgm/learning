#pragma once
#include "util.hh"

namespace kernel
{
	namespace screen
	{
		namespace color
		{
			using Type = u8;
			constexpr Type rgbh(bool r=0, bool g=0, bool b=0, bool h=0)
			{
				return (h<<3) | (r<<2) | (g<<1) | (b<<0);
			}

			static constexpr Type  black  = rgbh(0, 0, 0, 0);
			static constexpr Type hblack  = rgbh(0, 0, 0, 1);
			static constexpr Type  white  = rgbh(1, 1, 1, 0);
			static constexpr Type hwhite  = rgbh(1, 1, 1, 1);
			static constexpr Type  yellow = rgbh(1, 1, 0, 0);
			static constexpr Type hyellow = rgbh(1, 1, 0, 1);

			constexpr Type make(Type fg, Type bg = black)
			{
				return fg | (bg<<4);
			}
		};


		void cursor(int y=0, int x=0);
		void put(u8 ch, color::Type color = color::white);
		void put(const char* s, color::Type color = color::white);

		template <class T>
		void puthex(T hex, color::Type color = color::hyellow)
		{
			constexpr const char* digits = "0123456789abcdef";
			for (int i=(sizeof(T)<<1)-1; i>=0; i--)
				put(digits[(hex >> (i<<2)) & 0xF], color);
		}

		template <class T>
		void puthexf(T hex)
		{
			put("0x", color::yellow);
			puthex(hex);
		}
	};
};

