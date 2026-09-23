#include "to50_overlay99_record_sprite_resources_private.h"
void ov99_021E6A9C(Ov99ResourceWork *work)
{
    SpriteSystem *system = ov98_0221E5C0(work->spriteContext);
    SpriteManager *manager = ov98_0221E5D0(work->spriteContext);
    SpriteSystem_LoadCharResObjFromOpenNarc(system, manager, work->narc, 0x11, TRUE, 1, 0);
    SpriteSystem_LoadPlttResObjFromOpenNarc(system, manager, work->narc, 0x10, FALSE, 2, 1, 0);
    SpriteSystem_LoadCellResObjFromOpenNarc(system, manager, work->narc, 0x12, TRUE, 0);
    SpriteSystem_LoadAnimResObjFromOpenNarc(system, manager, work->narc, 0x13, TRUE, 0);
    SpriteSystem_LoadCharResObjFromOpenNarc(system, manager, work->narc, 9, TRUE, 2, 4);
    SpriteSystem_LoadPlttResObjFromOpenNarc(system, manager, work->narc, 8, FALSE, 1, 2, 4);
    SpriteSystem_LoadCharResObjFromOpenNarc(system, manager, work->narc, 9, TRUE, 1, 2);
    SpriteSystem_LoadPlttResObjFromOpenNarc(system, manager, work->narc, 8, FALSE, 1, 1, 2);
    SpriteSystem_LoadCellResObjFromOpenNarc(system, manager, work->narc, 10, TRUE, 1);
    SpriteSystem_LoadAnimResObjFromOpenNarc(system, manager, work->narc, 11, TRUE, 1);
    SpriteSystem_LoadCharResObjFromOpenNarc(system, manager, work->narc, 1, TRUE, 1, 1);
    SpriteSystem_LoadPlttResObjFromOpenNarc(system, manager, work->narc, 0, FALSE, 1, 1, 1);
    SpriteSystem_LoadCharResObjFromOpenNarc(system, manager, work->narc, 1, TRUE, 2, 3);
    SpriteSystem_LoadPlttResObjFromOpenNarc(system, manager, work->narc, 0, FALSE, 1, 2, 3);
    SpriteSystem_LoadCellResObjFromOpenNarc(system, manager, work->narc, 2, TRUE, 2);
    SpriteSystem_LoadAnimResObjFromOpenNarc(system, manager, work->narc, 3, TRUE, 2);
}
