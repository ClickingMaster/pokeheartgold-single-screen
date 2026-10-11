#include "to42_overlay15_bag_residual8_private.h"

void BagApp_RedrawPocket(void *work) {
    u8 *state;
    u8 *slot;
    BagApp_LoadPocketItemNames(work);
    state = PTR(work, 0x234);
    slot = state + 4 + U8_AT(state, 0x64) * 12;
    BagApp_LoadSubScreenLayout(work, 0, BagApp_GetPageItemCount(work), 0);
    BagApp_DrawItemList(work, S16_AT(slot, 6), -1, 0);
    BagApp_LoadPocketObjPalette(work, U8_AT(PTR(work, 0x234), 0x64));
    BagApp_LoadPocketSubBgPalette(work, 1, U8_AT(PTR(work, 0x234), 0x64));
    BagApp_PrintPageNumber(work, U8_AT(slot, 9), S16_AT(slot, 6), 0);
    BagApp_DrawItemIcons(work, slot, BagApp_GetPageItemCount(work), 0);
    BagApp_SetCursorPosition(work, U32_AT(work, 0x644));
    BagApp_LoadInfoPanelTilemap(work, 1);
}
