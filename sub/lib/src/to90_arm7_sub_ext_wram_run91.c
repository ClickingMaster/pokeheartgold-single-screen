#pragma force_active on
#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
extern u16 *__VENEER_WMSP_GetBuffer4Callback2Wm9(void);
extern void __VENEER_WMSP_ReturnResult2Wm9(void *);
extern u16 *WMSP_WL_ParamSetBeaconPeriod(void *,u16);
void WMSP_SetBeaconPeriod(u32 *arg);
// @fn WMSP_SetBeaconPeriod
void WMSP_SetBeaconPeriod(u32 *arg) {
 u32 buf[128]; u16 err=WMSP_WL_ParamSetBeaconPeriod(buf,(u16)arg[1])[2];
 u16 *out;
 if(err) { out=__VENEER_WMSP_GetBuffer4Callback2Wm9(); out[0]=0x24;out[1]=1;out[2]=0x242;out[3]=err;__VENEER_WMSP_ReturnResult2Wm9(out); }
 else {out=__VENEER_WMSP_GetBuffer4Callback2Wm9();out[0]=0x24;out[1]=0;__VENEER_WMSP_ReturnResult2Wm9(out);}
}
