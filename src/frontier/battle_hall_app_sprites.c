#include "global.h"

#include "frontier/battle_hall_app.h"

#include "filesystem.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "oam_manager.h"
#include "obj_char_transfer.h"
#include "obj_pltt_transfer.h"
#include "pokemon_icon_idx.h"
#include "sprite_resource_header.h"
#include "sprite_transfer.h"

static Sprite *BattleHallSpriteGfx_CreateSprite(BattleHallSpriteGfx *gfx, int resourceId, int animSeq, int drawPriority, u8 screen);
static void BattleHallSpriteGfx_InitTransfers(void);

static const u8 sBattleHallSpriteResObjCounts[] = { 2, 2, 2, 2 };

static const ObjCharTransferTemplate sBattleHallObjCharTransferTemplate = {
    .maxTasks = 3,
    .sizeMain = 0x800,
    .sizeSub = 0x800,
    .heapID = HEAP_ID_105,
};

void BattleHallSpriteGfx_Init(BattleHallSpriteGfx *gfx, Pokemon *mon) {
    NARC *narc;
    int i;

    BattleHallSpriteGfx_InitTransfers();
    NNS_G2dInitOamManagerModule();
    OamManager_Create(0, 128, 0, 32, 0, 128, 0, 32, HEAP_ID_105);
    gfx->spriteList = G2dRenderer_Init(2, &gfx->renderer, HEAP_ID_105);
    for (i = 0; i < 4; i++) {
        gfx->resMan[i] = Create2DGfxResObjMan(sBattleHallSpriteResObjCounts[i], (GfGfxResType)i, HEAP_ID_105);
    }
    gfx->resObj[0][GF_GFX_RES_TYPE_CHAR] = AddCharResObjFromNarc(gfx->resMan[GF_GFX_RES_TYPE_CHAR], NARC_a_1_8_4, 0xC, TRUE, 0, NNS_G2D_VRAM_TYPE_2DMAIN, HEAP_ID_105);
    gfx->resObj[0][GF_GFX_RES_TYPE_PLTT] = AddPlttResObjFromNarc(gfx->resMan[GF_GFX_RES_TYPE_PLTT], NARC_a_1_8_4, 0x36, FALSE, 0, NNS_G2D_VRAM_TYPE_2DMAIN, 1, HEAP_ID_105);
    gfx->resObj[0][GF_GFX_RES_TYPE_CELL] = AddCellOrAnimResObjFromNarc(gfx->resMan[GF_GFX_RES_TYPE_CELL], NARC_a_1_8_4, 0xE, TRUE, 0, GF_GFX_RES_TYPE_CELL, HEAP_ID_105);
    gfx->resObj[0][GF_GFX_RES_TYPE_ANIM] = AddCellOrAnimResObjFromNarc(gfx->resMan[GF_GFX_RES_TYPE_ANIM], NARC_a_1_8_4, 0xD, TRUE, 0, GF_GFX_RES_TYPE_ANIM, HEAP_ID_105);
    narc = NARC_New(NARC_poketool_icongra_poke_icon, HEAP_ID_105);
    gfx->resObj[1][GF_GFX_RES_TYPE_CHAR] = AddCharResObjFromOpenNarc(gfx->resMan[GF_GFX_RES_TYPE_CHAR], narc, Pokemon_GetIconNaix(mon), FALSE, 1, NNS_G2D_VRAM_TYPE_2DMAIN, HEAP_ID_105);
    gfx->resObj[1][GF_GFX_RES_TYPE_PLTT] = AddPlttResObjFromNarc(gfx->resMan[GF_GFX_RES_TYPE_PLTT], NARC_poketool_icongra_poke_icon, GetMonIconPlttNaix(), FALSE, 1, NNS_G2D_VRAM_TYPE_2DMAIN, 3, HEAP_ID_105);
    gfx->resObj[1][GF_GFX_RES_TYPE_CELL] = AddCellOrAnimResObjFromOpenNarc(gfx->resMan[GF_GFX_RES_TYPE_CELL], narc, GetMonIconCell32KNaix(), FALSE, 1, GF_GFX_RES_TYPE_CELL, HEAP_ID_105);
    gfx->resObj[1][GF_GFX_RES_TYPE_ANIM] = AddCellOrAnimResObjFromOpenNarc(gfx->resMan[GF_GFX_RES_TYPE_ANIM], narc, GetMonIconAnim32KNaix(), FALSE, 1, GF_GFX_RES_TYPE_ANIM, HEAP_ID_105);
    for (i = 0; i < 2; i++) {
        SpriteTransfer_CreateCharTransferTask(gfx->resObj[i][GF_GFX_RES_TYPE_CHAR]);
        SpriteTransfer_CreateExtPlttTransferTask(gfx->resObj[i][GF_GFX_RES_TYPE_PLTT]);
    }
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_OBJ, TRUE);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, TRUE);
    NARC_Delete(narc);
}

