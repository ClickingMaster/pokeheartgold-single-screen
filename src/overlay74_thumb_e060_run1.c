#include "constants/sndseq.h"

#include "overlay74_thumb_e060_private.h"

void ov74_0222E060(void *p) {
    W08Work *work = p;

    if (work->index >= 3) {
        GF_AssertFail();
    }
    SaveSubstruct_AssertCRC(0x1b);
    if (work->entries[work->index]->field_350 != 0xff) {
        work->entries[work->index]->field_350++;
    }
    if (work->entries[work->index]->field_348 != 0xff) {
        work->entries[work->index]->field_348--;
    }
    SaveSubstruct_UpdateCRC(0x1b);
    ov74_022360A0(work->save);
    work->callback = ov74_0222E03C;
}

void ov74_0222E0D4(void *p, BOOL enabled, int *state) {
    W08Work *work = p;
    int old = work->selection;
    int action;

    if ((gSystem.newKeys & PAD_KEY_RIGHT) && old != 1) {
        work->selection = 1;
    }
    if ((gSystem.newKeys & PAD_KEY_LEFT) && work->selection != 0) {
        work->selection = 0;
    }
    if (old != work->selection) {
        Sprite_SetAnimCtrlSeq(work->sprite_left, work->selection == 0);
        Sprite_SetAnimCtrlSeq(work->sprite_right, work->selection != 0);
    }

    action = 0;
    if (gSystem.newKeys & PAD_BUTTON_B) {
        action = 2;
    } else if ((gSystem.newKeys & PAD_BUTTON_A) && enabled && work->selection == 0) {
        action = 1;
    } else if ((gSystem.newKeys & PAD_BUTTON_A) && work->selection == 1) {
        action = 2;
    } else if ((gSystem.newKeys & PAD_BUTTON_A) && !enabled) {
        action = 3;
    }

    if (action == 1) {
        PlaySE(SEQ_SE_DP_SELECT);
        sub_02037AC0(0xab);
        work->counter = 1;
        *state = 0x16;
        ov74_0222D9E0(work, work->window, 0x11, 0x280);
        work->waiting_icon = WaitingIcon_New(work->window, 0x13);
    }
    if (action == 2) {
        PlaySE(SEQ_SE_DP_SELECT);
        ov74_0222EB28(p, state, 0x14);
    }
    if (action == 3) {
        PlaySE(SEQ_SE_DP_SELECT);
    }
}
