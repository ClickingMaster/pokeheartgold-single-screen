#pragma force_active on
#include <nitro/types.h>
#include <nitro/code32.h>
void WMSP_Init(void);
void __VENEER_WMSP_Init(void);
// @fn __VENEER_WMSP_Init
asm void __VENEER_WMSP_Init(void)
{
    ldr pc, =WMSP_Init
}
