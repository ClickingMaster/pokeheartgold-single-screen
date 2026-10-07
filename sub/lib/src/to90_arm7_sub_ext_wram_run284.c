#pragma force_active on
#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
extern u32 WSetAuthAlgo(u16);
u32 PARAMSET_AuthAlgoReqCmd(u16 *p,u16 *c);
// @fn PARAMSET_AuthAlgoReqCmd
u32 PARAMSET_AuthAlgoReqCmd(u16 *p,u16 *c){c[1]=1;return WSetAuthAlgo(p[8]);}
