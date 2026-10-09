#pragma force_active on
#include <nitro/types.h>
#include <nitro/code32.h>
void MI_CpuCopy8(void);
void __VENEER_MI_CpuCopy8(void);
// @fn __VENEER_MI_CpuCopy8
asm void __VENEER_MI_CpuCopy8(void)
{
    ldr pc, =MI_CpuCopy8
}
