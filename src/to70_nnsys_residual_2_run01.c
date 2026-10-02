#pragma force_active on
#include <nitro/types.h>
#include <nitro/code32.h>
typedef struct Heap {u32 signature;u8 link[8];u8 children[12];void *start,*end;union {u32 value;struct{u32 flags:8;u32 other:24;}bits;} attribute;} Heap;
extern int _021D84B0;extern u8 _021D84B4[];
void NNS_FndInitList(void*,u16);void *FindListContainHeap(Heap*);void NNS_FndAppendListObject(void*,void*);
void NNSi_FndInitHeapHead(Heap*,u32,void*,void*,u16);
void NNSi_FndInitHeapHead(Heap*h,u32 signature,void *start,void *end,u16 flags){h->signature=signature;h->start=start;h->end=end;h->attribute.value=0;h->attribute.bits.flags=flags;NNS_FndInitList(h->children,4);if(!_021D84B0){NNS_FndInitList(_021D84B4,4);_021D84B0=1;}NNS_FndAppendListObject(FindListContainHeap(h),h);}
