#include <nitro/types.h>

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

void GetRegionOfMBlock(MRegion *, MBlock *);
MBlock *RemoveMBlock(MBlockList *, MBlock *);
MBlock *InsertMBlock(MBlockList *, MBlock *, MBlock *);
MBlock *InitMBlock(MRegion *, u16);
void NNSi_FndFinalizeHeap(void *);
void NNS_FndDestroyExpHeap(void *);
u32 NNS_FndGetTotalFreeSizeForExpHeap(ExpHeap *);
u16 NNS_FndSetGroupIDForExpHeap(ExpHeap *, u16);
u32 NNS_FndGetSizeForMBlockExpHeap(const void *);
u32 NNS_FndGetGroupIDForMBlockExpHeap(const void *);
void *NNS_FndAllocFromExpHeapEx(ExpHeap *, u32, int);
void *AllocFromHead__ExpHeap(ExpHeap *, u32, int);
void *AllocFromTail__ExpHeap(ExpHeap *, u32, int);

void NNS_FndDestroyExpHeap(void *heap) {
    NNSi_FndFinalizeHeap(heap);
}

void *NNS_FndAllocFromExpHeapEx(ExpHeap *heap, u32 size, int alignment) {
    if (size == 0) {
        size = 1;
    }
    size = (size + 3) & ~3;
    if (alignment >= 0) {
        return AllocFromHead__ExpHeap(heap, size, alignment);
    }
    return AllocFromTail__ExpHeap(heap, size, -alignment);
}

extern u32 _021D84B0;
extern u8 _021D84B4[];
extern void *NNS_FndGetNextListObject(void *, void *);
extern void NNS_FndInitList(void *, u16);
extern void NNS_FndAppendListObject(void *, void *);
extern void NNS_FndRemoveListObject(void *, void *);
void *FindContainHeap(void *, void *);
void *FindListContainHeap(void *);
void NNSi_FndFinalizeHeap(void *);
ExpHeap *InitExpHeap(void *, void *, u16);
ExpHeap *NNS_FndCreateExpHeapEx(void *, u32, u16);
void NNS_FndFreeToExpHeap(ExpHeap *, void *);
void RecycleRegion(MBlockList *, MRegion *);
