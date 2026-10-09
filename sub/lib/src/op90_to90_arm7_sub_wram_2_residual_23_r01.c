#pragma force_active on
#include <nitro/types.h>
#include <nitro/code32.h>
void WMSP_CancelVAlarm(void);
void __VENEER_WMSP_CancelVAlarm(void);
void WMSP_CancelAllAlarms(void);
void __VENEER_WMSP_CancelAllAlarms(void);
void WMSP_GetRequestThread(void);
void __VENEER_WMSP_GetRequestThread(void);
void WL_Terminate(void);
void __VENEER_WL_Terminate(void);
void WMSP_GetIndicateThread(void);
void __VENEER_WMSP_GetIndicateThread(void);
// @fn __VENEER_WMSP_CancelVAlarm
asm void __VENEER_WMSP_CancelVAlarm(void)
{
    ldr pc, =WMSP_CancelVAlarm
}
// @fn __VENEER_WMSP_CancelAllAlarms
asm void __VENEER_WMSP_CancelAllAlarms(void)
{
    ldr pc, =WMSP_CancelAllAlarms
}
// @fn __VENEER_WMSP_GetRequestThread
asm void __VENEER_WMSP_GetRequestThread(void)
{
    ldr pc, =WMSP_GetRequestThread
}
// @fn __VENEER_WL_Terminate
asm void __VENEER_WL_Terminate(void)
{
    ldr pc, =WL_Terminate
}
// @fn __VENEER_WMSP_GetIndicateThread
asm void __VENEER_WMSP_GetIndicateThread(void)
{
    ldr pc, =WMSP_GetIndicateThread
}
