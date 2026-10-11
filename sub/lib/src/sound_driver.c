#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

#define REG_SOUNDCNT_VOL  (*(volatile u8 *)0x04000500)
#define REG_SOUNDCNT_CTRL (*(volatile u8 *)0x04000501)
#define REG_SNDCAP0CNT    (*(volatile u8 *)0x04000508)
#define REG_SNDCAP1CNT    (*(volatile u8 *)0x04000509)
#define REG_POWCNT2       (*(volatile u16 *)0x04000304)

#define SOUND_MASTER_ENABLE 0x80
#define POWCNT2_SPEAKER     1

void SND_StopChannel(int channel, int flags);
void OS_SpinWait(u32 cycles);
void PMi_ResetControl(u32 bit);
void PMi_SetControl(u32 bit);
void SVC_ResetSoundBias(u32 delay);
void SVC_SetSoundBias(u32 delay);

void SND_Enable(void);
void SND_Shutdown(void);
void SND_BeginSleep(void);
void __VENEER_SVC_ResetSoundBias(u32 delay);
void SND_EndSleep(void);
void __VENEER_SVC_SetSoundBias(u32 delay);
void SND_SetMasterVolume(s32 volume);
void SND_SetOutputSelector(u32 left, u32 right, u32 channel1, u32 channel3);

#pragma force_active on

// -ipa file would otherwise inline the veneers into their callers in this file.
#pragma dont_inline on

void SND_Enable(void) {
    REG_SOUNDCNT_CTRL |= SOUND_MASTER_ENABLE;
}

void SND_Shutdown(void) {
    int channel = 0;
    REG_SOUNDCNT_CTRL &= ~SOUND_MASTER_ENABLE;
    for (; channel < 16; channel++) {
        SND_StopChannel(channel, 1);
    }
    REG_SNDCAP0CNT = 0;
    REG_SNDCAP1CNT = 0;
}

void SND_BeginSleep(void) {
    REG_SOUNDCNT_CTRL &= ~SOUND_MASTER_ENABLE;
    __VENEER_SVC_ResetSoundBias(128);
    OS_SpinWait(0x40000);
    PMi_ResetControl(1);
    REG_POWCNT2 &= ~POWCNT2_SPEAKER;
}

void __VENEER_SVC_ResetSoundBias(u32 delay) {
    SVC_ResetSoundBias(delay);
}

void SND_EndSleep(void) {
    REG_POWCNT2 |= POWCNT2_SPEAKER;
    PMi_SetControl(1);
    __VENEER_SVC_SetSoundBias(256);
    OS_SpinWait(502656);
    REG_SOUNDCNT_CTRL |= SOUND_MASTER_ENABLE;
}

void __VENEER_SVC_SetSoundBias(u32 delay) {
    SVC_SetSoundBias(delay);
}

void SND_SetMasterVolume(s32 volume) {
    REG_SOUNDCNT_VOL = (u8)volume;
}

void SND_SetOutputSelector(u32 left, u32 right, u32 channel1, u32 channel3) {
    u32 enabled = (REG_SOUNDCNT_CTRL & SOUND_MASTER_ENABLE) ? 1 : 0;
    REG_SOUNDCNT_CTRL = (u8)(left | ((enabled << 7) | (channel3 << 5) | (channel1 << 4) | (right << 2)));
}

#pragma dont_inline reset
