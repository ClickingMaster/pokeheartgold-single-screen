#include "nintendo_wfc_connect_internal.h"

#include "global.h"

#include "sprite.h"

// Cursor of the Nintendo WFC connection screen's touch YES/NO menu.

// cursor x/y per choice (pairs of u16)
typedef struct NintendoWfcCursorPos {
    u16 x;
    u16 y;
} NintendoWfcCursorPos;

extern const NintendoWfcCursorPos gNintendoWfcYesNoCursorPositions[];

void NintendoWfcYesNo_MoveCursor(NintendoWfcYesNoMenu *menu, int choice) {
    VecFx32 pos;
    pos.x = gNintendoWfcYesNoCursorPositions[choice].x << FX32_SHIFT;
    pos.y = (menu->cursorOffsetY + gNintendoWfcYesNoCursorPositions[choice].y) << FX32_SHIFT;
    pos.z = 0;
    Sprite_SetMatrix(menu->cursorSprite, &pos);
}
