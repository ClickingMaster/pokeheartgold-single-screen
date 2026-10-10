#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#pragma force_active on

typedef struct MBlock {
    u16 signature;
    u16 attributes;
    u32 size;
    struct MBlock *prev;
    struct MBlock *next;
} MBlock;

typedef struct MBlockList {
    MBlock *head;
    MBlock *tail;
} MBlockList;

typedef struct MRegion {
    u8 *start;
    u8 *end;
} MRegion;

typedef struct ExpHeap {
    u8 pad[0x24];
    MBlockList freeBlocks;
    MBlockList usedBlocks;
    u16 groupId;
    u16 feature;
} ExpHeap;

extern void GetRegionOfMBlock(MRegion *, MBlock *);
extern MBlock *RemoveMBlock(MBlockList *, MBlock *);
extern MBlock *InsertMBlock(MBlockList *, MBlock *, MBlock *);
extern MBlock *InitMBlock(MRegion *, u16);
extern void NNSi_FndInitHeapHead(void *, u32, void *, void *, u32);
ExpHeap *InitExpHeap(void *start, void *end, u16 opt);
void *AllocUsedBlockFromFreeBlock(MBlockList *list, MBlock *block, u8 *address, u32 size, u16 direction);
void *AllocFromHead__ExpHeap(ExpHeap *heap, u32 size, int align);
void *AllocFromTail__ExpHeap(ExpHeap *heap, u32 size, int align);
BOOL RecycleRegion(MBlockList *list, MRegion *input);
NNSFndHeapHandle NNS_FndCreateExpHeapEx(void *start, u32 size, u16 opt);
static inline u8 HeapFlags(MBlockList *list);

typedef struct AttrFlags {
    u32 flags : 8;
    u32 reserved : 24;
} AttrFlags;

static inline u8 HeapFlags(MBlockList *list) {
    return *(u32 *)((u8 *)list - 4);
}

static inline u8 *BlockEnd(MBlock *b);

static inline u8 *BlockEnd(MBlock *b) {
    u32 size = b->size;
    u8 *start = (u8 *)(b + 1);
    return (u8 *)(size + (u32)start);
}

void *AllocFromHead__ExpHeap(ExpHeap *heap, u32 size, int align) {
    BOOL first = (u16)(heap->feature & 1) == 0;
    MBlock *block = heap->freeBlocks.head, *best = 0;
    u32 min = ~0u;
    u8 *address = 0;
    align -= 1;
    while (block) {
        u8 *start = (u8 *)(block + 1);
        u8 *aligned = (u8 *)((align + (u32)start) & ~align);
        u32 padding = aligned - start;
        u32 available = block->size;
        if (available >= size + padding && min > available) {
            best = block;
            min = available;
            address = aligned;
            if (first) {
                break;
            }
            if (available == size) {
                break;
            }
        }
        block = block->next;
    }
    if (!best) {
        return 0;
    }
    return AllocUsedBlockFromFreeBlock(&heap->freeBlocks, best, address, size, 0);
}
