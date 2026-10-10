#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

typedef struct {
    volatile u8 volume, gain, pan, reserved[13];
} W40856_Channel;

#define W40856_CHANNELS ((W40856_Channel *)0x04000400u)
extern int sSurroundDecay;
extern u8 sOrgVolume[16];
extern int CalcSurroundDecay(int, int);
void SNDi_SetSurroundDecay(int);
#pragma force_active off

static inline void W40856_Apply(int i, const u8 *volume) {
    u32 offset = (u32)i << 4;
    int pan = *(volatile u8 *)(0x04000000u + offset + 0x402u);
    int value = CalcSurroundDecay(volume[i], pan);
    *(volatile u8 *)(0x04000000u + offset + 0x400u) = value;
}

#pragma force_active on

void SNDi_SetSurroundDecay(int decay) {
    u32 mask = 0xfff5;
    int i;
    sSurroundDecay = decay;
    for (i = 0; i < 16; i++) {
        if (mask & (1u << i)) {
            W40856_Apply(i, sOrgVolume);
        }
    }
}
