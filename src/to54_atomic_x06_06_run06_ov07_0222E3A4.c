#include "to54_atomic_x06_private.h"
#define W(p, o) (*(u32 *)((u8 *)(p) + (o)))
#define H(p, o) (*(s16 *)((u8 *)(p) + (o)))
#define P(p, o) (*(void **)((u8 *)(p) + (o)))

void ov07_0222E3A4(void *p, u32 flip) {
    BattleAnimUtil_InitValueMotionFx((u8 *)p + 0x10, 0, 0xAAA, 3);
    if (flip != 0) {
        W(p, 0x18) = (u32)-1 * W(p, 0x18);
        Pokepic_SetAttr(P(p, 8), 10, 16);
    } else {
        Pokepic_SetAttr(P(p, 8), 10, -16);
    }
    W(p, 0x4C) = 0;
    W(p, 0x50) = 0;
}
