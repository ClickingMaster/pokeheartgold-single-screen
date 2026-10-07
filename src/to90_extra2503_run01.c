#pragma force_active on
#include "global.h"
#include "sprite.h"
typedef struct{s32 start,middle,end;}Keys;Sprite*ov96_021EB5B8(void*);void ov96_021F4E9C(u8,const Keys*,void*);
#pragma force_active on
void ov96_021F4E9C(u8 phase,const Keys*keys,void*arg){s32 y;VecFx32 v;if(phase<=15){s32 delta=(s32)((u32)keys->middle-(u32)keys->start);y=(s32)((u32)keys->start+(u32)((s32)((u32)delta*phase)/15));}else if(phase>15&&phase<=60)y=keys->middle;else{u8 t=phase-60;s32 delta=(s32)((u32)keys->end-(u32)keys->middle);y=(s32)((u32)keys->middle+(u32)((s32)((u32)delta*t)/10));}Sprite*sprite=ov96_021EB5B8(arg);v.x=0x80000;v.y=(s32)((u32)y+0x200000);v.z=0;Sprite_SetMatrix(sprite,&v);}
