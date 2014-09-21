#pragma once
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <unordered_map>
#include "manage.hh"

namespace imgui
{
	// cache opened direrently-sized font
	struct font_cache
	{
		using fontsize_type = int;
		using font_type = ALLEGRO_FONT;
		using font_ptr  = font_type *;
		using font_mptr = managed_ptr<font_type>;
		using font_table = std::unordered_map<fontsize_type, font_mptr>;

		font_ptr get(fontsize_type s);

	private:
		font_table cache;

		void massacre();	// sacrifice half of the cached font
		void sacrifice();	// randomly kill one cached font
		font_mptr allocate(fontsize_type s);
	};




	// cache different glyph at the different size
	struct glyph_cache
	{
		using fcache_type = font_cache;
		using fontsize_type = fcache_type::fontsize_type;
		using bitmap_type = ALLEGRO_BITMAP;
		using bitmap_ptr  = bitmap_type *;
		using bitmap_mptr = managed_ptr<bitmap_type>;
		using char_type = char;	// TODO: use wchar?

		template <class T, class U>
		struct pair_hash
		{
			using pair = std::pair<T, U>;

			template <class V>
			void hash_combine(size_t& seed, V const& x) const
			{
				std::hash<V> hash;
				seed ^= (hash(x) + 0x9e3779b9 + (seed << 6) + (seed >> 2));
			}

			size_t operator () (pair const& p) const
			{
				size_t seed = 0;
				hash_combine(seed, p.first);
				hash_combine(seed, p.second);
				return seed;
			}
		};
		using key_type = std::pair<fontsize_type, char_type>;
		using key_hash = pair_hash<fontsize_type, char_type>;

		using glyph_table = std::unordered_map<key_type, bitmap_mptr, key_hash>;

		bitmap_ptr get(key_type k);

		bitmap_ptr get(fontsize_type s, char_type ch)
		{
			return get({s, ch});
		}

	private:
		fcache_type fonts;
		glyph_table cache;

		void massacre();	// sacrifice half of the cached glyphs
		void sacrifice();	// randomly kill one cached glyph
		bitmap_mptr allocate(key_type ch);
	};




	void draw_text(float x1, float y1, float x2, float y2, char ch,
			ALLEGRO_COLOR color);
}

