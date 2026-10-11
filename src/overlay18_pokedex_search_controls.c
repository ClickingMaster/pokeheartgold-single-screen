#include "overlay18_pokedex_search_controls_private.h"

void PokedexApp_CreateSearchFormGrid(Ov18MenuState *state) {
    *(GridInputHandler **)&state->raw[0x1864] = GridInputHandler_Create(sSearchFormHitboxes, sSearchFormDpadBoxes, sSearchFormGridCallbacks, state, TRUE, 0, (enum HeapID)0x25);
    PokedexApp_SetSpriteVisible(state, 0, 1);
    PokedexApp_UpdateSearchFormCursor(state, 0);
}

int PokedexApp_HandleSearchFormInput(Ov18MenuState *state) {
    MenuInputStateMgr_SetState(*(MenuInputStateMgr **)((u8 *)*(void **)&state->raw[0] + 0xc), (MenuInputState)0);
    if (gSystem.newKeys & 8) {
        return 0xf;
    }
    return GridInputHandler_HandleInput_AllowHold(*(GridInputHandler **)&state->raw[0x1864]);
}

void PokedexApp_UpdateSearchFormCursor(Ov18MenuState *state, int index) {
    u8 pos[2];
    const DpadMenuBox *box = GridInputHandler_GetDpadBox(*(GridInputHandler **)&state->raw[0x1864], index);
    DpadMenuBox_GetPosition(box, &pos[1], &pos[0]);
    ManagedSprite_SetPositionXY(*(ManagedSprite **)&state->raw[0x670], pos[1], pos[0]);
    PokedexApp_SetSpriteAnim(state, 0, sSearchFormCursorAnims[index]);
}

void PokedexApp_OnSearchFormGridMove(Ov18MenuState *state, int index, int input) {
    int current;

    if (input == 0xf) {
        if (index == 0) {
            current = *(int *)&state->raw[0x189c];
            if (current >= 0 && current <= 2) {
                index = current;
                GridInputHandler_SetNextLastUnk0FInputs(*(GridInputHandler **)&state->raw[0x1864], (u8)index, (u8)input, 0xf);
            } else if (current >= 10 && current <= 12) {
                index = current % 5;
                GridInputHandler_SetNextLastUnk0FInputs(*(GridInputHandler **)&state->raw[0x1864], (u8)index, (u8)input, 0xf);
            }
        } else if (index == 10) {
            current = *(int *)&state->raw[0x189c];
            if (current >= 0 && current <= 2) {
                index = current + 10;
                GridInputHandler_SetNextLastUnk0FInputs(*(GridInputHandler **)&state->raw[0x1864], (u8)index, (u8)input, 0xf);
            } else if (current >= 10 && current <= 12) {
                index = current;
                GridInputHandler_SetNextLastUnk0FInputs(*(GridInputHandler **)&state->raw[0x1864], (u8)index, (u8)input, 0xf);
            }
        }
    } else if (input == 0x10) {
        if (index == 4) {
            current = *(int *)&state->raw[0x189c];
            if (current >= 3 && current <= 4) {
                index = current;
                GridInputHandler_SetNextLastUnk0FInputs(*(GridInputHandler **)&state->raw[0x1864], (u8)index, (u8)input, 0x10);
            } else if (current >= 13 && current <= 14) {
                index = current % 5;
                GridInputHandler_SetNextLastUnk0FInputs(*(GridInputHandler **)&state->raw[0x1864], (u8)index, (u8)input, 0x10);
            }
        } else if (index == 14) {
            current = *(int *)&state->raw[0x189c];
            if (current >= 3 && current <= 4) {
                index = current + 10;
                GridInputHandler_SetNextLastUnk0FInputs(*(GridInputHandler **)&state->raw[0x1864], (u8)index, (u8)input, 0x10);
            } else if (current >= 13 && current <= 14) {
                index = current;
                GridInputHandler_SetNextLastUnk0FInputs(*(GridInputHandler **)&state->raw[0x1864], (u8)index, (u8)input, 0x10);
            }
        }
    }
    PokedexApp_UpdateSearchFormCursor(state, index);
    *(int *)&state->raw[0x189c] = input;
    PlaySE(0x8e8);
}
