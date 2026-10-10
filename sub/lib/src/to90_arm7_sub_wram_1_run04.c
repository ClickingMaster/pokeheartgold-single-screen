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

int CalcSurroundDecay(int volume, int pan);

int CalcSurroundDecay(int volume, int pan) {
    u32 factor;
    if (pan < 24) {
        factor = (u32)sSurroundDecay * (u32)(pan + 40) + ((0x7fffu - (u32)sSurroundDecay) << 6);
        return (s32)((u32)volume * factor) >> 21;
    }
    if (pan > 104) {
        factor = (0u - (u32)sSurroundDecay) * (u32)(pan - 40) + (((u32)sSurroundDecay + 0x7fffu) << 6);
        return (s32)((u32)volume * factor) >> 21;
    }
    return volume;
}
