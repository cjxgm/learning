#include <iterator>
#include <algorithm>
#include "compiler.hh"
#include "merger.hh"
#include "log.hh"
#include "math.hh"
#include "benchmark.hh"

static imgui::benchmark bm_damage;
static imgui::benchmark bm_merge;
static imgui::benchmark bm_optimize;

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


	// compile the raw bunch of drawing commands
	// into optimized minimum drawing commands
	void compiler::compile()
	{
		auto cmds = std::move(commands);

		// detect damaged region (brute force)
		merger m;
		{
			auto measure = bm_damage.measure();

			for (auto& cmd: cmds) {
				auto it = last.find(cmd);
				if (it != last.end())
					last.erase(it);
				else m.add(cmd.clip);
			}
			for (auto& pair: last)
				m.add(pair.first.clip);
		}
		{
			auto measure = bm_merge.measure();
			m.merge();
		}


		//library::log() << "update:\n";
		//for (auto& rect: m)
		//	library::log() << "\t"
		//		<< rect[0] << ", "
		//		<< rect[1] << ", "
		//		<< rect[2] << ", "
		//		<< rect[3] << "\n";


		// move away to get space for new commands
		last.clear();
		for (auto& cmd: cmds) last[cmd] = true;


		// generate partial update command from last_commands
		// TODO: top to bottom, no alpha no update, alpha then down
		{
			auto measure = bm_optimize.measure();
			for (auto& rect: m) compile(rect, cmds);
		}

		library::log() << "benchmark compiler damage  : " << bm_damage  .ms() << " ms\n";
		library::log() << "benchmark compiler merge   : " << bm_merge   .ms() << " ms\n";
		library::log() << "benchmark compiler optimize: " << bm_optimize.ms() << " ms\n";
	}

	// recompile those clipped by "clip"
	void compiler::compile(xyxy const& clip, command_list const& cmds)
	{
		clip_guard guard(*this, clip);
		if (!guard) return;
		for (auto& cmd: cmds) add(cmd);
	}
}

