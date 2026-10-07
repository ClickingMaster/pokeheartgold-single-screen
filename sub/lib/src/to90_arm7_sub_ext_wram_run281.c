#pragma force_active on
#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
extern u32 WSetActiveZoneTime(u16,u32);
u32 PARAMSET_ActiveZoneReqCmd(u16 *p,u16 *c);
// @fn PARAMSET_ActiveZoneReqCmd
u32 PARAMSET_ActiveZoneReqCmd(u16 *p,u16 *c){c[1]=1;return WSetActiveZoneTime(p[8],0);}
