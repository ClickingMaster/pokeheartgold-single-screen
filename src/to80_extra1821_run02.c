#pragma force_active on
#include "global.h"
#pragma force_active on
#define P(p,o) (*(void **)((u8*)(p)+(o)))
#define U(p,o) (*(u32 *)((u8*)(p)+(o)))
#define H(p,o) (*(u16 *)((u8*)(p)+(o)))
#define B(p,o) (*((u8*)(p)+(o)))
extern void Sprite_SetMatrix(void*,const VecFx32*);void ov91_02260CE0(void*,int,int);
void ov91_02260CE0(void*w,int x,int y){VecFx32 v;v.x=x<<12;v.y=(y<<12)+0x200000;Sprite_SetMatrix(P(w,4),&v);}
