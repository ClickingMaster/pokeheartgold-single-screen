#include "overlay92_animation_tail_to50_private.h"

BOOL ov92_0225CC6C(Ov92AnimTo50 *work) {
    Ov92AnimTableTo50 table = ov92_02263A94;
    s32 x;
    s32 frame;
    s32 y;
    s32 index;
    frame = ManagedSprite_GetAnimationFrame(work->sprite);
    index = frame + work->state * 12;
    x = table.v[index].x;
    y = table.v[index].y;
    if (x == 0 && y == 0) {
        ov92_0225CB2C((void *)work, 0, 0);
    } else {
        ov92_0225CB2C((void *)work, x + 0x80, y + 0x80);
        if (x != 0) {
            ov92_02260A38(work, (s16)(x + 0x58), (s16)(y + 0x80));
        } else {
            ov92_02260A38(work, (s16)(x + 0x80), (s16)(y + 0x80));
        }
    }
    if (frame == 0 && work->prevFrame != frame) {
        work->state++;
    }
    work->prevFrame = frame;
    if (work->state == 2 && ManagedSprite_GetActiveAnim(work->sprite) != 2) {
        ManagedSprite_SetPositionXY(work->sprite, 0x80, 0x64);
        ManagedSprite_SetAnimNoRestart(work->sprite, 2);
        ov92_022630E8(work->obj2A4);
        ov92_022630E8(work->obj2B4);
        ov92_02260428(work->obj114, 0, 0, 5, 5, 0.8f, 0);
        ov92_02260428(work->obj114, 0, 0, -5, -5, 0.8f, 0);
    }
    if (work->state == 3 && frame == 11) {
        work->prevFrame = 0;
        work->state = 0;
        ManagedSprite_SetDrawFlag(work->sprite, FALSE);
        ov92_022630E8(work->obj2A4);
        ov92_022630E8(work->obj2B4);
        ov92_02260428(work->obj114, 0, 0, 5, 5, 0.8f, 0);
        ov92_02260428(work->obj114, 0, 0, -5, -5, 0.8f, 0);
        return TRUE;
    }
    ManagedSprite_TickTwoFrames(work->sprite);
    return FALSE;
}
