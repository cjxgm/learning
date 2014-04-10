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
			static constexpr Type  green  = rgbh(0, 1, 0, 0);
			static constexpr Type hgreen  = rgbh(0, 1, 0, 1);

			constexpr Type make(Type fg, Type bg = black)
			{
				return fg | (bg<<4);
			}
		};


		void cursor(int y=0, int x=0);


		template <int B, class T>
		void put(T data)
		{
			T    unimplemented    [B?-1:-1];
		}
		template <int B, class T, class X>
		void put(T data, X x) { put<B>(data); }
		template <int B, class T, class X, class Y>
		void put(T data, X x, Y y) { put<B>(data); }

		template <>
		void put<10>(char ch, color::Type color);
		template <>
		inline void put<10>(char ch) { put<10>(ch, color::white); }

		template <>
		void put<10>(const char* s, color::Type color);
		template <>
		inline void put<10>(const char* s) { put<10>(s, color::white); }

		template <>
		void put<10>(int num, color::Type color);
		template <>
		inline void put<10>(int num) { put<10>(num, color::hgreen); }

		template <>
		void put<10>(u32 num, color::Type color);
		template <>
		inline void put<10>(u32 num) { put<10>(num, color::hgreen); }


		template <>
		void put<16>(u32 hex, int size, color::Type color);
		template <>
		inline void put<16>(u8 hex, color::Type color) { put<16>(u32(hex), 1, color); }
		template <>
		inline void put<16>(u16 hex, color::Type color) { put<16>(u32(hex), 2, color); }
		template <>
		inline void put<16>(u32 hex, color::Type color) { put<16>(u32(hex), 4, color); }
		template <>
		inline void put<16>(u8 hex) { put<16>(hex, color::hyellow); }
		template <>
		inline void put<16>(u16 hex) { put<16>(hex, color::hyellow); }
		template <>
		inline void put<16>(u32 hex) { put<16>(hex, color::hyellow); }


		template <class T>
		void put(T data) { put<10>(data); }
		template <class T>
		void put(T data, color::Type color) { put<10>(data, color); }
	};
};

