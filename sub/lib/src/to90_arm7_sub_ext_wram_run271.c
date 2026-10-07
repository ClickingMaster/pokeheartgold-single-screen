#pragma force_active on
#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
extern u32 WSetRetryLimit(u16);
u32 PARAMSET_RetryReqCmd(u16 *p,u16 *c);
// @fn PARAMSET_RetryReqCmd
u32 PARAMSET_RetryReqCmd(u16 *p,u16 *c){c[1]=1;return WSetRetryLimit(p[8]);}