static Sprite *BattleHallSpriteGfx_CreateSprite(BattleHallSpriteGfx *gfx, int resourceId, int animSeq, int drawPriority, u8 screen) {
    SpriteResourcesHeader header;
    SpriteTemplate template;
    Sprite *sprite;

    CreateSpriteResourcesHeader(&header, resourceId, resourceId, resourceId, resourceId, -1, -1, 0, 1, gfx->resMan[GF_GFX_RES_TYPE_CHAR], gfx->resMan[GF_GFX_RES_TYPE_PLTT], gfx->resMan[GF_GFX_RES_TYPE_CELL], gfx->resMan[GF_GFX_RES_TYPE_ANIM], NULL, NULL);
    template.spriteList = gfx->spriteList;
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

void BattleHallSpriteGfx_Free(BattleHallSpriteGfx *gfx) {
    u8 i;

    for (i = 0; i < 2; i++) {
        SpriteTransfer_DeleteCharTransferTask(gfx->resObj[i][GF_GFX_RES_TYPE_CHAR]);
        SpriteTransfer_DeletePlttTransferTask(gfx->resObj[i][GF_GFX_RES_TYPE_PLTT]);
    }
    for (i = 0; i < 4; i++) {
        Destroy2DGfxResObjMan(gfx->resMan[i]);
    }
    SpriteList_Delete(gfx->spriteList);
    OamManager_Free();
    ObjCharTransfer_Destroy();
    ObjPlttTransfer_Destroy();
}

static void BattleHallSpriteGfx_InitTransfers(void) {
    ObjCharTransferTemplate template = sBattleHallObjCharTransferTemplate;
    ObjCharTransfer_InitEx(&template, GX_OBJVRAMMODE_CHAR_1D_32K, GX_OBJVRAMMODE_CHAR_1D_32K);
    ObjPlttTransfer_Init(4, HEAP_ID_105);
    ObjCharTransfer_ClearBuffers();
    ObjPlttTransfer_Reset();
}

BattleHallSprite *BattleHallSprite_Create(BattleHallSpriteGfx *gfx, u32 resourceId, u32 animSeq, s32 x, u16 y, const u8 *anim) {
    BattleHallSprite *sprite = Heap_Alloc(HEAP_ID_105, sizeof(BattleHallSprite));
    VecFx32 position;

    memset(sprite, 0, sizeof(BattleHallSprite));
    sprite->anim = anim;
    sprite->sprite = BattleHallSpriteGfx_CreateSprite(gfx, resourceId, animSeq, 0, 0);
    position.x = x << FX32_SHIFT;
    position.y = y << FX32_SHIFT;
    Sprite_SetMatrix(sprite->sprite, &position);
    return sprite;
}

u32 BattleHallSprite_Delete(BattleHallSprite *sprite) {
    Sprite_Delete(sprite->sprite);
    Heap_Free(sprite);
    return 0;
}

void BattleHallSprite_SetVisible(BattleHallSprite *sprite, BOOL visible) {
    Sprite_SetDrawFlag(sprite->sprite, visible);
}

void BattleHallSprite_SetPosition(BattleHallSprite *sprite, s32 x, s32 y) {
    VecFx32 position;

    if (sprite->anim != NULL) {
        Sprite_TryChangeAnimSeq(sprite->sprite, sprite->anim[0]);
    }
    position = *Sprite_GetMatrixPtr(sprite->sprite);
    position.x = x << FX32_SHIFT;
    position.y = y << FX32_SHIFT;
    Sprite_SetMatrix(sprite->sprite, &position);
}

void BattleHallSprite_SetAnim(BattleHallSprite *sprite, u32 animSeq) {
    Sprite_SetAnimSpeed(sprite->sprite, FX32_ONE);
    Sprite_TryChangeAnimSeq(sprite->sprite, animSeq);
}

void BattleHallSprite_SetIconPalette(BattleHallSprite *sprite, Pokemon *mon) {
    Sprite_SetPalOffsetRespectVramOffset(sprite->sprite, Pokemon_GetIconPalette(mon));
}
