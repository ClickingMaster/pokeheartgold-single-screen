#pragma force_active on
#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
extern u16*__VENEER_WMSP_GetBuffer4Callback2Wm9(void);extern void __VENEER_WMSP_ReturnResult2Wm9(void*);
void WMSP_StartTestRxMode(void);
// @fn WMSP_StartTestRxMode
void WMSP_StartTestRxMode(void){u16*p=__VENEER_WMSP_GetBuffer4Callback2Wm9();p[0]=41;p[1]=4;__VENEER_WMSP_ReturnResult2Wm9(p);}
