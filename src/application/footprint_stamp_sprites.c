#include "global.h"

#include "application/footprint_stamp.h"

// Built with -O2 (Makefile), so it cannot share a translation unit with its neighbours.

extern const ManagedSpriteTemplate sFootprintStampMyCursorTemplate;
extern const ManagedSpriteTemplate sFootprintStampPlayerFootprintTemplate;
extern const s16 sFootprintStampPlayerFootprintPositions[FOOTPRINT_STAMP_PLAYERS_MAX][2];

#pragma force_active on
#pragma opt_common_subs off

void FootprintStamp_CreatePlayerSprites(FootprintStamp *work) {
    ManagedSpriteTemplate template;
    int i;

    work->myCursorSprite = SpriteSystem_NewSprite(work->spriteSystem, work->spriteManager, &sFootprintStampMyCursorTemplate);
    Sprite_TickFrame(work->myCursorSprite->sprite);
    ManagedSprite_SetDrawFlag(work->myCursorSprite, FALSE);
    template = sFootprintStampPlayerFootprintTemplate;
    for (i = 0; i < FOOTPRINT_STAMP_PLAYERS_MAX; i++) {
        template.resIdList[0] = 10010 + i;
        work->playerFootprintSprites[i] = SpriteSystem_NewSprite(work->spriteSystem, work->spriteManager, &template);
        ManagedSprite_SetPositionXYWithSubscreenOffset(work->playerFootprintSprites[i], *(const s16 *)((u8 *)sFootprintStampPlayerFootprintPositions + i * 4), *(const s16 *)((u8 *)sFootprintStampPlayerFootprintPositions + i * 4 + 2), 0x110000); // byte offsets: indexing compiles differently
        Sprite_TickFrame(work->playerFootprintSprites[i]->sprite);
        ManagedSprite_SetDrawFlag(work->playerFootprintSprites[i], FALSE);
    }
}
