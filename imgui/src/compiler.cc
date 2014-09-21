#include <iterator>
#include <algorithm>
#include "compiler.hh"
#include "merger.hh"
#include "log.hh"
#include "math.hh"

namespace imgui
{
	void compiler::text(
			xyxy const& clip,
			char const* s,
			rgba const& color,
			xywh const& skip_and_size)
	{
		clip_guard guard(*this, clip);
		if (!guard) return;

		auto xskip = skip_and_size.x;
		auto yskip = skip_and_size.y;
		auto xsize = skip_and_size.w;
		auto ysize = skip_and_size.h;
		auto w = xsize + xskip;
		auto h = ysize + yskip;

		for (auto cy=clip.y1; *s; cy+=h) {
			// exceeds bottom boundary, stop now
			if (cy > clip.y2) break;

			for (auto cx=clip.x1, i=0; *s; cx+=w, i++) {
				// exceeds right boundary, jump to next line
				if (cx > clip.x2) {
					while (*s && *s != '\n') s++;
					if (*s) s++;
					break;
				}

				auto ch = *s++;
				switch (ch) {
					case '\n': goto next_line;
					case '\t':
						i = (((i >> 2) + 1) << 2) - 1;	// will "++" in the end
						cx = clip.x1 + w * i;
						continue;
					case ' ' : continue;
					default:
						text(xywh{ cx, cy, xsize, ysize }, ch, color);
						break;
				}
			}

		next_line:
			continue;
		}
	}


	void compiler::compile()
	{
		// compile the raw bunch of drawing commands
		// into optimized minimum drawing commands


		// detect damaged region (brute force)
		merger m;
		for (auto& cmd: commands) {
			auto it = std::find(
					last_commands.begin(),
					last_commands.end(),
					cmd);
			if (it != last_commands.end())
				last_commands.erase(it);
			else m.add(cmd.clip);
		}
		for (auto& cmd: last_commands)
			m.add(cmd.clip);


		library::log() << "update:\n";
		for (auto& rect: m)
			library::log() << "\t"
				<< rect[0] << ", "
				<< rect[1] << ", "
				<< rect[2] << ", "
				<< rect[3] << "\n";


		// move away to get space for new commands
		last_commands = std::move(commands);


		// generate partial update command from last_commands
		// TODO: top to bottom, no alpha no update, alpha then down
		for (auto& rect: m) compile(rect, last_commands);
	}

	// recompile those clipped by "clip"
	void compiler::compile(xyxy const& clip, command_list const& cmds)
	{
		clip_guard guard(*this, clip);
		if (!guard) return;
		for (auto& cmd: cmds) add(cmd);
	}
}

