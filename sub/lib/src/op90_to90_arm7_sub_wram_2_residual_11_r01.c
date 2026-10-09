#pragma force_active on
#include <nitro/types.h>
#include <nitro/code32.h>
void SVC_CpuSet(void);
void __VENEER_SVC_CpuSet(void);
// @fn __VENEER_SVC_CpuSet
asm void __VENEER_SVC_CpuSet(void)
{
    ldr r12, =SVC_CpuSet
    bx r12
}
