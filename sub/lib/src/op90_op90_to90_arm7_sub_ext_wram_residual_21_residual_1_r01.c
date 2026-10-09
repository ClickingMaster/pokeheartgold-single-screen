#pragma force_active on
#include <nitro/types.h>
#include <nitro/code32.h>
void OS_Sleep(void);
void __VENEER_OS_Sleep(void);
// @fn __VENEER_OS_Sleep
asm void __VENEER_OS_Sleep(void)
{
    ldr pc, =OS_Sleep
}
