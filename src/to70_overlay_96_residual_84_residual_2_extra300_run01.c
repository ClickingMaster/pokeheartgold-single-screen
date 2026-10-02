#pragma force_active on
#include "global.h"
#pragma force_active on
struct Pair{void*image;void*palette;};struct S{u8 pad[0x24];struct Pair data[3];u32 image[3],palette[3];};void DC_FlushRange(const void*,u32);void GXS_LoadOBJ(const void*,u32,u32);void GXS_LoadOBJPltt(const void*,u32,u32);void ov96_02203F0C(struct S*);
void ov96_02203F0C(struct S*s){u8 i;for(i=0;i<3;i++){DC_FlushRange(s->data[i].image,3200);GXS_LoadOBJ(s->data[i].image,s->image[i],3200);DC_FlushRange(s->data[i].palette,32);GXS_LoadOBJPltt(s->data[i].palette,s->palette[i],32);}}
