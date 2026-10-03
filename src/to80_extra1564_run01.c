#pragma force_active on
#include "global.h"
#pragma force_active on
#define P(p,o) (*(void **)((u8 *)(p)+(o)))
#define U(p,o) (*(u32 *)((u8 *)(p)+(o)))
#define B(p,o) (*((u8 *)(p)+(o)))
extern void *SysTask_CreateOnMainQueue(void (*)(void *,void *),void *,int);extern void ov85_021E7CE0(void *,void *);
void ov85_021E7D08(void *ctx);
void ov85_021E7D08(void *ctx){memset((u8 *)ctx+0xab4,0,0xa0);P(ctx,0xdb0)=SysTask_CreateOnMainQueue(ov85_021E7CE0,ctx,256);GF_ASSERT(P(ctx,0xdb0)!=0);}
