#pragma force_active on
#include <nitro/types.h>
#include <nitro/code32.h>
void OS_ExitThread(void);
void __VENEER_OS_ExitThread(void);
void MI_CpuFill8(void);
void __VENEER_MI_CpuFill8(void);
void OS_GetTick(void);
void __VENEER_OS_GetTick(void);
void MIi_CpuCopy16(void);
void __VENEER_MIi_CpuCopy16(void);
void OS_CancelAlarm(void);
void __VENEER_OS_CancelAlarm(void);
void OS_SetAlarm(void);
void __VENEER_OS_SetAlarm(void);
// @fn __VENEER_OS_ExitThread
asm void __VENEER_OS_ExitThread(void)
{
    ldr pc, =OS_ExitThread
}
// @fn __VENEER_MI_CpuFill8
asm void __VENEER_MI_CpuFill8(void)
{
    ldr pc, =MI_CpuFill8
}
// @fn __VENEER_OS_GetTick
asm void __VENEER_OS_GetTick(void)
{
    ldr pc, =OS_GetTick
}
// @fn __VENEER_MIi_CpuCopy16
asm void __VENEER_MIi_CpuCopy16(void)
{
    ldr pc, =MIi_CpuCopy16
}
// @fn __VENEER_OS_CancelAlarm
asm void __VENEER_OS_CancelAlarm(void)
{
    ldr pc, =OS_CancelAlarm
}
// @fn __VENEER_OS_SetAlarm
asm void __VENEER_OS_SetAlarm(void)
{
    ldr pc, =OS_SetAlarm
}
