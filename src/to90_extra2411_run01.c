#pragma force_active on
#include "global.h"
#pragma force_active on
int ov112_021E935C(u8*,int,int,int);
int ov112_021E935C(u8*data,int width,int x,int y){int tile=(s32)((u32)(y/8)*(u32)width+(u32)(x/8));int pixel=(s32)((u32)(x%8)+((u32)(y%8)<<3));if((pixel&1)==1)return data[(s32)((u32)(pixel/2)+(u32)tile*32)]>>4;return data[(s32)((u32)(pixel/2)+(u32)tile*32)]&15;}
