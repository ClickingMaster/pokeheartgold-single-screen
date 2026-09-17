#include "to50_overlay65_residual6_private.h"

s32 ov65_0221DF1C(void *p) {
    Ov65StateR6 *state = p;
    if (gSystem.newKeys & PAD_BUTTON_B) {
        PlaySE(0x5DC);
        state->selection = 12;
        ov65_0221CB5C(state);
        ov65_0221DD34(state->selection, PTR(state, 0x344), 0);
    } else {
        ov65_0221DBF4(&state->menu);
        if ((gSystem.newKeys & PAD_BUTTON_A) && state->menu == NULL) {
            if (state->selection == 12) {
                state->callback = ov65_0221E144;
            } else if (state->selection < 6) {
                state->callback = ov65_0221E9A8;
            } else if (state->selection >= 6 && state->selection < 12) {
                state->callback = ov65_0221F3F4;
            }
        }
    }
    return 0;
}

s32 ov65_0221DFB8(void *p) {
    Ov65StateR6 *state = p;
    s32 hit = TouchscreenHitbox_FindRectAtTouchNew(ov65_0221FE6C);
    if (hit != -1) {
        if (hit == 12) {
            PlaySE(0x5DC);
            state->callback = ov65_0221E144;
            state->selection = 12;
        } else if (state->enabled[hit][0] != 0) {
            state->selection = hit;
            if (state->selection < 6) {
                state->callback = ov65_0221E9A8;
            } else if (state->selection >= 6 && state->selection < 12) {
                state->callback = ov65_0221F3F4;
            }
        }
        ov65_0221DD34(state->selection, PTR(state, 0x344), 0);
        ov65_0221CB5C(state);
        return 1;
    }
    return 0;
}
