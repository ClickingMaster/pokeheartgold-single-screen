#pragma force_active on
#include <nitro/types.h>
#include <nitro/code32.h>
void SVC_SetSoundBias(void);
void __VENEER_SVC_SetSoundBias(void);
// @fn __VENEER_SVC_SetSoundBias
asm void __VENEER_SVC_SetSoundBias(void)
{
    ldr r12, =SVC_SetSoundBias
    bx r12
}
