#pragma force_active on
#include "global.h"
typedef struct{u16 first,other[3];u32 word;u16 values[8];u8 last;u8 p[3];} Input;
void ov96_021E7CC8(void *,u16 *,Input *);
void ov96_021E7CC8(void *unused,u16 *out,Input *in){int i=0,j;u8 *base=(u8*)out;for(;i<5;i++){out[0]=in->first;for(j=0;j<3;j++)out[j+1]=in->other[j];*(u32*)(base+0x2c)=in->word;{int k=0;u8 *src,*dst;src=(u8*)in;dst=base;for(;k<8;k++){*(u16*)(dst+0x30)=*(u16*)(src+0xc);src+=2;dst+=2;}}base[0x40]=in->last;in++;out+=4;base+=24;}}
