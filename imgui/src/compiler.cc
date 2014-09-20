#include "compiler.hh"

namespace imgui
{
	void compiler::text(int x, int y, int w, int h, char const* s,
			int size, int xskip, int yskip,
			uint32_t r, uint32_t g, uint32_t b, uint32_t a)
	{
		auto tw = size/2;
		auto th = size;

		for (auto cy=y; *s; cy+=th+yskip) {
			if (cy+th > y+h) break;	// too high, stop now

			for (auto cx=x; *s; cx+=tw+xskip) {
				if (cx+tw > x+w) {	// too wide, go newline
					while (*s && *s != '\n') s++;
					if (*s) s++;
					break;
				}
				auto ch = *s++;
				switch (ch) {
					case '\n': goto newline;
					case ' ' : continue;
					default:
						text(cx, cy, tw, th, ch, r, g, b, a);
						break;
				}
			}

		newline:
			continue;
		}
	}

	void compiler::compile()
	{
		// TODO
		// compile the raw bunch of drawing commands
		// into optimized minimum drawing commands

		// detect damaged region (brute force)

		// move away to get space for new commands
		//last_commands = std::move(commands);

		// generate partial update command from last_commands
		// top to bottom, no alpha no update, alpha then down
	}
}

