#pragma once
#include <cstdint>

namespace imgui
{
	struct command
	{
		enum class kind_type { rect, clip, text };
		kind_type kind;
		int xyxy[4];
		uint8_t rgba[4];
		char ch;

		static command make(kind_type k,
				int x, int y, int w, int h,
				uint8_t r=0, uint8_t g=0, uint8_t b=0, uint8_t a=0xFF,
				char ch=0)
		{
			return {
				.kind = k,
				.xyxy = {x, y, x+w, y+h},
				.rgba = {r, g, b, a},
				.ch   = ch
			};
		}

		static command rect(int x, int y, int w, int h,
				uint8_t r=0xFF, uint8_t g=0xFF, uint8_t b=0xFF, uint8_t a=0xFF)
		{
			return make(kind_type::rect, x, y, w, h, r, g, b, a);
		}

		static command clip(int x, int y, int w, int h)
		{
			return make(kind_type::clip, x, y, w, h);
		}

		static command text(int x, int y, int w, int h, char ch,
				uint8_t r=0, uint8_t g=0, uint8_t b=0, uint8_t a=0)
		{
			return make(kind_type::text, x, y, w, h, r, g, b, a, ch);
		}
	};
}

