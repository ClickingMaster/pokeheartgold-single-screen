#include "to56_w13_overlay08_bank32_private.h"

#pragma force_active on

void ov08_02223F14(W13Work *w) {
    W13Work *base = w;
    u32 offset = 0x310;
    u32 i = 0;
    W13Work *cursor = base;
    do {
        *(void **)((u8 *)cursor + offset) = ov08_02223EA4(base, i);
        i++;
        cursor = (W13Work *)((u8 *)cursor + 4);
    } while (i < 6);
}

void ov08_02223F34(W13Work *w) {
    W13Work *base = w;
    void *system = BattleSystem_GetSpriteSystem(base->root->battleSystem);
    u32 offset = 0x310;
    u32 i = 0;
    W13Work *cursor = base;
    do {
        Sprite_DeleteAndFreeResources(*(void **)((u8 *)cursor + offset));
        i++;
        cursor = (W13Work *)((u8 *)cursor + 4);
    } while (i < 6);
    ov08_02224108(base);
    ov08_022241F4(base);
    SpriteSystem_FreeResourcesAndManager(system, base->spriteManager);
}
