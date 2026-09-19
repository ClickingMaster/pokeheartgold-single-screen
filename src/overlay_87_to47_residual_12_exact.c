#include "overlay_87_to47_residual_12_exact_private.h"

u32 ov87_021E788C(State *state) {
    u32 ret;
    int i;
    State *walker;
    ret = 0;
    i = 0;
    walker = state;
    do {
        if (state->raw[0x360 + state->raw[0x3A2 + i]] == 4) {
            ov87_021E80A8(*(SpriteWrap **)&walker->raw[0x350], 1);
            ret = 1;
        }
        i++;
        walker = (State *)((u8 *)walker + 4);
    } while (i < 3);
    state->raw[0x14] = 0;
    *(vu8 *)0x0400004D = state->raw[0x14] | (state->raw[0x14] << 4);
    return ret;
}

int ov87_021E78D8(State *state, int dir) {
    int i;
    for (i = 0; i < 3; i++) {
        if (state->raw[0x360 + state->raw[0x3A2 + i]] == 4) {
            break;
        }
    }
    if (i == 3) {
        return 0;
    }
    if (dir == 0) {
        if (state->raw[0x14] == 0) {
            PlaySE(0x563);
        }
        if (state->raw[0x14] < 4) {
            state->raw[0x14]++;
        } else {
            State *walker;
            i = 0;
            walker = state;
            do {
                if (state->raw[0x360 + state->raw[0x3A2 + i]] == 4) {
                    ov87_021E803C(*(SpriteWrap **)&walker->raw[0x350], state->raw[0x39F]);
                }
                i++;
                walker = (State *)((u8 *)walker + 4);
            } while (i < 3);
            return 0;
        }
    } else {
        if (state->raw[0x14] != 0) {
            state->raw[0x14]--;
        } else {
            int offset;
            int zero;
            i = 0;
            offset = 0x350;
            zero = i;
            do {
                ov87_021E80A8(*(SpriteWrap **)&state->raw[offset], zero);
                i++;
                state = (State *)((u8 *)state + 4);
            } while (i < 3);
            return 0;
        }
    }
    *(vu8 *)0x0400004D = state->raw[0x14] | (state->raw[0x14] << 4);
    return 1;
}
