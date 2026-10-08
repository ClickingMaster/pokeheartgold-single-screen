#pragma force_active on
#include "global.h"
#include "sprite_system.h"
#include "gf_gfx_loader.h"

typedef struct {
    u8 unk0[0x220];
    NARC *narc;
    SpriteSystem *spriteSys;
    SpriteManager *spriteMan;
    BgConfig *bgConfig;
    PaletteData *plttData;
} Ov40Work;

void ov40_02243A28(Ov40Work *work);

// @fn ov40_02243A28
void ov40_02243A28(Ov40Work *work) {
    NARC *narc;
    SpriteSystem *spriteSys;
    SpriteManager *spriteMan;
    BgConfig *bgConfig;
    PaletteData *plttData;

    spriteSys = work->spriteSys;
    spriteMan = work->spriteMan;
    plttData = work->plttData;
    bgConfig = work->bgConfig;
    narc = work->narc;

    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 54, bgConfig, GF_BG_LYR_MAIN_3, 0, 0, FALSE, HEAP_ID_109);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 43, bgConfig, GF_BG_LYR_MAIN_3, 0, 0, FALSE, HEAP_ID_109);
    SpriteSystem_LoadPaletteBufferFromOpenNarc(plttData, PLTTBUF_MAIN_OBJ, spriteSys, spriteMan, narc, 44, FALSE, 1, NNS_G2D_VRAM_TYPE_2DMAIN, 1000);
    SpriteSystem_LoadCharResObjFromOpenNarc(spriteSys, spriteMan, narc, 40, FALSE, NNS_G2D_VRAM_TYPE_2DMAIN, 1000);
    SpriteSystem_LoadCellResObjFromOpenNarc(spriteSys, spriteMan, narc, 41, FALSE, 1000);
    SpriteSystem_LoadAnimResObjFromOpenNarc(spriteSys, spriteMan, narc, 42, FALSE, 1000);
    SpriteSystem_LoadPaletteBufferFromOpenNarc(plttData, PLTTBUF_MAIN_OBJ, spriteSys, spriteMan, narc, 88, FALSE, 1, NNS_G2D_VRAM_TYPE_2DMAIN, 1001);
    SpriteSystem_LoadCharResObjFromOpenNarc(spriteSys, spriteMan, narc, 87, FALSE, NNS_G2D_VRAM_TYPE_2DMAIN, 1001);
    SpriteSystem_LoadCellResObjFromOpenNarc(spriteSys, spriteMan, narc, 85, FALSE, 1001);
    SpriteSystem_LoadAnimResObjFromOpenNarc(spriteSys, spriteMan, narc, 86, FALSE, 1001);
}
