#pragma force_active on
#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
u32 RtcBCD2HEX(u32 value);
// @fn RtcBCD2HEX
u32 RtcBCD2HEX(u32 value){u32 result=0,multiplier;int i,j;for(i=0;i<8;i++){if(((value>>(i*4))&15)>=10)return 0;}j=0;multiplier=1;for(;j<8;j++){result+=multiplier*((value>>(j*4))&15);multiplier*=10;}return result;}
