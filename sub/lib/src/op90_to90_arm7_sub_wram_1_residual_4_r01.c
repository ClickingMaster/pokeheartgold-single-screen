#pragma force_active on
#include <nitro/types.h>
#include <nitro/code32.h>
void SVC_GetVolumeTable(void);
void __VENEER_SVC_GetVolumeTable(void);
// @fn __VENEER_SVC_GetVolumeTable
void __VENEER_SVC_GetVolumeTable(void)
{
    SVC_GetVolumeTable();
}
