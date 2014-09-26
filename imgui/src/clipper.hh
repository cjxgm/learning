#pragma once
#include <tuple>
#include <vector>
#include <utility>
#include "quad.hh"
#include "math.hh"

namespace imgui
{
	// you must set the max clip the first time you use this!
	struct clipper
	{
		using value_type = std::pair<bool, xyxy>;
		using value_ref  = value_type &;
		using value_cref = value_type const&;
		using stack_type = std::vector<value_type>;

		clipper() : stack(1) {}

		value_ref   top()       { return *stack. rbegin(); }
		value_cref  top() const { return *stack.crbegin(); }
		xyxy      & get()       { return top().second; }
		xyxy const& get() const { return top().second; }
		void get(xyxy & r) const { r = get(); }
		void set(xyxy const& r) { top() = { true, r }; }
		bool valid() const { return top().first; }

		bool clip(xyxy & r)
		{
			if (!valid()) return false;
			return math::clip(get(), r);
		}

		void pop() { stack.pop_back(); }

		bool push(xyxy & r)
		{
			auto valid = clip(r);
			stack.emplace_back(valid, r);
			return valid;
		}

		bool push(xyxy const& r)
		{
			auto copy = r;
			return push(copy);
		}

	private:
		stack_type stack;
	};
}

