#pragma force_active on
#include <nitro/types.h>
#include <nitro/code32.h>
void OS_DisableIrqMask(void);
void __VENEER_OS_DisableIrqMask(void);
void OS_EnableIrqMask(void);
void __VENEER_OS_EnableIrqMask(void);
// @fn __VENEER_OS_DisableIrqMask
asm void __VENEER_OS_DisableIrqMask(void)
{
    ldr pc, =OS_DisableIrqMask
}
// @fn __VENEER_OS_EnableIrqMask
asm void __VENEER_OS_EnableIrqMask(void)
{
    ldr pc, =OS_EnableIrqMask
}
