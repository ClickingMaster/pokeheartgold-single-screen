#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

typedef struct Player {
    u8 p[0x1c];
    u8 fader[16];
    u8 state;
} Player;

void NNSi_SndFaderSet(void *, int, int);
void NNS_SndPlayerMoveVolume(Player *volatile *, int, int);

void NNS_SndPlayerMoveVolume(Player *volatile *handle, int volume, int duration) {
    Player *p;
    if (*handle && (p = *handle)->state != 2) {
        NNSi_SndFaderSet(p->fader, volume << 8, duration);
    }
}
