#pragma force_active on
#include "global.h"
#pragma force_active on
int GF_SinDeg(u16);void GX_LoadOBJPltt(const void*,u32,u32);void ov73_021E72F4(u16*);
void ov73_021E72F4(u16*angle){u16 color;*angle+=10;if(*angle>360)*angle=0;color=29|((GF_SinDeg(*angle)*10/4096+15)<<5);GX_LoadOBJPltt(&color,10,2);GX_LoadOBJPltt(&color,42,2);}
