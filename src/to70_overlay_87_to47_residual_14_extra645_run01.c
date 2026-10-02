#pragma force_active on
#include "global.h"
extern void *SpriteResourceCollection_Find(void *, int);
extern void ReplacePlttResObjFromNarc(void *,void *,int,int,int,int);
extern void SpriteTransfer_ReplacePlttData(void *);
extern u8 ov87_021E84C0[];
extern void *Heap_Alloc(int,int);
extern void *ov87_021E7DF8(void *,int,int,int,int,u8);
extern void ov87_021E7FEC(void *,int,u16);
typedef struct {u8 pad[0x130];void *collection;} Work;
typedef struct {u16 a;u8 b,pad;u32 c,d;void *sprite;} Item;
void ov87_021E7F28(Work *w,int index);
Item *ov87_021E7F6C(void *w,int x,int y,volatile int a,volatile u16 b,u8 c,int d,int e);
void ov87_021E7F28(Work *w,int index){void *resource;if(index>=8)GF_AssertFail();resource=SpriteResourceCollection_Find(w->collection,3);ReplacePlttResObjFromNarc(w->collection,resource,0xcf,ov87_021E84C0[index],0,0x7a);SpriteTransfer_ReplacePlttData(resource);}
