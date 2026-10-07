#pragma force_active on
#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
extern u32 WSetSsidMask(const u16*);
u32 PARAMSET_SSIDMaskReqCmd(u16 *p,u16 *c);
// @fn PARAMSET_SSIDMaskReqCmd
u32 PARAMSET_SSIDMaskReqCmd(u16 *p,u16 *c){c[1]=1;return WSetSsidMask(p+8);}
