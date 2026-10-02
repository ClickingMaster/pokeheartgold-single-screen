#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
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


extern void GetRegionOfMBlock(MRegion*,MBlock*);
extern MBlock *RemoveMBlock(MBlockList*,MBlock*),*InsertMBlock(MBlockList*,MBlock*,MBlock*),*InitMBlock(MRegion*,u16);
extern void NNSi_FndInitHeapHead(void*,u32,void*,void*,u32);
ExpHeap *InitExpHeap(void *start,void *end,u16 opt);
void *AllocUsedBlockFromFreeBlock(MBlockList *list,MBlock *block,u8 *address,u32 size,u16 direction);
void *AllocFromHead__ExpHeap(ExpHeap *heap,u32 size,int align);
void *AllocFromTail__ExpHeap(ExpHeap *heap,u32 size,int align);
BOOL RecycleRegion(MBlockList *list,MRegion *input);
NNSFndHeapHandle NNS_FndCreateExpHeapEx(void *start,u32 size,u16 opt);
static inline u8 HeapFlags(MBlockList *list);
typedef struct AttrFlags {u32 flags:8;u32 reserved:24;} AttrFlags;
static inline u8 HeapFlags(MBlockList *list){return *(u32*)((u8*)list-4);}
static inline u8 *BlockEnd(MBlock *b);
static inline u8 *BlockEnd(MBlock *b){u32 size=b->size;u8 *start=(u8*)(b+1);return (u8*)(size+(u32)start);}
ExpHeap *InitExpHeap(void *start,void *end,u16 opt){ExpHeap *heap=start;MRegion region;MBlock *block;NNSi_FndInitHeapHead(heap,0x45585048,(u8*)heap+0x38,end,opt);heap->groupId=0;heap->feature=0;heap->feature&=~1;region.start=*(u8**)((u8*)heap+0x18);region.end=*(u8**)((u8*)heap+0x1c);block=InitMBlock(&region,0x4652);heap->freeBlocks.head=block;heap->freeBlocks.tail=block;heap->usedBlocks.head=0;heap->usedBlocks.tail=0;return heap;}
