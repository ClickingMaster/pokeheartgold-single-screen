#pragma force_active on
#include "global.h"
#pragma force_active on
extern void *memset(void *,int,u32),*memcpy(void *,const void *,u32);
u8 *ov13_022247DC(u8 *,int,const void *,int);
#pragma opt_propagation off
u8 *ov13_022247DC(u8 *out,int tag,const void *source,int size){int aligned;out[0]=0;out[1]=0;out[2]=0;out[3]=0;*(u16 *)out=(u8)(((u16)tag)>>8)|(((u16)tag<<8)&0xff00);aligned=((size+11)&~7)-4;*(u16 *)(out+2)=(u8)(((u16)size)>>8)|(((u16)size<<8)&0xff00);out+=4;memset(out,0,aligned);memcpy(out,source,size);return out+aligned;}
