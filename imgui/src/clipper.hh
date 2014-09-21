#pragma once
#include <tuple>
#include <vector>

namespace imgui
{
	// you must set the max clip the first time you use this!
	struct clipper
	{
		using xyxy = std::tuple<int, int, int, int>;
		using xyxy_ref = xyxy &;
		using xyxy_cref = xyxy const&;
		using stack_type = std::vector<xyxy>;

		clipper() : stack(1) {}

		void set(int x1, int y1, int x2, int y2);
		void set(int const clip[4])
		{
			set(
				clip[0],
				clip[1],
				clip[2],
				clip[3]);
		}

		xyxy_ref  get()       { return *stack. rbegin(); }
		xyxy_cref get() const { return *stack.crbegin(); }
		void get(int clip[4]) const
		{
			std::tie(
				clip[0],
				clip[1],
				clip[2],
				clip[3]) = get();
		}

		void push(int x1, int y1, int x2, int y2);
		void push(int const clip[4])
		{
			push(
				clip[0],
				clip[1],
				clip[2],
				clip[3]);
		}

		void pop() { stack.pop_back(); }

	private:
		stack_type stack;

		static void clip(xyxy boundary,
				int& x1, int& y1, int& x2, int& y2);
	};
}

