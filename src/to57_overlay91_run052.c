#include "to57_overlay91_run052_private.h"

void ov91_0225FA9C(u8 *work, enum HeapID heapId) {
    int i;
    u8 *walker;
    ObjCharTransferTemplate template;

    NNS_G2dInitOamManagerModule();
    OamManager_Create(0, 0x7e, 0, 0x1f, 0, 0x7e, 0, 0x1f, heapId);
    template = ov91_02261C64;
    template.heapID = heapId;
    ObjCharTransfer_InitEx(&template, (GXOBJVRamModeChar)0x00200010, (GXOBJVRamModeChar)0x00200010);
    ObjPlttTransfer_Init(0x10, heapId);
    ObjCharTransfer_ClearBuffers();
    ObjPlttTransfer_Reset();
    G2dRenderer_SetObjCharTransferReservedRegion(NNS_G2D_VRAM_TYPE_2DMAIN, (GXOBJVRamModeChar)0x00200010);
    G2dRenderer_SetPlttTransferReservedRegion(NNS_G2D_VRAM_TYPE_2DMAIN);
    PTR_AT(work, 0x1c) = G2dRenderer_Init(0x80, (G2dRenderer *)(work + 0x20), heapId);
    G2dRenderer_SetSubSurfaceCoords((G2dRenderer *)(work + 0x20), 0, 2 << 20);
    i = 0;
    walker = work;
    while (i < 4) {
        PTR_AT(walker, 0x148) = Create2DGfxResObjMan(0x10, (GfGfxResType)i, heapId);
        i++;
        walker += 4;
    }
    PTR_AT(work, 0x18c) = FontSystem_NewInit(8, heapId);
}

void ov91_0225FB48(u8 *work) {
    int i;
    u8 *walker;
    sub_020135AC((UnkStruct_02013534 *)PTR_AT(work, 0x18c));
    SpriteList_Delete((SpriteList *)PTR_AT(work, 0x1c));
    walker = work;
    for (i = 0; i < 4; i++) {
        Destroy2DGfxResObjMan((GF_2DGfxResMan *)PTR_AT(walker, 0x148));
        walker += 4;
    }
    ObjCharTransfer_Destroy();
    ObjPlttTransfer_Destroy();
    OamManager_Free();
}

void ov91_0225FB80(u8 *work, NARC *narc, enum HeapID heapId) {
    PTR_AT(work, 0x17c) = AddCharResObjFromOpenNarc((GF_2DGfxResMan *)PTR_AT(work, 0x148), narc, 2, FALSE, 0xc8, 1, heapId);
    PTR_AT(work, 0x180) = AddPlttResObjFromOpenNarc((GF_2DGfxResMan *)PTR_AT(work, 0x14c), narc, 3, FALSE, 0xc8, 1, 8, heapId);
    PTR_AT(work, 0x184) = AddCellOrAnimResObjFromOpenNarc((GF_2DGfxResMan *)PTR_AT(work, 0x150), narc, 1, FALSE, 0xc8, GF_GFX_RES_TYPE_CELL, heapId);
    PTR_AT(work, 0x188) = AddCellOrAnimResObjFromOpenNarc((GF_2DGfxResMan *)PTR_AT(work, 0x154), narc, 0, FALSE, 0xc8, GF_GFX_RES_TYPE_ANIM, heapId);
    if (!SpriteTransfer_CreateCharTransferTask_UpdateMappingTypeFromHW_AllocAtEnd(PTR_AT(work, 0x17c))) {
        GF_AssertFail();
    }
    if (!SpriteTransfer_CreatePlttTransferTask(PTR_AT(work, 0x180))) {
        GF_AssertFail();
    }
    sub_0200A740(PTR_AT(work, 0x17c));
    sub_0200A740(PTR_AT(work, 0x180));
    CreateSpriteResourcesHeader((SpriteResourcesHeader *)(work + 0x158), 0xc8, 0xc8, 0xc8, 0xc8, -1, -1, 0, 0, PTR_AT(work, 0x148), PTR_AT(work, 0x14c), PTR_AT(work, 0x150), PTR_AT(work, 0x154), NULL, NULL);
}

void ov91_0225FC84(u8 *work) {
    SpriteTransfer_DeleteCharTransferTask(PTR_AT(work, 0x17c));
    SpriteTransfer_DeletePlttTransferTask(PTR_AT(work, 0x180));
    DestroySingle2DGfxResObj((GF_2DGfxResMan *)PTR_AT(work, 0x148), (SpriteResource *)PTR_AT(work, 0x17c));
    DestroySingle2DGfxResObj((GF_2DGfxResMan *)PTR_AT(work, 0x14c), (SpriteResource *)PTR_AT(work, 0x180));
    DestroySingle2DGfxResObj((GF_2DGfxResMan *)PTR_AT(work, 0x150), (SpriteResource *)PTR_AT(work, 0x184));
    DestroySingle2DGfxResObj((GF_2DGfxResMan *)PTR_AT(work, 0x154), (SpriteResource *)PTR_AT(work, 0x188));
}
