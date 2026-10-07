#pragma force_active on
#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
extern u32 WSetBeaconPeriod(u16);
u32 PARAMSET_BeaconPeriodReqCmd(u16 *p,u16 *c);
// @fn PARAMSET_BeaconPeriodReqCmd
u32 PARAMSET_BeaconPeriodReqCmd(u16 *p,u16 *c){c[1]=1;if(*(u16*)(*(u8*volatile*)0x0380fff4+0x32e)!=1)return 11;return WSetBeaconPeriod(p[8]);}
