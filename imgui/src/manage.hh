#pragma once
#include "assert.hh"
#include <memory>
#include <utility>

namespace imgui
{
	template <class T, class Deleter, class Message=char const*>
	static std::unique_ptr<T, Deleter> manage(T* obj, Deleter&& deleter, Message&& msg="bad object: got nullptr")
	{
		assert(obj, std::forward<Message>(msg));
		return { obj, std::forward<Deleter>(deleter) };
	}

	template <class T>
	struct managed_ptr
	{
		using value_type = T;
		using value_cptr = value_type const*;
		using value_ptr  = value_type *;
		using deleter_type = void (value_ptr);
		using deleter_ptr  = deleter_type *;
		using value_uptr = std::unique_ptr<value_type, deleter_ptr>;

		template <class Message=char const*>
		void manage(value_ptr x, deleter_ptr d,
				Message&& msg="unexpected nullptr")
		{
			assert(x, std::forward<Message>(msg));
			value = { x, d };
		}

		value_ptr  get()       { return value.get(); }
		value_cptr get() const { return value.get(); }

		operator value_ptr  ()       { return get(); }
		operator value_cptr () const { return get(); }
		operator bool       () const { return get(); }

	private:
		value_uptr value{{}, [](value_ptr){}};
	};
}

