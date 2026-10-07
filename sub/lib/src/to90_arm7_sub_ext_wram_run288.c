#pragma force_active on
#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
extern u32 WSetMainAntenna(u16);
u32 PARAMSET_MainAntennaReqCmd(u16 *p,u16 *c);
// @fn PARAMSET_MainAntennaReqCmd
u32 PARAMSET_MainAntennaReqCmd(u16 *p,u16 *c){u32 result=1;u16 state;c[1]=1;state=*(u16*)(*(u8*volatile*)0x0380fff4+0x34c);if(state>=16)result=WSetMainAntenna(p[8]);return result;}
