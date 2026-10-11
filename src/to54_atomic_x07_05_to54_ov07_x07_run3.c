#include "to54_atomic_x07_private.h"
BOOL ov07_0222D590(X07ScaleSprite *work);
void ov07_0222D56C(X07ScaleSprite *work);
void ov07_0222D578(X07ScaleSprite *work);
void ov07_0222D60C(void *task, X07Group *work);
void ov07_0222DA60(void *task, X07Popup *work);

BOOL ov07_0222DBD8(X07PicPulse *work) {
    BOOL done = FALSE;
    switch (work->state) {
    case 0:
        if (BattleAnimUtil_UpdateScaleMotion(work->anim)) {
            Pokepic_SetAttr(work->pokepic, 13, work->value);
        } else {
            work->state++;
            BattleAnimUtil_InitScaleMotion(work->anim, 15, 10, 10, 12);
        }
        break;
    case 1:
        if (BattleAnimUtil_UpdateScaleMotion(work->anim)) {
            Pokepic_SetAttr(work->pokepic, 13, work->value);
        } else {
            Pokepic_SetAttr(work->pokepic, 13, 0x100);
            work->state++;
        }
        break;
    case 2:
        done = TRUE;
        break;
    }
    return done;
}

BOOL ov07_0222DC54(X07SpritePulse *work) {
    BOOL done = FALSE;
    switch (work->state) {
    case 0:
        BattleAnimUtil_UpdateScaleMotionSprite(work->anim0, work->sprite);
        if (BattleAnimUtil_UpdateLinearMotion(work->anim1)) {
            ov07_0222207C(work->anim1, work->sprite, work->x, work->y);
        } else {
            work->state++;
            BattleAnimUtil_StartBlendAlphaMotion(work->anim2, 16, 0, 14, 16, 8);
        }
        break;
    case 1:
        if (BattleAnimUtil_IsBlendAlphaMotionDone(work->anim2)) {
            ManagedSprite_SetDrawFlag(work->sprite, FALSE);
            work->state++;
        }
        break;
    case 2:
        done = TRUE;
        break;
    }
    return done;
}
