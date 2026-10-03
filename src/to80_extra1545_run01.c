#pragma force_active on
#include "global.h"
#pragma force_active on
#define P(p,o) (*(void **)((u8 *)(p)+(o)))
#define U(p,o) (*(u32 *)((u8 *)(p)+(o)))
#define B(p,o) (*((u8 *)(p)+(o)))
extern void ov72_0223A350(void *,void *,int,int,int);extern void ov72_02238680(void *,int,int);
int ov72_02239FD8(void *ctx);
int ov72_02239FD8(void *ctx){ov72_0223A350(ctx,P(ctx,0xbdc),27,1,0xf0f);ov72_02238680(ctx,49,46);U(ctx,0xf60)=0;return 3;}
