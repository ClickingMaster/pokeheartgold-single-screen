#include "to52_r6_w18_overlay80_r38_private.h"

void ov80_0222F33C(W18FrontierMap *map) {
    NARC *narc = NARC_New(NARC_graphic_plist_gra, HEAP_ID_101);

    SpriteSystem_LoadPaletteBufferFromOpenNarc(map->paletteData, PLTTBUF_MAIN_OBJ, map->spriteSystem, map->spriteManager, narc, sub_0207CAA0(), FALSE, 1, 1, 0x7D1);
    SpriteSystem_LoadCellResObjFromOpenNarc(map->spriteSystem, map->spriteManager, narc, sub_0207CAA4(), FALSE, 0x7D1);
    SpriteSystem_LoadAnimResObjFromOpenNarc(map->spriteSystem, map->spriteManager, narc, sub_0207CAA8(), FALSE, 0x7D1);
    SpriteSystem_LoadCharResObjAtEndWithHardwareMappingType(map->spriteSystem, map->spriteManager, NARC_graphic_plist_gra, sub_0207CA9C(), FALSE, 1, 0x7D9);
    NARC_Delete(narc);
}

void ov80_0222F3CC(W18FrontierMap *map) {
    SpriteManager_UnloadCharObjById(map->spriteManager, 0x7D9);
    SpriteManager_UnloadCellObjById(map->spriteManager, 0x7D1);
    SpriteManager_UnloadAnimObjById(map->spriteManager, 0x7D1);
    SpriteManager_UnloadPlttObjById(map->spriteManager, 0x7D1);
}

ManagedSprite *ov80_0222F3FC(W18FrontierMap *map, s16 x, s16 y) {
    ManagedSpriteTemplate template = ov80_0223BD4C;
    ManagedSprite *sprite;

    template.x = x;
    template.y = y;
    template.drawPriority = 0x12C;
    sprite = SpriteSystem_NewSprite(map->spriteSystem, map->spriteManager, &template);
    ManagedSprite_TickFrame(sprite);
    return sprite;
}
