#include "global.h"

#include "sprite_transfer.h"
#include "unk_02009D48.h"
#include "unk_0200A090.h"
#include "unk_0200B150.h"

typedef struct Ov37WorkR2 {
    u8 pad00[0x34];
    SpriteList *spriteList;
    G2dRenderer renderer;
    u8 padAfterRenderer[0x160 - 0x38 - sizeof(G2dRenderer)];
    GF_2DGfxResMan *resMan[4];
    SpriteResource *resources[8];
} Ov37WorkR2;

void ov37_021E60C0(Ov37WorkR2 *work, NARC *narc);

void ov37_021E60C0(Ov37WorkR2 *work, NARC *narc) {
    int i;
    int zero = 0;

    NNS_G2dInitOamManagerModule();
    OamManager_Create(0, 0x7e, 0, 0x20, 0, 0x7e, 0, 0x20, HEAP_ID_39);
    work->spriteList = G2dRenderer_Init(0x35, &work->renderer, HEAP_ID_39);
    G2dRenderer_SetSubSurfaceCoords(&work->renderer, 0, 1 << 20);
    for (i = 0; i < 4; i++) {
        work->resMan[i] = Create2DGfxResObjMan(2, (GfGfxResType)i, HEAP_ID_39);
    }

    work->resources[0] = AddCharResObjFromOpenNarc(work->resMan[0], narc, 6, TRUE, 0, 1, HEAP_ID_39);
    work->resources[1] = AddPlttResObjFromOpenNarc(work->resMan[1], narc, zero, zero, zero, 1, 7, HEAP_ID_39);
    work->resources[2] = AddCellOrAnimResObjFromOpenNarc(work->resMan[2], narc, 7, TRUE, 0, GF_GFX_RES_TYPE_CELL, HEAP_ID_39);
    work->resources[3] = AddCellOrAnimResObjFromOpenNarc(work->resMan[3], narc, 8, TRUE, 0, GF_GFX_RES_TYPE_ANIM, HEAP_ID_39);
    work->resources[4] = AddCharResObjFromOpenNarc(work->resMan[0], narc, 6, TRUE, 1, 2, HEAP_ID_39);
    work->resources[5] = AddPlttResObjFromOpenNarc(work->resMan[1], narc, zero, zero, 1, 2, 3, HEAP_ID_39);
    work->resources[6] = AddCellOrAnimResObjFromOpenNarc(work->resMan[2], narc, 7, TRUE, 1, GF_GFX_RES_TYPE_CELL, HEAP_ID_39);
    work->resources[7] = AddCellOrAnimResObjFromOpenNarc(work->resMan[3], narc, 8, TRUE, 1, GF_GFX_RES_TYPE_ANIM, HEAP_ID_39);
    SpriteTransfer_CreateCharTransferTask(work->resources[0]);
    SpriteTransfer_CreateCharTransferTask(work->resources[4]);
    SpriteTransfer_CreateExtPlttTransferTask(work->resources[1]);
    SpriteTransfer_CreateExtPlttTransferTask(work->resources[5]);
}
