#include "to50_overlay70_r39r2_private.h"

int ov70_02243598(void *w) {
    int n;
    BgCellManager_LoadCellTilemap(PTR(w, 0x1c), 0, 0x64, 0x23, 1);
    BgCellManager_GetCellTilemap(PTR(w, 0x1c), 0);
    ov70_0224190C(w, 0);
    n = S16(w, 0x3e) + ov70_02245E84[S16(w, 0x3c)];
    U32(w, 0x5c) = ov70_02243458((void **)((u8 *)w + 0x34), PTR(w, 0x28), PTR(w, 0x24), PTR(w, 0x30), n, PTR(w, 0x20));
    S16(w, 0x5a) = 0;
    ov70_022434C0(w, 0, U32(w, 0x5c));
    n = ov70_02242508(U32(w, 0x5c), 4);
    ov70_02243F00(PTR(w, 0x1c), PTR(w, 0x38), (u8 *)PTR(w, 4) + 0x40, S16(w, 0x5a), n);
    ov70_02243EB8(PTR(w, 0x1c), PTR(w, 0x24), (u8 *)PTR(w, 4) + 0xe0, 0x44);
    BgCellManager_DrawCellAt(PTR(w, 0x1c), 0, 0x10, 0);
    Sprite_SetAnimCtrlSeq(PTR(w, 0xc), 0x31);
    U32(w, 0x48) = 0;
    Gts_SetSpritePosition(PTR(w, 0xc), ov70_02245D76[U32(w, 0x48) * 2], ov70_02245D77[U32(w, 0x48) * 2]);
    Gts_SetSpritePosition(PTR(w, 0x10), 0xe4, 0x78);
    Gts_SetSpritePosition(PTR(w, 0x14), 0x9a, 0x78);
    Sprite_SetDrawFlag(PTR(w, 0xc), 1);
    Sprite_SetDrawFlag(PTR(w, 0x10), 1);
    Sprite_SetDrawFlag(PTR(w, 0x14), 1);
    U32(w, 0x4c) = 13;
    return -1;
}
int ov70_02243688(void *w) {
    int r = ov70_0224261C(w);
    switch (r) {
    case -2:
        Sprite_SetDrawFlag(PTR(w, 0x10), 0);
        Sprite_SetDrawFlag(PTR(w, 0x14), 0);
        PlaySE(0x5dc);
        U32(w, 0x4c) = 14;
        S16(w, 0x40) = -1;
        break;
    case -1:
        break;
    default:
        BgCellManager_StartCellMove(PTR(w, 0x1c), 0, 4, 0, 4);
        Sprite_SetDrawFlag(PTR(w, 0xc), 0);
        Sprite_SetDrawFlag(PTR(w, 0x10), 0);
        Sprite_SetDrawFlag(PTR(w, 0x14), 0);
        U32(w, 0x4c) = 3;
        U32(w, 0x50) = 15;
        S16(w, 0x40) = r;
        break;
    }
    return -1;
}
