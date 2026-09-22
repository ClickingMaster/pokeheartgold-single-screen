#include "to57_r3_04_overlay07_motion_private.h"

void ov07_02222864(R304Work *work, const R304SinMotion *motion, BOOL (*callback)(R304SinMotion *), s16 x, s16 y, u16 duration, u8 count, u8 affine, void *sprite0, void *sprite1, void *sprite2, void *sprite3) {
    int i;

    work->x = x;
    work->y = y;
    work->callback = callback;
    work->duration = duration;
    work->counter = 0;
    work->count = count;
    work->active = 0;
    work->affine = affine;
    for (i = 0; i < count; i++) {
        work->motions[i] = *motion;
    }
    work->sprites[0] = sprite0;
    work->sprites[1] = sprite1;
    work->sprites[2] = sprite2;
    work->sprites[3] = sprite3;
    for (i = 0; i < work->count; i++) {
        ManagedSprite_SetDrawFlag(work->sprites[i], FALSE);
    }
}

BOOL ov07_02222914(R304Work *work) {
    BOOL results[4];
    int i;

    for (i = 0; i < 4; i++) {
        results[i] = TRUE;
    }

    if (work->duration <= work->counter) {
        ManagedSprite_SetDrawFlag(work->sprites[work->active], TRUE);
        work->active++;
        work->counter = 0;
    }
    if (work->active < work->count) {
        work->counter++;
    }
    for (i = 0; i < work->active; i++) {
        results[i] = work->callback(&work->motions[i]);
        if (results[i]) {
            if (work->affine == 0) {
                ManagedSprite_SetPositionXY(work->sprites[i], work->x + work->motions[i].x, work->y + work->motions[i].y);
            } else {
                u32 x;
                u32 y;
                ov07_02222644(&work->motions[i], &x, &y);
                ManagedSprite_SetAffineScale(work->sprites[i], x, y);
            }
        } else {
            ManagedSprite_SetDrawFlag(work->sprites[i], FALSE);
        }
    }
    for (i = 0; i < work->count; i++) {
        if (results[i] == TRUE) {
            return TRUE;
        }
    }
    return FALSE;
}

void ov07_02222A44(R304SinMotion *motion, int multiplier, int duration) {
    ov07_022220FC(motion, 0, 0xFFFF, 0, 0xFFFF, 2 << 16, -0x8000, duration);
    motion->duration *= multiplier;
}

void ov07_02222A7C(SysTask *task, R304Transition *trans) {
    R304SinMotion *motion = &trans->motion;
    if (!ov07_022222B4(motion)) {
        trans->done = 1;
        SysTask_Destroy(task);
        return;
    }
    if (motion->x < 0) {
        motion->x = 0;
    }
    if (motion->y < 0) {
        motion->y = 0;
    }
    *(vu16 *)0x04000052 = motion->x | (motion->y << 8);
}

void ov07_02222AC4(R304Transition *trans, s16 x, s16 xEnd, s16 y, s16 yEnd, int duration) {
    ov07_02222268(&trans->motion, x, xEnd, y, yEnd, (u16)duration);
    trans->done = 0;
    SysTask_CreateOnMainQueue((void (*)(SysTask *, void *))ov07_02222A7C, trans, 0);
}
