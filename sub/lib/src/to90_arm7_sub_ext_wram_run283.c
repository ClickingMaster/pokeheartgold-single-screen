#pragma force_active on
#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
extern u32 WSetPreambleType(u16);
u32 PARAMSET_PreambleTypeReqCmd(u16 *p,u16 *c);
// @fn PARAMSET_PreambleTypeReqCmd
u32 PARAMSET_PreambleTypeReqCmd(u16 *p,u16 *c){c[1]=1;return WSetPreambleType(p[8]);}
