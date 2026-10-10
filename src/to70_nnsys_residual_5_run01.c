#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#pragma force_active on
#define U32(p, o) (*(u32 *)((u8 *)(p) + (o)))
#define U16(p, o) (*(u16 *)((u8 *)(p) + (o)))
extern void GetRegionOfMBlock(void *, void *);
extern void *RemoveMBlock(void *, void *);
extern int RecycleRegion(void *, void *);
extern void *InitMBlock(void *, u16);
extern void InsertMBlock(void *, void *, void *);
u16 NNS_FndSetGroupIDForExpHeap(NNSFndHeapHandle, u16);
u16 NNS_FndGetGroupIDForMBlockExpHeap(const void *);

void NNS_FndFreeToExpHeap(NNSFndHeapHandle heap, void *mem) {
    u32 region[2];
    void *block = (u8 *)mem - 16;
    GetRegionOfMBlock(region, block);
    RemoveMBlock((u8 *)heap + 0x2c, block);
    RecycleRegion((u8 *)heap + 0x24, region);
}

u32 NNS_FndGetTotalFreeSizeForExpHeap(NNSFndHeapHandle heap) {
    void *block = (void *)U32(heap, 0x24);
    u32 size = 0;
    while (block) {
        size += U32(block, 4);
        block = (void *)U32(block, 12);
    }
    return size;
}

u16 NNS_FndSetGroupIDForExpHeap(NNSFndHeapHandle heap, u16 id) {
    u16 old = U16(heap, 0x34);
    U16(heap, 0x34) = id;
    return old;
}

u32 NNS_FndGetSizeForMBlockExpHeap(const void *mem) {
    return U32(mem, -12);
}

u16 NNS_FndGetGroupIDForMBlockExpHeap(const void *mem) {
    return U16(mem, -14) & 255;
}
