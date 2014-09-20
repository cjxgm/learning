#pragma once
#include <cstdint>

namespace imgui
{
	struct command
	{
		enum class kind_type { rect, text };
		kind_type kind;
		int xyxy[4];
		uint8_t rgba[4];
		char ch;

		static command make(kind_type k,
				int x, int y, int w, int h,
				uint8_t r, uint8_t g, uint8_t b, uint8_t a=0xFF)
		{
			return {
				.kind = k,
				.xyxy = {x, y, x+w, y+h},
				.rgba = {r, g, b, a}
			};
		}

		static command rect(int x, int y, int w, int h,
				uint8_t r=0xFF, uint8_t g=0xFF, uint8_t b=0xFF, uint8_t a=0xFF)
		{
			return make(kind_type::rect, x, y, w, h, r, g, b, a);
		}

		static command text(int x, int y, int w, int h, char ch,
				uint8_t r=0, uint8_t g=0, uint8_t b=0, uint8_t a=0)
		{
			auto cmd = make(kind_type::text, x, y, w, h, r, g, b, a);
			cmd.ch = ch;
			return cmd;
		}
	};
}

