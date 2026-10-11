#include "../include/to57_overlay08_bank14_private.h"
#pragma force_active on

void BattlePartyMenu_CreateSprites(void *arg) {
    Ov08R2W11Work *work = arg;
    u32 i;
    for (i = 0; i < 38; i++) {
        work->sprites[i] = BattlePartyMenu_CreateSprite(work, i);
    }
    BattlePartyMenu_SetMonIconPalettes(work);
}

void BattlePartyMenu_FreeSprites(void *arg) {
    Ov08R2W11Work *work = arg;
    u32 i;
    void *system = BattleSystem_GetSpriteSystem(*(void **)((u8 *)work->root + 8));
    for (i = 0; i < 38; i++) {
        Sprite_DeleteAndFreeResources(work->sprites[i]);
    }
    BattlePartyMenu_FreeCursor(work);
    SpriteSystem_FreeResourcesAndManager(system, work->spriteManager);
}
