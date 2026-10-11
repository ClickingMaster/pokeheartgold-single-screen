#include "to55_w05_private.h"
#define W(p, o)  (*(u32 *)((u8 *)(p) + (o)))
#define SW(p, o) (*(s32 *)((u8 *)(p) + (o)))
#define H(p, o)  (*(s16 *)((u8 *)(p) + (o)))
#define P(p, o)  (*(void **)((u8 *)(p) + (o)))

int ov07_0222F6C0(void *p) {
    int ret = 0, done;
    float x, y;
    switch (W(p, 0x94)) {
    case 0:
        W(p, 0x98)
        --;
        BattleAnimUtil_UpdateLinearMotionSprite((u8 *)p + 0x70, P(p, 0x1c));
        if (SW(p, 0x98) < 0) {
            W(p, 0x94)
            ++;
        }
        break;
    case 1:
        BattleAnimUtil_UpdateLinearMotionSprite((u8 *)p + 0x70, P(p, 0x1c));
        done = BattleAnimUtil_UpdateScaleMotionXY((u8 *)p + 0x28);
        BattleAnimUtil_GetScaleFloat((u8 *)p + 0x28, &x, &y);
        if (W(p, 0xcc) == 1) {
            x = 0.0f - x;
        }
        ManagedSprite_SetAffineScale(P(p, 0x1c), x, y);
        if (!done) {
            W(p, 0x94)
            ++;
            ret = 1;
        }
        break;
    case 2:
        ret = 1;
        break;
    }
    return ret;
}
