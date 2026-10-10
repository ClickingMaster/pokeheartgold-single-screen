#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#pragma force_active on

struct CaptureState {
    int active, mode;
    u32 pad8;
    void *left, *right;
    u32 bytes;
    u32 pad18, pos, lockedChannels;
    u32 channels, capture;
    int alarm;
    u8 pad30[12];
    u8 fader[16];
    int stopping, volume;
};

extern u8 _021E0314[];
extern OSMessageQueue _021E031C;
extern struct CaptureState _021E035C;
extern void NNSi_SndFaderUpdate(void *);
extern BOOL NNSi_SndFaderIsFinished(void *);
extern int NNSi_SndFaderGet(void *);
extern void NNSi_SndCaptureStop(void);
extern void NNS_SndUnlockCapture(u32);
extern void NNS_SndUnlockChannel(u32);
extern void NNS_SndFreeAlarm(int);
extern void NNSi_SndCaptureMain(void);
extern void NNSi_SndCaptureBeginSleep(void);
extern void NNSi_SndCaptureEndSleep(void);
extern void SND_SetChannelVolume(u32, s32, s32);
extern void SND_StopTimer(u32, u32, u32, u32);
extern void SND_StartTimer(u32, u32, u32, u32);
extern void SND_SetOutputSelector(s32, s32, s32, s32);
extern u32 SND_GetCurrentCommandTag(void);
extern BOOL SND_FlushCommand(u32);
extern void SND_WaitForCommandProc(u32);

void NNSi_SndCaptureMain(void) {
    struct CaptureState *state = &_021E035C;
    int volume;
    if (!*(int *)(_021E0314 + 0x48)) {
        return;
    }
    if (state->mode) {
        return;
    }
    NNSi_SndFaderUpdate(state->fader);
    if (state->stopping && NNSi_SndFaderIsFinished(state->fader)) {
        NNSi_SndCaptureStop();
        return;
    }
    volume = NNSi_SndFaderGet(state->fader) >> 8;
    if (volume == state->volume) {
        return;
    }
    SND_SetChannelVolume(state->channels, volume, 0);
    state->volume = volume;
}

void NNSi_SndCaptureStop(void) {
    struct CaptureState *state = &_021E035C;
    BOOL hasAlarm;
    u32 mask;
    if (!*(int *)(_021E0314 + 0x48)) {
        return;
    }
    hasAlarm = state->alarm >= 0;
    mask = hasAlarm ? (1 << state->alarm) : 0;
    SND_StopTimer(state->channels, state->capture, mask, 0);
    if (hasAlarm) {
        u32 tag = SND_GetCurrentCommandTag();
        SND_FlushCommand(1);
        SND_WaitForCommandProc(tag);
        while (OS_ReceiveMessage(&_021E031C, NULL, 0)) {}
    }
    if (state->capture) {
        NNS_SndUnlockCapture(state->capture);
    }
    if (state->lockedChannels) {
        NNS_SndUnlockChannel(state->lockedChannels);
    }
    if (hasAlarm) {
        NNS_SndFreeAlarm(state->alarm);
    }
    if (state->mode == 1) {
        SND_SetOutputSelector(0, 0, 0, 0);
    }
    state->active = 0;
}

void NNSi_SndCaptureBeginSleep(void) {
    struct CaptureState *state = &_021E035C;
    u32 tag;
    if (!*(int *)(_021E0314 + 0x48)) {
        return;
    }
    SND_StopTimer(state->channels, state->capture, state->alarm >= 0 ? (1 << state->alarm) : 0, 0);
    tag = SND_GetCurrentCommandTag();
    SND_FlushCommand(1);
    SND_WaitForCommandProc(tag);
}

void NNSi_SndCaptureEndSleep(void) {
    struct CaptureState *state = &_021E035C;
    if (!*(int *)(_021E0314 + 0x48)) {
        return;
    }
    state->pos = 0;
    MIi_CpuClear32(0, state->left, state->bytes);
    MIi_CpuClear32(0, state->right, state->bytes);
    DC_FlushRange(state->left, state->bytes);
    DC_FlushRange(state->right, state->bytes);
    SND_StartTimer(state->channels, state->capture, state->alarm >= 0 ? (1 << state->alarm) : 0, 0);
}
