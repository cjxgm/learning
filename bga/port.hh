#pragma once
#include "util.hh"

namespace kernel
{
	namespace port
	{
		template <class T>
		static inline void out(u16 port, T data)
		{
			asm volatile("out %[data], %[port]"
				::
				[data] "a" (data),
				[port] "Nd" (port)
			);
		}

		template <class T>
		static inline T in(u16 port)
		{
			T data;
			asm("in %[port], %[data]"
				: [data] "=a" (data)
				: [port] "Nd" (port)
			);
			return data;
		}
	};
};

