#pragma force_active on
#include "global.h"
#pragma force_active on

extern int ov13_02225358(void*,const void*,u32,const void*,u32);
static inline u16 W31792Swap(u16 x){return (u8)(x>>8)|((x<<8)&0xff00);}
u32 ov13_02224718(u8*,u32,u8*,u32,const void*);
u32 ov13_02224718(u8*dest,u32 type,u8*data,u32 length,const void*key){u8*end=dest;u8*p;u32 sum=0;data[0]=0;data[1]=0;data[2]=0;data[3]=0;data[4]=0;data[5]=0;data[6]=0;data[7]=0;*(u16*)data=W31792Swap((u16)(length-8));if(key){ov13_02225358(dest+6,data,length,key,16);length+=8;}else memcpy(dest+6,data,length);dest[0]=0;dest[1]=0;dest[2]=0;dest[3]=0;dest[4]=0;dest[5]=0;*(u16*)dest=W31792Swap((u16)type);*(u16*)(dest+2)=W31792Swap((u16)length);end+=6;end+=length;for(p=dest;p<end;p++)sum+=*p;*(u16*)end=W31792Swap((u16)sum);return (u32)(end+2-dest);}
