#pragma force_active on
#include <nitro/types.h>
#include <nitro/code32.h>
void MIi_CpuClearFast(void);
void __VENEER_MIi_CpuClearFast(void);
void OS_GetLockID(void);
void __VENEER_OS_GetLockID(void);
// @fn __VENEER_MIi_CpuClearFast
asm void __VENEER_MIi_CpuClearFast(void)
{
    ldr pc, =MIi_CpuClearFast
}
// @fn __VENEER_OS_GetLockID
asm void __VENEER_OS_GetLockID(void)
{
    ldr pc, =OS_GetLockID
}
