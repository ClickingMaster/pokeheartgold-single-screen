#include "ov70_wave34_menu_private.h"

Ov70MenuState *ov70_02242014(void *input, int arg54, void *arg60) {
    Ov70MenuState *state = Heap_Alloc(0x3d, 0x80);
    u32 *src = input;
    u32 *dst = (u32 *)state;
    dst[0] = src[0];
    dst[1] = src[1];
    dst[2] = src[2];
    dst[3] = src[3];
    dst[4] = src[4];
    dst[5] = src[5];
    dst[6] = src[6];
    dst[9] = src[7];
    dst[10] = src[8];
    dst[11] = src[9];
    dst[8] = src[10];
    dst[12] = src[11];
    state->arg54 = (void *)arg54;
    state->arg60 = arg60;
    state->selection = 0;
    state->unk3e = 0;
    state->unk40 = 0;
    state->result42 = 0;
    state->result44 = 0;
    state->result45 = 0;
    state->cursor = 0;
    state->savedCursor = 0;
    state->cursorLimit = 0;
    state->messageData = BgCellManager_New(state->bg, 2, 1, 0x3d);
    BgCellManager_InitCell(state->messageData, 0, 2, 0x20, 0x14);
    Gts_SetSpritePosition(state->sprite, 0x88, 0x28);
    Sprite_SetAnimCtrlSeq(state->sprite, 0x2f);
    state->printer = MessagePrinter_New(0xf, 0xe, 2, 0x3d);
    return state;
}

void ov70_022420C4(Ov70MenuState *state, int mode) {
    state->mode58 = mode;
    switch (mode) {
    case 0:
        state->cursor = 0;
        state->savedCursor = 0;
        state->selection = 0;
        state->unk3e = 0;
        state->cursorLimit = 4;
        break;
    case 3:
        state->cursor = 0;
        state->savedCursor = 0;
        state->selection = 0;
        state->unk3e = 0;
        state->cursorLimit = 0x10;
        break;
    case 1:
        state->cursor = 0;
        state->savedCursor = 0;
        state->cursorLimit = 0x1c;
        break;
    case 2:
        state->cursor = 0;
        state->savedCursor = 0;
        state->cursorLimit = 0x1f;
        break;
    }
}

void ov70_0224212C(Ov70MenuState *state) {
    MessagePrinter_Delete(state->printer);
    BgCellManager_Delete(state->messageData);
    Heap_Free(state);
}

int ov70_02242144(Ov70MenuState *state) {
    int result = ov70_022466F8[state->cursorLimit](state);
    BgCellManager_Update(state->messageData);
    return result;
}
