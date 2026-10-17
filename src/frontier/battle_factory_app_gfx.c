#include "global.h"

#include "frontier/battle_factory_app_internal.h"
#include "frontier/battle_factory_setup.h"

#include "bg_cell_manager.h"
#include "bg_window.h"
#include "filesystem.h"
#include "font.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "msgdata.h"
#include "oam_manager.h"
#include "obj_char_transfer.h"
#include "obj_pltt_transfer.h"
#include "pm_string.h"
#include "pokemon.h"
#include "pokemon_icon_idx.h"
#include "pokepic.h"
#include "render_window.h"
#include "sprite.h"
#include "sprite_resource.h"
#include "sprite_resource_header.h"
#include "sprite_transfer.h"
#include "swap_3d_buffers.h"

static Sprite *BattleFactorySpriteGfx_CreateSprite(BattleFactorySpriteGfx *gfx, int resourceId, int animSeq, int drawPriority, int priority, u8 screen);
static void BattleFactoryApp_InitObjTransfers(void);
static void BattleFactoryBall_SetSelected(BattleFactoryBall *ball, u16 selected);
static void BattleFactoryApp_PrintStringAligned(Window *window, String *string, int x, int y, int fontId, u32 color, int align);

static const u8 sBattleFactorySpriteResManSizes[] = { 6, 6, 6, 6 };

static const ObjCharTransferTemplate sBattleFactoryObjCharTransferTemplate = {
    .maxTasks = 32,
    .sizeMain = 0x800,
    .sizeSub = 0x800,
    .heapID = HEAP_ID_100,
};

static const WindowTemplate sBattleFactoryAppWindowTemplates[] = {
    { GF_BG_LYR_MAIN_1, 1,  8,  8,  2, 13, 1    },
    { GF_BG_LYR_MAIN_1, 1,  1,  8,  2, 13, 0x11 },
    { GF_BG_LYR_MAIN_1, 1,  11, 10, 2, 13, 0x21 },
    { GF_BG_LYR_MAIN_1, 11, 11, 10, 2, 13, 0x35 },
    { GF_BG_LYR_MAIN_1, 21, 11, 10, 2, 13, 0x49 },
    { GF_BG_LYR_MAIN_1, 4,  4,  10, 2, 13, 0x6D },
    { GF_BG_LYR_MAIN_1, 18, 4,  10, 2, 13, 0x81 },
    { GF_BG_LYR_SUB_1,  2,  19, 27, 4, 12, 1    },
    { GF_BG_LYR_SUB_1,  12, 1,  10, 2, 13, 0x6D },
    { GF_BG_LYR_SUB_1,  2,  15, 7,  2, 9,  0x81 },
    { GF_BG_LYR_SUB_1,  23, 15, 7,  2, 9,  0x8F },
    { GF_BG_LYR_SUB_1,  2,  12, 12, 2, 9,  0x9D },
    { GF_BG_LYR_SUB_1,  18, 12, 12, 2, 9,  0xB5 },
    { GF_BG_LYR_SUB_1,  18, 12, 12, 2, 9,  0xCD },
    { GF_BG_LYR_SUB_1,  10, 15, 12, 2, 9,  0xE5 },
    { GF_BG_LYR_SUB_1,  18, 12, 12, 2, 9,  0xB5 },
    { GF_BG_LYR_SUB_1,  2,  15, 12, 2, 9,  0xCD },
    { GF_BG_LYR_SUB_1,  18, 15, 12, 2, 9,  0xE5 },
};

BOOL BattleFactory_IsLinkMode(int frontierType) {
    return BattleFactory_IsMultiType((u8)frontierType);
}

BOOL BattleFactoryApp_IsLinkMode(BattleFactoryApp *app) {
    return BattleFactory_IsMultiType(app->frontierType);
}

void BattleFactoryApp_InitBgCellManager(BattleFactoryApp *app) {
    app->bgCellManager = BgCellManager_New(app->bgConfig, 2, 1, HEAP_ID_100);
    BgCellManager_InitCell(app->bgCellManager, 0, 4, 18, 4);
    BgCellManager_LoadCellTilemapFromOpenNarc(app->bgCellManager, 0, app->narc, 144, 1);
}

void BattleFactoryApp_FreeBgCellManager(BattleFactoryApp *app) {
    BgCellManager_Delete(app->bgCellManager);
}

