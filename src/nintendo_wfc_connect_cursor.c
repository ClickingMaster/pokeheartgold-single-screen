#pragma force_active on
#include "global.h"

#include "sprite.h"

// Cursor of the Nintendo WFC connection screen's touch YES/NO menu (src/nintendo_wfc_connect.c);
// linked after the overlay's data, so it is its own TU.

// fields of NintendoWfcYesNoMenu (src/nintendo_wfc_connect.c) used here
typedef struct NintendoWfcYesNoCursor {
    void *bgConfig;       // 0x00
    Sprite *cursorSprite; // 0x04
    u8 filler_08[0x2C];
    int cursorOffsetY; // 0x34
} NintendoWfcYesNoCursor;

// cursor x/y per choice (pairs of u16)
extern const u16 ov72_0223B4C0[];
extern const u16 ov72_0223B4C2[];

void NintendoWfcYesNo_MoveCursor(NintendoWfcYesNoCursor *menu, int choice);

void NintendoWfcYesNo_MoveCursor(NintendoWfcYesNoCursor *menu, int choice) {
    VecFx32 pos;
    pos.x = ov72_0223B4C0[choice * 2] << FX32_SHIFT;
    pos.y = (menu->cursorOffsetY + ov72_0223B4C2[choice * 2]) << FX32_SHIFT;
    pos.z = 0;
    Sprite_SetMatrix(menu->cursorSprite, &pos);
}
