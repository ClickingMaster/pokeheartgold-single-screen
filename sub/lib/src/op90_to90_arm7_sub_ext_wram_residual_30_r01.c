#pragma force_active on
#include <nitro/types.h>
#include <nitro/code32.h>
void MIi_CpuClear32(void);
void __VENEER_MIi_CpuClear32(void);
// @fn __VENEER_MIi_CpuClear32
asm void __VENEER_MIi_CpuClear32(void)
{
    ldr pc, =MIi_CpuClear32
}
