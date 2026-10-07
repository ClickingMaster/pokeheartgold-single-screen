#pragma force_active on
#include "global.h"
#include <nitro/code32.h>
typedef struct MBlock {
    u16 signature;
    u16 attribute;
    u32 size;
    struct MBlock *prev;
    struct MBlock *next;
} MBlock;
typedef struct MBlockList { MBlock *head; MBlock *tail; } MBlockList;
typedef struct MRegion { u8 *start; u8 *end; } MRegion;
typedef struct HeapHead { u8 pad[0x20]; u32 attribute; } HeapHead;
typedef struct ExpHeapHead { MBlockList freeBlocks; MBlockList usedBlocks; u16 groupID; u16 feature; } ExpHeapHead;
extern void GetRegionOfMBlock(MRegion *, MBlock *);
extern MBlock *RemoveMBlock(MBlockList *, MBlock *), *InsertMBlock(MBlockList *, MBlock *, MBlock *), *InitMBlock(MRegion *, u16);
#define SET_BITS(data, st, bits, val)                     \
    do {                                                  \
        u32 maskBits = (u32)((1 << (bits)) - 1);          \
        u32 newVal = (u32)(val) & maskBits;               \
        (data) &= ~(maskBits << (st));                    \
        (data) |= newVal << (st);                         \
    } while (0)
static inline void FillAllocMemory(ExpHeapHead *eh, void *address, u32 size) {
    if ((u8)(((HeapHead *)((u8 *)eh - sizeof(HeapHead)))->attribute & 0xff) & 1) {
        MIi_CpuClear32(0, (u32 *)address, size);
    }
}
void *AllocUsedBlockFromFreeBlock(ExpHeapHead *eh, MBlock *block, u8 *address, u32 size, u16 direction);
#define SET_BITS2(data, st, bits, val)                    \
    do {                                                  \
        u32 maskBits = (u32)((1 << (bits)) - 1);          \
        u32 newVal = (u32)(val) & maskBits;               \
        (data) &= ~(maskBits << (st));                    \
        (data) = (data) | (newVal << (st));               \
    } while (0)
#define SET_BITS3(data, st, bits, val)                    \
    do {                                                  \
        u32 maskBits = (u32)((1 << (bits)) - 1);          \
        u32 newVal = (u32)(val) & maskBits;               \
        (data) &= ~(maskBits << (st));                    \
        (data) |= (u16)(newVal << (st));                  \
    } while (0)
#define SET_BITS4(data, st, bits, val)                    \
    do {                                                  \
        u32 maskBits = (u32)((1 << (bits)) - 1);          \
        u32 newVal = (u32)(val) & maskBits;               \
        (data) &= ~(maskBits << (st));                    \
        (data) |= (newVal << (st));                       \
    } while (0)
#define SET_BITS5(data, st, bits, val)                    \
    do {                                                  \
        u16 newVal = (u16)((val) & ((1 << (bits)) - 1));  \
        (data) &= ~(((1 << (bits)) - 1) << (st));         \
        (data) |= newVal << (st);                         \
    } while (0)
#define SET_BITS6(data, st, bits, val)                    \
    do {                                                  \
        u32 maskBits = (u32)((1 << (bits)) - 1);          \
        u32 newVal = (u32)(val) & maskBits;               \
        (data) &= ~(maskBits << (st));                    \
        (data) |= newVal << (st);                         \
    } while (0)
static inline void SetAllocDir(MBlock *b, u16 mode) { SET_BITS(b->attribute, 15, 1, mode); }
static inline void SetGroupID(MBlock *b, u16 id) { SET_BITS(b->attribute, 0, 8, id); }
static inline void SetAlignmentA(MBlock *b, u16 alignment) { b->attribute &= ~(0x7f << 8); b->attribute |= (alignment & 0x7f) << 8; }
// @fn AllocUsedBlockFromFreeBlock
void *AllocUsedBlockFromFreeBlock(ExpHeapHead *eh, MBlock *block, u8 *address, u32 size, u16 direction) {
    MRegion freeRgnT;
    MRegion freeRgnB;
    MBlock *prev;
    u8 *head;
    GetRegionOfMBlock(&freeRgnT, block);
    head = address - sizeof(MBlock);
    freeRgnB.end = freeRgnT.end;
    freeRgnB.start = (u8 *)(size + (u32)address);
    freeRgnT.end = head;
    prev = RemoveMBlock(&eh->freeBlocks, block);
    if (freeRgnT.end - freeRgnT.start < sizeof(MBlock) + 4) {
        freeRgnT.end = freeRgnT.start;
    } else {
        prev = InsertMBlock(&eh->freeBlocks, InitMBlock(&freeRgnT, 0x4652), prev);
    }
    if (freeRgnB.end - freeRgnB.start < sizeof(MBlock) + 4) {
        freeRgnB.start = freeRgnB.end;
    } else {
        InsertMBlock(&eh->freeBlocks, InitMBlock(&freeRgnB, 0x4652), prev);
    }
    FillAllocMemory(eh, freeRgnT.end, freeRgnB.start - freeRgnT.end);
    {
        MBlock *newBlock;
        MRegion region;
        region.start = head;
        region.end = freeRgnB.start;
        newBlock = InitMBlock(&region, 0x5544);
        SetAllocDir(newBlock, direction);
        SetAlignmentA(newBlock, (u16)((u8 *)newBlock - freeRgnT.end));
        SetGroupID(newBlock, eh->groupID);
        InsertMBlock(&eh->usedBlocks, newBlock, eh->usedBlocks.tail);
    }
    return address;
}
