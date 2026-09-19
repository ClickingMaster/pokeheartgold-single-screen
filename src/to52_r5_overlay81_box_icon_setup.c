#include "to52_r5_overlay81_box_icon_setup_private.h"

void ov81_02241FEC(u8 *work) {
    u32 i;
    u8 *it;
    i = 0;
    if (i < U32(work, 0x47c)) {
        it = work;
        do {
            if (i == work[0x11]) {
                ov81_02242DFC(PTR(it, 0x39c), 3);
                ov81_02242D88(PTR(it, 0x39c), 1);
            } else if (i < work[0x11]) {
                ov81_02242DFC(PTR(it, 0x39c), 2);
                ov81_02242D88(PTR(it, 0x39c), 1);
            } else {
                ov81_02242D88(PTR(it, 0x39c), 0);
            }
            i++;
            it += 4;
        } while (i < U32(work, 0x47c));
    }
}

void ov81_02242058(u8 *work, int flag) {
    u32 i;
    u8 *it;
    i = 0;
    if (i < U32(work, 0x47c)) {
        it = work;
        do {
            if (i == U32(work, 0x468) && flag == 1) {
                ov81_02242DFC(PTR(it, 0x39c), 3);
            } else {
                ov81_02242DFC(PTR(it, 0x39c), 2);
            }
            ov81_02242D88(PTR(it, 0x39c), 1);
            i++;
            it += 4;
        } while (i < U32(work, 0x47c));
    }
}

void ov81_022420B4(u8 *work) {
    u16 i;
    u16 id;
    int xStep;
    int xBase;

    if (U32(work, 0x47c) == 3) {
        xBase = 0x10;
        xStep = 0x50;
    } else {
        xBase = 0x28;
        xStep = 0x70;
    }
    id = 0;
    i = 0;
    do {
        u8 *slot = work + i * 4;
        PTR(slot, 0x3a8) = ov81_02242CBC(work + 0x1c4, 2, id, 0, 0, xBase + xStep * i, 0xb0);
        id++;
        i++;
    } while (i < 3);
    if (U32(work, 0x47c) == 2) {
        ov81_02242D88(PTR(work, 0x3b0), 0);
    }
    id = 0;
    while (i < 6) {
        u8 *slot = work + i * 4;
        PTR(slot, 0x3a8) = ov81_02242D18(work + 0x1c4, 3, id, 0, 0, 0, 0);
        ov81_02242D88(PTR(slot, 0x3a8), 0);
        id++;
        i++;
    }
}