void BattleFactorySpriteGfx_Init(BattleFactorySpriteGfx *gfx) {
    int i;

    BattleFactoryApp_InitObjTransfers();
    NNS_G2dInitOamManagerModule();
    OamManager_Create(0, 128, 0, 32, 0, 128, 0, 32, HEAP_ID_100);
    gfx->spriteList = G2dRenderer_Init(32, &gfx->renderer, HEAP_ID_100);
    G2dRenderer_SetSubSurfaceCoords(&gfx->renderer, 0, 2 << 20);
    for (i = 0; i < 4; i++) {
        gfx->resMan[i] = Create2DGfxResObjMan(sBattleFactorySpriteResManSizes[i], (GfGfxResType)i, HEAP_ID_100);
    }
    gfx->resObj[1][0] = AddCharResObjFromNarc(gfx->resMan[0], NARC_a_1_8_4, 0x28, TRUE, 1, NNS_G2D_VRAM_TYPE_2DMAIN, HEAP_ID_100);
    gfx->resObj[1][1] = AddPlttResObjFromNarc(gfx->resMan[1], NARC_a_1_8_4, 0x3E, FALSE, 1, NNS_G2D_VRAM_TYPE_2DMAIN, 4, HEAP_ID_100);
    gfx->resObj[1][2] = AddCellOrAnimResObjFromNarc(gfx->resMan[2], NARC_a_1_8_4, 0x29, TRUE, 1, GF_GFX_RES_TYPE_CELL, HEAP_ID_100);
    gfx->resObj[1][3] = AddCellOrAnimResObjFromNarc(gfx->resMan[3], NARC_a_1_8_4, 0x2A, TRUE, 1, GF_GFX_RES_TYPE_ANIM, HEAP_ID_100);
    gfx->resObj[2][0] = AddCharResObjFromNarc(gfx->resMan[0], NARC_a_1_8_4, 0x2B, TRUE, 2, NNS_G2D_VRAM_TYPE_2DMAIN, HEAP_ID_100);
    gfx->resObj[2][1] = AddPlttResObjFromNarc(gfx->resMan[1], NARC_a_1_8_4, 0x3F, FALSE, 2, NNS_G2D_VRAM_TYPE_2DMAIN, 1, HEAP_ID_100);
    gfx->resObj[2][2] = AddCellOrAnimResObjFromNarc(gfx->resMan[2], NARC_a_1_8_4, 0x2C, TRUE, 2, GF_GFX_RES_TYPE_CELL, HEAP_ID_100);
    gfx->resObj[2][3] = AddCellOrAnimResObjFromNarc(gfx->resMan[3], NARC_a_1_8_4, 0x2D, TRUE, 2, GF_GFX_RES_TYPE_ANIM, HEAP_ID_100);
    gfx->resObj[0][0] = AddCharResObjFromNarc(gfx->resMan[0], NARC_a_1_8_4, 0, TRUE, 0, NNS_G2D_VRAM_TYPE_2DSUB, HEAP_ID_100);
    gfx->resObj[0][1] = AddPlttResObjFromNarc(gfx->resMan[1], NARC_a_1_8_4, 0x34, FALSE, 0, NNS_G2D_VRAM_TYPE_2DSUB, 8, HEAP_ID_100);
    gfx->resObj[0][2] = AddCellOrAnimResObjFromNarc(gfx->resMan[2], NARC_a_1_8_4, 2, TRUE, 0, GF_GFX_RES_TYPE_CELL, HEAP_ID_100);
    gfx->resObj[0][3] = AddCellOrAnimResObjFromNarc(gfx->resMan[3], NARC_a_1_8_4, 1, TRUE, 0, GF_GFX_RES_TYPE_ANIM, HEAP_ID_100);
    gfx->resObj[3][0] = AddCharResObjFromNarc(gfx->resMan[0], NARC_a_1_8_4, 0x2B, TRUE, 3, NNS_G2D_VRAM_TYPE_2DSUB, HEAP_ID_100);
    gfx->resObj[3][1] = AddPlttResObjFromNarc(gfx->resMan[1], NARC_a_1_8_4, 0x3F, FALSE, 3, NNS_G2D_VRAM_TYPE_2DSUB, 1, HEAP_ID_100);
    gfx->resObj[3][2] = AddCellOrAnimResObjFromNarc(gfx->resMan[2], NARC_a_1_8_4, 0x2C, TRUE, 3, GF_GFX_RES_TYPE_CELL, HEAP_ID_100);
    gfx->resObj[3][3] = AddCellOrAnimResObjFromNarc(gfx->resMan[3], NARC_a_1_8_4, 0x2D, TRUE, 3, GF_GFX_RES_TYPE_ANIM, HEAP_ID_100);
    for (i = 0; i < 2; i++) {
        gfx->resObj[i + 4][0] = AddCharResObjFromNarc(gfx->resMan[0], NARC_a_1_8_4, 0x2E, TRUE, i + 4, NNS_G2D_VRAM_TYPE_2DSUB, HEAP_ID_100);
        gfx->resObj[i + 4][1] = AddPlttResObjFromNarc(gfx->resMan[1], NARC_poketool_icongra_poke_icon, GetMonIconPlttNaix(), FALSE, i + 4, NNS_G2D_VRAM_TYPE_2DSUB, 3, HEAP_ID_100);
        gfx->resObj[i + 4][2] = AddCellOrAnimResObjFromNarc(gfx->resMan[2], NARC_a_1_8_4, 0x2F, TRUE, i + 4, GF_GFX_RES_TYPE_CELL, HEAP_ID_100);
        gfx->resObj[i + 4][3] = AddCellOrAnimResObjFromNarc(gfx->resMan[3], NARC_a_1_8_4, 0x30, TRUE, i + 4, GF_GFX_RES_TYPE_ANIM, HEAP_ID_100);
    }
    for (i = 0; i < 6; i++) {
        SpriteTransfer_CreateCharTransferTask(gfx->resObj[i][0]);
        SpriteTransfer_CreateExtPlttTransferTask(gfx->resObj[i][1]);
    }
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
}

