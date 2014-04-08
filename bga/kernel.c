
#define PAGE_ALIGN			(1<<0)
#define MEM_INFO			(1<<1)

#define MAGIC				(0x1badb002)
#define FLAGS				(PAGE_ALIGN | MEM_INFO)
#define CHECKSUM			(-(MAGIC + FLAGS))

#define UNUSED				__attribute__((unused))
#define PACKED				__attribute__((packed))
#define ANY					(void*)

typedef unsigned int u32;
typedef unsigned char u8;

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

typedef struct Glyph
{
	u8 ch;
	u8 color;
}
PACKED
Glyph;

static Glyph* text = ANY 0xb8000;

void _start()
{
	text[0].color = 0b0100;
	text[0].ch = 'a';
	while (1);
}

