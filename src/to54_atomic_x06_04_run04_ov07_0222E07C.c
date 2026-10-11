#include "to54_atomic_x06_private.h"
#define W(p, o) (*(u32 *)((u8 *)(p) + (o)))
#define H(p, o) (*(s16 *)((u8 *)(p) + (o)))
#define P(p, o) (*(void **)((u8 *)(p) + (o)))

void ov07_0222E07C(void *p) {
    switch (W(p, 0x50)) {
    case 0:
        BattleAnimUtil_InitLinearMotion((u8 *)p + 0x28, 0, 0x50, 0, 0x50, 4);
        break;
    case 1:
        BattleAnimUtil_InitLinearMotion((u8 *)p + 0x28, 0, 0, 0, -0x50, 4);
        break;
    case 2:
        BattleAnimUtil_InitLinearMotion((u8 *)p + 0x28, 0, -0x50, 0, 0x50, 4);
        break;
    case 3:
        BattleAnimUtil_InitLinearMotion((u8 *)p + 0x28, 0, 0x28, 0, -0x28, 4);
        break;
    default:
        GF_AssertFail();
        break;
    }
}
