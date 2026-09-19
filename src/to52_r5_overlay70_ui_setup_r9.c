#include "to52_r5_overlay70_ui_setup_r9_private.h"

void ov70_02239304(Ov70Work9 *work) {
    GfGfx_EngineATogglePlanes(1, 0);
    GfGfx_EngineATogglePlanes(2, 0);
    GfGfx_EngineATogglePlanes(4, 0);
    Sprite_SetDrawFlag(work->spriteDCC, 0);
}

void ov70_02239330(Ov70Work9 *work) {
    void *bgConfig = work->bgConfig;
    void *narc = NARC_New(0x64, 0x3D);

    GfGfxLoader_GXLoadPalFromOpenNarc(narc, 4, 0, 0, 0x60, 0x3D);
    LoadFontPal1(0, 0x1A0, 0x3D);
    LoadUserFrameGfx2(bgConfig, 0, 1, 0xE, (u8)Options_GetFrame(work->root->options), 0x3D);
    LoadUserFrameGfx1(bgConfig, 0, 0x1F, 0xB, 0, 0x3D);
    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 0x10, bgConfig, 1, 0, 0xC00, 1, 0x3D);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 0x27, bgConfig, 1, 0, 0x600, 1, 0x3D);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 0x26, bgConfig, 2, 0, 0x600, 1, 0x3D);
    ov70_02239C6C(work);
    if (work->initialized != 0) {
        ov70_02239CF8(work);
        GfGfx_EngineBTogglePlanes(1, 1);
    } else {
        GfGfx_EngineBTogglePlanes(1, 0);
    }
    NARC_Delete(narc);
}

void ov70_02239414(Ov70Work9 *work) {
    Ov70SpriteTemplate9 template;
    Ov70SpriteResourcesHeader9 header;

    CreateSpriteResourcesHeader(&header, 0, 0, 0, 0, -1, -1, 0, 1, work->resourceManagers[0], work->resourceManagers[1], work->resourceManagers[2], work->resourceManagers[3], NULL, NULL);
    ov70_02238B54(&template, work, &header, 1);
    template.x = ov70_02245240[work->spriteKind * 2] << 12;
    template.y = ov70_02245242[work->spriteKind * 2] << 12;
    work->spriteDCC = Sprite_CreateAffine(&template);
    Sprite_SetAnimActiveFlag(work->spriteDCC, 1);
    Sprite_SetAnimCtrlSeq(work->spriteDCC, 0x2B);
}
