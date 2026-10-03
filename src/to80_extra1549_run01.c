#pragma force_active on
#pragma force_active on
#include "global.h"
#define I(p,o) (*(int*)((u8*)(p)+(o)))
#define U(p,o) (*(u32*)((u8*)(p)+(o)))
#define H(p,o) (*(u16*)((u8*)(p)+(o)))
#define B(p,o) (*((u8*)(p)+(o)))
#define V(p,o) (*(void**)((u8*)(p)+(o)))
#include "heap.h"
extern void ov75_02249780(void*,void*,enum HeapID),sub_0203175C(void*,void*);void ov75_02246C3C(u8*);
void ov75_02246C3C(u8*p){void*temp=Heap_Alloc((enum HeapID)115,100);ov75_02249780(V(p,272),temp,(enum HeapID)115);sub_0203175C(V(p,4),temp);Heap_Free(temp);}
