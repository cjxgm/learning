#pragma once
#include <utility>
#include <stdexcept>

namespace imgui
{
	#ifdef assert
		#undef assert
	#endif

	template <class Message=char const*>
	static void assert(bool pred, Message&& msg="unknown error")
	{
		if (!pred) throw std::runtime_error{std::forward<Message>(msg)};
	}
}

