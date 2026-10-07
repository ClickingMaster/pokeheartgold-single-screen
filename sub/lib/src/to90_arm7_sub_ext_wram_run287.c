#pragma force_active on
#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
u32 PARAMSET_MaxConnReqCmd(u16 *p,u16 *c);
// @fn PARAMSET_MaxConnReqCmd
u32 PARAMSET_MaxConnReqCmd(u16 *p,u16 *c){c[1]=1;u8*w=*(u8*volatile*)0x0380fff4;if(*(u16*)(w+0x34c)>32)return 1;p[8]++;w=*(u8*volatile*)0x0380fff4;if(p[8]>*(u16*)(w+0x320))return 5;*(u16*)(w+0x322)=p[8];return 0;}
