#pragma force_active on
#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
extern u32 WSetDiversity(u16,u16);
u32 PARAMSET_DiversityReqCmd(u16 *p,u16 *c);
// @fn PARAMSET_DiversityReqCmd
u32 PARAMSET_DiversityReqCmd(u16 *p,u16 *c){c[1]=1;if(*(u16*)(*(u8*volatile*)0x0380fff4+0x34c)<16)return 1;return WSetDiversity(p[8],p[9]);}
