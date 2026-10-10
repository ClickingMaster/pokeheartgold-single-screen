#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

typedef struct Block {
    u16 magic, flags;
    u32 size;
    struct Block *prev, *next;
} Block;

typedef struct {
    Block *head, *tail;
} List;

typedef struct {
    u8 pad[0x20];
    u32 flags;
    List free;
} Heap;

typedef struct {
    u8 *start, *end;
} Region;

extern void GetRegionOfMBlock(Region *, Block *);
extern Block *RemoveMBlock(List *, Block *);
extern Block *InitMBlock(Region *, int);
extern void InsertMBlock(List *, Block *, Block *);
extern int RecycleRegion(List *, Region *);
u32 NNS_FndResizeForMBlockExpHeap(NNSFndHeapHandle, void *, u32);
#pragma force_active on

u32 NNS_FndResizeForMBlockExpHeap(NNSFndHeapHandle handle, void *memory, u32 size) {
    Heap *heap = (Heap *)handle;
    Region grow, shrink;
    u8 *data = memory;
    Block *free;
    u32 old;
    Block *block = (Block *)(data - 16);
    u8 *start;
    Block *prev;
    Block *adjacent;
    old = block->size;
    size = (size + 3) & ~3;
    if (size == old) {
        return size;
    }
    if (size > old) {
        free = heap->free.head;
        adjacent = (Block *)((u8 *)(old + ((u32)block + 16)));
        if (free) {
            do {
                if (free == adjacent) {
                    break;
                }
                free = free->next;
            } while (free);
        }
        if (!free || size > old + 16 + free->size) {
            return 0;
        }
        GetRegionOfMBlock(&grow, free);
        prev = RemoveMBlock(&heap->free, free);
        {
            start = grow.start;
            grow.start = (u8 *)(size + (u32)data);
            if ((u32)(grow.end - grow.start) < 16) {
                grow.start = grow.end;
            }
            block->size = grow.start - data;
            if ((u32)(grow.end - grow.start) >= 16) {
                InsertMBlock(&heap->free, InitMBlock(&grow, 0x4652), prev);
            }
            {
                u32 delta = grow.start - start;
                if (((u8)heap->flags) & 1) {
                    MIi_CpuClear32(0, (u32 *)start, delta);
                }
            }
        }
    } else {
        shrink.start = (u8 *)(size + (u32)data);
        shrink.end = (u8 *)(block->size + ((u32)block + 16));
        block->size = size;
        if (!RecycleRegion(&heap->free, &shrink)) {
            block->size = old;
        }
    }
    return block->size;
}
