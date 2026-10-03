#pragma force_active on
#include "global.h"
#pragma force_active on
#define P(p,o) (*(void **)((u8*)(p)+(o)))
#define U(p,o) (*(u32 *)((u8*)(p)+(o)))
#define H(p,o) (*(u16 *)((u8*)(p)+(o)))
#define B(p,o) (*((u8*)(p)+(o)))
extern void G2dRenderer_SetSubSurfaceCoords(void*,int,int),G2dRenderer_SetMainSurfaceCoords(void*,int,int);void ov96_021EB5C8(void*,int,int,int,int);
void ov96_021EB5C8(void*w,int x,int y,int sx,int sy){G2dRenderer_SetSubSurfaceCoords((u8*)w+0x18,sx,sy);G2dRenderer_SetMainSurfaceCoords((u8*)w+0x18,x,y);}
