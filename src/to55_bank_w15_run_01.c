#include "to55_bank_w15_private.h"

BOOL ov07_0222CFCC(W15Transition *work) {
    BOOL done = FALSE;
    switch (work->state) {
    case 0:
        if (ov07_02222AF4(work->anim) == TRUE) {
            work->state++;
            work->timer = 32;
        }
        break;
    case 1:
        if (--work->timer < 0) {
            work->state++;
            ov07_02222AC4(work->anim, 8, 0, 8, 16, 24);
        }
        break;
    case 2:
        if (ov07_02222AF4(work->anim) == TRUE) {
            work->state++;
            ov07_02222D3C(work->effect);
        }
        break;
    case 3:
        done = TRUE;
        break;
    }
    return done;
}
