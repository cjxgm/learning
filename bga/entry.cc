#include "util.hh"
#include "main.hh"

namespace kernel
{
	namespace multiboot
	{
		namespace Header
		{
			namespace Flag
			{
				constexpr u32 page_align  = 1<<0;
				constexpr u32 memory_info = 1<<1;
			};

			static constexpr UNUSED u32 magic = 0x1badb002;
			static constexpr UNUSED u32 flags = Flag::page_align | Flag::memory_info;
			static constexpr UNUSED u32 checksum = -(magic + flags);
		};
	};
};

extern "C"
{
	void _start()
	{
		kernel::main();
		while (1);
	}
}

