#include "to56_w13_overlay08_bank32_private.h"

#pragma force_active on

void ov08_02223D08(W13Work *w) {
    ov08_02223D34(w);
    ov08_02223D80(w);
    ov08_02223F14(w);
    ov08_022240A8(w);
    ov08_0222419C(w);
    GfGfx_EngineBTogglePlanes(0x10, TRUE);
}

void ov08_02223D34(W13Work *w) {
    W13Caps caps = ov08_02225D14;
    void *system = BattleSystem_GetSpriteSystem(w->root->battleSystem);
    w->spriteManager = SpriteManager_New();
    SpriteSystem_InitSprites(system, w->spriteManager, 12);
    SpriteSystem_InitManagerWithCapacities(system, w->spriteManager, &caps);
}
