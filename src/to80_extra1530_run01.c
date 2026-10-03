#pragma force_active on
#include "global.h"
#pragma force_active on
#define P(p,o) (*(void **)((u8 *)(p)+(o)))
#define U(p,o) (*(u32 *)((u8 *)(p)+(o)))
#define B(p,o) (*((u8 *)(p)+(o)))
extern void *GF_3DVramMan_Create(int,int,int,int,int,void (*)(void)); extern void ov48_02259798(void);
void ov48_02259750(void *ctx,int heap);
void ov48_02259750(void *ctx,int heap){P(ctx,0x140)=GF_3DVramMan_Create(heap,0,2,0,4,ov48_02259798);NNS_G3dGlbLightVector(GX_LIGHTID_0,0,0,-4095);}
