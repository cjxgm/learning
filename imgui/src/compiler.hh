#pragma once
#include "context.hh"
#include "command.hh"
#include "clipper.hh"
#include "stack_guard.hh"

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

		void clip(int x, int y, int w, int h)
		{
			add(command::clip(x, y, w, h));
		}

		void clip(clipper const& cl)
		{
			int xyxy[4];
			cl.get(xyxy);
			if (xyxy[2] < xyxy[0]) xyxy[2] = xyxy[0];
			if (xyxy[3] < xyxy[1]) xyxy[3] = xyxy[1];
			add(command::clip(
						xyxy[0], xyxy[1],
						xyxy[2]-xyxy[0], xyxy[3]-xyxy[1]));
		}

		// for stack_guard
		void push(int x, int y, int w, int h)
		{
			cl.push(x, y, x+w, y+h);
			clip(cl);
		}
		void pop()
		{
			cl.pop();
			clip(cl);
		}

		void text(int x, int y, int w, int h, char ch,
				uint32_t r=0, uint32_t g=0, uint32_t b=0, uint32_t a=0xFF)
		{
			add(command::text(x, y, w, h, ch, r, g, b, a));
		}

		void text(int x, int y, int w, int h, char const* s,
				int size=16, int xskip=1, int yskip=1,
				uint32_t r=0, uint32_t g=0, uint32_t b=0, uint32_t a=0xFF);

		void text(int x, int y, int w, int h, std::string const& s,
				int size=16, int xskip=1, int yskip=1,
				uint32_t r=0, uint32_t g=0, uint32_t b=0, uint32_t a=0xFF)
		{
			text(x, y, w, h, s.c_str(), size, xskip, yskip, r, g, b, a);
		}

		void size(int w, int h)
		{
			cl.set(0, 0, w, h);
		}

		void compile();

		command_list get()
		{
			return std::move(commands);
		}

	private:
		command_list commands;
		command_list last_commands;
		clipper cl;

		void add(command const& cmd)
		{
			commands.push_back(cmd);
		}
	};

	using clip_guard = stack_guard<compiler>;
}

