#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

typedef struct Block {
    u32 address, size;
    struct Block *prev, *next;
} Block;

Block *NNSi_GfdInitLnkVramBlockPool(Block *, u32);
int NNSi_GfdAddNewFreeBlock(Block **, Block **, u32, u32);

Block *NNSi_GfdInitLnkVramBlockPool(Block *blocks, u32 count) {
    for (u32 i = 0; i < count - 1; i++) {
        blocks[i].next = &blocks[i + 1];
        blocks[i + 1].prev = &blocks[i];
    }
    blocks[0].prev = 0;
    blocks[count - 1].next = 0;
    return blocks;
}

int NNSi_GfdAddNewFreeBlock(Block **list, Block **pool, u32 address, u32 size) {
    Block *b = *pool;
    if (b) {
        *pool = b->next;
    }
    if (b) {
        b->address = address;
        b->size = size;
        b->prev = 0;
        b->next = 0;
        if (*list) {
            (*list)->prev = b;
        }
        b->next = *list;
        b->prev = 0;
        *list = b;
        return 1;
    }
    return 0;
}