static Sprite *BattleFactorySpriteGfx_CreateSprite(BattleFactorySpriteGfx *gfx, int resourceId, int animSeq, int drawPriority, int priority, u8 screen) {
    SpriteResourcesHeader header;
    SpriteTemplate template;
    Sprite *sprite;

    CreateSpriteResourcesHeader(&header, resourceId, resourceId, resourceId, resourceId, -1, -1, 0, priority, gfx->resMan[0], gfx->resMan[1], gfx->resMan[2], gfx->resMan[3], NULL, NULL);
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
    template.heapID = HEAP_ID_100;
    if (screen == 1) {
        template.position.y += 2 << 20;
    }
    sprite = Sprite_CreateAffine(&template);
    Sprite_SetAnimActiveFlag(sprite, TRUE);
    Sprite_SetAnimCtrlSeq(sprite, animSeq);
    return sprite;
}

void BattleFactorySpriteGfx_Free(BattleFactorySpriteGfx *gfx) {
    u8 i;

    for (i = 0; i < 6; i++) {
        SpriteTransfer_DeleteCharTransferTask(gfx->resObj[i][0]);
        SpriteTransfer_DeletePlttTransferTask(gfx->resObj[i][1]);
    }
    for (i = 0; i < 4; i++) {
        Destroy2DGfxResObjMan(gfx->resMan[i]);
    }
    SpriteList_Delete(gfx->spriteList);
    OamManager_Free();
    ObjCharTransfer_Destroy();
    ObjPlttTransfer_Destroy();
}

static void BattleFactoryApp_InitObjTransfers(void) {
    ObjCharTransferTemplate template = sBattleFactoryObjCharTransferTemplate;
    ObjCharTransfer_InitEx(&template, GX_OBJVRAMMODE_CHAR_1D_64K, GX_OBJVRAMMODE_CHAR_1D_64K);
    ObjPlttTransfer_Init(8, HEAP_ID_100);
    ObjCharTransfer_ClearBuffers();
    ObjPlttTransfer_Reset();
}

void BattleFactoryApp_Setup3D(void) {
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG0, GF_PLANE_TOGGLE_ON);
    G2_SetBG0Priority(0);
    G3X_SetShading(GX_SHADING_TOON);
    G3X_AntiAlias(TRUE);
    G3X_AlphaTest(FALSE, 0);
    G3X_AlphaBlend(TRUE);
    G3X_EdgeMarking(FALSE);
    G3X_SetFog(FALSE, GX_FOGBLEND_COLOR_ALPHA, GX_FOGSLOPE_0x8000, 0);
    G3X_SetClearColor(0, 0, 0x7FFF, 0x3F, FALSE);
    G3_ViewPort(0, 0, 255, 191);
}

