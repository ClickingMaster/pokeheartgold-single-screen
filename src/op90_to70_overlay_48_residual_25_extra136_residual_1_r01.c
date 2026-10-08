#pragma force_active on
#include "global.h"
#include "sprite.h"
#include "sprite_transfer.h"
#include "unk_02009D48.h"
#include "unk_0200A090.h"
typedef struct Gfx48 {
    u8 unk000[4];
    SpriteList *spriteList;
    u8 unk008[0x128];
    GF_2DGfxResMan *resMan[4];
    u8 unk140[4];
    NARC *narc;
} Gfx48;
typedef struct Res48 {
    SpriteResource *charRes;
    SpriteResource *plttRes;
    SpriteResource *cellRes;
    SpriteResource *animRes;
} Res48;
typedef struct Work48 {
    u8 unk000[0x30];
    u16 unk30;
    u16 unk32;
    u8 unk34[8];
    Sprite *sprites[12];
    Res48 res[12];
    u8 unk12C[0];
} Work48;
void ov48_0225AD38(Work48 *work);
void ov48_0225A668(void *arg, int a1, int a2);
void ov48_0225AAAC(Work48 *work, Gfx48 *gfx, void *arg, enum HeapID heapId);
// @fn ov48_0225AAAC
void ov48_0225AAAC(Work48 *work, Gfx48 *gfx, void *arg, enum HeapID heapId) {
    SpriteResourcesHeader header;
    SimpleSpriteTemplate template = { 0 };
    int i;
    memset(work, 0, sizeof(Work48));
    work->unk30 = 2;
    work->unk32 = 0x80;
    template.spriteList = gfx->spriteList;
    template.header = &header;
    template.priority = 0x40;
    template.heapID = heapId;
    template.whichScreen = NNS_G2D_VRAM_TYPE_2DSUB;
    work->res[0].plttRes = AddPlttResObjFromOpenNarc(gfx->resMan[1], gfx->narc, 20, FALSE, 0, 2, 12, heapId);
    GF_ASSERT(SpriteTransfer_CreatePlttTransferTask(work->res[0].plttRes));
    sub_0200A740(work->res[0].plttRes);
    for (i = 0; i < 12; i++) {
        work->res[i].charRes = AddCharResObjFromOpenNarc(gfx->resMan[0], gfx->narc, 21 + i * 3, FALSE, i, 2, heapId);
        work->res[i].cellRes = AddCellOrAnimResObjFromOpenNarc(gfx->resMan[2], gfx->narc, 22 + i * 3, FALSE, i, GF_GFX_RES_TYPE_CELL, heapId);
        work->res[i].animRes = AddCellOrAnimResObjFromOpenNarc(gfx->resMan[3], gfx->narc, 23 + i * 3, FALSE, i, GF_GFX_RES_TYPE_ANIM, heapId);
        GF_ASSERT(SpriteTransfer_CreateCharTransferTask_AllocAtEnd(work->res[i].charRes));
        sub_0200A740(work->res[i].charRes);
        CreateSpriteResourcesHeader(&header, i, 0, i, i, -1, -1, 0, 1, gfx->resMan[0], gfx->resMan[1], gfx->resMan[2], gfx->resMan[3], NULL, NULL);
        work->sprites[i] = Sprite_Create(&template);
        Sprite_SetAnimActiveFlag(work->sprites[i], TRUE);
        Sprite_SetAnimSpeed(work->sprites[i], FX32_ONE);
    }
    ov48_0225AD38(work);
    ov48_0225A668(arg, 0, 0);
}
