#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
u32 RtcBCD2HEX(u32);
u32 RtcBCD2HEX(u32 value){int i,shift;u32 result=0;for(i=0,shift=0;i<8;i++,shift+=4){u32 digit=(value>>shift)&15;if(digit>=10)return 0;}{u32 weight=1;int offset,count;count=0;offset=count;do{result+=weight*((value>>offset)&15);weight*=10;count++;offset+=4;}while(count<8);}return result;}
