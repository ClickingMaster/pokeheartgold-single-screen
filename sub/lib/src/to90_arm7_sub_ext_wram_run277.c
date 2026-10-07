#pragma force_active on
#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
extern u32 WSetWepKey(const u16*);
u32 PARAMSET_WepKeyReqCmd(u16 *p,u16 *c);
// @fn PARAMSET_WepKeyReqCmd
u32 PARAMSET_WepKeyReqCmd(u16 *p,u16 *c){c[1]=1;return WSetWepKey(p+8);}
