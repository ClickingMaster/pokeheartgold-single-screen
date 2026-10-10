#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
extern int sMasterPan;
extern int sSurroundDecay;
extern u8 sOrgPan[];
extern u8 sOrgVolume[];
extern int CalcSurroundDecay(int, int);
#define R8(c, o)  (*(volatile u8 *)(0x04000000u + ((u32)(c) << 4) + (o)))
#define R16(c, o) (*(volatile u16 *)(0x04000000u + ((u32)(c) << 4) + (o)))
#define R32(c, o) (*(volatile u32 *)(0x04000000u + ((u32)(c) << 4) + (o)))

void SND_SetupChannelNoise(int c, int volume, int shift, int timer, int pan);

void SND_SetupChannelNoise(int c, int volume, int shift, int timer, int pan) {
    u32 channelOffset = (u32)c << 4;
    {
        int master = sMasterPan;
        sOrgPan[c] = pan;
        if (master >= 0) {
            pan = master;
        }
    }
    sOrgVolume[c] = volume;
    if (sSurroundDecay > 0 && (0xfff5u & (1u << c))) {
        volume = CalcSurroundDecay(volume, pan);
    }
    {
        u32 control = ((u32)pan << 16) | 0x60000000u | ((u32)shift << 8);
        *(volatile u32 *)(0x04000400u + channelOffset) = (u32)volume | control;
    }
    *(volatile u16 *)(0x04000408u + channelOffset) = 0x10000u - (u32)timer;
}
