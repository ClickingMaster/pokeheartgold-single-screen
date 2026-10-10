#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
extern int sMasterPan;
extern int sSurroundDecay;
extern u8 sOrgPan[16];
extern u8 sOrgVolume[16];
extern int CalcSurroundDecay(int, int);
#define R8(c, o)  (*(volatile u8 *)(0x04000000u + ((u32)(c) << 4) + (o)))
#define R16(c, o) (*(volatile u16 *)(0x04000000u + ((u32)(c) << 4) + (o)))
#define R32(c, o) (*(volatile u32 *)(0x04000000u + ((u32)(c) << 4) + (o)))

void SND_SetupChannelPcm(int c, u32 src, int format, int repeat, int loop, u32 len, int volume, int shift, int timer, int pan);

void SND_SetupChannelPcm(int c, u32 src, int format, int repeat, int loop, u32 len, int volume, int shift, int timer, int pan) {
    int master = sMasterPan;
    u32 offset = (u32)c << 4;
    sOrgPan[c] = pan;
    if (master >= 0) {
        pan = master;
    }
    sOrgVolume[c] = volume;
    if (sSurroundDecay > 0 && (0xfff5u & (1u << c))) {
        volume = CalcSurroundDecay(volume, pan);
    }
    R32(0, offset + 0x400) = ((u32)format << 29) | ((u32)repeat << 27) | ((u32)pan << 16) | ((u32)shift << 8) | (u32)volume;
    R16(0, offset + 0x408) = 0x10000u - timer;
    R16(0, offset + 0x40a) = loop;
    R32(0, offset + 0x40c) = len;
    R32(0, offset + 0x404) = (u32)src;
}
