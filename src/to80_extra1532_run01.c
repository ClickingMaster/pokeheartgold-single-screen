#pragma force_active on
#include "global.h"
#pragma force_active on
#define P(p,o) (*(void **)((u8 *)(p)+(o)))
#define U(p,o) (*(u32 *)((u8 *)(p)+(o)))
#define B(p,o) (*((u8 *)(p)+(o)))
extern void ov49_0225BFC4(void *,void *,void *,int,u32);
void ov49_0225B8A8(void *ctx,void *a,u32 index,int b);
void ov49_0225B8A8(void *ctx,void *a,u32 index,int b){GF_ASSERT(index<27);GF_ASSERT(B(ctx,1)==2);if(B(ctx,1)==2)ov49_0225BFC4((u8 *)ctx+8,a,P(ctx,0x14c),b,index);}
