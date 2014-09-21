#pragma once
// some common quad-value (aka 4-element-value)
#include <allegro5/allegro.h>
#include <array>

namespace imgui
{
	// region
	struct xyxy;
	struct xywh;

	struct xyxy
	{
		using self_type = xyxy;
		using self_cref = self_type const&;

		union {
			struct {
				int x1;
				int y1;
				int x2;
				int y2;
			};
			std::array<int, 4> data;
		};

		xyxy() = default;
		xyxy(int x1, int y1, int x2, int y2)
			: x1{x1}, y1{y1}, x2{x2}, y2{y2} {}
		xyxy(xywh const& r);

		int  operator [] (int i) const { return data[i]; }
		int& operator [] (int i)       { return data[i]; }
		bool operator == (self_cref x) const { return (data == x.data); }
		bool operator != (self_cref x) const { return (data != x.data); }
	};

	struct xywh
	{
		using self_type = xywh;
		using self_cref = self_type const&;

		union {
			struct {
				int x;
				int y;
				int w;
				int h;
			};
			std::array<int, 4> data;
		};

		xywh() = default;
		xywh(int x, int y, int w, int h)
			: x{x}, y{y}, w{w}, h{h} {}
		xywh(xyxy const& r);

		int  operator [] (int i) const { return data[i]; }
		int& operator [] (int i)       { return data[i]; }
		bool operator == (self_cref x) const { return (data == x.data); }
		bool operator != (self_cref x) const { return (data != x.data); }
	};

	inline xyxy::xyxy(xywh const& r)
		: x1{r.x}, y1{r.y}, x2{r.x+r.w}, y2{r.y+r.h} {}
	inline xywh::xywh(xyxy const& r)
		: x{r.x1}, y{r.y1}, w{r.x2-r.x1}, h{r.y2-r.y1} {}




	// color
	struct rgba
	{
		using self_type = rgba;
		using self_cref = self_type const&;

		union {
			struct {
				uint8_t r;
				uint8_t g;
				uint8_t b;
				uint8_t a;
			};
			std::array<uint8_t, 4> data;
		};

		rgba() = default;
		rgba(uint8_t lightness, uint8_t a=0xFF)
			: rgba{lightness, lightness, lightness, a} {}
		rgba(uint8_t r, uint8_t g, uint8_t b, uint8_t a=0xFF)
			: r{r}, g{g}, b{b}, a{a} {}

		uint8_t  operator [] (int i) const { return data[i]; }
		uint8_t& operator [] (int i)       { return data[i]; }
		bool operator == (self_cref x) const { return (data == x.data); }
		bool operator != (self_cref x) const { return (data != x.data); }
		operator ALLEGRO_COLOR () const { return al_map_rgba(r, g, b, a); }
		bool     visible() const { return (a != 0x00); }
		bool transparent() const { return (a != 0xFF && visible()); }
	};
}

