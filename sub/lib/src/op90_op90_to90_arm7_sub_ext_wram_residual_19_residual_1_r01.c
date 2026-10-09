#pragma force_active on
#include <nitro/types.h>
#include <nitro/code32.h>
void OS_CancelVAlarm(void);
void __VENEER_OS_CancelVAlarm(void);
void OS_SetVAlarm(void);
void __VENEER_OS_SetVAlarm(void);
// @fn __VENEER_OS_CancelVAlarm
asm void __VENEER_OS_CancelVAlarm(void)
{
    ldr pc, =OS_CancelVAlarm
}
// @fn __VENEER_OS_SetVAlarm
asm void __VENEER_OS_SetVAlarm(void)
{
    ldr pc, =OS_SetVAlarm
}
