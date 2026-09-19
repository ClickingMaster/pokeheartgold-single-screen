#include "overlay82_sprite_lifecycle_private.h"

void ov82_0223F95C(Ov82SpriteWorkW17 *work, Pokemon *mon) {
    NARC *narc;
    int i;

    ov82_0223FC14();
    NNS_G2dInitOamManagerModule();
    OamManager_Create(0, 0x80, 0, 0x20, 0, 0x80, 0, 0x20, HEAP_ID_105);
    work->spriteList = G2dRenderer_Init(2, (G2dRenderer *)&work->rendererAndPadding, HEAP_ID_105);

    for (i = 0; i < 4; i++) {
        work->resourceManagers[i] = Create2DGfxResObjMan(ov82_0223FEEC[i], (GfGfxResType)i, HEAP_ID_105);
    }

    work->resourceObjects[0][0] = AddCharResObjFromNarc(work->resourceManagers[0], (NarcId)0xB8, 0xC, TRUE, 0, 1, HEAP_ID_105);
    work->resourceObjects[0][1] = AddPlttResObjFromNarc(work->resourceManagers[1], (NarcId)0xB8, 0x36, FALSE, 0, 1, 1, HEAP_ID_105);
    work->resourceObjects[0][2] = AddCellOrAnimResObjFromNarc(work->resourceManagers[2], (NarcId)0xB8, 0xE, TRUE, 0, GF_GFX_RES_TYPE_CELL, HEAP_ID_105);
    work->resourceObjects[0][3] = AddCellOrAnimResObjFromNarc(work->resourceManagers[3], (NarcId)0xB8, 0xD, TRUE, 0, GF_GFX_RES_TYPE_ANIM, HEAP_ID_105);

    narc = NARC_New((NarcId)0x14, HEAP_ID_105);
    work->resourceObjects[1][0] = AddCharResObjFromOpenNarc(work->resourceManagers[0], narc, Pokemon_GetIconNaix(mon), FALSE, 1, 1, HEAP_ID_105);
    work->resourceObjects[1][1] = AddPlttResObjFromNarc(work->resourceManagers[1], (NarcId)0x14, sub_02074490(), FALSE, 1, 1, 3, HEAP_ID_105);
    work->resourceObjects[1][2] = AddCellOrAnimResObjFromOpenNarc(work->resourceManagers[2], narc, sub_02074498(), FALSE, 1, GF_GFX_RES_TYPE_CELL, HEAP_ID_105);
    work->resourceObjects[1][3] = AddCellOrAnimResObjFromOpenNarc(work->resourceManagers[3], narc, sub_020744A4(), FALSE, 1, GF_GFX_RES_TYPE_ANIM, HEAP_ID_105);

    for (i = 0; i < 2; i++) {
        SpriteTransfer_CreateCharTransferTask(work->resourceObjects[i][0]);
        SpriteTransfer_CreateExtPlttTransferTask(work->resourceObjects[i][1]);
    }

    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_OBJ, TRUE);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, TRUE);
    NARC_Delete(narc);
}

Sprite *ov82_0223FB08(Ov82SpriteWorkW17 *work, int resourceId, int animSeq, int drawPriority, u8 screen) {
    SpriteResourcesHeader header;
    SpriteTemplate template;
    Sprite *sprite;

    CreateSpriteResourcesHeader(&header, resourceId, resourceId, resourceId, resourceId, -1, -1, 0, 1, work->resourceManagers[0], work->resourceManagers[1], work->resourceManagers[2], work->resourceManagers[3], NULL, NULL);

    template.spriteList = work->spriteList;
    template.header = &header;
    template.position.x = 0;
    template.position.y = 0;
    template.position.z = 0;
    template.scale.x = FX32_ONE;
    template.scale.y = FX32_ONE;
    template.scale.z = FX32_ONE;
    template.rotation = 0;
    template.drawPriority = drawPriority;
    if (screen == 0) {
        template.whichScreen = NNS_G2D_VRAM_TYPE_2DMAIN;
    } else {
        template.whichScreen = NNS_G2D_VRAM_TYPE_2DSUB;
    }
    template.heapID = HEAP_ID_105;
    if (screen == 1) {
        template.position.y += 3 << 18;
    }

    sprite = Sprite_CreateAffine(&template);
    Sprite_SetAnimActiveFlag(sprite, TRUE);
    Sprite_SetAnimSpeed(sprite, FX32_ONE);
    Sprite_SetAnimCtrlSeq(sprite, animSeq);
    return sprite;
}

void ov82_0223FBBC(Ov82SpriteWorkW17 *work) {
    u8 i;

    for (i = 0; i < 2; i++) {
        SpriteTransfer_DeleteCharTransferTask(work->resourceObjects[i][0]);
        SpriteTransfer_DeletePlttTransferTask(work->resourceObjects[i][1]);
    }
    for (i = 0; i < 4; i++) {
        Destroy2DGfxResObjMan(work->resourceManagers[i]);
    }
    SpriteList_Delete(work->spriteList);
    OamManager_Free();
    ObjCharTransfer_Destroy();
    ObjPlttTransfer_Destroy();
}
