#pragma force_active on
#include <nitro/types.h>
#include <nitro/code32.h>
void OS_SendMessage(void);
void __VENEER_OS_SendMessage(void);
void OS_ReceiveMessage(void);
void __VENEER_OS_ReceiveMessage(void);
void WMSP_GetBuffer4Callback2Wm9(void);
void __VENEER_WMSP_GetBuffer4Callback2Wm9(void);
void WMSP_ReturnResult2Wm9(void);
void __VENEER_WMSP_ReturnResult2Wm9(void);
void SND_BeginSleep(void);
void __VENEER_SND_BeginSleep(void);
void OS_Terminate(void);
void __VENEER_OS_Terminate(void);
// @fn __VENEER_OS_SendMessage
asm void __VENEER_OS_SendMessage(void)
{
    ldr pc, =OS_SendMessage
}
// @fn __VENEER_OS_ReceiveMessage
asm void __VENEER_OS_ReceiveMessage(void)
{
    ldr pc, =OS_ReceiveMessage
}
// @fn __VENEER_WMSP_GetBuffer4Callback2Wm9
asm void __VENEER_WMSP_GetBuffer4Callback2Wm9(void)
{
    ldr pc, =WMSP_GetBuffer4Callback2Wm9
}
// @fn __VENEER_WMSP_ReturnResult2Wm9
asm void __VENEER_WMSP_ReturnResult2Wm9(void)
{
    ldr pc, =WMSP_ReturnResult2Wm9
}
// @fn __VENEER_SND_BeginSleep
asm void __VENEER_SND_BeginSleep(void)
{
    ldr pc, =SND_BeginSleep
}
// @fn __VENEER_OS_Terminate
asm void __VENEER_OS_Terminate(void)
{
    ldr pc, =OS_Terminate
}
