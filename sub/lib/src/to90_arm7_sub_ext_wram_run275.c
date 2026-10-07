#pragma force_active on
#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
extern u32 WSetWepMode(u16);
u32 PARAMSET_WepModeReqCmd(u16 *p,u16 *c);
// @fn PARAMSET_WepModeReqCmd
u32 PARAMSET_WepModeReqCmd(u16 *p,u16 *c){c[1]=1;return WSetWepMode(p[8]);}
