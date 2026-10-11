#include "to50_overlay70_r39_r3_private.h"
int ov70_022437C8(void *w) {
    u32 r = ov70_022429B8(w, (u8 *)w + 0x64);
    if (r == 10) {
        goto ten;
    }
    if (r == (u32)-2) {
        goto cancel;
    }
    if (r == (u32)-1) {
        goto done;
    }
    goto normal;
cancel:
    BgCellManager_StartCellMove(PTR(w, 0x1c), 0, 4, 0, 4);
    Sprite_SetDrawFlag(PTR(w, 0xc), 0);
    U32(w, 0x4c) = 3;
    U32(w, 0x50) = 18;
    S16(w, 0x3c) = -1;
    goto done;
ten:
    BgCellManager_StartCellMove(PTR(w, 0x1c), 0, 4, 0, 4);
    Sprite_SetDrawFlag(PTR(w, 0xc), 0);
    U32(w, 0x4c) = 3;
    U32(w, 0x50) = 18;
    S16(w, 0x3c) = -2;
    goto done;
normal:
    if (r > 8) {
        GF_AssertFail();
    }
    S16(w, 0x3c) = r;
    U32(w, 0x4c) = 18;
done:
    return -1;
}
