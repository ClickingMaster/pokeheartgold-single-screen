#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#pragma force_active on
extern u16 gGXState[];
void GX_VRAMCNT_SetLCDC_(u32);

void GX_SetBankForTexPltt(GXVRamTexPltt bank) {
    gGXState[0] = (gGXState[0] | gGXState[5]) & ~bank;
    gGXState[5] = bank;
    switch (bank) {
    case 0x60:
        *(volatile u8 *)0x04000246 = 0x8b;
    case 0x20:
        *(volatile u8 *)0x04000245 = 0x83;
        break;
    case 0x40:
        *(volatile u8 *)0x04000246 = 0x83;
        break;
    case 0x70:
        *(volatile u8 *)0x04000246 = 0x9b;
    case 0x30:
        *(volatile u8 *)0x04000245 = 0x93;
    case 0x10:
        *(volatile u8 *)0x04000244 = 0x83;
        break;
    case 0:
        break;
    default:
        break;
    }
    GX_VRAMCNT_SetLCDC_(gGXState[0]);
}
