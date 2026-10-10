#pragma force_active on
#include <nitro/types.h>
#include <nitro/code32.h>
void SVC_GetPitchTable(void);
void __VENEER_SVC_GetPitchTable(void);
// @fn __VENEER_SVC_GetPitchTable
void __VENEER_SVC_GetPitchTable(void)
{
    SVC_GetPitchTable();
}
