#include "to55_bank_w15_private.h"

BOOL ov07_0222CFCC(W15Transition *work) {
    BOOL done = FALSE;
    switch (work->state) {
    case 0:
        if (BattleAnimUtil_IsBlendAlphaMotionDone(work->anim) == TRUE) {
            work->state++;
            work->timer = 32;
        }
        break;
    case 1:
        if (--work->timer < 0) {
            work->state++;
            BattleAnimUtil_StartBlendAlphaMotion(work->anim, 8, 0, 8, 16, 24);
        }
        break;
    case 2:
        if (BattleAnimUtil_IsBlendAlphaMotionDone(work->anim) == TRUE) {
            work->state++;
            BattleAnimScanline_FreeWave(work->effect);
        }
        break;
    case 3:
        done = TRUE;
        break;
    }
    return done;
}
