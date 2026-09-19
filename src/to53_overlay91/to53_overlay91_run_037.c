#include "private_w19.h"

void ov91_0225E67C(u8 *p, u8 *base) {
    if (p[1] != 0) {
        if (p[0] == 3 && *(u16 *)(p + 6) == 1) {
            p[0] = 4;
        } else {
            ov91_0225E400(p);
        }
        /* external object begins at base + 0x1ab4 */
        extern void ov91_02260754(void *, void *);
        ov91_02260754(base + 0x1ab4, p);
    }
}


void ov91_0225E6B0(u8 *base) {
    int i;
    for (i = 0; i < 0x60; i++, base += 0x44) {
        if (base[0x4c] == 1) {
            base[0x4c] = 2;
        }
    }
}


BOOL ov91_0225E6D0(u8 *base) {
    int i;
    for (i = 0; i < 0x60; i++, base += 0x44) {
        u8 v = base[0x4c];
        if (v == 1 || v == 2 || v == 5 || v == 3) {
            return TRUE;
        }
    }
    return FALSE;
}
