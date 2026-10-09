#pragma force_active on
#include <nitro/types.h>
#include <nitro/code32.h>
void OS_CreateAlarm(void);
void __VENEER_OS_CreateAlarm(void);
// @fn __VENEER_OS_CreateAlarm
asm void __VENEER_OS_CreateAlarm(void)
{
    ldr pc, =OS_CreateAlarm
}
