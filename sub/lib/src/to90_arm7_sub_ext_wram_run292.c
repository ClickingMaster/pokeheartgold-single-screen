#pragma force_active on
#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
extern u32 WSetBssid(const u16*);
u32 PARAMSET_BSSIDReqCmd(u16 *p,u16 *c);
// @fn PARAMSET_BSSIDReqCmd
u32 PARAMSET_BSSIDReqCmd(u16 *p,u16 *c){c[1]=1;return WSetBssid(p+8);}
