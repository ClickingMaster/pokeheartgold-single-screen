#pragma force_active on
#include <nitro/types.h>
// The SDK headers below expect <nitro/types.h> to be included first.
#include <nitro/SND_work_shared.h>
// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

int SND_IsChannelActive(int channel);
int SND_IsCaptureActive(int capture);

void SND_SetPlayerLocalVariable(int player, int index, s16 value);
void SND_SetPlayerGlobalVariable(int index, s16 value);
void SND_UpdateSharedWork(void);

// -ipa file would otherwise inline these small functions into their callers in this file.
#pragma dont_inline on

void SND_SetPlayerLocalVariable(int player, int index, s16 value) {
    SNDi_SharedWork->players[player].localVars[index] = value;
}

void SND_SetPlayerGlobalVariable(int index, s16 value) {
    SNDi_SharedWork->globalVars[index] = value;
}

void SND_UpdateSharedWork(void) {
    u16 channels = 0, capture = 0;
    int i;
    if (!SNDi_SharedWork) {
        return;
    }
    for (i = 0; i < 16; i++) {
        if (SND_IsChannelActive(i)) {
            channels |= 1 << i;
        }
    }
    if (SND_IsCaptureActive(0)) {
        capture |= 1;
    }
    if (SND_IsCaptureActive(1)) {
        capture |= 2;
    }
    SNDi_SharedWork->channelStatus = channels;
    SNDi_SharedWork->captureStatus = capture;
}

#pragma dont_inline reset
