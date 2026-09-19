#include "to52_r5_overlay49_touch_state_r34_r4_private.h"
#define PTR(p, o) (*(void **)((u8 *)(p) + (o)))

int ov49_0225C8D4(W08State *state, void *arg1, void *arg2, int arg3) {
    int result = 0;
    if (state->disabled == 0) {
        TouchHitboxController_IsTriggered(state->touch);
    } else {
        state->selected = 1;
    }
    if (state->active == 1 && state->timer == 0) {
        state->active = 0;
        ov49_0225CAD4(state, arg1, 0, arg2, arg3);
    }
    if (state->selected != state->previous) {
        state->previous = state->selected;
        if (state->selected == 2) {
            ov49_0225CAD4(state, arg1, 1, arg2, arg3);
        } else if (state->active == 0) {
            ov49_0225CAD4(state, arg1, 0, arg2, arg3);
        } else {
            ov49_0225CAD4(state, arg1, 2, arg2, arg3);
        }
        if (state->active == 0 && state->selected == 2) {
            state->active = 1;
            result = 1;
            state->timer = 1;
        }
    }
    return result;
}

void ov49_0225C970(W08State *state, void *owner, void *messageCtx, void *input, void *narc, int heapId) {
    void *text;
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 0x37, PTR(owner, 0), 4, 0, 0, 0, heapId);
    state->flag = 0;
    state->resource = ov45_0222AAC8(input);
    GfGfxLoader_LoadCharDataFromOpenNarc(narc, state->resource, PTR(owner, 0), 6, 0x140, 0, 0, heapId);
    ov49_0225BB14(owner, narc, state->resource + 0x1b, 6, 0x140, heapId);
    text = ov49_0225B388(messageCtx, 1, 0x3f);
    FillWindowPixelBuffer(state->window, 0);
    AddTextPrinterParameterizedWithColor(state->window, 0, text, 0, 4, 0, 0xf0e00, NULL);
    if (state->active == 0) {
        ov49_0225CAD4(state, owner, 0, narc, heapId);
    } else {
        ov49_0225CAD4(state, owner, 2, narc, heapId);
    }
}

void ov49_0225CA30(W08State *state, void *owner, int selection, void *narc, int heapId) {
    if ((u32)selection >= 3) {
        GF_AssertFail();
    }
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 0x37, PTR(owner, 0), 4, 0, 0, 0, heapId);
    state->flag = 1;
    state->resource = selection;
    GfGfxLoader_LoadCharDataFromOpenNarc(narc, state->resource + 0x5d, PTR(owner, 0), 6, 0x140, 0, 0, heapId);
    if (state->active == 0) {
        ov49_0225CAD4(state, owner, 0, narc, heapId);
    } else {
        ov49_0225CAD4(state, owner, 2, narc, heapId);
    }
}
