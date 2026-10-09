#pragma force_active on
#include <nitro/types.h>
#include <nitro/code32.h>
void OS_SetPeriodicAlarm(void);
void __VENEER_OS_SetPeriodicAlarm(void);
// @fn __VENEER_OS_SetPeriodicAlarm
asm void __VENEER_OS_SetPeriodicAlarm(void)
{
    ldr pc, =OS_SetPeriodicAlarm
}
