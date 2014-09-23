#include <random>
#include <iterator>
#include <algorithm>
#include "font.hh"
#include "log.hh"

uint_fast32_t rand_less(uint_fast32_t max)
{
	static std::random_device rd;
	static std::mt19937 engine{rd()};
	return engine() % max;
}

static size_t  font_cache_massacre_threshold =     16;
static size_t glyph_cache_massacre_threshold = 256*16;

namespace imgui
{
	auto font_cache::get(fontsize_type s) -> font_ptr
	{
		try { return cache.at(s); }
		catch (std::out_of_range) {
			auto font = allocate(s);
			return cache.emplace(s, std::move(font)).first->second;
		}
	}

	void font_cache::massacre()
	{
		auto size = cache.size() >> 1;
		while (size--) sacrifice();
	}

	void font_cache::sacrifice()
	{
		auto it = cache.begin();
		std::advance(it, rand_less(cache.size()));
		library::log() << "font: - [" << it->first << "]\n";
		cache.erase(it);
	}

	auto font_cache::allocate(fontsize_type s) -> font_mptr
	{
		if (cache.size() > font_cache_massacre_threshold)
			massacre();

		font_mptr font;
		font.manage(
				al_load_font("font.ttf", -s, 0),	// "-s" to make height to be "s"
				&al_destroy_font,
				"cannot create font");

		auto rate = int(10000.0f * cache.size() / font_cache_massacre_threshold) / 100.0f;
		library::log() << "font: + [" << s << "]"
			<< "\ttotal = " << cache.size()
			<< "\trate = " << rate << "%\n";

		return std::move(font);
	}




	auto glyph_cache::get(key_type k) -> bitmap_ptr
	{
		try { return cache.at(k); }
		catch (std::out_of_range) {
			auto glyph = allocate(k);
			return cache.emplace(k, std::move(glyph)).first->second;
		}
	}

	void glyph_cache::massacre()
	{
		auto size = cache.size() >> 1;
		while (size--) sacrifice();
	}

	void glyph_cache::sacrifice()
	{
		auto it = cache.begin();
		std::advance(it, rand_less(cache.size()));
		library::log() << "glyph: - [" << it->first.first << "] \""
			<< it->first.second << "\"\n";
		cache.erase(it);
	}

	auto glyph_cache::allocate(key_type k) -> bitmap_mptr
	{
		if (cache.size() > glyph_cache_massacre_threshold)
			massacre();

		auto font = fonts.get(k.first);
		char buf[2] = { k.second, 0 };
		// FIXME: how to do this right for non-monospace font?
		//
		//int d[4];
		//al_get_text_dimensions(font, buf, d+0, d+1, d+2, d+3);
		//auto h = d[3];
		//auto w = d[2];

		auto h = k.first;
		auto w = h >> 1;
		bitmap_mptr glyph;
		glyph.manage(
				al_create_bitmap(w, h),
				&al_destroy_bitmap,
				"cannot create bitmap");

		auto old = al_get_target_bitmap();
		al_set_target_bitmap(glyph);
		al_clear_to_color(al_map_rgba(0, 0, 0, 0));
		al_draw_text(font, rgba{0xFF}, 0, 0, 0, buf);
		al_set_target_bitmap(old);

		auto rate = int(10000.0f * cache.size() / glyph_cache_massacre_threshold) / 100.0f;
		library::log() << "glyph: + [" << k.first << "] \"" << k.second
			<< "\"\ttotal = " << cache.size()
			<< "\trate = " << rate << "%\n";

		return glyph;
	}




	void draw_text(xywh const& region, rgba const& color, char_type ch)
	{
		static glyph_cache glyphs;

		auto size = std::max(region.h, region.w << 1);
		auto glyph = glyphs.get(size, ch);
		al_draw_tinted_bitmap(glyph, color, region.x, region.y, 0);

		// TODO: is this a better way?
		//al_draw_tinted_scaled_bitmap(
		//		glyph, color,
		//		0, 0,
		//		al_get_bitmap_width(glyph), al_get_bitmap_height(glyph),
		//		region.x, region.y,
		//		region.w, region.h,
		//		0);
	}
}

