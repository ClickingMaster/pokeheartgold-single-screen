#pragma force_active on
#include <nitro/types.h>
#include <nitro/code32.h>
void SVC_GetVolumeTable(void);
void __VENEER_SVC_GetVolumeTable(void);
// @fn __VENEER_SVC_GetVolumeTable
asm void __VENEER_SVC_GetVolumeTable(void)
{
    ldr r12, =SVC_GetVolumeTable
    bx r12
}
