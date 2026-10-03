#pragma force_active on
#include "global.h"
extern void ToggleBgLayer(u8,u8),ov83_022477E4(u32 *);
void ov83_022477EC(int,int,volatile u32 *),ov83_0224780C(u32 *);
void ov83_0224780C(u32 *bits){u16 i;for(i=0;i<=7;i++){u16 value=(*bits>>(i*4))&15;if(value)ToggleBgLayer((u8)i,(u8)(value-1));}ov83_022477E4(bits);}
