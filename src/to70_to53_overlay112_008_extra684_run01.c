#pragma force_active on
#include "global.h"
extern void MI_CpuCopy8(const void*,void*,u32);extern int MI_CompressLZImpl(const void*,u32,void*,int);

void ov112_021E5D8C(u8*src,u8*dst,u16 size,u8*out);

void ov112_021E5D8C(u8*src,u8*dst,u16 size,u8*out){int i=0,blocks=(u16)((size+127)/128);for(;i<blocks;i++){u32 n=size;if(n>128)n=128;if(n!=128){MI_CpuCopy8(src,dst,n);*out++=128;}else{u8 result=MI_CompressLZImpl(src,n,dst,0);if(result==0){MI_CpuCopy8(src,dst,n);*out++=128;}else *out++=result;}size-=n;src+=128;dst+=128;}}
