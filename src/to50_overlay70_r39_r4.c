#include "to50_overlay70_r39_r4_private.h"
int ov70_02243928(Work39 *w) {
    int i;
    int win;
    sub_02019688(PTR(w, 0x1c), 0, 0x64, 0x20, 1);
    sub_02019B08(PTR(w, 0x1c), 0);
    ov70_0224190C(w, 5);
    MI_CpuFill8((u8 *)w + 0x64, 1, 0x1a);
    ov70_02243E8C(PTR(w, 0x1c), PTR(w, 0x24), PTR(w, 4), S16(w, 0x3c) + 0x6e, 2, 0xf0e02);
    i = 1;
    win = 0x10;
    do {
        int color;
        if (ov70_02243FD4(w, ov70_02245E84[S16(w, 0x3c)] + i - 1) > 0) {
            color = 0xf0e02;
            w->flags[ov70_02245E84[S16(w, 0x3c)] + i - 1] = 1;
        } else {
            color = 0x80902;
            w->flags[ov70_02245E84[S16(w, 0x3c)] + i - 1] = 0;
        }
        ov70_02243E8C(PTR(w, 0x1c), PTR(w, 0x24), (u8 *)PTR(w, 4) + win, ov70_02245FA0[ov70_02245F58[S16(w, 0x3c)].a + i - 1], 5, color);
        i++;
        win += 0x10;
    } while (i < 4);
    ov70_02243EB8(PTR(w, 0x1c), PTR(w, 0x24), (u8 *)PTR(w, 4) + 0xe0, 0x44);
    sub_020196E8(PTR(w, 0x1c), 0, 0x10, 0);
    Sprite_SetAnimCtrlSeq(PTR(w, 0xc), 0x2f);
    if (S16(w, 0x3e) < 0) {
        U32(w, 0x48) = 0;
    } else {
        U32(w, 0x48) = S16(w, 0x3e);
    }
    i = U32(w, 0x48);
    ov70_02238F9C(PTR(w, 0xc), (ov70_02245E26[i * 2] + 16) * 8, ov70_02245E27[i * 2] * 8);
    Sprite_SetDrawFlag(PTR(w, 0xc), 1);
    if (PTR(w, 0x18)) {
        Sprite_SetDrawFlag(PTR(w, 0x18), 1);
    }
    U32(w, 0x4c) = 21;
    return -1;
}
int ov70_02243A6C(void *w) {
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
    U32(w, 0x4c) = 22;
    S16(w, 0x3e) = r;
    goto done;
cancel:
    Sprite_SetDrawFlag(PTR(w, 0xc), 0);
    U32(w, 0x4c) = 22;
    S16(w, 0x3e) = -1;
done:
    return -1;
}
