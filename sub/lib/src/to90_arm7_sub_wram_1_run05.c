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
extern u32 __VENEER_SVC_GetVolumeTable(int);
u16 SND_CalcChannelVolume(int db);

u16 SND_CalcChannelVolume(int db) {
    u32 value, shift;
    if (db < -723) {
        db = -723;
    } else if (db > 0) {
        db = 0;
    }
    value = __VENEER_SVC_GetVolumeTable(db + 723);
    if (db < -240) {
        shift = 3;
    } else if (db < -120) {
        shift = 2;
    } else if (db < -60) {
        shift = 1;
    } else {
        shift = 0;
    }
    return (u16)(value | (shift << 8));
}
