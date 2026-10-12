#include "global.h"

#include "field/sprite_gfx_manager.h"

#include "filesystem.h"
#include "heap.h"
#include "sprite.h"
#include "sprite_transfer.h"
#include "unk_02009D48.h"
#include "unk_0200A090.h"

// Defined with overlay 2's Pokecenter healing animation.
void *FlyLanding_AllocZeroed(enum HeapID heapID, u32 size);

#pragma force_active on
#pragma opt_optimizecontrolflow off

void SpriteGfxManager_Init(SpriteGfxManager *manager, int numSprites, int numChar, int numPltt, int numCell, int numAnim, int charNone, int plttNone, int cellNone, int animNone) {
    int i;

    manager->charCount = numChar;
    manager->plttCount = numPltt;
    manager->cellCount = numCell;
    manager->animCount = numAnim;
    manager->charNone = charNone;
    manager->plttNone = plttNone;
    manager->cellNone = cellNone;
    manager->animNone = animNone;
    manager->spriteList = G2dRenderer_Init(numSprites, &manager->renderer, HEAP_ID_FIELD1);
    G2dRenderer_SetSubSurfaceCoords(&manager->renderer, 0, FX32_CONST(512));
    manager->charMan = Create2DGfxResObjMan(numChar, GF_GFX_RES_TYPE_CHAR, HEAP_ID_FIELD1);
    manager->plttMan = Create2DGfxResObjMan(numPltt, GF_GFX_RES_TYPE_PLTT, HEAP_ID_FIELD1);
    manager->cellMan = Create2DGfxResObjMan(numCell, GF_GFX_RES_TYPE_CELL, HEAP_ID_FIELD1);
    manager->animMan = Create2DGfxResObjMan(numAnim, GF_GFX_RES_TYPE_ANIM, HEAP_ID_FIELD1);
    manager->charRes = FlyLanding_AllocZeroed(HEAP_ID_FIELD1, numChar * sizeof(SpriteGfxResource));
    manager->plttRes = FlyLanding_AllocZeroed(HEAP_ID_FIELD1, numPltt * sizeof(SpriteGfxResource));
    manager->cellRes = FlyLanding_AllocZeroed(HEAP_ID_FIELD1, numCell * sizeof(SpriteGfxResource));
    manager->animRes = FlyLanding_AllocZeroed(HEAP_ID_FIELD1, numAnim * sizeof(SpriteGfxResource));
    i = 0;
    if (numChar > 0) {
        for (; i < numChar; i++) {
            manager->charRes[i].id = charNone;
        }
    }
    i = 0;
    if (numPltt > 0) {
        for (; i < numPltt; i++) {
            manager->plttRes[i].id = plttNone;
        }
    }
    i = 0;
    if (numCell > 0) {
        for (; i < numCell; i++) {
            manager->cellRes[i].id = cellNone;
        }
    }
    i = 0;
    if (numAnim > 0) {
        for (; i < numAnim; i++) {
            manager->animRes[i].id = animNone;
        }
    }
}

#pragma opt_optimizecontrolflow reset
#pragma force_active reset

void SpriteGfxManager_Free(SpriteGfxManager *manager) {
    int i;

    for (i = 0; i < manager->charCount; i++) {
        if (manager->charNone != manager->charRes[i].id) {
            SpriteTransfer_DeleteCharTransferTask(manager->charRes[i].res);
        }
    }
    for (i = 0; i < manager->plttCount; i++) {
        if (manager->plttNone != manager->plttRes[i].id) {
            SpriteTransfer_DeletePlttTransferTask(manager->plttRes[i].res);
        }
    }
    for (i = 0; i < manager->cellCount; i++) {
        if (manager->cellNone != manager->cellRes[i].id) {
            sub_0200A740(manager->cellRes[i].res);
        }
    }
    for (i = 0; i < manager->animCount; i++) {
        if (manager->animNone != manager->animRes[i].id) {
            sub_0200A740(manager->animRes[i].res);
        }
    }
    Destroy2DGfxResObjMan(manager->charMan);
    Destroy2DGfxResObjMan(manager->plttMan);
    Destroy2DGfxResObjMan(manager->cellMan);
    Destroy2DGfxResObjMan(manager->animMan);
    Heap_Free(manager->charRes);
    Heap_Free(manager->plttRes);
    Heap_Free(manager->cellRes);
    Heap_Free(manager->animRes);
    SpriteList_DeleteAllSprites(manager->spriteList);
    SpriteList_Delete(manager->spriteList);
}

void SpriteGfxManager_LoadChar(SpriteGfxManager *manager, NARC *narc, int fileId, int id) {
    int i;

    for (i = 0; i < manager->charCount; i++) {
        if (manager->charNone == manager->charRes[i].id) {
            manager->charRes[i].id = id;
            manager->charRes[i].unused = 0;
            manager->charRes[i].res = AddCharResObjFromOpenNarc(manager->charMan, narc, fileId, FALSE, id, NNS_G2D_VRAM_TYPE_2DMAIN, HEAP_ID_FIELD1);
            return;
        }
    }
    GF_ASSERT(FALSE);
}

