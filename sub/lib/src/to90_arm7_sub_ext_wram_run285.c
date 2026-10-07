#pragma force_active on
#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
extern u32 WSetCCA_ED(u16,u16);
extern void BBP_Write(u32,u16);
u32 PARAMSET_CCAModeEDThReqCmd(u16 *p,u16 *c);
// @fn PARAMSET_CCAModeEDThReqCmd
u32 PARAMSET_CCAModeEDThReqCmd(u16 *p,u16 *c){c[1]=1;u32 r;if(p[10]>63)return 5;r=WSetCCA_ED(p[8],p[9]);if(r==0)BBP_Write(46,p[10]);return r;}
