#include "landing_private.h"

void ov45_0222BB60(Ov45TimedState *state, Ov45FlagOwner *owner, void *arg2, void *arg3) {
    if (state->timer4 - 1 >= 0) {
        state->timer4--;
        if (state->timer4 == 0x348) {
            GF_SndStartFadeOutBGM(0, 0x7f);
            state->unkE = 1;
        }
    } else if (state->timer4 == 0) {
        state->flag1 = 1;
        owner->flags |= 0x80;
        state->timer4--;
    }

    if (state->timer6 - 1 >= 0) {
        state->timer6--;
    } else if (state->timer6 == 0) {
        owner->flags |= 4;
        owner->flags |= 8;
        owner->flags |= 2;
        state->timer6--;
    }

    if (state->timer8 - 1 >= 0) {
        state->timer8--;
    } else if (state->timer8 == 0) {
        state->flag23 = 1;
        owner->flags |= 0x20;
        state->timer8--;
        ov45_0222CA8C(arg3);
    }

    if (state->timerA - 1 >= 0) {
        state->timerA--;
    } else if (state->timerA == 0) {
        ov45_0222C3A8(arg2);
        if (state->unkC == 0) {
            PlayBGM(0x481);
        }
        state->flag4 = 1;
        owner->flags |= 0x40;
        state->timerA--;
    }
}

void ov45_0222BC3C(Ov45TimedState *state) {
    state->flag0 = 0;
    state->flag1 = 0;
    state->flag23 = 0;
    state->flag4 = 0;
    state->flag56 = 0;
    state->flag7 = 0;
    state->unk1 = 1;
    state->unk2 = 7;
    state->unk3 = 11;
    state->timer4 = -1;
    state->timer6 = -1;
    state->timer8 = -1;
    state->timerA = -1;
}

void ov45_0222BC84(Ov45TimedState *state) {
    if (state->unkD == 1) {
        GF_SndHandleSetPlayerVolume(7, 0x2a);
    } else {
        GF_SndHandleSetPlayerVolume(7, 0x7f);
    }
}
