#include "to50_overlay27_input_handlers_private.h"

u32 ov27_0225CD94(Ov27Work *work) {
    int input = TouchscreenHitbox_FindRectAtTouchNew(ov27_0225D120);

    if (input != -1) {
        if (input == 0) {
            work->unk394 = 0;
            ov27_0225CCE0(work, work->unk394);
            Sprite_SetAnimCtrlSeq(work->unk388, 3);
            PlaySE(1500);
            work->unk00 = 5;
        } else if (input == 1) {
            work->unk394 = 1;
            ov27_0225CCE0(work, work->unk394);
            Sprite_SetAnimCtrlSeq(work->unk388, 3);
            PlaySE(1500);
            work->unk00 = 5;
        }
    } else if (NEW_KEYS & 0x40) {
        if (work->unk394 == 1) {
            PlaySE(1500);
        }
        work->unk394 = 0;
        ov27_0225CCE0(work, work->unk394);
    } else if (NEW_KEYS & 0x80) {
        if (work->unk394 == 0) {
            PlaySE(1500);
        }
        work->unk394 = 1;
        ov27_0225CCE0(work, work->unk394);
    } else if (NEW_KEYS & 1) {
        Sprite_SetAnimCtrlSeq(work->unk388, 3);
        work->unk00 = 5;
        PlaySE(1500);
    } else if (NEW_KEYS & 2) {
        work->unk394 = 1;
        ov27_0225CCE0(work, work->unk394);
        Sprite_SetAnimCtrlSeq(work->unk388, 3);
        PlaySE(1500);
        work->unk00 = 5;
    }
    return 0;
}

void ov27_0225CEAC(Ov27Work *work) {
    ov27_0225C5E4(work);
    ov27_0225C9CC(work);
}
