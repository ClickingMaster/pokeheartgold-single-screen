#include "overlay59_input_gap4_private.h"

int ov59_0223A2D0(Ov59Gap4Work *work, int *hit) {
    int input;
    int value;

    input = TouchscreenHitbox_FindHitboxAtTouchNew(ov59_0223C924);
    value = 0;

    if (input == ~value) {
        return 4;
    }

    value = 1;
    *hit = value;
    *(u32 *)&work->raw[0x40] = value;
    value = 0;
    work->raw[0x4C] = value;
    work->raw[0x4B] = work->raw[0x4C];
    work->raw[0x4A] = value;

    switch (input) {
    case 7:
        work->raw[0x49] = 2;
        break;
    case 8:
        work->raw[0x49] = 3;
        break;
    default:
        work->raw[0x49] = input % 2;
        work->raw[0x4A] = input / 2;
        break;
    }

    return ov59_0223A194(work, input);
}

int ov59_0223A350(Ov59Gap4Work *work, u32 input) {
    u8 *selected;

    if (input <= 6) {
        PlaySE(0x5DC);
        ov59_022388C4(work);
        if (work->raw[0x1A + input] == 0) {
            return 5;
        }
        work->raw[0x4D] = input;
        work->raw[0x4E] = 1;
        selected = (u8 *)work + work->raw[0x4D];
        work->raw[0x4F] = selected[0x1A];
        return 6;
    }

    if (input == 8) {
        PlaySE(0x5DC);
        ov59_022387D0(work, 1);
        return 1;
    }
    return 5;
}
