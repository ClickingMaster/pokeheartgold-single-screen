#include "to56_w13_overlay08_bank32_private.h"

#pragma force_active on

void ov08_02223E3C(W13Work *w, int item, int resId) {
    void *system = BattleSystem_GetSpriteSystem(w->root->battleSystem);
    SpriteSystem_ReplaceCharResObj(system, w->spriteManager, 0x12, GetItemIndexMapping(item, 1), FALSE, resId);
}

void ov08_02223E74(W13Work *w, int item, int slot) {
    PaletteData_LoadNarc(w->palette, 0x12, GetItemIndexMapping(item, 2), w->root->heapId, 3, 0x20, (u16)(slot << 4));
}