void BattleFactoryApp_Draw3D(PokepicManager *pokepicManager) {
    Thunk_G3X_Reset();
    G3_MtxMode(GX_MTXMODE_PROJECTION);
    G3_Identity();
    G3_MtxMode(GX_MTXMODE_POSITION_VECTOR);
    G3_Identity();
    NNS_G3dGlbFlushP();
    NNS_G2dSetupSoftwareSpriteCamera();
    PokepicManager_DrawAll(pokepicManager);
    G3_SwapBuffers(GX_SORTMODE_MANUAL, GX_BUFFERMODE_Z);
}

Pokepic *BattleFactory_CreatePokepic(PokepicManager *pokepicManager, int polygonId, Pokemon *mon, int x, int y, int z) {
    PokepicTemplate template;

    GetPokemonSpriteCharAndPlttNarcIds(&template, mon, 2);
    return PokepicManager_CreatePokepic(pokepicManager, &template, x, y, z, polygonId, NULL, NULL);
}

void BattleFactory_SetPokepicHFlip(Pokepic *pokepic, int hFlip) {
    Pokepic_SetAttr(pokepic, POKEPIC_HFLIP, hFlip);
}

BattleFactorySprite *BattleFactorySprite_Create(BattleFactorySpriteGfx *gfx, int resourceId, int animSeq, int priority, int drawPriority, int x, int y) {
    BattleFactorySprite *sprite = Heap_Alloc(HEAP_ID_100, sizeof(BattleFactorySprite));

    ((u8 *)sprite)[0] = 0;
    ((u8 *)sprite)[1] = 0;
    ((u8 *)sprite)[2] = 0;
    ((u8 *)sprite)[3] = 0;
    ((u8 *)sprite)[4] = 0;
    ((u8 *)sprite)[5] = 0;
    ((u8 *)sprite)[6] = 0;
    ((u8 *)sprite)[7] = 0;
    ((u8 *)sprite)[8] = 0;
    ((u8 *)sprite)[9] = 0;
    ((u8 *)sprite)[10] = 0;
    ((u8 *)sprite)[11] = 0;
    sprite->x = x;
    sprite->y = y;
    sprite->sprite = BattleFactorySpriteGfx_CreateSprite(gfx, resourceId, animSeq, drawPriority, priority, 0);
    BattleFactorySprite_SetPosition(sprite, x, y);
    return sprite;
}

BattleFactorySprite *BattleFactorySprite_CreateSub(BattleFactorySpriteGfx *gfx, int resourceId, int animSeq, int priority, int drawPriority, int x, int y) {
    BattleFactorySprite *sprite = Heap_Alloc(HEAP_ID_100, sizeof(BattleFactorySprite));

    ((u8 *)sprite)[0] = 0;
    ((u8 *)sprite)[1] = 0;
    ((u8 *)sprite)[2] = 0;
    ((u8 *)sprite)[3] = 0;
    ((u8 *)sprite)[4] = 0;
    ((u8 *)sprite)[5] = 0;
    ((u8 *)sprite)[6] = 0;
    ((u8 *)sprite)[7] = 0;
    ((u8 *)sprite)[8] = 0;
    ((u8 *)sprite)[9] = 0;
    ((u8 *)sprite)[10] = 0;
    ((u8 *)sprite)[11] = 0;
    sprite->x = x;
    sprite->y = y;
    sprite->sprite = BattleFactorySpriteGfx_CreateSprite(gfx, resourceId, animSeq, drawPriority, priority, 1);
    BattleFactorySprite_SetPositionSub(sprite, x, y);
    return sprite;
}

BattleFactorySprite *BattleFactorySprite_Delete(BattleFactorySprite *sprite) {
    Sprite_Delete(sprite->sprite);
    Heap_Free(sprite);
    return NULL;
}

void BattleFactorySprite_SetVisible(BattleFactorySprite *sprite, int visible) {
    Sprite_SetDrawFlag(sprite->sprite, visible);
}

void BattleFactorySprite_SetPosition(BattleFactorySprite *sprite, int x, int y) {
    VecFx32 pos;

    pos.x = x * FX32_ONE;
    pos.y = y * FX32_ONE;
    Sprite_SetMatrix(sprite->sprite, &pos);
}

void BattleFactorySprite_SetPositionSub(BattleFactorySprite *sprite, int x, int y) {
    VecFx32 pos;

    pos.x = x * FX32_ONE;
    pos.y = y * FX32_ONE + (2 << 20);
    Sprite_SetMatrix(sprite->sprite, &pos);
}

void BattleFactorySprite_SetMatrix(BattleFactorySprite *sprite, VecFx32 *pos) {
    Sprite_SetMatrix(sprite->sprite, pos);
}

