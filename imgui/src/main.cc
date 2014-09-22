#include <string>
#include "context.hh"
#include "compiler.hh"
#include "log.hh"
#include "benchmark.hh"


int main()
{
	using namespace std::literals;

	library::log() << "main(): enter\n";

	imgui::context ctx{640, 480};
	imgui::state s;
	imgui::compiler cpr;
	cpr.size(640, 480);

	using imgui::xywh;
	using imgui::xyxy;
	using imgui::rgba;

	bool a = true;
	int size = 1;
	imgui::benchmark bm_input;
	imgui::benchmark bm_compile;
	imgui::benchmark bm_render;
	while (ctx) {

		library::log() << "main(): render\n";

		{
			auto measure = bm_input.measure();
			cpr.rect(xywh{0, 0, 640, 480}, rgba{255, 0, 0});

			cpr.text(xyxy{10, 30, 640, 480},
					"#pragma once\n"
					"#include <allegro5/allegro.h>\n"
					"#include <allegro5/allegro_font.h>\n"
					"#include <unordered_map>\n"
					"#include \"manage.hh\"\n"
					"#include \"quad.hh\"\n"
					"#include \"math.hh\"\n"
					"\n"
					"namespace imgui\n"
					"{\n"
					"	// cache opened direrently-sized font\n"
					"	struct font_cache\n"
					"	{\n"
					"		using fontsize_type = int;\n"
					"		using font_type = ALLEGRO_FONT;\n"
					"		using font_ptr  = font_type *;\n"
					"		using font_mptr = managed_ptr<font_type>;\n"
					"		using font_table = std::unordered_map<fontsize_type, font_mptr>;\n"
					"\n"
					"		font_ptr get(fontsize_type s);\n"
					"\n"
					"	private:\n"
					"		font_table cache;\n"
					"\n"
					"		void massacre();	// sacrifice half of the cached font\n"
					"		void sacrifice();	// randomly kill one cached font\n"
					"		font_mptr allocate(fontsize_type s);\n"
					"	};\n"
					"\n"
					"\n"
					"\n"
					"\n"
					"	// cache different glyph at the different size\n"
					"	struct glyph_cache\n"
					"	{\n"
					"		using fcache_type = font_cache;\n"
					"		using fontsize_type = fcache_type::fontsize_type;\n"
					"		using bitmap_type = ALLEGRO_BITMAP;\n"
					"		using bitmap_ptr  = bitmap_type *;\n"
					"		using bitmap_mptr = managed_ptr<bitmap_type>;\n"
					"		using char_type = char;	// TODO: use wchar?\n"
					"\n"
					"		template <class T, class U>\n"
					"		struct pair_hash\n"
					"		{\n"
					"			using pair = std::pair<T, U>;\n"
					"\n"
					"			size_t operator () (pair const& p) const\n"
					"			{\n"
					"				size_t seed = 0;\n"
					"				hash_combine(seed, p.first);\n"
					"				hash_combine(seed, p.second);\n"
					"				return seed;\n"
					"			}\n"
					"		};\n"
					"		using key_type = std::pair<fontsize_type, char_type>;\n"
					"		using key_hash = pair_hash<fontsize_type, char_type>;\n"
					"\n"
					"		using glyph_table = std::unordered_map<key_type, bitmap_mptr, key_hash>;\n"
					"\n"
					"		bitmap_ptr get(key_type k);\n"
					"\n"
					"		bitmap_ptr get(fontsize_type s, char_type ch)\n"
					"		{\n"
					"			return get({s, ch});\n"
					"		}\n"
					"\n"
					"	private:\n"
					"		fcache_type fonts;\n"
					"		glyph_table cache;\n"
					"\n"
					"		void massacre();	// sacrifice half of the cached glyphs\n"
					"		void sacrifice();	// randomly kill one cached glyph\n"
					"		bitmap_mptr allocate(key_type ch);\n"
					"	};\n"
					"\n"
					"	using char_type = glyph_cache::char_type;\n"
					"\n"
					"\n"
					"\n"
					"	void draw_text(xywh const& region, rgba const& color, char_type ch);\n"
					"}\n"
					"\n"
					, {0}, cpr.make_skip_and_size(s.mouse.x / 20 + 4)
			);
			std::string text{"hello\ta\tab\tabc\tabcd\tabcde\nworld 123 yes or no?\n\t"};
			text += std::to_string(size);
			text += "\t";
			text += std::to_string(a);
			{
				imgui::clip_guard guard(cpr, xywh{100, 100, 400, 200});
				if (guard) {
					cpr.rect(xywh{100, 100, 400, 200}, rgba{0, 0, 255, (uint8_t)(s.mouse.down ? 200 : 100)});
					for (int y=0; y<480; y+=30)
						for (int x=0; x<640; x+=30)
							cpr.text(xywh{x, y, 20, 20}, "hi", rgba{0, 255, 0}, cpr.make_skip_and_size(20));
					cpr.rect(xywh{s.mouse.x+10, s.mouse.y+10, 200, 80}, rgba{255, 255, 100, 200});
					cpr.text(xywh{s.mouse.x+10, s.mouse.y+10, 200, 80}, text, rgba{255, 0, 0, 150}, cpr.make_skip_and_size(size));
				}
			}

			if (a) cpr.rect(xywh{10, 10, 100, 10}, rgba{255, 200});
			a = !a;

			if (size++ > 64) size = 1;
		}

		{
			auto measure = bm_compile.measure();
			cpr.compile();
		}
		auto cmds = cpr.get();

		{
			auto measure = bm_render.measure();
			ctx.render(cmds);
		}

		auto total = bm_input.ms() + bm_compile.ms() + bm_render.ms();
		library::log() << "benchmark input  : " << bm_input  .ms() << " ms\n";
		library::log() << "benchmark compile: " << bm_compile.ms() << " ms\n";
		library::log() << "benchmark render : " << bm_render .ms() << " ms\n";
		library::log() << "benchmark total  : " << total           << " ms\n";

		ctx.update(s);
	}

	library::log() << "main(): leave\n";
}

