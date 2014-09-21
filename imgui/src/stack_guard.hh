#pragma once
// a stack-like's guard
// push on construction
// pop  on  destruction
#include <utility>

namespace imgui
{
	template <class T>
	struct stack_guard
	{
		using value_type = T;
		using value_ref  = value_type &;
		using self_type = stack_guard;
		using self_ref  = self_type &;
		using self_cref = self_type const&;
		using self_rref = self_type &&;

		template <class ...Args>
		stack_guard(value_ref v, Args&& ...args)
			: value{v}, valid{value.push(std::forward<Args>(args)...)} {}

		~stack_guard() { value.pop(); }

		stack_guard(self_cref) = delete;
		stack_guard(self_rref) = delete;
		self_ref operator = (self_cref) = delete;
		self_ref operator = (self_rref) = delete;

		operator bool () const { return valid; }

	private:
		value_ref value;
		bool valid;
	};
}

