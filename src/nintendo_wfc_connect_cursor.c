#include "global.h"

#include "sprite.h"

// Cursor of the Nintendo WFC connection screen's touch YES/NO menu.

// fields of NintendoWfcYesNoMenu used here
typedef struct NintendoWfcYesNoCursor {
    void *bgConfig;       // 0x00
    Sprite *cursorSprite; // 0x04
    u8 filler_08[0x2C];
    int cursorOffsetY; // 0x34
} NintendoWfcYesNoCursor;

// cursor x/y per choice (pairs of u16)
extern const u16 gNintendoWfcYesNoCursorPositions[];
extern const u16 gNintendoWfcYesNoCursorPositionsY[];

void NintendoWfcYesNo_MoveCursor(NintendoWfcYesNoCursor *menu, int choice);

void NintendoWfcYesNo_MoveCursor(NintendoWfcYesNoCursor *menu, int choice) {
    VecFx32 pos;
    pos.x = gNintendoWfcYesNoCursorPositions[choice * 2] << FX32_SHIFT;
    pos.y = (menu->cursorOffsetY + gNintendoWfcYesNoCursorPositionsY[choice * 2]) << FX32_SHIFT;
    pos.z = 0;
    Sprite_SetMatrix(menu->cursorSprite, &pos);
}
