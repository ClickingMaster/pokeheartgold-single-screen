#include "global.h"

#include "frontier/frontier_map.h"

#include "sprite_system.h"

// The four extra sprite kinds frontier scripts can put on the map (NARC a/1/8/4).

extern const ManagedSpriteTemplate ov80_0223DA54[4];

void FrontierMapSprite_LoadResources(SpriteSystem *spriteSystem, SpriteManager *spriteManager, NARC *narc, PaletteData *paletteData, u16 kind) {
    const ManagedSpriteTemplate *template;

    if (kind >= 4) {
        GF_AssertFail();
    }
    template = &ov80_0223DA54[kind];
    SpriteSystem_LoadCharResObjFromOpenNarc(spriteSystem, spriteManager, narc, template->resIdList[GF_GFX_RES_TYPE_CHAR], TRUE, NNS_G2D_VRAM_TYPE_2DMAIN, template->resIdList[GF_GFX_RES_TYPE_CHAR]);
    SpriteSystem_LoadPaletteBufferFromOpenNarc(paletteData, PLTTBUF_MAIN_OBJ, spriteSystem, spriteManager, narc, template->resIdList[GF_GFX_RES_TYPE_PLTT], FALSE, 1, NNS_G2D_VRAM_TYPE_2DMAIN, template->resIdList[GF_GFX_RES_TYPE_PLTT]);
    SpriteSystem_LoadCellResObjFromOpenNarc(spriteSystem, spriteManager, narc, template->resIdList[GF_GFX_RES_TYPE_CELL], TRUE, template->resIdList[GF_GFX_RES_TYPE_CELL]);
    SpriteSystem_LoadAnimResObjFromOpenNarc(spriteSystem, spriteManager, narc, template->resIdList[GF_GFX_RES_TYPE_ANIM], TRUE, template->resIdList[GF_GFX_RES_TYPE_ANIM]);
}

void FrontierMapSprite_UnloadResources(SpriteManager *spriteManager, u32 kind) {
    const ManagedSpriteTemplate *template;

    if (kind >= 4) {
        GF_AssertFail();
    }
    template = &ov80_0223DA54[kind];
    SpriteManager_UnloadCharObjById(spriteManager, template->resIdList[GF_GFX_RES_TYPE_CHAR]);
    SpriteManager_UnloadPlttObjById(spriteManager, template->resIdList[GF_GFX_RES_TYPE_PLTT]);
    SpriteManager_UnloadCellObjById(spriteManager, template->resIdList[GF_GFX_RES_TYPE_CELL]);
    SpriteManager_UnloadAnimObjById(spriteManager, template->resIdList[GF_GFX_RES_TYPE_ANIM]);
}

ManagedSprite *FrontierMapSprite_Create(SpriteSystem *spriteSystem, SpriteManager *spriteManager, u32 kind) {
    ManagedSprite *sprite;

    if (kind >= 4) {
        GF_AssertFail();
    }
    sprite = SpriteSystem_NewSprite(spriteSystem, spriteManager, &ov80_0223DA54[kind]);
    Sprite_TickFrame(sprite->sprite);
    return sprite;
}

void FrontierMapSprite_Delete(ManagedSprite *sprite) {
    Sprite_DeleteAndFreeResources(sprite);
}
