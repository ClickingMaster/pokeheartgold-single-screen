#pragma force_active on
#pragma force_active on
#include "global.h"
#define I(p,o) (*(int*)((u8*)(p)+(o)))
#define U(p,o) (*(u32*)((u8*)(p)+(o)))
#define H(p,o) (*(u16*)((u8*)(p)+(o)))
#define B(p,o) (*((u8*)(p)+(o)))
#define V(p,o) (*(void**)((u8*)(p)+(o)))
extern int*ov81_02242F30(void*);extern void Sprite_SetMatrix(void*,const VecFx32*);void ov81_02242F60(u8*);
void ov81_02242F60(u8*p){VecFx32 v;int*xy=ov81_02242F30(p);v.x=(xy[0]/4096+248)<<12;v.y=xy[1];Sprite_SetMatrix(V(p,12),&v);}
