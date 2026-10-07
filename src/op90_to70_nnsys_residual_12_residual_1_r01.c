#pragma force_active on
#include "global.h"
#include <nitro/code32.h>
typedef struct Block { u32 address, size; struct Block *prev, *next; } Block;
int NNSi_GfdAllocLnkVramAligned(Block **freeList, Block **pool, u32 *pAddr, u32 size, u32 alignment);
static inline Block *GetNewBlock_(Block **pool) {
    Block *b = *pool;
    if (b) *pool = b->next;
    return b;
}
static inline void InitBlock_(Block *b, u32 address, u32 size) {
    b->address = address;
    b->size = size;
    b->prev = NULL;
    b->next = NULL;
}
static inline void InsertBlock_(Block **list, Block *b) {
    if (*list) (*list)->prev = b;
    b->next = *list;
    b->prev = NULL;
    *list = b;
}
static inline void RemoveBlock_(Block **list, Block *b) {
    Block *prev = b->prev;
    Block *next = b->next;
    if (prev) prev->next = next;
    else *list = next;
    if (next) next->prev = prev;
}
// @fn NNSi_GfdAllocLnkVramAligned
int NNSi_GfdAllocLnkVramAligned(Block **freeList, Block **pool, u32 *pAddr, u32 size, u32 alignment) {
    u32 alignedAddr, need, gap;
    Block *found = NULL;
    Block *it = *freeList;
    while (it) {
        if (alignment > 1) {
            alignedAddr = (it->address + (alignment - 1)) & ~(alignment - 1);
            gap = alignedAddr - it->address;
            need = size + gap;
        } else {
            alignedAddr = it->address;
            gap = 0;
            need = size;
        }
        if (it->size >= need) {
            found = it;
            break;
        }
        it = it->next;
    }
    if (found) {
        if (gap != 0) {
            Block *b = GetNewBlock_(pool);
            if (!b) goto fail;
            InitBlock_(b, found->address, gap);
            InsertBlock_(freeList, b);
        }
        found->size -= need;
        found->address += need;
        if (found->size == 0) {
            RemoveBlock_(freeList, found);
            InsertBlock_(pool, found);
        }
        *pAddr = alignedAddr;
        return 1;
    }
fail:
    *pAddr = 0;
    return 0;
}
