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

		void compile();

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

