#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

typedef struct W35228Shared {
    u8 pad[32];

    struct {
        s16 vars[16];
        u32 other;
    } players[16];

    s16 globals[16];
} W35228Shared;

extern W35228Shared *SNDi_SharedWork;
void SND_SetPlayerLocalVariable(int p, int i, s16 v);

void SND_SetPlayerLocalVariable(int p, int i, s16 v) {
    SNDi_SharedWork->players[p].vars[i] = v;
}
