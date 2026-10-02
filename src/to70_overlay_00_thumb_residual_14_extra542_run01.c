#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {u8 p[0xf98];void *buffer,*heap;u32 size;u8 q[0x1084-0xfa4];int old,current;} State;
extern State *_0221A680;extern void ov00_021EE448(void);extern void *Heap_Alloc(int,u32);extern void Heap_Free(void *),sub_02039AD8(int);
extern u16 NNS_FndSetGroupIDForExpHeap(NNSFndHeapHandle,u16);
void ov00_021E7144(void);void ov00_021E714C(int,int);
extern u16 NNS_FndSetGroupIDForExpHeap(NNSFndHeapHandle,u16);
void ov00_021E7144(void){ov00_021EE448();}
void ov00_021E714C(int enable,int heap){State *s;if(enable){_0221A680->current=11;if(_0221A680->buffer==0){_0221A680->buffer=Heap_Alloc(heap,0xf020);_0221A680->heap=NNS_FndCreateExpHeapEx((void*)(((u32)_0221A680->buffer+31)&~31),0xf000,0);NNS_FndSetGroupIDForExpHeap(_0221A680->heap,16);_0221A680->size=NNS_FndGetTotalFreeSizeForExpHeap(_0221A680->heap);}}else{_0221A680->current=_0221A680->old;s=_0221A680;if(s->buffer){if(s->size!=NNS_FndGetTotalFreeSizeForExpHeap(s->heap)){sub_02039AD8(1);return;}NNS_FndDestroyExpHeap(s->heap);Heap_Free(_0221A680->buffer);_0221A680->buffer=0;}}}
