#pragma force_active on
#include "global.h"
#pragma force_active on
#define P(p,o) (*(void **)((u8 *)(p)+(o)))
#define U(p,o) (*(u32 *)((u8 *)(p)+(o)))
#define B(p,o) (*((u8 *)(p)+(o)))
extern void *ov40_0222F950(void *,int,int,int); extern void ManagedSprite_SetPositionXY(void *,int,int);
void ov40_0222F9E0(void *ctx,int a,int b);
void ov40_0222F9E0(void *ctx,int a,int b){P(ctx,0x18)=ov40_0222F950(ctx,a,2,b);P(ctx,0x1c)=ov40_0222F950(ctx,a,2,b);ManagedSprite_SetPositionXY(P(ctx,0x18),24,136);ManagedSprite_SetPositionXY(P(ctx,0x1c),232,136);}
