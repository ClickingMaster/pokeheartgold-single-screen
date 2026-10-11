#include "to56_overlay08_bank34_private.h"
#pragma force_active on

void BattleBagMenu_ShowPageSprites(u8 *work, u32 mode) {
    u32 i = 0;
    u8 *cursor = work;
    for (; i < 6; i++) {
        ManagedSprite_SetDrawFlag(PTR(cursor, 0x310), FALSE);
        cursor += 4;
    }
    switch (mode) {
    case 0:
        BattleBagMenu_ShowLastUsedItemIcon(work);
        break;
    case 1:
        BattleBagMenu_ShowItemListIcons(work);
        break;
    case 2:
        BattleBagMenu_ShowUseItemIcon(work);
        break;
    }
}

void BattleBagMenu_ShowLastUsedItemIcon(u8 *work) {
    u16 value = U16(PTR(work, 0), 0x20);
    if (value != 0) {
        BattleBagMenu_SetItemIconGraphics(work, value, 0xB4B7);
        BattleBagMenu_SetItemIconPalette(work, U16(PTR(work, 0), 0x20), 0, 0xB4B7);
        BattleBagMenu_ShowSpriteAt(PTR(work, 0x310), 0x18, 0xB2);
    }
}

void BattleBagMenu_ShowItemListIcons(u8 *work) {
    const s32(*pos)[2] = sBattleBagItemListIconPos;
    u32 i = 0;
    u8 *spriteWork = work;
    for (; i < 6; i++) {
        u32 value = BattleBagMenu_GetItemOnPage(work, i);
        if (value != 0) {
            BattleBagMenu_SetItemIconGraphics(work, value, 0xB4B7 + i);
            BattleBagMenu_SetItemIconPalette(work, value, (u16)i, 0xB4B7 + i);
            BattleBagMenu_ShowSpriteAt(PTR(spriteWork, 0x310), (*pos)[0], (*pos)[1]);
        }
        pos++;
        spriteWork += 4;
    }
}

void BattleBagMenu_ShowUseItemIcon(u8 *work) {
    u8 *entry = (u8 *)PTR(work, 0) + U8(work, 0x114D);
    u32 value = BattleBagMenu_GetItemOnPage(work, entry[0x27]);
    BattleBagMenu_SetItemIconGraphics(work, value, 0xB4B7);
    BattleBagMenu_SetItemIconPalette(work, value, 0, 0xB4B7);
    BattleBagMenu_ShowSpriteAt(PTR(work, 0x310), 0x28, 0x2C);
}

void BattleBagMenu_CreateCursor(u8 *work) {
    void *spriteSystem = BattleSystem_GetSpriteSystem(PTR(PTR(work, 0), 0));
    BattleCursor_LoadResources(spriteSystem, PTR(work, 0x30C), PTR(work, 8), *(u32 *)((u8 *)PTR(work, 0) + 0xC), 0xB4BE, 0xB4BE, 0xB4B9, 0xB4B9);
    BattleDpadCursor_SetBattleCursor(PTR(work, 0x34), BattleCursor_New(spriteSystem, PTR(work, 0x30C), *(u32 *)((u8 *)PTR(work, 0) + 0xC), 0xB4BE, 0xB4BE, 0xB4B9, 0xB4B9, 0, 1));
}

void BattleBagMenu_FreeCursor(u8 *work) {
    BattleCursor_Delete(BattleDpadCursor_GetBattleCursor(PTR(work, 0x34)));
    BattleCursor_FreeResources(PTR(work, 0x30C), 0xB4BE, 0xB4BE, 0xB4B9, 0xB4B9);
}
