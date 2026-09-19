#include "private.h"

int ov73_021E9224(void *p) {
    DrawFrameAndWindow1((u8 *)p + 0xDF8, 0, 0x1F, 0xE);
    ov73_021EA0F0((u8 *)p + 0xDF8, PTR(p, 0xBE8), U32(p, 0x9C), 3);
    ov73_021EA134(p, 0, 1);
    CopyWindowToVram((u8 *)p + 0xDF8);
    U32(p, 0x1C) = 0xF;
    return 3;
}

int ov73_021E926C(void *p) {
    u8 time[12];
    u8 date[16];
    if (U32(gSystem, 0x4C) & 0x40) {
        S32(p, 0x9C)
        ++;
        if (S32(p, 0x9C) > S32(p, 0x98)) {
            S32(p, 0x9C) = 1;
        }
        ov73_021EA0F0((u8 *)p + 0xDF8, PTR(p, 0xBE8), U32(p, 0x9C), 3);
        PlaySE(0x5DC);
    } else if (U32(gSystem, 0x4C) & 0x80) {
        S32(p, 0x9C)
        --;
        if (S32(p, 0x9C) < 1) {
            S32(p, 0x9C) = S32(p, 0x98);
        }
        ov73_021EA0F0((u8 *)p + 0xDF8, PTR(p, 0xBE8), U32(p, 0x9C), 3);
        PlaySE(0x5DC);
    } else if (U32(gSystem, 0x48) & 1) {
        ov00_021ECB94(date, time);
        if (sub_0202D720(PTR(PTR(p, 0), 4), U32(p, 0x94), U32(p, 0x9C), date)) {
            U32(p, 0x1C) = 0x14;
        } else {
            U32(p, 0x1C) = 0x10;
        }
        ov73_021EA134(p, 0, 0);
        PlaySE(0x5DC);
    } else if (U32(gSystem, 0x48) & 2) {
        ov73_021EA134(p, 0, 0);
        U32(p, 0x1C) = 0x15;
        PlaySE(0x5DC);
    }
    return 3;
}

int ov73_021E9378(void *p) {
    BufferIntegerAsString(PTR(p, 0xBD0), 0, U32(p, 0x9C), 3, 2, 1);
    BufferIntegerAsString(PTR(p, 0xBD0), 1, U32(p, 0x94), 2, 2, 1);
    ov73_021E9F34(p, PTR(p, 0xBD4), 1, 1, 0xF0F);
    ov73_021E83EC(p, 0x2F, 0x11);
    MIi_CpuClearFast(0, (u32 *)((u8 *)p + 0xA0), 0xA38);
    return 3;
}
