#include <iterator>
#include "compiler.hh"

namespace imgui
{
	void compiler::text(int x, int y, int w, int h, char const* s,
			int size, int xskip, int yskip,
			uint32_t r, uint32_t g, uint32_t b, uint32_t a)
	{
		clip_guard _(*this, x, y, w, h);

		auto tw = size/2;
		auto th = size;

		for (auto cy=y; *s; cy+=th+yskip) {
			if (cy > y+h) break;	// too high, stop now

			for (auto cx=x, i=0; *s; cx+=tw+xskip, i++) {
				if (cx > x+w) {	// too wide, go newline
					while (*s && *s != '\n') s++;
					if (*s) s++;
					break;
				}
				auto ch = *s++;
				switch (ch) {
					case '\n': goto newline;
					case '\t': {
						auto ni = (((i >> 2) + 1) << 2) - 1;
						cx += (tw+xskip) * (ni-i);
						i = ni;
						continue;
					}
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
		// FIXME
		clip(cl);
		commands.splice(commands.begin(), commands, std::prev(commands.end()));

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

