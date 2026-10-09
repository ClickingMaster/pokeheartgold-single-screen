#pragma force_active on
#include <nitro/types.h>
#include <nitro/code32.h>
void PM_SetLEDPattern(void);
void __VENEER_PM_SetLEDPattern(void);
// @fn __VENEER_PM_SetLEDPattern
asm void __VENEER_PM_SetLEDPattern(void)
{
    ldr pc, =PM_SetLEDPattern
}
