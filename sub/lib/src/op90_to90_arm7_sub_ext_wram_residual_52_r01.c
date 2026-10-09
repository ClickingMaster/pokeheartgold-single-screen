#pragma force_active on
#include <nitro/types.h>
#include <nitro/code32.h>
void MIi_CpuCopy32(void);
void __VENEER_MIi_CpuCopy32(void);
// @fn __VENEER_MIi_CpuCopy32
asm void __VENEER_MIi_CpuCopy32(void)
{
    ldr pc, =MIi_CpuCopy32
}
