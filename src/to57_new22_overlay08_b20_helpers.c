#include "to57_new22_overlay08_b20_private.h"
void BattlePartyMenu_LoadPagePalette(To57New22Work *work, int state) {
    if (state == 3) {
        PaletteData_LoadPalette(work->palette, work->palette1, (PaletteBufferId)1, 0xC0, 0x20);
    } else {
        PaletteData_LoadPalette(work->palette, work->palette0, (PaletteBufferId)1, 0xC0, 0x20);
    }
}
u8 BattlePartyMenu_HasMultipleNonEggMons(To57New22Entry50 *entries) {
    u16 i;
    u16 count = 0;
    for (i = 0; i < 6; i++) {
        if (entries[i].active != 0 && !entries[i].disabled) {
            count++;
        }
    }
    if (count >= 2) {
        return (u8)TRUE;
    } else {
        return (u8)FALSE;
    }
}
void BattlePartyMenu_RedrawSelectedMonButton(u8 *work) {
    if (BattlePartyMenu_GetSlotType(work, (*(u8 **)work)[0x11]) == 2) {
        BattlePartyMenu_DrawButton(work, (*(u8 **)work)[0x11], 0, 1);
    } else {
        BattlePartyMenu_DrawButton(work, (*(u8 **)work)[0x11], 0, 0);
    }
}
