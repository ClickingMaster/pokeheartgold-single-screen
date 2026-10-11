#include "overlay18_pokedex_grid_navigation_r28_private.h"

void PokedexApp_CreateSearchTypeGrid(PokedexAppData *pokedexApp) {
    pokedexApp->unk_1864 = GridInputHandler_Create(sSearchTypeHitboxes, sSearchTypeDpadBoxes, sSearchTypeGridCallbacks, pokedexApp, TRUE, 0, HEAP_ID_POKEDEX_APP);
    PokedexApp_SetSpriteVisible(pokedexApp, 0, 1);
    PokedexApp_UpdateSearchTypeCursor(pokedexApp, 0);
}

u32 PokedexApp_HandleSearchTypeInput(PokedexAppData *pokedexApp) {
    MenuInputStateMgr_SetState(*(MenuInputStateMgr **)((u8 *)pokedexApp->args + 0xC), (MenuInputState)0);
    if (gSystem.newKeys & 8) {
        return 0x12;
    }
    return GridInputHandler_HandleInput_AllowHold(pokedexApp->unk_1864);
}

void PokedexApp_UpdateSearchTypeCursor(PokedexAppData *pokedexApp, int index) {
    u8 position[2];
    const DpadMenuBox *box = GridInputHandler_GetDpadBox(pokedexApp->unk_1864, index);

    DpadMenuBox_GetPosition(box, &position[1], &position[0]);
    ManagedSprite_SetPositionXY(*(ManagedSprite **)((u8 *)pokedexApp + 0x670), position[1], position[0]);
    PokedexApp_SetSpriteAnim(pokedexApp, 0, sSearchTypeCursorAnims[index]);
}

void PokedexApp_OnSearchTypeGridMove(PokedexAppData *pokedexApp, int currentInput, int lastInput) {
    if (lastInput == 0x12) {
        if (currentInput == 0) {
            s32 previousInput = *(s32 *)pokedexApp->filler_189C;

            if (previousInput >= 0x10 && previousInput <= 0x11) {
                currentInput = previousInput % 4;
                GridInputHandler_SetNextLastUnk0FInputs(pokedexApp->unk_1864, (u8)currentInput, (u8)lastInput, 0x12);
            } else if (previousInput >= 0 && previousInput <= 1) {
                currentInput = previousInput;
                GridInputHandler_SetNextLastUnk0FInputs(pokedexApp->unk_1864, (u8)previousInput, (u8)lastInput, 0x12);
            }
        } else if (currentInput == 0x10) {
            s32 previousInput = *(s32 *)pokedexApp->filler_189C;

            if (previousInput >= 0 && previousInput <= 1) {
                currentInput = previousInput + 0x10;
                GridInputHandler_SetNextLastUnk0FInputs(pokedexApp->unk_1864, (u8)currentInput, (u8)lastInput, 0x12);
            } else if (previousInput >= 0x10 && previousInput <= 0x11) {
                currentInput = previousInput;
                GridInputHandler_SetNextLastUnk0FInputs(pokedexApp->unk_1864, (u8)previousInput, (u8)lastInput, 0x12);
            }
        }
    } else if (lastInput == 0x13) {
        if (currentInput == 3) {
            s32 previousInput = *(s32 *)pokedexApp->filler_189C;

            if (previousInput >= 0xE && previousInput <= 0xF) {
                currentInput = previousInput % 4;
                GridInputHandler_SetNextLastUnk0FInputs(pokedexApp->unk_1864, (u8)currentInput, (u8)lastInput, 0x13);
            } else if (previousInput >= 2 && previousInput <= 3) {
                currentInput = previousInput;
                GridInputHandler_SetNextLastUnk0FInputs(pokedexApp->unk_1864, (u8)previousInput, (u8)lastInput, 0x13);
            }
        } else if (currentInput == 0xF) {
            s32 previousInput = *(s32 *)pokedexApp->filler_189C;

            if (previousInput >= 2 && previousInput <= 3) {
                currentInput = previousInput + 0xC;
                GridInputHandler_SetNextLastUnk0FInputs(pokedexApp->unk_1864, (u8)currentInput, (u8)lastInput, 0x13);
            } else if (previousInput >= 0xE && previousInput <= 0xF) {
                currentInput = previousInput;
                GridInputHandler_SetNextLastUnk0FInputs(pokedexApp->unk_1864, (u8)previousInput, (u8)lastInput, 0x13);
            }
        }
    }
    PokedexApp_UpdateSearchTypeCursor(pokedexApp, currentInput);
    *(s32 *)pokedexApp->filler_189C = lastInput;
    PlaySE(SEQ_SE_GS_ZKN03);
}
