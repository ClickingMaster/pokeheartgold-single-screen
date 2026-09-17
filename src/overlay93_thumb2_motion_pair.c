#include "overlay93_thumb2_motion_pair_private.h"

void ov93_02260A8C(void *e) {
    s16 x, y;
    s16 target = 0x48 - U8AT(e, 6) * 16;
    ManagedSprite_GetPositionXYWithSubscreenOffset(PAT(e, 0), &x, &y, 0x160000);
    if (target > x) {
        x += 2;
        if (x > target) {
            x = target;
        }
        ManagedSprite_SetPositionXYWithSubscreenOffset(PAT(e, 0), x, y, 0x160000);
    }
}

void ov93_02260AD8(void *sys, void *e) {
    int i;
    u8 *p;
    u8 state;
    if (U8AT(e, 6) != 0) {
        return;
    }
    state = U8AT(e, 5);
    if (state != 1) {
        if (state != 3) {
            return;
        }
        if (ManagedSprite_IsAnimated(PAT(e, 0)) != 0) {
            return;
        }
        S16AT(e, 8) += 5;
        U8AT(e, 4) = ov93_02260B70(S16AT(e, 8));
        U8AT(e, 5) = 1;
        ManagedSprite_SetAnim(PAT(e, 0), U8AT(e, 4));
        ManagedSprite_SetPositionXYWithSubscreenOffset(PAT(e, 0), -8, 0x20, 0x160000);
        p = (u8 *)sys + 0x380c;
        for (i = 0; i < 5; i++, p += 0xc) {
            if (U8AT(p, 6) == 0) {
                U8AT(p, 6) = 4;
            } else {
                U8AT(p, 6)
                --;
            }
        }
        return;
    }
    i = ov93_0225FEAC(sys);
    if (U8AT(e, 5) == 1 && i > 0x4b) {
        U8AT(e, 5) = 2;
        ManagedSprite_SetAnim(PAT(e, 0), U8AT(e, 4) + 3);
        PlaySE(0x596);
    }
}
