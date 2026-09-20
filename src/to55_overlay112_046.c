#include "to55_overlay112_046_private.h"

void ov112_021EA1AC(W07Work *work) {
    GfGfx_EngineATogglePlanes(0x10, 1);
    GfGfx_EngineBTogglePlanes(0x10, 1);
    GF_CreateVramTransferManager(0x40, HEAP_ID_154);
    ov112_021EA230(work);
    ov112_021EA2C4(work);
    ov112_021EA3C0(work);
    G2dRenderer_SetSubSurfaceCoords(SpriteSystem_GetRenderer(work->spriteSystem), 0, 1 << 20);
    ov112_021EAB58();
}

void ov112_021EA1F8(W07Work *work) {
    u32 i;
    for (i = 0; i < 217; i++) {
        Sprite_DeleteAndFreeResources(work->sprites[i]);
    }
    SpriteSystem_FreeResourcesAndManager(work->spriteSystem, work->spriteManager);
    SpriteSystem_Free(work->spriteSystem);
}

void ov112_021EA230(W07Work *work) {
    SpriteResourceCountsListUnion counts = ov112_021FECAC;
    OamManagerParam oam;
    OamCharTransferParam transfer;

    work->spriteSystem = SpriteSystem_Alloc(HEAP_ID_154);
    work->spriteManager = SpriteManager_New(work->spriteSystem);
    oam = ov112_021FECC4;
    transfer = ov112_021FEC98;
    SpriteSystem_Init(work->spriteSystem, &oam, &transfer, 0x20);
    SpriteSystem_InitSprites(work->spriteSystem, work->spriteManager, 217);
    SpriteSystem_InitManagerWithCapacities(work->spriteSystem, work->spriteManager, &counts);
}

void ov112_021EA2C4(W07Work *work) {
    NARC *narc = NARC_New((NarcId)0xF9, HEAP_ID_154);

    SpriteSystem_LoadCharResObjFromOpenNarc(work->spriteSystem, work->spriteManager, narc, 7, TRUE, 2, 0xC226);
    SpriteSystem_LoadPlttResObjFromOpenNarc(work->spriteSystem, work->spriteManager, narc, 3, FALSE, 2, 2, 0xC226);
    SpriteSystem_LoadCellResObjFromOpenNarc(work->spriteSystem, work->spriteManager, narc, 0x14, FALSE, 0xC226);
    SpriteSystem_LoadAnimResObjFromOpenNarc(work->spriteSystem, work->spriteManager, narc, 0x16, FALSE, 0xC226);
    SpriteSystem_LoadCharResObjFromOpenNarc(work->spriteSystem, work->spriteManager, narc, 6, TRUE, 1, 0xC227);
    SpriteSystem_LoadPlttResObjFromOpenNarc(work->spriteSystem, work->spriteManager, narc, 2, FALSE, 7, 1, 0xC227);
    SpriteSystem_LoadCellResObjFromOpenNarc(work->spriteSystem, work->spriteManager, narc, 0x13, FALSE, 0xC227);
    SpriteSystem_LoadAnimResObjFromOpenNarc(work->spriteSystem, work->spriteManager, narc, 0x15, FALSE, 0xC227);
    NARC_Delete(narc);
}

void ov112_021EA3C0(W07Work *work) {
    u32 i = 0;
    const ManagedSpriteTemplate *template = ov112_021FED1C;
    const u8 *palette = ov112_021FEC88;
    u8 *slot = (u8 *)work;

    for (; i < 17;) {
        *(ManagedSprite **)(slot + 0x1E530) = SpriteSystem_NewSpriteWithYOffset(work->spriteSystem, work->spriteManager, template, 1 << 20);
        ManagedSprite_SetPaletteOverride(*(ManagedSprite **)(slot + 0x1E530), *palette);
        ManagedSprite_SetDrawFlag(*(ManagedSprite **)(slot + 0x1E530), FALSE);
        i++;
        template++;
        slot += 4;
        palette++;
    }
    ManagedSprite_SetPriority(work->sprites[1], 2);
    for (; i < 117; i++) {
        work->sprites[i] = SpriteSystem_NewSprite(work->spriteSystem, work->spriteManager, &ov112_021FF028);
        ManagedSprite_SetPaletteOverride(work->sprites[i], 0);
        ManagedSprite_SetDrawFlag(work->sprites[i], FALSE);
        ManagedSprite_SetPriority(work->sprites[i], 2);
    }
    for (; i < 217; i++) {
        work->sprites[i] = SpriteSystem_NewSpriteWithYOffset(work->spriteSystem, work->spriteManager, &ov112_021FF05C, 1 << 20);
        ManagedSprite_SetPaletteOverride(work->sprites[i], 0);
        ManagedSprite_SetDrawFlag(work->sprites[i], FALSE);
        ManagedSprite_SetPriority(work->sprites[i], 2);
    }
    ov112_021EA984(work);
    ManagedSprite_SetDrawPriority(work->sprites[8], 2);
    ManagedSprite_SetAffineOverwriteMode(work->sprites[8], 1);
    ManagedSprite_SetAnimateFlag(work->sprites[3], 1);
    ManagedSprite_SetAnimateFlag(work->sprites[4], 1);
}
