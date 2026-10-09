#pragma force_active on
#include <nitro/types.h>
#include <nitro/code32.h>
void OS_InitMessageQueue(void);
void __VENEER_OS_InitMessageQueue(void);
void OS_InitMutex(void);
void __VENEER_OS_InitMutex(void);
void OS_CreateThread(void);
void __VENEER_OS_CreateThread(void);
void OS_WakeupThreadDirect(void);
void __VENEER_OS_WakeupThreadDirect(void);
void OS_IsVAlarmAvailable(void);
void __VENEER_OS_IsVAlarmAvailable(void);
void OS_InitVAlarm(void);
void __VENEER_OS_InitVAlarm(void);
void PXI_Init(void);
void __VENEER_PXI_Init(void);
void PXI_SetFifoRecvCallback(void);
void __VENEER_PXI_SetFifoRecvCallback(void);
// @fn __VENEER_OS_InitMessageQueue
asm void __VENEER_OS_InitMessageQueue(void)
{
    ldr pc, =OS_InitMessageQueue
}
// @fn __VENEER_OS_InitMutex
asm void __VENEER_OS_InitMutex(void)
{
    ldr pc, =OS_InitMutex
}
// @fn __VENEER_OS_CreateThread
asm void __VENEER_OS_CreateThread(void)
{
    ldr pc, =OS_CreateThread
}
// @fn __VENEER_OS_WakeupThreadDirect
asm void __VENEER_OS_WakeupThreadDirect(void)
{
    ldr pc, =OS_WakeupThreadDirect
}
// @fn __VENEER_OS_IsVAlarmAvailable
asm void __VENEER_OS_IsVAlarmAvailable(void)
{
    ldr pc, =OS_IsVAlarmAvailable
}
// @fn __VENEER_OS_InitVAlarm
asm void __VENEER_OS_InitVAlarm(void)
{
    ldr pc, =OS_InitVAlarm
}
// @fn __VENEER_PXI_Init
asm void __VENEER_PXI_Init(void)
{
    ldr pc, =PXI_Init
}
// @fn __VENEER_PXI_SetFifoRecvCallback
asm void __VENEER_PXI_SetFifoRecvCallback(void)
{
    ldr pc, =PXI_SetFifoRecvCallback
}
