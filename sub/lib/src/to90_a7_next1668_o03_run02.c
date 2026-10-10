#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
void PMi_SetControl(u32);
void __VENEER_SVC_SetSoundBias(u32);
void OS_SpinWait(u32);
void SND_EndSleep(void);

void SND_EndSleep(void) {
    *(volatile u16 *)0x04000304 |= 1;
    PMi_SetControl(1);
    __VENEER_SVC_SetSoundBias(256);
    OS_SpinWait(502656);
    *(volatile u8 *)0x04000501 |= 128;
}
