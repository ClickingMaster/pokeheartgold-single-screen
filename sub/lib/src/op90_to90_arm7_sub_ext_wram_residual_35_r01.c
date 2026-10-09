#pragma force_active on
#include <nitro/types.h>
#include <nitro/code32.h>
void OS_IsAlarmAvailable(void);
void __VENEER_OS_IsAlarmAvailable(void);
// @fn __VENEER_OS_IsAlarmAvailable
asm void __VENEER_OS_IsAlarmAvailable(void)
{
    ldr pc, =OS_IsAlarmAvailable
}
