#include "../include/to57_overlay08_bank14_private.h"
#pragma force_active on

void ov08_02220A28(void *arg) {
    Ov08R2W11Work *work = arg;
    u32 i;
    for (i = 0; i < 38; i++) {
        work->sprites[i] = ov08_022209B8(work, i);
    }
    ov08_02220AAC(work);
}

void ov08_02220A50(void *arg) {
    Ov08R2W11Work *work = arg;
    u32 i;
    void *system = BattleSystem_GetSpriteSystem(*(void **)((u8 *)work->root + 8));
    for (i = 0; i < 38; i++) {
        Sprite_DeleteAndFreeResources(work->sprites[i]);
    }
    ov08_02221698(work);
    SpriteSystem_FreeResourcesAndManager(system, work->spriteManager);
}
