#include "overlay59_touch_drag_gap3_private.h"

int ov59_02239F38(Ov59Gap3Work *work) {
    s16 x;
    s16 y;
    u8 hit;

    Sprite_GetPositionXY(work->sprites[work->raw[0x4D] + 2], &x, &y);
    hit = TouchscreenHitbox_PointIsIn(ov59_0223C940, x, y);

    if (System_GetTouchHeld()) {
        x = gSystem.touchX;
        y = gSystem.touchY;
        Sprite_SetPositionXY(work->sprites[work->raw[0x4D] + 2], x, y);
        Sprite_SetPositionXY(work->sprites[1], x, y - 6);

        if (hit) {
            if (work->raw[0x53] == 0) {
                work->raw[0x53] = 1;
                PlaySE(0x8E6);
                Sprite_SetAnimCtrlSeq(work->sprites[10], 0x15);
            }
        } else if (work->raw[0x53] != 0) {
            work->raw[0x53] = 0;
            PlaySE(0x8E6);
            Sprite_SetAnimCtrlSeq(work->sprites[10], 0x14);
        }
        return -1;
    }

    work->raw[0x53] = 0;
    if (hit) {
        PlaySE(0x5EA);
        return 1;
    }
    PlaySE(0x682);
    return 0;
}
