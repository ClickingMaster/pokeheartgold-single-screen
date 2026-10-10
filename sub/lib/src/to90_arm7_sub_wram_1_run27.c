#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

typedef struct {
    u8 id, type, state, flags;
    u8 p04[12];
    s32 env;
    u8 p14[8];
    u8 attack, sustain;
    u16 decay, release;
    u8 p22[22];
    u32 wave[3];
    u32 data;
    void *cb, *arg;
    u8 p50[4];
} Ch35188;

extern u32 sWeakLockChannel[2];
u32 SND_GetLockedChannel(u32 v);

u32 SND_GetLockedChannel(u32 v) {
    return v & 1 ? sWeakLockChannel[0] : sWeakLockChannel[1];
}
