#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#define U32(p,o) (*(u32*)((u8*)(p)+(o)))
#define U16(p,o) (*(u16*)((u8*)(p)+(o)))
#define PTR(p,o) (*(void**)((u8*)(p)+(o)))
typedef struct{int*current,*begin,*pivot,*end;} Iter;
int ov45_0223CDBC(Iter*,Iter*);
int ov45_0223CDBC(Iter*a,Iter*b){int*pivot;int*other;int*current;int count;pivot=a->pivot;other=b->current;current=a->current;count=current-other;if(current>=pivot){if(other<pivot)count-=a->end-a->begin;}else{if(other>=pivot)count+=a->end-a->begin;}return count;}
