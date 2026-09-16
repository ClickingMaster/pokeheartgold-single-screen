#include "overlay02_port_private.h"

WIP_LOCAL void *ov02_0224A468(void *mgr, VecFx32 *pos, int drawPriority, int d) {
    VecFx32 affineMatrix = { 0, 0, 0 };
    VecFx32 affineScale;
    Sprite *sprite;

    affineScale = ov02_02253390;
    sprite = ov02_0224A33C(mgr, pos, 0, 0, 0, -1, 0, drawPriority);
    Sprite_SetAffineOverwriteMode(sprite, 2);
    Sprite_SetAffineMatrix(sprite, &affineMatrix);
    Sprite_SetAffineScale(sprite, &affineScale);
    Sprite_SetAffineZRotation(sprite, GF_DegreeToSinCosIdx(0));
    return sprite;
}

WIP_LOCAL void ov02_0224A4D0(void *work) {
    NARC *narc = ov02_0224A074();
    int i;
    for (i = 0; i < 4; i++) {
        if (((SpriteResource **)work)[0x6b + i] == NULL) {
            *(SpriteResource **)((u8 *)work + i * 4 + 0x1ac) = AddCharResObjFromOpenNarc(*(GF_2DGfxResMan **)((u8 *)work + 0x19c), narc, 0xb, FALSE, 0, 1, HEAP_ID_FIELD1);
            break;
        }
    }
    if (i >= 4) {
        GF_AssertFail();
    }
    for (i = 0; i < 4; i++) {
        if (((SpriteResource **)work)[0x72 + i] == NULL) {
            *(SpriteResource **)((u8 *)work + i * 4 + 0x1c8) = AddCellOrAnimResObjFromOpenNarc(*(GF_2DGfxResMan **)((u8 *)work + 0x1a4), narc, 0xc, FALSE, 0, GF_GFX_RES_TYPE_CELL, HEAP_ID_FIELD1);
            break;
        }
    }
    if (i >= 4) {
        GF_AssertFail();
    }
    NARC_Delete(narc);
}

WIP_LOCAL void ov02_0224A570(NARC *narc, u32 fileId, NNSG2dPaletteData **a2) {
    void *data = NARC_AllocAndReadWholeMember(narc, fileId, HEAP_ID_FIELD1);
    NNS_G2dGetUnpackedPaletteData(data, a2);
    BG_LoadPlttData(3, *(const void **)((u8 *)(*a2) + 0xc), 0x20, 0x180);
    Heap_Free(data);
}

WIP_LOCAL void ov02_0224A598(BgConfig *bgConfig, NARC *narc, u32 fileId, NNSG2dCharacterData **a3) {
    void *data = NARC_AllocAndReadWholeMember(narc, fileId, HEAP_ID_FIELD1);
    NNS_G2dGetUnpackedCharacterData(data, a3);
    BG_LoadCharTilesData(bgConfig, 3, *(const void **)((u8 *)(*a3) + 0x14), *(u32 *)((u8 *)(*a3) + 0x10), 0);
    Heap_Free(data);
}

WIP_LOCAL void ov02_0224A5D0(BgConfig *bgConfig, NARC *narc, u32 fileId, NNSG2dScreenData **a3) {
    void *data;
    BgSetPosTextAndCommit(bgConfig, 3, BG_POS_OP_SET_X, 0);
    BgSetPosTextAndCommit(bgConfig, 3, BG_POS_OP_SET_Y, 0);
    data = NARC_AllocAndReadWholeMember(narc, fileId, HEAP_ID_FIELD1);
    NNS_G2dGetUnpackedScreenData(data, a3);
    BG_LoadScreenTilemapData(bgConfig, 3, (u8 *)(*a3) + 0xc, *(u32 *)((u8 *)(*a3) + 8));
    BgTilemapRectChangePalette(bgConfig, 3, 0, 0, 0x20, 0x20, 0xc);
    BgCommitTilemapBufferToVram(bgConfig, 3);
    Heap_Free(data);
}
