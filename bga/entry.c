#include "util.h"

#define PAGE_ALIGN			(1<<0)
#define MEM_INFO			(1<<1)

#define MAGIC				(0x1badb002)
#define FLAGS				(PAGE_ALIGN | MEM_INFO)
#define CHECKSUM			(-(MAGIC + FLAGS))

typedef struct Header
{
	u32 magic;
	u32 flags;
	u32 checksum;
}
Header;

static UNUSED Header header = {
	.magic = MAGIC,
	.flags = FLAGS,
	.checksum = CHECKSUM,
};


void _start()
{
	void main();
	main();

	while (1);
}

