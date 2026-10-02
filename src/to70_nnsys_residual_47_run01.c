#pragma force_active on
#include <nitro/types.h>
#include <nitro/code32.h>
typedef struct Heap {u32 link[2];void *heap,*owner;u32 index;u8 data[1];} Heap;
extern u8 _021DFDC4[];
void *NNS_SndHeapAlloc(void*,u32,void(*)(Heap*),u32,u32);void *NNS_SndHeapCreate(void*,u32);void PlayerHeapDisposeCallback(Heap*);void NNS_FndAppendListObject(void*,void*);
int NNS_SndPlayerCreateHeap(int,void*,u32);
int NNS_SndPlayerCreateHeap(int index,void *parent,u32 size){Heap *h=NNS_SndHeapAlloc(parent,size+20,PlayerHeapDisposeCallback,0,0);void *sub;if(!h)return 0;h->owner=0;h->index=index;h->heap=0;sub=NNS_SndHeapCreate(h->data,size);if(!sub)return 0;h->heap=sub;NNS_FndAppendListObject(_021DFDC4+index*36+12,h);return 1;}
