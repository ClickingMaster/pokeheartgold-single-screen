#pragma force_active on
#include "global.h"
#pragma force_active on
#define P(p,o) (*(void **)((u8 *)(p)+(o)))
#define U(p,o) (*(u32 *)((u8 *)(p)+(o)))
#define B(p,o) (*((u8 *)(p)+(o)))
extern void ov48_0225A2A0(void *,int);
int ov48_0225A20C(void *ctx,int a);
int ov48_0225A20C(void *ctx,int a){switch(U(ctx,0)){case 0:U(ctx,0)=1;break;case 2:U(ctx,0)=3;return 0;case 1:case 3:case 4:return 0;}ov48_0225A2A0(ctx,a);return 1;}
