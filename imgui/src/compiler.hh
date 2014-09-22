#pragma once
#include <string>
#include <utility>
#include <unordered_map>
#include "context.hh"
#include "command.hh"
#include "clipper.hh"
#include "stack_guard.hh"
#include "quad.hh"


namespace imgui
{
	struct compiler
	{
		using clip_guard = stack_guard<compiler>;
		using command_list = context::command_list;
		using command_table = std::unordered_map<command, bool, command_hash>;

		void rect(
				xyxy const& clip,
				rgba const& color={0xFF})
		{
			add(command::rect(clip, color));
		}

		void text(
				xyxy const& clip,
				char ch,
				rgba const& color={0})
		{
			add(command::text(clip, color, clip, ch));
		}

		void text(
				xyxy const& clip,
				char const* s,
				rgba const& color={0},
				xywh const& skip_and_size = { 1, 1, 8, 16 });

		void text(
				xyxy const& clip,
				std::string const& s,
				rgba const& color={0},
				xywh const& skip_and_size = { 1, 1, 8, 16 })
		{
			text(clip, s.c_str(), color, skip_and_size);
		}

		static xywh make_skip_and_size(
				int xskip, int yskip,
				int xsize, int ysize)
		{
			return { xskip, yskip, xsize, ysize };
		}

		static xywh make_skip_and_size(int xskip, int yskip, int size)
		{
			return make_skip_and_size(xskip, yskip, size>>1, size);
		}

		static xywh make_skip_and_size(int skip, int size)
		{
			return make_skip_and_size(skip, skip, size);
		}

		static xywh make_skip_and_size(int size)
		{
			return make_skip_and_size(1, size);
		}

		static xywh make_skip_and_size()
		{
			return make_skip_and_size(1, 16);
		}

		void size(int w, int h) { cl.set(xywh{0, 0, w, h}); }
		void compile();
		command_list get() { return std::move(commands); }

	private:
		command_list commands;
		command_table last;
		clipper cl;

		void add(command cmd)
		{
			if (!cmd.color.visible()) return;
			if (!cl.clip(cmd.clip)) return;
			commands.emplace_back(std::move(cmd));
		}

		bool push(xyxy      & r) { return cl.push(r); }
		bool push(xyxy const& r) { return cl.push(r); }
		void pop() { cl.pop(); }
		friend clip_guard;

		void compile(xyxy const& clip, command_list const& cmds);
	};

	using clip_guard = compiler::clip_guard;
}

