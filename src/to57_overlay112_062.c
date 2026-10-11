#include "to57_overlay112_062_private.h"

int ov112_021EB208(Ov112State *p) {
    FillBgTilemapRect(P(p, 0x18), 1, 0, 0, 0, 0x20, 0x18, 0x10);
    BgCommitTilemapBufferToVram(P(p, 0x18), 1);
    ov112_021EC460(p, 0, 8, 7);
    return 0x18;
}

int ov112_021EB244(Ov112State *p) {
    PokewalkerApp_LoadTilemap(p, 2, 10);
    ov112_021E9FA4(p, 5, P(p, 0x1e46c), 4, 0x000f0100);
    ov112_021E9F40(p, 1);
    W(p, 0x1ec50) = 0;
    ov112_021EA51C(p, 2);
    return 8;
}

int ov112_021EB28C(Ov112State *p) {
    int selection = ov112_021E9888(1);
    if (selection != -1) {
        ov112_021EC440(p, selection, selection + 2, 9);
        PlaySE(0x5dd);
        return 0x19;
    }
    if (gSystem.newKeys & 0x40) {
        if (W(p, 0x1ec50) != 0) {
            PlaySE(0x5dc);
        }
        ov112_021EA51C(p, 2);
        W(p, 0x1ec50) = 0;
    } else if (gSystem.newKeys & 0x80) {
        if (W(p, 0x1ec50) != 1) {
            PlaySE(0x5dc);
        }
        ov112_021EA51C(p, 3);
        W(p, 0x1ec50) = 1;
    } else if (gSystem.newKeys & 1) {
        PlaySE(0x5dd);
        selection = W(p, 0x1ec50);
        ov112_021EC440(p, selection, selection + 2, 9);
        return 0x19;
    } else if (gSystem.newKeys & 2) {
        PlaySE(0x5dd);
        ov112_021EC440(p, 1, 3, 9);
        return 0x19;
    } else if (ov112_021EB9C0()) {
        PlaySE(0x5dc);
        ov112_021EB90C(p, 3);
        return 0x1a;
    } else if (ov112_021EB9E4() && ov112_021EB950(p, 3)) {
        PlaySE(0x5dc);
        return 0x1a;
    }
    return 8;
}

int ov112_021EB388(Ov112State *p) {
    ov112_021EA570(p);
    if (W(p, 0x1ec50) == 0) {
        W(p, 4) = 3;
    } else {
        W(p, 4) = 11;
    }
    return 0x1a;
}
