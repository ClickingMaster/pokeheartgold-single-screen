#include "to57_overlay112_066_private.h"

int ov112_021EB5DC(Ov112State *p) {
    PokewalkerApp_LoadTilemap(p, 2, 12);
    PokewalkerApp_LoadTilemap(p, 5, 16);
    PokewalkerApp_PrintString(p, 3, P(p, 0x1e45c), 4, 0xf0100);
    PokewalkerApp_PrintString(p, 4, P(p, 0x1e460), 4, 0xf0100);
    PokewalkerApp_PrintString(p, 5, P(p, 0x1e464), 4, 0xf0100);
    PokewalkerApp_PrintExit(p, 1);
    W(p, 0x1ec50) = 0;
    PokewalkerApp_SetCursorPosition(p, ov112_021FF09C[W(p, 0x1ec50)]);
    return 18;
}

int ov112_021EB664(Ov112State *p) {
    int selection = PokewalkerApp_GetTouchedButton(3);
    if (selection != -1) {
        ov112_021EC440(p, selection, ov112_021FF09C[selection], 19);
        PlaySE(1501);
        return 25;
    }
    if (gSystem.newKeys & 0x40) {
        if (W(p, 0x1ec50) != 0) {
            PlaySE(1500);
            W(p, 0x1ec50)
            --;
        }
        PokewalkerApp_SetCursorPosition(p, ov112_021FF09C[W(p, 0x1ec50)]);
    } else if (gSystem.newKeys & 0x80) {
        if (W(p, 0x1ec50) < 3) {
            PlaySE(1500);
            W(p, 0x1ec50)
            ++;
        }
        PokewalkerApp_SetCursorPosition(p, ov112_021FF09C[W(p, 0x1ec50)]);
    } else if (gSystem.newKeys & 1) {
        PlaySE(1501);
        selection = W(p, 0x1ec50);
        ov112_021EC440(p, selection, ov112_021FF09C[selection], 19);
        return 25;
    } else if (gSystem.newKeys & 2) {
        PlaySE(1500);
        ov112_021EC440(p, 3, 3, 19);
        return 25;
    } else if (ov112_021EB9C0()) {
        PlaySE(1500);
        ov112_021EB90C(p, 7);
        return 26;
    } else if (ov112_021EB9E4() && ov112_021EB950(p, 3)) {
        PlaySE(1500);
        return 26;
    }
    return 18;
}

int ov112_021EB780(Ov112State *p) {
    PokewalkerApp_HideCursor(p);
    switch (W(p, 0x1ec50)) {
    case 0:
        W(p, 4) = 3;
        W(p, 0x10) = 0;
        break;
    case 1:
        W(p, 4) = 3;
        W(p, 0x10) = 1;
        break;
    case 2:
        W(p, 4) = 3;
        W(p, 0x10) = 2;
        break;
    case 3:
        W(p, 4) = 11;
        break;
    default:
        if (W(p, 0x1ec50) >= 4) {
            GF_AssertFail();
        }
        break;
    }
    return 26;
}

int ov112_021EB7D8(Ov112State *p) {
    PokewalkerApp_HideCursor(p);
    FillBgTilemapRect(P(p, 0x18), 2, 0, 0, 0, 32, 24, 16);
    FillBgTilemapRect(P(p, 0x18), 1, 0, 0, 0, 32, 24, 16);
    BgCommitTilemapBufferToVram(P(p, 0x18), 2);
    BgCommitTilemapBufferToVram(P(p, 0x18), 1);
    ov112_021EC460(p, 2, 15, 22);
    return 24;
}