BOOL BattleFactorySprite_IsAnimated(BattleFactorySprite *sprite) {
    return Sprite_IsAnimated(sprite->sprite);
}

void BattleFactorySprite_SetAnim(BattleFactorySprite *sprite, int animSeq) {
    Sprite_SetAnimationFrame(sprite->sprite, 0);
    Sprite_SetAnimCtrlSeq(sprite->sprite, animSeq);
}

void BattleFactorySprite_SetPaletteOverride(BattleFactorySprite *sprite, int palette) {
    Sprite_SetPaletteOverride(sprite->sprite, palette);
}

void BattleFactorySprite_SetPalette(BattleFactorySprite *sprite, int palette) {
    Sprite_SetPalIndexRespectVramOffset(sprite->sprite, palette);
}

void BattleFactorySprite_LoadCharData(BattleFactorySprite *sprite, void *data, u32 size, int screen) {
    u32 addr = NNS_G2dGetImageLocation(Sprite_GetImageProxy(sprite->sprite), (NNS_G2D_VRAM_TYPE)screen);

    DC_FlushRange(data, size);
    if (screen == NNS_G2D_VRAM_TYPE_2DMAIN) {
        GX_LoadOBJ(data, addr, size);
    } else {
        GXS_LoadOBJ(data, addr, size);
    }
}

BattleFactoryBall *BattleFactoryBall_Create(BattleFactorySpriteGfx *gfx, u32 x, u32 y, enum HeapID heapID) {
    BattleFactoryBall *ball = Heap_Alloc(heapID, sizeof(BattleFactoryBall));

    memset(ball, 0, sizeof(BattleFactoryBall));
    ball->selected = FALSE;
    ball->x = x;
    ball->y = y;
    ball->sprite = BattleFactorySpriteGfx_CreateSprite(gfx, 0, 0, 10, 0, 1);
    BattleFactoryBall_SetPosition(ball, x, y);
    Sprite_SetPaletteOverride(ball->sprite, 1);
    return ball;
}

BattleFactoryBall *BattleFactoryBall_Delete(BattleFactoryBall *ball) {
    Sprite_Delete(ball->sprite);
    Heap_Free(ball);
    return NULL;
}

void BattleFactoryBall_SetVisible(BattleFactoryBall *ball, int visible) {
    Sprite_SetDrawFlag(ball->sprite, visible);
}

void BattleFactoryBall_Move(VecFx32 *out, BattleFactoryBall *ball, int dx, int dy) {
    VecFx32 pos = *Sprite_GetMatrixPtr(ball->sprite);

    pos.x += dx * FX32_ONE;
    pos.y += dy * FX32_ONE;
    Sprite_SetMatrix(ball->sprite, &pos);
    *out = *Sprite_GetMatrixPtr(ball->sprite);
}

void BattleFactoryBall_SetPosition(BattleFactoryBall *ball, int x, int y) {
    VecFx32 pos;

    pos.x = x * FX32_ONE;
    pos.y = y * FX32_ONE + (2 << 20);
    Sprite_SetMatrix(ball->sprite, &pos);
}

VecFx32 *BattleFactoryBall_GetPosition(BattleFactoryBall *ball) {
    return (VecFx32 *)Sprite_GetMatrixPtr(ball->sprite);
}

static void BattleFactoryBall_SetSelected(BattleFactoryBall *ball, u16 selected) {
    ball->selected = selected;
}

u8 BattleFactoryBall_IsSelected(BattleFactoryBall *ball) {
    return ball->selected;
}

void BattleFactoryBall_Select(BattleFactoryBall *ball) {
    BattleFactoryBall_SetSelected(ball, TRUE);
}

void BattleFactoryBall_Deselect(BattleFactoryBall *ball) {
    BattleFactoryBall_SetSelected(ball, FALSE);
}

void BattleFactoryBall_MoveOffscreen(BattleFactoryBall *ball) {
    VecFx32 pos;
    VecFx32 *cur = BattleFactoryBall_GetPosition(ball);

    pos.x = (cur->x / FX32_ONE + 248) * FX32_ONE;
    pos.y = cur->y;
    Sprite_SetMatrix(ball->sprite, &pos);
}

int BattleFactoryBall_GetX(BattleFactoryBall *ball) {
    return ball->x;
}

int BattleFactoryBall_GetY(BattleFactoryBall *ball) {
    return ball->y;
}

