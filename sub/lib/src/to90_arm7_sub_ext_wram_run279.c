#pragma force_active on
#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
extern u32 WSetBcSsidResponse(u16);
u32 PARAMSET_ResBcSsidReqCmd(u16 *p,u16 *c);
// @fn PARAMSET_ResBcSsidReqCmd
u32 PARAMSET_ResBcSsidReqCmd(u16 *p,u16 *c){c[1]=1;return WSetBcSsidResponse(p[8]);}
