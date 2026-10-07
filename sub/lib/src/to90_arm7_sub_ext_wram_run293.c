#pragma force_active on
#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
extern u32 WSetSsid(u16,const u16*);
u32 PARAMSET_SSIDReqCmd(u16 *p,u16 *c);
// @fn PARAMSET_SSIDReqCmd
u32 PARAMSET_SSIDReqCmd(u16 *p,u16 *c){c[1]=1;return WSetSsid(p[8],p+9);}
