#include "to58_bank_ov96_c15c_private.h"
static inline s16 AbsS16(s32 x) { if (x < 0) x = -x; return x; }
void ManagedSprite_ResetSpriteAnimCtrlState(void *);
void ManagedSprite_GetPositionXY(void *, s16 *, s16 *);
void ov96_0220C15C(void *task, void *e) {
    void *spr;
    switch (EW38(e).b.b) {
    case 0:
        spr = SPRC(e, 0);
        ManagedSprite_ResetSpriteAnimCtrlState(spr);
        ManagedSprite_SetDrawFlag(spr, 1);
        EW38(e).b.b++;
        return;
    case 1:
        EW38(e).b.c++;
        if (EW38(e).b.c < 2) break;
        spr = SPRC(e, 1);
        ov96_021EAB38(EA(e, EW38(e).b.f), 0);
        ManagedSprite_SetAnim(spr, 0x1b);
        ManagedSprite_SetAnimateFlag(spr, 0);
        ManagedSprite_SetDrawFlag(spr, 1);
        EW38(e).b.c = 0;
        EW38(e).b.b++;
        return;
    case 2: {
        s16 x, y;
        s16 step;
        ManagedSprite_GetPositionXY(SPRC(e, 1), &x, &y);
        step = (-0x40 - y) / 4;
        if (y <= -0x40 || step == 0) {
            EW38(e).b.b++;
            return;
        }
        ManagedSprite_SetPositionXYWithSubscreenOffset(SPRC(e, 1), x, y + step, 0x100000);
        return;
    }
    case 3:
        EW38(e).b.c++;
        if (EW38(e).b.c < 4) break;
        EW38(e).b.c = 0;
        EW38(e).b.b++;
        return;
    case 4: {
        s16 x, y;
        ManagedSprite_GetPositionXY(SPRC(e, 1), &x, &y);
        {
            s16 step = AbsS16(-0x18 - y);
            if (step != 0) step /= 2;
            y += step + 2;
        }
        if (y >= 0x30) {
            x = 0x48 + EW38(e).b.h * 0x40;
            y = 0x30;
            ManagedSprite_SetPositionXY(SPRC(e, 1), x, y);
            ManagedSprite_SetAnimateFlag(SPRC(e, 1), 1);
            ManagedSprite_ResetSpriteAnimCtrlState(SPRC(e, 0));
            EW38(e).b.b++;
            return;
        }
        ManagedSprite_SetPositionXYWithSubscreenOffset(SPRC(e, 1), x, y, 0x100000);
        return;
    }
    case 5:
        EW38(e).b.c++;
        if (EW38(e).b.c < 2) break;
        ManagedSprite_SetDrawFlag(SPRC(e, 1), 0);
        ov96_021EAB38(EA(e, EW38(e).b.a), 1);
        ov96_021EAB74(EA(e, EW38(e).b.a), 0);
        EW38(e).b.f = EW38(e).b.a;
        EW38(e).b.c = 0;
        EW38(e).b.b++;
        return;
    case 6:
        ETASK(e) = NULL;
        EW38(e).b.d = 0;
        SysTask_Destroy(task);
        return;
    default:
        GF_AssertFail();
        break;
    }
}
