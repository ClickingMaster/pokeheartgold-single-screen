#pragma force_active on
#include "global.h"
void Sprite_SetAnimCtrlSeq(void*,u32);void ov96_021FC28C(u8*,u32,u32);
#pragma force_active on
void ov96_021FC28C(u8*p,u32 index,u32 state){GF_ASSERT(state!=0&&state<=8);Sprite_SetAnimCtrlSeq(*(void**)(p+index*16+0x18C),state+15);}
