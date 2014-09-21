#pragma once
#include <cstdint>
#include <array>
#include "quad.hh"

namespace imgui
{
	// command: rect
	// 		draw a rectangle of color "color" inside "clip".
	//
	// command: text
	// 		draw the text "ch" of color "color"
	// 		at "region.x, region.y" of size "region.w, region.h",
	// 		clipped inside "clip"
	//
	struct command
	{
		enum class kind_type { rect, text };

		kind_type kind;
		xyxy clip;
		rgba color;
		xywh region;
		char ch;

		static command make(
				kind_type kind,
				xyxy const& clip,
				rgba const& color={0},
				xywh const& region={},
				char ch={})
		{
			return {
				.kind   = kind,
				.clip   = clip,
				.color  = color,
				.region = region,
				.ch     = ch
			};
		}

		static command rect(
				xyxy const& clip,
				rgba const& color)
		{
			return make(kind_type::rect, clip, color);
		}

		static command text(
				xyxy const& clip,
				rgba const& color,
				xywh const& region,
				char ch)
		{
			return make(kind_type::text, clip, color, region, ch);
		}
	};

	inline bool operator == (command const& a, command const& b)
	{
		if (a.kind != b.kind) return false;
		switch (a.kind) {
			case command::kind_type::text:
				if (a.ch != b.ch) return false;
				if (a.region != b.region) return false;
				// pass through intentionally
			case command::kind_type::rect:
				if (a.clip != b.clip) return false;
				if (a.color != b.color) return false;
				break;
		}
		return true;
	}

	inline bool operator != (command const& a, command const& b)
	{
		return !(a == b);	// FIXME: is it correct and efficient to do so?
	}
}

