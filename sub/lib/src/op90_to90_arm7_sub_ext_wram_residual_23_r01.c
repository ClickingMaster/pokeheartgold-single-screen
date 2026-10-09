#pragma force_active on
#include <nitro/types.h>
#include <nitro/code32.h>
void WMSP_GetAllowedChannel(void);
void __VENEER_WMSP_GetAllowedChannel(void);
// @fn __VENEER_WMSP_GetAllowedChannel
asm void __VENEER_WMSP_GetAllowedChannel(void)
{
    ldr pc, =WMSP_GetAllowedChannel
}
