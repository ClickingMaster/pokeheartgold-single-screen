#include "to56_w13_overlay08_bank32_private.h"

#pragma force_active on

void BattleBagMenu_InitSprites(W13Work *w) {
    BattleBagMenu_CreateSpriteManager(w);
    BattleBagMenu_LoadItemIconResources(w);
    BattleBagMenu_CreateSprites(w);
    BattleBagMenu_CreateCursor(w);
    BattleBagMenu_CreateFinger(w);
    GfGfx_EngineBTogglePlanes(0x10, TRUE);
}

void BattleBagMenu_CreateSpriteManager(W13Work *w) {
    W13Caps caps = sBattleBagSpriteCapacities;
    void *system = BattleSystem_GetSpriteSystem(w->root->battleSystem);
    w->spriteManager = SpriteManager_New();
    SpriteSystem_InitSprites(system, w->spriteManager, 12);
    SpriteSystem_InitManagerWithCapacities(system, w->spriteManager, &caps);
}
