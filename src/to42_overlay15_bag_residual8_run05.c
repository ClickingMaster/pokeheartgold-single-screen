#include "to42_overlay15_bag_residual8_private.h"

void BagApp_DrawItemMenu(void *work, const u8 *choices) {
    U32_AT(work, 0x66c) = 0;
    BagApp_SetCursorPosition(work, sItemMenuCursorButtons[U32_AT(work, 0x66c)]);
    BagApp_ShowItemMenuSprites(work, choices);
    BagApp_PrintItemMenuLabel((u8 *)work + 0x184, (u8 *)work + 0x300, choices[0]);
    BagApp_PrintItemMenuLabel((u8 *)work + 0x194, (u8 *)work + 0x300, choices[1]);
    BagApp_PrintItemMenuLabel((u8 *)work + 0x1a4, (u8 *)work + 0x300, choices[2]);
    BagApp_PrintItemMenuLabel((u8 *)work + 0x1b4, (u8 *)work + 0x300, choices[3]);
}
