#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

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

void *AllocFromTail__ExpHeap(ExpHeap *heap, u32 size, int align);

void *AllocFromTail__ExpHeap(ExpHeap *heap, u32 size, int align) {
    BOOL first = (u16)(heap->feature & 1) == 0;
    MBlock *block = heap->freeBlocks.tail, *best = 0;
    u32 min = ~0u;
    u8 *address = 0;
    while (block) {
        u8 *start = (u8 *)(block + 1);
        u8 *end = (u8 *)(block->size + (u32)start);
        u8 *aligned = (u8 *)((u32)(end - size) & ~(align - 1));
        if ((s32)(aligned - start) >= 0 && min > block->size) {
            best = block;
            min = block->size;
            address = aligned;
            if (first) {
                break;
            }
            if (block->size == size) {
                break;
            }
        }
        block = block->prev;
    }
    if (!best) {
        return 0;
    }
    return AllocUsedBlockFromFreeBlock(&heap->freeBlocks, best, address, size, 1);
}
