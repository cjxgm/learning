#pragma once

#include "util.h"

#define outb(port, data)	asm("out %%al, %1" :: "a" (port), "N" (data))
#define outw(port, data)	asm("out %%ax, %1" :: "a" (port), "N" (data))
#define outl(port, data)	asm("out %%eax, %1" :: "a" (port), "N" (data))

