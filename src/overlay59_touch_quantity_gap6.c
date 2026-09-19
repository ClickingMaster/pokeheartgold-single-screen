#include "overlay59_touch_quantity_gap6_private.h"
int ov59_0223A614(Ov59Gap6Work *work, int *hit) {
    int input = TouchscreenHitbox_FindHitboxAtTouchNew(ov59_0223C908);
    if (input == -1) {
        return 6;
    }
    if ((u32)(input - 4) <= 1 && work->raw[0x4F] < 10) {
        return 6;
    }
    if (input >= 2) {
        Sprite **sprites = work->sprites;
        u32 offset = (input + 9) * 4;
        Sprite_SetAnimActiveFlag(*(Sprite **)((u8 *)sprites + offset), TRUE);
        Sprite_ResetAnimCtrlState(*(Sprite **)((u8 *)sprites + offset));
    }
    *hit = 1;
    *(u32 *)&work->raw[0x40] = 1;
    return ov59_0223A48C(work, input);
}
