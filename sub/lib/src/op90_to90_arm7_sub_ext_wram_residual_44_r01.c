#pragma force_active on
#include <nitro/types.h>
#include <nitro/code32.h>
void OS_SetIrqFunction(void);
void __VENEER_OS_SetIrqFunction(void);
// @fn __VENEER_OS_SetIrqFunction
asm void __VENEER_OS_SetIrqFunction(void)
{
    ldr pc, =OS_SetIrqFunction
}