void SpriteGfxManager_TransferChar(SpriteGfxManager *manager, int id) {
    int i;

    for (i = 0; i < manager->charCount; i++) {
        if (id == manager->charRes[i].id) {
            SpriteTransfer_CreateCharTransferTask_AllocAtEnd(manager->charRes[i].res);
            return;
        }
    }
    GF_ASSERT(FALSE);
}

// Frees the char data once it is in VRAM.
void SpriteGfxManager_ReleaseCharData(SpriteGfxManager *manager, int id) {
    int i;

    for (i = 0; i < manager->charCount; i++) {
        if (id == manager->charRes[i].id) {
            sub_0200A740(manager->charRes[i].res);
            return;
        }
    }
    GF_ASSERT(FALSE);
}

// The palette, cell and animation loaders search for a free slot up to the char count.
void SpriteGfxManager_LoadPltt(SpriteGfxManager *manager, NARC *narc, int fileId, int id) {
    int i;

    for (i = 0; i < manager->charCount; i++) {
        if (manager->plttNone == manager->plttRes[i].id) {
            manager->plttRes[i].id = id;
            manager->plttRes[i].unused = 0;
            manager->plttRes[i].res = AddPlttResObjFromOpenNarc(manager->plttMan, narc, fileId, FALSE, id, NNS_G2D_VRAM_TYPE_2DMAIN, 1, HEAP_ID_FIELD1);
            return;
        }
    }
    GF_ASSERT(FALSE);
}

void SpriteGfxManager_TransferPltt(SpriteGfxManager *manager, int id) {
    int i;

    for (i = 0; i < manager->plttCount; i++) {
        if (id == manager->plttRes[i].id) {
            SpriteTransfer_CreatePlttTransferTask(manager->plttRes[i].res);
            return;
        }
    }
    GF_ASSERT(FALSE);
}

void SpriteGfxManager_ReleasePlttData(SpriteGfxManager *manager, int id) {
    int i;

    for (i = 0; i < manager->plttCount; i++) {
        if (id == manager->plttRes[i].id) {
            sub_0200A740(manager->plttRes[i].res);
            return;
        }
    }
    GF_ASSERT(FALSE);
}

void SpriteGfxManager_LoadCell(SpriteGfxManager *manager, NARC *narc, int fileId, int id) {
    int i;

    for (i = 0; i < manager->charCount; i++) {
        if (manager->cellNone == manager->cellRes[i].id) {
            manager->cellRes[i].id = id;
            manager->cellRes[i].unused = 0;
            manager->cellRes[i].res = AddCellOrAnimResObjFromOpenNarc(manager->cellMan, narc, fileId, FALSE, id, GF_GFX_RES_TYPE_CELL, HEAP_ID_FIELD1);
            return;
        }
    }
    GF_ASSERT(FALSE);
}

void SpriteGfxManager_LoadAnim(SpriteGfxManager *manager, NARC *narc, int fileId, int id) {
    int i;

    for (i = 0; i < manager->charCount; i++) {
        if (manager->animNone == manager->animRes[i].id) {
            manager->animRes[i].id = id;
            manager->animRes[i].unused = 0;
            manager->animRes[i].res = AddCellOrAnimResObjFromOpenNarc(manager->animMan, narc, fileId, FALSE, id, GF_GFX_RES_TYPE_ANIM, HEAP_ID_FIELD1);
            return;
        }
    }
    GF_ASSERT(FALSE);
}

// animId may be the animation `none` id for a sprite without animation.
Sprite *SpriteGfxManager_CreateSprite(SpriteGfxManager *manager, const VecFx32 *pos, int charId, int plttId, int cellId, int animId, int priority, u32 drawPriority) {
    SpriteResourcesHeader header;
    SimpleSpriteTemplate template;
    Sprite *sprite;

    if (animId == manager->animNone) {
        animId = -1;
    }
    CreateSpriteResourcesHeader(&header, charId, plttId, cellId, animId, -1, -1, FALSE, priority, manager->charMan, manager->plttMan, manager->cellMan, manager->animMan, NULL, NULL);
    template.spriteList = manager->spriteList;
    template.header = &header;
    template.position = *pos;
    template.priority = drawPriority;
    template.whichScreen = NNS_G2D_VRAM_TYPE_2DMAIN;
    template.heapID = HEAP_ID_FIELD1;
    sprite = Sprite_Create(&template);
    GF_ASSERT(sprite != NULL);
    return sprite;
}

void Sprite_CopyPositionVector(Sprite *sprite, VecFx32 *pos) {
    *pos = *Sprite_GetMatrixPtr(sprite);
}
