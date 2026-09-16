#include "overlay02_port_private.h"

WIP_LOCAL void ov02_0224A080(void *work, NARC *narc) {
    int i;

    ov02_0224A7A8(work, (PokepicTemplate *)((u8 *)work + 0x1f8));
    *(SpriteList **)((u8 *)work + 0x70) = G2dRenderer_Init(0x20, (G2dRenderer *)((u8 *)work + 0x74), HEAP_ID_FIELD1);
    G2dRenderer_SetSubSurfaceCoords((G2dRenderer *)((u8 *)work + 0x74), 0, 0x200000);

    *(GF_2DGfxResMan **)((u8 *)work + 0x19c) = Create2DGfxResObjMan(4, GF_GFX_RES_TYPE_CHAR, HEAP_ID_FIELD1);
    *(GF_2DGfxResMan **)((u8 *)work + 0x1a0) = Create2DGfxResObjMan(3, GF_GFX_RES_TYPE_PLTT, HEAP_ID_FIELD1);
    *(GF_2DGfxResMan **)((u8 *)work + 0x1a4) = Create2DGfxResObjMan(4, GF_GFX_RES_TYPE_CELL, HEAP_ID_FIELD1);
    *(GF_2DGfxResMan **)((u8 *)work + 0x1a8) = Create2DGfxResObjMan(2, GF_GFX_RES_TYPE_ANIM, HEAP_ID_FIELD1);

    *(SpriteResource **)((u8 *)work + 0x1ac) = AddCharResObjFromOpenNarc(*(GF_2DGfxResMan **)((u8 *)work + 0x19c), narc, 0xe, FALSE, 1, 1, HEAP_ID_FIELD1);
    *(SpriteResource **)((u8 *)work + 0x1b0) = AddCharResObjFromOpenNarc(*(GF_2DGfxResMan **)((u8 *)work + 0x19c), narc, ov02_02253304[*(u16 *)((u8 *)work + 0xe)], FALSE, 2, 1, HEAP_ID_FIELD1);
    *(SpriteResource **)((u8 *)work + 0x1b4) = ov02_0224A810(work, narc);

    i = 0;
    *(SpriteResource **)((u8 *)work + i * 4 + 0x1bc) = AddPlttResObjFromOpenNarc(*(GF_2DGfxResMan **)((u8 *)work + 0x1a0), narc, 6, FALSE, 0, 1, 1, HEAP_ID_FIELD1);
    i++;
    if (*(u16 *)((u8 *)work + 0xe) != 0) {
        *(SpriteResource **)((u8 *)work + i * 4 + 0x1bc) = AddPlttResObjFromOpenNarc(*(GF_2DGfxResMan **)((u8 *)work + 0x1a0), narc, ov02_022532FC[*(u16 *)((u8 *)work + 0xe) - 1], FALSE, 1, 1, 1, HEAP_ID_FIELD1);
        i++;
    }
    *(SpriteResource **)((u8 *)work + i * 4 + 0x1bc) = ov02_0224A868(work, narc);

    *(SpriteResource **)((u8 *)work + 0x1c8) = AddCellOrAnimResObjFromOpenNarc(*(GF_2DGfxResMan **)((u8 *)work + 0x1a4), narc, 0xf, FALSE, 1, GF_GFX_RES_TYPE_CELL, HEAP_ID_FIELD1);
    *(SpriteResource **)((u8 *)work + 0x1cc) = AddCellOrAnimResObjFromOpenNarc(*(GF_2DGfxResMan **)((u8 *)work + 0x1a4), narc, ov02_02253310[*(u16 *)((u8 *)work + 0xe)], FALSE, 2, GF_GFX_RES_TYPE_CELL, HEAP_ID_FIELD1);
    *(SpriteResource **)((u8 *)work + 0x1d0) = AddCellOrAnimResObjFromOpenNarc(*(GF_2DGfxResMan **)((u8 *)work + 0x1a4), narc, 0xa, FALSE, 3, GF_GFX_RES_TYPE_CELL, HEAP_ID_FIELD1);
    *(SpriteResource **)((u8 *)work + 0x1d8) = AddCellOrAnimResObjFromOpenNarc(*(GF_2DGfxResMan **)((u8 *)work + 0x1a8), narc, 0x10, FALSE, 0, GF_GFX_RES_TYPE_ANIM, HEAP_ID_FIELD1);
    *(SpriteResource **)((u8 *)work + 0x1dc) = AddCellOrAnimResObjFromOpenNarc(*(GF_2DGfxResMan **)((u8 *)work + 0x1a8), narc, ov02_0225330A[*(u16 *)((u8 *)work + 0xe)], FALSE, 1, GF_GFX_RES_TYPE_ANIM, HEAP_ID_FIELD1);

    *(void **)((u8 *)work + 0x218) = ov02_0224A7B8(*(Pokemon **)((u8 *)work + 0x5c), (u8 *)work + 0x1f8, HEAP_ID_FIELD1);
    *(void **)((u8 *)work + 0x21c) = ov02_0224A800((u16 *)((u8 *)work + 0x1f8), HEAP_ID_FIELD1);
}

WIP_LOCAL void ov02_0224A288(void *work) {
    int i;
    for (i = 0; i < 4; i++) {
        if (((SpriteResource **)work)[0x6b + i] != NULL) {
            SpriteTransfer_DeleteCharTransferTask(((SpriteResource **)work)[0x6b + i]);
        }
    }
    for (i = 0; i < 3; i++) {
        if (((SpriteResource **)work)[0x6f + i] != NULL) {
            SpriteTransfer_DeletePlttTransferTask(((SpriteResource **)work)[0x6f + i]);
        }
    }
    for (i = 0; i < 4; i++) {
        if (((SpriteResource **)work)[0x72 + i] != NULL) {
            sub_0200A740(((SpriteResource **)work)[0x72 + i]);
        }
    }
    for (i = 0; i < 2; i++) {
        if (((SpriteResource **)work)[0x76 + i] != NULL) {
            sub_0200A740(((SpriteResource **)work)[0x76 + i]);
        }
    }
    Destroy2DGfxResObjMan(*(GF_2DGfxResMan **)((u8 *)work + 0x19c));
    Destroy2DGfxResObjMan(*(GF_2DGfxResMan **)((u8 *)work + 0x1a0));
    Destroy2DGfxResObjMan(*(GF_2DGfxResMan **)((u8 *)work + 0x1a4));
    Destroy2DGfxResObjMan(*(GF_2DGfxResMan **)((u8 *)work + 0x1a8));
    SpriteList_DeleteAllSprites(*(SpriteList **)((u8 *)work + 0x70));
    SpriteList_Delete(*(SpriteList **)((u8 *)work + 0x70));
}
