#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

typedef struct Block {
    u16 magic, flags;
    u32 size, pad[2];
} Block;

void GetRegionOfMBlock(void **, Block *);

void GetRegionOfMBlock(void **out, Block *b) {
    out[0] = (u8 *)b - (u16)((b->flags >> 8) & 127);
    out[1] = (void *)(b->size + (u32)(b + 1));
}
