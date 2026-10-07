#pragma force_active on
#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
extern u32 WSetMode(u16);
u32 PARAMSET_ModeReqCmd(u16 *p,u16 *c);
// @fn PARAMSET_ModeReqCmd
u32 PARAMSET_ModeReqCmd(u16 *p,u16 *c){c[1]=1;u8*w=*(u8*volatile*)0x0380fff4;u16 state=*(u16*)(w+0x34c);if(state>32)return 1;if(state==32&&*(u16*)(w+0x356)!=0)return 1;return WSetMode(p[8]);}
