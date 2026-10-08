#pragma force_active on
#include "overlay89_r2u1_private.h"
#pragma opt_common_subs off
// @fn ov89_022597FC
void ov89_022597FC(Ov89WorkHead *work) {
    ManagedSpriteTemplate tmpl;
    int i;
    work->spriteMain = SpriteSystem_NewSprite(work->spriteSystem, work->spriteManager, &ov89_0225CAA0);
    Sprite_TickFrame(work->spriteMain->sprite);
    ManagedSprite_SetDrawFlag(work->spriteMain, FALSE);
    tmpl = ov89_0225CAD4;
    for (i = 0; i < 8; i++) {
        tmpl.resIdList[0] = i + 0x271A;
        work->spritesD[i] = SpriteSystem_NewSprite(work->spriteSystem, work->spriteManager, &tmpl);
        ManagedSprite_SetPositionXYWithSubscreenOffset(work->spritesD[i], *(const s16 *)((u8 *)ov89_0225CA38 + i * 4), *(const s16 *)((u8 *)ov89_0225CA38 + i * 4 + 2), 0x110000);
        Sprite_TickFrame(work->spritesD[i]->sprite);
        ManagedSprite_SetDrawFlag(work->spritesD[i], FALSE);
    }
}
