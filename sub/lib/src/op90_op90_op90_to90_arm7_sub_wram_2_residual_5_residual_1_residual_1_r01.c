#pragma force_active on
#include <nitro/types.h>
#include <nitro/code32.h>
void SVC_Sleep(void);
void __VENEER_SVC_Sleep(void);
// @fn __VENEER_SVC_Sleep
asm void __VENEER_SVC_Sleep(void)
{
    ldr r12, =SVC_Sleep
    bx r12
}
