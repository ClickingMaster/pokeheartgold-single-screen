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

void SND_SetChannelPan(int c, int pan);

void SND_SetChannelPan(int c, int pan) {
    sOrgPan[c] = pan;
    if (sMasterPan >= 0) {
        pan = sMasterPan;
    }
    R8(c, 0x402) = pan;
    if (sSurroundDecay > 0 && (0xfff5u & (1u << c))) {
        R8(c, 0x400) = CalcSurroundDecay(sOrgVolume[c], pan);
    }
}
