#include "to50_overlay70_r38r1_private.h"

int ov70_022433A8(void *w) {
    int r = ov70_02242390(w, (u8 *)w + 0x64);
    switch (r) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
        goto accepted;
    default:
        break;
    }
    if (r == -2) {
        goto cancel;
    }
    goto done;
accepted:
    Sprite_SetDrawFlag(PTR(w, 0xc), 0);
    U32(w, 0x4c) = 10;
    S16(w, 0x3e) = r;
    goto done;
cancel:
    Sprite_SetDrawFlag(PTR(w, 0xc), 0);
    U32(w, 0x4c) = 10;
    S16(w, 0x3e) = -1;
done:
    return -1;
}
