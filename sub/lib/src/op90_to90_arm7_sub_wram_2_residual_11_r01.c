#pragma force_active on
#include <nitro/types.h>
#include <nitro/code32.h>
void SVC_CpuSet(void);
void __VENEER_SVC_CpuSet(void);
// @fn __VENEER_SVC_CpuSet
void __VENEER_SVC_CpuSet(void)
{
    SVC_CpuSet();
}
