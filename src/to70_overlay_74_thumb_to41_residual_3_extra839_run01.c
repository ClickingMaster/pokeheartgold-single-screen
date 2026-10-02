#pragma force_active on
#include "global.h"
u32 ov74_022311F4(u8*);s16 ov74_02231214(u8*);void ov74_02231238(u32,u32*,u32*);
u32 ov74_022311F4(u8*p) {union {u32 value;u8 bytes[4];} v;u32 i;u8 *out;v.value=0;out=v.bytes;for(i=0;i<4;i++,out++)*out=p[i+2];return v.value;}
s16 ov74_02231214(u8*p) {union {s16 value;u8 bytes[2];} v;u32 i;u8*out;v.value=0;out=v.bytes;for(i=0;i<2;i++,out++)*out=p[i];return v.value;}
void ov74_02231238(u32 value,u32*lo,u32*hi) {*lo=value%10000;*hi=(value/10000)%10000;}
