#pragma force_active on
#include "global.h"
u32 ov13_02222A1C(u32),ov13_02222A5C(u32);u16 ov13_02222A44(int),ov13_02222A84(int);
u32 ov13_02222A1C(u32 x){return ((u8)(x>>24)|((x>>8)&0xff00)|((x<<8)&0xff0000)|((x<<24)&0xff000000));}
u16 ov13_02222A44(int x){return (u8)(x>>8)|((x<<8)&0xff00);}
u32 ov13_02222A5C(u32 x){return ((u8)(x>>24)|((x>>8)&0xff00)|((x<<8)&0xff0000)|((x<<24)&0xff000000));}
u16 ov13_02222A84(int x){return (u8)(x>>8)|((x<<8)&0xff00);}
