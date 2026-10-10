#pragma force_active on
#include <nitro/types.h>
#include <nitro/code32.h>
void SVC_ResetSoundBias(void);
void __VENEER_SVC_ResetSoundBias(void);
// @fn __VENEER_SVC_ResetSoundBias
void __VENEER_SVC_ResetSoundBias(void)
{
    SVC_ResetSoundBias();
}
