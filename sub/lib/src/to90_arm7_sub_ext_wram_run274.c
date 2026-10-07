#pragma force_active on
#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
extern u32 WSetRate(u16);
u32 PARAMSET_RateReqCmd(u16 *p,u16 *c);
// @fn PARAMSET_RateReqCmd
u32 PARAMSET_RateReqCmd(u16 *p,u16 *c){c[1]=1;return WSetRate(p[8]);}
