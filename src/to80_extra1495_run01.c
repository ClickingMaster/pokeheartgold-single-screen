#pragma force_active on
#include "global.h"
#pragma force_active on
#define P(p,o) (*(void **)((u8 *)(p)+(o)))
#define U(p,o) (*(u32 *)((u8 *)(p)+(o)))
#define B(p,o) (*((u8 *)(p)+(o)))
extern void *ov74_0222B0C8(void *,int);extern void Sprite_TryChangeAnimSeq(void *,int);extern void Sprite_SetAnimationFrame(void *,int);
void ov74_0222B30C(void *ctx);
void ov74_0222B30C(void *ctx){int i=0;u8 *p=ctx;for(;i<80;i++,p+=0x4c){P(p,0x1854)=ov74_0222B0C8(ctx,2);Sprite_TryChangeAnimSeq(P(p,0x1854),5);Sprite_SetAnimationFrame(P(p,0x1854),0);}}
