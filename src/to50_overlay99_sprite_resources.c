#include "to50_overlay99_sprite_resources_private.h"

void ov99_021E6274(Ov99ResourceWork *work) {
    SpriteSystem *system = ov98_0221E5C0(work->spriteContext);
    SpriteManager *manager = ov98_0221E5D0(work->spriteContext);

    SpriteSystem_LoadCharResObjFromOpenNarc(system, manager, work->narc, 0x15, TRUE, 1, 0);
    SpriteSystem_LoadPlttResObjFromOpenNarc(system, manager, work->narc, 0x14, FALSE, 1, 1, 0);
    SpriteSystem_LoadCellResObjFromOpenNarc(system, manager, work->narc, 0x16, TRUE, 0);
    SpriteSystem_LoadAnimResObjFromOpenNarc(system, manager, work->narc, 0x17, TRUE, 0);

    SpriteSystem_LoadCharResObjFromOpenNarc(system, manager, work->narc, 1, TRUE, 1, 1);
    SpriteSystem_LoadPlttResObjFromOpenNarc(system, manager, work->narc, 0, FALSE, 1, 1, 1);
    SpriteSystem_LoadCellResObjFromOpenNarc(system, manager, work->narc, 2, TRUE, 1);
    SpriteSystem_LoadAnimResObjFromOpenNarc(system, manager, work->narc, 3, TRUE, 1);

    SpriteSystem_LoadCharResObjFromOpenNarc(system, manager, work->narc, 5, TRUE, 1, 2);
    SpriteSystem_LoadPlttResObjFromOpenNarc(system, manager, work->narc, 4, FALSE, 5, 1, 2);
    SpriteSystem_LoadCellResObjFromOpenNarc(system, manager, work->narc, 6, TRUE, 2);
    SpriteSystem_LoadAnimResObjFromOpenNarc(system, manager, work->narc, 7, TRUE, 2);

    SpriteSystem_LoadCharResObjFromOpenNarc(system, manager, work->narc, 9, TRUE, 2, 6);
    SpriteSystem_LoadPlttResObjFromOpenNarc(system, manager, work->narc, 8, FALSE, 1, 2, 3);
    SpriteSystem_LoadCellResObjFromOpenNarc(system, manager, work->narc, 10, TRUE, 3);
    SpriteSystem_LoadAnimResObjFromOpenNarc(system, manager, work->narc, 11, TRUE, 3);

    ov99_021E6400(work);
    ov98_0221E8A8(work->spriteContext, ov99_021E9554, 3, 0, TRUE);
}