void BattleFactoryBall_SetAnim(BattleFactoryBall *ball, int animSeq) {
    Sprite_SetAnimSpeed(ball->sprite, FX32_ONE);
    Sprite_SetAnimCtrlSeq(ball->sprite, animSeq);
}

void BattleFactoryBall_SetPaletteOverride(BattleFactoryBall *ball, int palette) {
    Sprite_SetPaletteOverride(ball->sprite, palette);
}

void BattleFactoryBall_SetOamMode(BattleFactoryBall *ball, GXOamMode mode) {
    Sprite_SetOamMode(ball->sprite, mode);
}

BattleFactoryBall *BattleFactoryBall_Free(BattleFactoryBall *ball) {
    Sprite_Delete(ball->sprite);
    Heap_Free(ball);
    return NULL;
}

void BattleFactoryApp_AddWindows(BgConfig *bgConfig, Window *windows) {
    u8 i;

    for (i = 0; i < 18; i++) {
        AddWindow(bgConfig, &windows[i], &sBattleFactoryAppWindowTemplates[i]);
        FillWindowPixelBuffer(&windows[i], 0);
    }
}

void BattleFactoryApp_RemoveWindows(Window *windows) {
    u16 i;

    for (i = 0; i < 18; i++) {
        RemoveWindow(&windows[i]);
    }
}

void BattleFactoryApp_DrawMessageFrame(Window *window, int frame) {
    LoadUserFrameGfx2(window->bgConfig, (GFBgLayer)GetWindowBgId(window), 994, 11, (u8)frame, HEAP_ID_100);
    FillWindowPixelBuffer(window, 15);
    DrawFrameAndWindow2(window, TRUE, 994, 11);
}

static void BattleFactoryApp_PrintStringAligned(Window *window, String *string, int x, int y, int fontId, u32 color, int align) {
    if (align == 1) {
        x -= FontID_String_GetWidth(fontId, string, 0);
    } else if (align == 2) {
        x -= FontID_String_GetWidth(fontId, string, 0) / 2;
    }
    AddTextPrinterParameterizedWithColor(window, fontId, string, x, y, TEXT_SPEED_NOTRANSFER, color, NULL);
}

void BattleFactoryApp_PrintMsgAligned(Window *window, MsgData *msgData, int msgId, int x, int y, int fontId, u32 color, int align) {
    String *string = NewString_ReadMsgData(msgData, msgId);
    BattleFactoryApp_PrintStringAligned(window, string, x, y, fontId, color, align);
    String_Delete(string);
}

void BattleFactoryApp_SetLinkNameWindows(Window *windows) {
    SetWindowX(&windows[0], 4);
    SetWindowX(&windows[1], 18);
}

void BattleFactoryApp_LoadButtonGraphics(BgConfig *bgConfig, NARC *narc) {
    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 0x86, bgConfig, GF_BG_LYR_SUB_1, 0x3D9, 0, TRUE, HEAP_ID_100);
    GfGfxLoader_GXLoadPalFromOpenNarc(narc, 0xC2, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_9_OFFSET, 0x40, HEAP_ID_100);
}

void BattleFactoryApp_DrawButtons(BgConfig *bgConfig, NARC *narc, int which) {
    NNSG2dScreenData *scrnData;
    void *data;
    u8 top;
    u16 *tiles;
    u8 rows;
    u8 y;
    u8 x;

    data = GfGfxLoader_GetScrnDataFromOpenNarc(narc, which + 0x87, TRUE, &scrnData, HEAP_ID_100);
    if (which == 0) {
        top = 11;
        rows = 7;
    } else {
        top = 14;
        rows = 4;
    }
    tiles = (u16 *)scrnData->rawData;
    for (y = 0; y < rows; y++) {
        for (x = 0; x < 32; x++) {
            FillBgTilemapRect(bgConfig, GF_BG_LYR_SUB_1, tiles[y * 32 + x] + 0x3D9, x, top + y, 1, 1, 9);
        }
    }
    Heap_Free(data);
}

void BattleFactoryApp_ClearButtons(BgConfig *bgConfig, int which) {
    int top;
    int height;

    if (which == 0) {
        top = 11;
        height = 7;
    } else {
        top = 14;
        height = 4;
    }
    FillBgTilemapRect(bgConfig, GF_BG_LYR_SUB_1, 0, 0, (u8)top, 32, (u8)height, 9);
    ScheduleBgTilemapBufferTransfer(bgConfig, GF_BG_LYR_SUB_1);
}
