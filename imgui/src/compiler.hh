#pragma once
#include "context.hh"
#include "command.hh"

namespace imgui
{
	struct compiler
	{
		using command_list = context::command_list;

		void rect(int x, int y, int w, int h,
				uint32_t r=0xFF, uint32_t g=0xFF, uint32_t b=0xFF, uint32_t a=0xFF)
		{
			add(command::rect(x, y, w, h, r, g, b, a));
		}

		void text(int x, int y, int w, int h, char ch,
				uint32_t r=0, uint32_t g=0, uint32_t b=0, uint32_t a=0)
		{
			add(command::text(x, y, w, h, ch, r, g, b, a));
		}

		void text(int x, int y, int w, int h, char const* s,
				int xskip=1, int yskip=1,
				uint32_t r=0, uint32_t g=0, uint32_t b=0, uint32_t a=0)
		{
			constexpr auto tw = 8;
			constexpr auto th = 16;

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

		void compile()
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

		command_list get()
		{
			return std::move(commands);
		}

	private:
		command_list commands;
		command_list last_commands;

		void add(command const& cmd)
		{
			commands.push_back(cmd);
		}
	};
}

