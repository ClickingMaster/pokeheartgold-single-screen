#pragma force_active on
#include <nitro/types.h>
#include <nitro/code32.h>
void SVC_ResetSoundBias(void);
void __VENEER_SVC_ResetSoundBias(void);
// @fn __VENEER_SVC_ResetSoundBias
asm void __VENEER_SVC_ResetSoundBias(void)
{
    ldr r12, =SVC_ResetSoundBias
    bx r12
}
