#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

typedef struct W35230Shared {
    u8 pad[8];
    u16 channels, capture;
} W35230Shared;

extern W35230Shared *SNDi_SharedWork;
extern int SND_IsChannelActive(int);
extern int SND_IsCaptureActive(int);
void SND_UpdateSharedWork(void);

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
    SNDi_SharedWork->channels = channels;
    SNDi_SharedWork->capture = capture;
}
