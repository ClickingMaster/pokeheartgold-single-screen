#include <nnsys/gfd_internal.h>

#include "global.h"

#include "application/dress_up/dress_up_internal.h"

#include "bg_window.h"
#include "filesystem.h"
#include "gf_3d_loader.h"
#include "gf_3d_vramman.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "graphics_object_manager.h"
#include "heap.h"
#include "oam_manager.h"
#include "obj_char_transfer.h"
#include "obj_pltt_transfer.h"
#include "pokepic.h"
#include "sprite.h"
#include "sprite_transfer.h"
#include "system.h"
#include "touchscreen.h"
#include "unk_02009D48.h"
#include "unk_0200A090.h"
#include "swap_3d_buffers.h"

// Graphics core of the Dress-Up editor and the Accessory Portrait renderer: slot objects (3D textured quads of
// the graphics manager), Pokepic manager, 2D sprite system, BGs and the accessory/backdrop textures.

typedef struct DressUpPoint {
    u16 x;
    u16 y;
} DressUpPoint;

typedef struct DressUpSignedPoint {
    s16 x;
    s16 y;
} DressUpSignedPoint;

extern const GraphicsManagerTemplate sDressUpGraphicsManagerTemplate;
extern const ObjCharTransferTemplate sDressUpObjCharTransferTemplate;
extern const BgTemplate sDressUpBgTemplate_Sub0;
extern const BgTemplate sDressUpBgTemplate_Main1;
extern const BgTemplate sDressUpBgTemplate_Main2;
extern const BgTemplate sAccessoryPortraitBgTemplate_Main2;
extern const BgTemplate sDressUpBgTemplate_Main3;
extern const BgTemplate sDressUpBgTemplate_Sub1;
extern const GraphicsBanks sDressUpGraphicsBanks;
extern const GraphicsModes sDressUpGraphicsModes;

DressUpSlotList *DressUpGfx_CreateSlotList(int count, enum HeapID heapID) {
    DressUpSlotList *list = Heap_Alloc(heapID, sizeof(DressUpSlotList));
    list->slots = Heap_Alloc(heapID, count * sizeof(DressUpSlot));
    memset(list->slots, 0, count * sizeof(DressUpSlot));
    list->count = count;
    return list;
}

void DressUpGfx_FreeSlotList(DressUpSlotList *list) {
    Heap_Free(list->slots);
    Heap_Free(list);
}

DressUpSlot *DressUpGfx_AddSlotObject(DressUpSlotTemplate *template) {
    DressUpSlot *slot = DressUpGfx_FindFreeSlot(template->list);
    if (slot == NULL) {
        GF_AssertFail();
    }
    slot->id = template->id;
    slot->object = DressUpGfx_CreateSlotGraphicsObject(template);
    return slot;
}

BOOL DressUpGfx_IsTouchHeldOnSlot(DressUpSlot *slot) {
    TouchscreenHitbox hitbox;
    DressUpGfx_GetSlotHitbox(slot, &hitbox);
    return TouchscreenHitbox_TouchHeldIsIn(&hitbox);
}

#pragma force_active on

// TRUE if the touch is held on an opaque pixel of the slot (within 4 pixels of the touch point)
BOOL DressUpGfx_HitTestSlotPixels(DressUpSlot *slot, int *x, int *y, NNSG2dCharacterData *hitMap) {
    int slotX, slotY;
    int i, j;
    if (!DressUpGfx_IsTouchHeldOnSlot(slot)) {
        return FALSE;
    }
    DressUpGfx_GetSlotPos(slot, &slotX, &slotY);
    *x = gSystem.touchX - slotX;
    *y = gSystem.touchY - slotY;
    for (i = *y - 4; i < *y + 4; i++) {
        if (i >= 0) {
            for (j = *x - 4; j < *x + 4; j++) {
                if (j >= 0) {
                    if (DressUpGfx_CharPixelEquals(hitMap, j, i, 0) == 0) {
                        return TRUE;
                    }
                }
            }
        }
    }
    return FALSE;
}

#pragma force_active reset

void DressUpGfx_SetSlotPos(DressUpSlot *slot, s16 x, s16 y) {
    GraphicsObject_SetPosition(slot->object, x, y);
}

static inline DressUpPoint DressUpGfx_UnpackPoint(u32 packed) {
    DressUpPoint point;
    point.x = packed;
    point.y = packed >> 16;
    return point;
}

void DressUpGfx_GetSlotPos(DressUpSlot *slot, s32 *x, s32 *y) {
    DressUpPoint point;
    point = DressUpGfx_UnpackPoint(GraphicsObject_GetPosition(slot->object));
    *x = (s16)point.x;
    *y = (s16)point.y;
}

void DressUpGfx_GetSlotSize(DressUpSlot *slot, s32 *width, s32 *height) {
    DressUpPoint point;
    point = DressUpGfx_UnpackPoint(GraphicsObject_GetSize(slot->object));
    *width = (s16)point.x;
    *height = (s16)point.y;
}

void DressUpGfx_SetSlotVisible(DressUpSlot *slot, BOOL visible) {
    GraphicsObject_SetVisible(slot->object, visible);
}

void DressUpGfx_SetSlotPriority(DressUpSlot *slot, u32 depth) {
    GraphicsObject_SetDepth(slot->object, depth);
}

void ov41_02246020(DressUpSlot *slot, int *x, int *y) {
    u32 width;
    u32 height;

    DressUpGfx_GetSlotSize(slot, (s32 *)&width, (s32 *)&height);
    switch (width) {
    case 16:
        *x = 0;
        break;
    case 32:
        *x = 10;
        break;
    case 64:
        *x = 20;
        break;
    }
    switch (height) {
    case 16:
        *y = 0;
        break;
    case 32:
        *y = 10;
        break;
    case 64:
        *y = 20;
        break;
    }
}

DressUpSlot *DressUpGfx_FindFreeSlot(DressUpSlotList *list) {
    int i;

    for (i = 0; i < list->count; i++) {
        if (list->slots[i].object == NULL) {
            return &list->slots[i];
        }
    }
    return NULL;
}

GraphicsObject *DressUpGfx_CreateSlotGraphicsObject(const DressUpSlotTemplate *template) {
    GraphicsObjectTemplate objectTemplate;

    objectTemplate.manager = template->graphicsManager;
    objectTemplate.image = template->image;
    objectTemplate.palette = template->palette;
    objectTemplate.x = template->x;
    objectTemplate.y = template->y;
    objectTemplate.rotation = 0;
    objectTemplate.alpha = 31;
    objectTemplate.depth = 0;
    objectTemplate.paletteSlot = template->paletteSlot;
    return GraphicsManager_AddObject(&objectTemplate);
}

#pragma force_active on

void DressUpGfx_GetSlotHitbox(DressUpSlot *slot, TouchscreenHitbox *hitbox) {
    // the volatile locals keep the halfword stack round trips
    volatile DressUpSignedPoint pos, dim;
    volatile DressUpPoint position, size;
    u32 packed = GraphicsObject_GetPosition(slot->object);
    position.x = packed;
    position.y = packed >> 16;
    pos.x = position.x;
    pos.y = position.y;
    packed = GraphicsObject_GetSize(slot->object);
    size.x = packed;
    size.y = packed >> 16;
    dim.x = size.x;
    dim.y = size.y;
    {
        s16 y = pos.y, x;
        hitbox->rect.top = y;
        hitbox->rect.bottom = y + dim.y;
        x = pos.x;
        hitbox->rect.left = x;
        hitbox->rect.right = x + dim.x;
    }
}

#pragma force_active reset

void DressUpGfx_InitDisplay(void) {
    DressUpGfx_SetVramBanks();
    DressUpGfx_Init3D();
    DressUpGfx_Init2DModes();
    gSystem.screensFlipped = TRUE;
    GfGfx_SwapDisplay();
}

void DressUpGfx_ResetDisplay(void) {
    gSystem.screensFlipped = FALSE;
    GfGfx_SwapDisplay();
    DressUpGfx_DisablePlanes();
    DressUpGfx_ResetFrameVram();
    GX_ResetBankForTex();
}

void DressUp_InitGfxCore(DressUpGfx *gfx) {
    GraphicsManagerTemplate template = sDressUpGraphicsManagerTemplate;

    gfx->narc = NARC_New(NARC_a_0_2_6, HEAP_ID_14);
    DressUpGfx_CreateGraphicsManager(gfx, &template);
    DressUpGfx_CreatePokepicManager(gfx, HEAP_ID_14, 0x2800, 0x20);
    DressUp_CreateCharRawResMan(gfx);
    ov41_02246C90(gfx, HEAP_ID_13);
    DressUpGfx_InitSpriteSystem(gfx);
    gfx->bgConfig = BgConfig_Alloc(HEAP_ID_14);
    DressUpGfx_InitBgs(gfx);
}

void DressUp_FreeGfxCore(DressUpGfx *gfx) {
    ov41_02246CB0(gfx);
    DressUpGfx_DestroyGraphicsManager(gfx);
    DressUpGfx_FreeBgs(gfx);
    Heap_Free(gfx->bgConfig);
    DressUpGfx_DestroyPokepicManager(gfx);
    DressUpGfx_FreeSpriteSystem(gfx);
    DressUp_DestroyCharRawResMan(gfx);
    NARC_Delete(gfx->narc);
    Heap_Free(gfx->images);
    gfx->images = NULL;
    Heap_Free(gfx->palettes);
    gfx->palettes = NULL;
}

void DressUp_DrawFrame3D(DressUpGfx *gfx) {
    Thunk_G3X_Reset();
    NNS_G2dSetupSoftwareSpriteCamera();
    if (gfx->graphicsManagerActive) {
        DressUpGfx_DrawGraphicsManager(gfx);
    }
    if (gfx->pokepicActive) {
        PokepicManager_DrawAll(gfx->pokepicManager);
    }
    RequestSwap3DBuffers(GX_SORTMODE_AUTO, GX_BUFFERMODE_Z);
    DressUpGfx_RenderSprites(gfx);
}

void DressUpGfx_UploadRawResTables(DressUpGfx *gfx, DressUpGfxResources *resources) {
    DressUpGfx_AddImages(gfx, resources->images, resources->imageCount);
    DressUpGfx_AddPalettes(gfx, resources->palettes, resources->paletteCount);
}

void DressUpGfx_FreeAllTexAndPltt(DressUpGfx *gfx) {
    GraphicsManager_FreeImages(gfx->graphicsManager);
    GraphicsManager_FreePalettes(gfx->graphicsManager);
}

#pragma force_active on

Sprite *DressUpGfx_CreateSprite(DressUpGfx *gfx, int id, int x, int y, u32 priority, NNS_G2D_VRAM_TYPE whichScreen) {
    SpriteResourcesHeader header;
    SimpleSpriteTemplate template;
    CreateSpriteResourcesHeader(&header, id, id, id, id, -1, -1, 0, 0, gfx->charResMan, gfx->plttResMan, gfx->cellResMan, gfx->animResMan, NULL, NULL);
    template.spriteList = gfx->spriteList;
    template.header = &header;
    template.position.x = x << FX32_SHIFT;
    template.position.y = y << FX32_SHIFT;
    template.position.z = 0;
    template.priority = priority;
    template.whichScreen = whichScreen;
    template.heapID = HEAP_ID_14;
    return Sprite_Create(&template);
}

#pragma force_active reset

void DressUpGfx_LoadCharRes(DressUpGfx *gfx, NARC *narc, int fileId, BOOL compressed, int vram, int id) {
    SpriteResource *resource = AddCharResObjFromOpenNarc(gfx->charResMan, narc, fileId, compressed, id, vram, HEAP_ID_14);
    SpriteTransfer_CreateCharTransferTask_AllocAtEnd(resource);
}

void DressUpGfx_LoadPlttRes(DressUpGfx *gfx, NARC *narc, int fileId, BOOL compressed, int vram, int plttNum, int id) {
    SpriteResource *resource = AddPlttResObjFromOpenNarc(gfx->plttResMan, narc, fileId, compressed, id, vram, plttNum, HEAP_ID_14);
    SpriteTransfer_CreatePlttTransferTask(resource);
}

void DressUpGfx_LoadCellRes(DressUpGfx *gfx, NARC *narc, int fileId, BOOL compressed, int id) {
    AddCellOrAnimResObjFromOpenNarc(gfx->cellResMan, narc, fileId, compressed, id, GF_GFX_RES_TYPE_CELL, HEAP_ID_14);
}

void DressUpGfx_LoadAnimRes(DressUpGfx *gfx, NARC *narc, int fileId, BOOL compressed, int id) {
    AddCellOrAnimResObjFromOpenNarc(gfx->animResMan, narc, fileId, compressed, id, GF_GFX_RES_TYPE_ANIM, HEAP_ID_14);
}

void DressUpGfx_FreeCharRes(DressUpGfx *gfx, int id) {
    SpriteResource *resource = SpriteResourceCollection_Find(gfx->charResMan, id);
    DestroySingle2DGfxResObj(gfx->charResMan, resource);
}

void DressUpGfx_FreePlttRes(DressUpGfx *gfx, int id) {
    SpriteResource *resource = SpriteResourceCollection_Find(gfx->plttResMan, id);
    DestroySingle2DGfxResObj(gfx->plttResMan, resource);
}

void DressUpGfx_FreeCellRes(DressUpGfx *gfx, int id) {
    SpriteResource *resource = SpriteResourceCollection_Find(gfx->cellResMan, id);
    DestroySingle2DGfxResObj(gfx->cellResMan, resource);
}

void DressUpGfx_FreeAnimRes(DressUpGfx *gfx, int id) {
    SpriteResource *resource = SpriteResourceCollection_Find(gfx->animResMan, id);
    DestroySingle2DGfxResObj(gfx->animResMan, resource);
}

void DressUp_LoadRawResTables(DressUpGfx *gfx, DressUpGfxResources *resources) {
    DressUpGfx_AllocRawResTables(resources, DRESS_UP_IMAGE_MAX, DRESS_UP_PALETTE_MAX, HEAP_ID_14);
    DressUp_LoadAccessoryChars(gfx, resources);
    DressUp_LoadBackdropCharsAndPalettes(gfx, resources);
}

void DressUpGfx_DestroyRawResTables(DressUpGfxResources *resources) {
    DressUpGfx_FreeRawResTables(resources);
}

NNSG2dCharacterData *DressUpGfx_StoreUnpackedChar(DressUpGfx *gfx, void *data, int index) {
    GF2dGfxRawResMan_AllocObj(gfx->hitMapResMan, data, index);
    NNS_G2dGetUnpackedCharacterData(data, &gfx->hitMaps[index]);
    return gfx->hitMaps[index];
}

#pragma force_active on

void DressUp_DisableDragWindow(void) {
    reg_GX_DISPCNT &= ~0xE000; // windows 0, 1 and OBJ window off
    reg_G2_BG0CNT = (reg_G2_BG0CNT & ~3) | 1;
    reg_G2_BG1CNT &= ~3;
}

// window 0 around the stage while an object is dragged, BG0 above BG1
void DressUp_EnableDragWindow(void) {
    reg_GX_DISPCNT = (reg_GX_DISPCNT & ~0xE000) | 0x2000;
    reg_G2_WININ = (reg_G2_WININ & ~0x3F) | 0x1F;
    reg_G2_WINOUT = (reg_G2_WINOUT & ~0x3F) | 0x12;
    reg_G2_WIN0H = 0xAF6;
    reg_G2_WIN0V = 0x128F;
    reg_G2_BG0CNT = reg_G2_BG0CNT & ~3;
    reg_G2_BG1CNT = (reg_G2_BG1CNT & ~3) | 1;
}

#pragma force_active reset

void DressUp_VBlank(DressUpGfx *gfx) {
    DoScheduledBgGpuUpdates(gfx->bgConfig);
    PokepicManager_HandleLoadImgAndOrPltt(gfx->pokepicManager);
    OamManager_ApplyAndResetBuffers();
}

void AccessoryPortrait_AllocRawResTables(DressUpGfxResources *resources, enum HeapID heapID) {
    DressUpGfx_AllocRawResTables(resources, DRESS_UP_IMAGE_MAX, DRESS_UP_PALETTE_MAX, heapID);
}

// 4bpp pixel test: 2 = out of bounds, 1 = the pixel has colour value, 0 = it has not
int DressUpGfx_CharPixelEquals(NNSG2dCharacterData *character, int x, int y, int value) {
    int width = character->W;
    int height = character->H;
    int index;
    int shift;
    u32 pixel;

    width <<= 3;
    if (x < 0 || y < 0 || x >= width || y >= height * 8) {
        return 2;
    }
    index = y;
    index *= width;
    x += index;
    index = x;
    shift = index % 8 * 4;
    pixel = ((u32 *)character->pRawData)[index / 8];
    if ((value << shift) == (pixel & (15 << shift))) {
        return 1;
    }
    return 0;
}

void AccessoryPortrait_InitGfxCore(DressUpGfx *gfx, GraphicsManagerTemplate *template, enum HeapID heapID) {
    DressUpGfx_CreateGraphicsManager(gfx, template);
    DressUpGfx_CreatePokepicManager(gfx, heapID, 0x2800, 0x20);
    PokepicManager_SetNeedG3IdentityFlag(gfx->pokepicManager, TRUE);
    ov41_02246C90(gfx, heapID);
}

void AccessoryPortrait_InitBg(DressUpGfx *gfx, BgConfig *bgConfig, enum HeapID heapID) {
    BgTemplate template;

    gfx->bgConfig = bgConfig;
    template = sAccessoryPortraitBgTemplate_Main2;
    FreeBgTilemapBuffer(gfx->bgConfig, GF_BG_LYR_MAIN_2);
    InitBgFromTemplate(gfx->bgConfig, GF_BG_LYR_MAIN_2, &template, GF_BG_TYPE_TEXT);
    BG_ClearCharDataRange(GF_BG_LYR_MAIN_2, 0x20, 0, heapID);
    BgClearTilemapBufferAndCommit(gfx->bgConfig, GF_BG_LYR_MAIN_2);
}

void AccessoryPortrait_FreeGfxCore(DressUpGfx *gfx) {
    ov41_02246CB0(gfx);
    DressUpGfx_DestroyGraphicsManager(gfx);
    DressUpGfx_DestroyPokepicManager(gfx);
    Heap_Free(gfx->images);
    gfx->images = NULL;
    Heap_Free(gfx->palettes);
    gfx->palettes = NULL;
}

void AccessoryPortrait_FreeBg(DressUpGfx *gfx) {
    FreeBgTilemapBuffer(gfx->bgConfig, GF_BG_LYR_MAIN_2);
}

void DressUpGfx_HandlePokepicLoad(DressUpGfx *gfx) {
    PokepicManager_HandleLoadImgAndOrPltt(gfx->pokepicManager);
}

static inline void DressUpGfx_G3Scale(fx32 x, fx32 y, fx32 z) {
    reg_G3_MTX_SCALE = x;
    reg_G3_MTX_SCALE = y;
    reg_G3_MTX_SCALE = z;
}

// draws the stage rotated by angle and scaled around (x, y)
void AccessoryPortrait_DrawTransformed(DressUpGfx *gfx, int x, int y, u16 angle, const DressUpScale *scale) {
    reg_G3_MTX_IDENTITY = 0;
    reg_G3_MTX_PUSH = 0;
    NNS_G2dSetupSoftwareSpriteCamera();
    G3_Translate(x << FX32_SHIFT, y << FX32_SHIFT, 0);
    G3_RotZ(FX_SinIdx(angle), FX_CosIdx(angle));
    DressUpGfx_G3Scale(scale->x, scale->y, scale->z);
    G3_Translate(-x << FX32_SHIFT, -y << FX32_SHIFT, 0);
    reg_G3_MTX_PUSH = 0;
    if (gfx->graphicsManagerActive) {
        DressUpGfx_DrawGraphicsManager(gfx);
    }
    if (gfx->pokepicActive) {
        PokepicManager_DrawAll(gfx->pokepicManager);
    }
    reg_G3_MTX_POP = 1;
    reg_G3_MTX_POP = 1;
}

void AccessoryPortrait_InitAppGfx(DressUpGfx *gfx, enum HeapID heapID) {
    gfx->narc = NARC_New(NARC_a_0_2_6, HEAP_ID_14);
    DressUpGfx_InitSpriteSystem(gfx);
    gfx->bgConfig = BgConfig_Alloc(HEAP_ID_14);
    DressUpGfx_InitBgs(gfx);
}

void AccessoryPortrait_FreeAppGfx(DressUpGfx *gfx) {
    DressUpGfx_FreeBgs(gfx);
    Heap_Free(gfx->bgConfig);
    NARC_Delete(gfx->narc);
    DressUpGfx_FreeSpriteSystem(gfx);
}

void AccessoryPortrait_AppVBlank(DressUpGfx *gfx) {
    DoScheduledBgGpuUpdates(gfx->bgConfig);
    OamManager_ApplyAndResetBuffers();
}

void AccessoryPortrait_RenderSprites(DressUpGfx *gfx) {
    DressUpGfx_RenderSprites(gfx);
}

void DressUpGfx_SetVramBanks(void) {
    GraphicsBanks banks = sDressUpGraphicsBanks;
    GfGfx_SetBanks(&banks);
}

void DressUpGfx_Init3D(void) {
    NNS_G3dInit();
    G3X_InitMtxStack();
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG0, TRUE);
    G2_SetBG0Priority(1);
    G3X_SetShading(GX_SHADING_TOON);
    G3X_AntiAlias(TRUE);
    G3X_AlphaTest(FALSE, 0);
    G3X_AlphaBlend(TRUE);
    G3X_SetClearColor(0, 0, 0x7FFF, 0x3F, FALSE);
    G3_SwapBuffers(GX_SORTMODE_AUTO, GX_BUFFERMODE_W);
    G3_ViewPort(0, 0, 255, 191);
    GF_3DVramMan_InitFrameTexVramManager(2, TRUE);
    GF_3DVramMan_InitFramePlttVramManager(0x4000, TRUE);
}

void DressUpGfx_Init2DModes(void) {
    GraphicsModes modes = sDressUpGraphicsModes;
    SetBothScreensModesAndDisable(&modes);
    reg_GX_DISPCNT = (reg_GX_DISPCNT & 0xFFCFFFEF) | GX_PLANEMASK_OBJ;
    NNS_G2dInitOamManagerModule();
    GfGfx_DisableEngineAPlanes();
    GfGfx_DisableEngineBPlanes();
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_ALL, TRUE);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG0 | GX_PLANEMASK_BG1 | GX_PLANEMASK_OBJ, TRUE);
}

void DressUpGfx_ResetFrameVram(void) {
    NNS_GfdResetFrmTexVramState();
    NNS_GfdResetFrmPlttVramState();
}

void DressUpGfx_DisablePlanes(void) {
    GfGfx_DisableEngineAPlanes();
    GfGfx_DisableEngineBPlanes();
    NNS_G2dInitOamManagerModule();
}

void DressUpGfx_CreateGraphicsManager(DressUpGfx *gfx, GraphicsManagerTemplate *template) {
    gfx->graphicsManager = GraphicsManager_New(template);
    gfx->images = Heap_Alloc(template->heapId, DRESS_UP_IMAGE_MAX * sizeof(GraphicsImage *));
    gfx->imageMax = DRESS_UP_IMAGE_MAX;
    gfx->imageCount = 0;
    gfx->palettes = Heap_Alloc(template->heapId, DRESS_UP_PALETTE_MAX * sizeof(GraphicsPalette *));
    gfx->paletteMax = DRESS_UP_PALETTE_MAX;
    gfx->paletteCount = 0;
    gfx->graphicsManagerActive = TRUE;
}

void DressUpGfx_DestroyGraphicsManager(DressUpGfx *gfx) {
    GraphicsManager_Delete(gfx->graphicsManager);
    gfx->graphicsManager = NULL;
}

void DressUpGfx_DrawGraphicsManager(DressUpGfx *gfx) {
    GraphicsManager_DrawObjects(gfx->graphicsManager);
}

#pragma force_active on

void DressUpGfx_AddImages(DressUpGfx *gfx, GraphicsImageTemplate *templates, int count) {
    int i = 0;
    u8 *cursor;
    if (i < count) {
        cursor = (u8 *)templates;
        do {
            if (gfx->imageCount >= gfx->imageMax) {
                GF_AssertFail();
            }
            if (templates->character) {
                gfx->images[gfx->imageCount] = GraphicsManager_LoadImage((GraphicsImageTemplate *)cursor);
            } else {
                gfx->images[gfx->imageCount] = NULL;
            }
            gfx->imageCount++;
            templates++;
            cursor += sizeof(GraphicsImageTemplate);
            i++;
        } while (i < count);
    }
}

void DressUpGfx_AddPalettes(DressUpGfx *gfx, GraphicsPaletteTemplate *templates, int count) {
    int i = 0;
    u8 *cursor;
    if (i < count) {
        cursor = (u8 *)templates;
        do {
            if (gfx->paletteCount >= gfx->paletteMax) {
                GF_AssertFail();
            }
            if (templates->palette) {
                gfx->palettes[gfx->paletteCount] = GraphicsManager_LoadPalette((GraphicsPaletteTemplate *)cursor);
            } else {
                gfx->palettes[gfx->paletteCount] = NULL;
            }
            gfx->paletteCount++;
            templates++;
            cursor += sizeof(GraphicsPaletteTemplate);
            i++;
        } while (i < count);
    }
}

#pragma force_active reset

void DressUpGfx_InitBgs(DressUpGfx *gfx) {
    {
        BgTemplate template = sDressUpBgTemplate_Main1;
        InitBgFromTemplate(gfx->bgConfig, GF_BG_LYR_MAIN_1, &template, GF_BG_TYPE_TEXT);
        BG_ClearCharDataRange(GF_BG_LYR_MAIN_1, 0x20, 0, HEAP_ID_14);
        BgClearTilemapBufferAndCommit(gfx->bgConfig, GF_BG_LYR_MAIN_1);
    }
    {
        BgTemplate template = sDressUpBgTemplate_Main2;
        InitBgFromTemplate(gfx->bgConfig, GF_BG_LYR_MAIN_2, &template, GF_BG_TYPE_TEXT);
        BG_ClearCharDataRange(GF_BG_LYR_MAIN_2, 0x20, 0, HEAP_ID_14);
        BgClearTilemapBufferAndCommit(gfx->bgConfig, GF_BG_LYR_MAIN_2);
    }
    {
        BgTemplate template = sDressUpBgTemplate_Main3;
        InitBgFromTemplate(gfx->bgConfig, GF_BG_LYR_MAIN_3, &template, GF_BG_TYPE_TEXT);
        BG_ClearCharDataRange(GF_BG_LYR_MAIN_3, 0x20, 0, HEAP_ID_14);
        BgClearTilemapBufferAndCommit(gfx->bgConfig, GF_BG_LYR_MAIN_3);
    }
    {
        BgTemplate template = sDressUpBgTemplate_Sub0;
        InitBgFromTemplate(gfx->bgConfig, GF_BG_LYR_SUB_0, &template, GF_BG_TYPE_TEXT);
        BG_ClearCharDataRange(GF_BG_LYR_SUB_0, 0x20, 0, HEAP_ID_14);
        BgClearTilemapBufferAndCommit(gfx->bgConfig, GF_BG_LYR_SUB_0);
    }
    {
        BgTemplate template = sDressUpBgTemplate_Sub1;
        InitBgFromTemplate(gfx->bgConfig, GF_BG_LYR_SUB_1, &template, GF_BG_TYPE_TEXT);
        BG_ClearCharDataRange(GF_BG_LYR_SUB_1, 0x20, 0, HEAP_ID_14);
        BgClearTilemapBufferAndCommit(gfx->bgConfig, GF_BG_LYR_SUB_1);
    }
}

void DressUpGfx_FreeBgs(DressUpGfx *gfx) {
    FreeBgTilemapBuffer(gfx->bgConfig, GF_BG_LYR_MAIN_1);
    FreeBgTilemapBuffer(gfx->bgConfig, GF_BG_LYR_MAIN_2);
    FreeBgTilemapBuffer(gfx->bgConfig, GF_BG_LYR_MAIN_3);
    FreeBgTilemapBuffer(gfx->bgConfig, GF_BG_LYR_SUB_0);
    FreeBgTilemapBuffer(gfx->bgConfig, GF_BG_LYR_SUB_1);
}

void DressUp_CreateCharRawResMan(DressUpGfx *gfx) {
    gfx->hitMapResMan = GF2dGfxRawResMan_Create(DRESS_UP_HIT_MAP_MAX, HEAP_ID_14);
    gfx->hitMaps = Heap_Alloc(HEAP_ID_14, DRESS_UP_HIT_MAP_MAX * sizeof(NNSG2dCharacterData *));
    memset(gfx->hitMaps, 0, DRESS_UP_HIT_MAP_MAX * sizeof(NNSG2dCharacterData *));
    gfx->hitMapCount = DRESS_UP_HIT_MAP_MAX;
}

void DressUp_DestroyCharRawResMan(DressUpGfx *gfx) {
    Heap_Free(gfx->hitMaps);
    GF2dGfxRawResObj_Destroy(gfx->hitMapResMan);
    gfx->hitMapCount = 0;
}

void DressUpGfx_InitSpriteSystem(DressUpGfx *gfx) {
    ObjCharTransferTemplate template = sDressUpObjCharTransferTemplate;

    ObjCharTransfer_InitEx(&template, GX_OBJVRAMMODE_CHAR_1D_32K, GX_OBJVRAMMODE_CHAR_1D_32K);
    ObjPlttTransfer_Init(5, HEAP_ID_14);
    ObjCharTransfer_ClearBuffers();
    ObjPlttTransfer_Reset();
    NNS_G2dInitOamManagerModule();
    OamManager_Create(0, 0x7C, 0, 0x1F, 0, 0x7C, 0, 0x1F, HEAP_ID_14);
    gfx->spriteList = G2dRenderer_Init(0x30, &gfx->renderer, HEAP_ID_14);
    G2dRenderer_SetSubSurfaceCoords(&gfx->renderer, 0, 2 << 20);
    gfx->charResMan = Create2DGfxResObjMan(8, GF_GFX_RES_TYPE_CHAR, HEAP_ID_14);
    gfx->plttResMan = Create2DGfxResObjMan(5, GF_GFX_RES_TYPE_PLTT, HEAP_ID_14);
    gfx->cellResMan = Create2DGfxResObjMan(0x30, GF_GFX_RES_TYPE_CELL, HEAP_ID_14);
    gfx->animResMan = Create2DGfxResObjMan(0x30, GF_GFX_RES_TYPE_ANIM, HEAP_ID_14);
}

void DressUpGfx_FreeSpriteSystem(DressUpGfx *gfx) {
    int i;
    DressUpGfx *walk;

    SpriteList_Delete(gfx->spriteList);
    i = 0;
    walk = gfx;
    // walks the four managers from charResMan on
    while (i < 4) {
        Destroy2DGfxResObjMan(walk->charResMan);
        i++;
        walk = (DressUpGfx *)((u8 *)walk + 4);
    }
    ObjCharTransfer_Destroy();
    ObjPlttTransfer_Destroy();
    OamManager_Free();
}

void DressUpGfx_RenderSprites(DressUpGfx *gfx) {
    SpriteList_RenderAndAnimateSprites(gfx->spriteList);
}

#pragma force_active on

// NARC_a_0_2_6 members 1..100 -> hit maps 0..99 and image templates, member 0 = the shared accessory palette
void DressUp_LoadAccessoryChars(DressUpGfx *gfx, DressUpGfxResources *resources) {
    int i;
    void *data;
    for (i = 0; i < DRESS_UP_ACCESSORY_NUM; i++) {
        data = GfGfxLoader_LoadFromOpenNarc(gfx->narc, i + 1, FALSE, HEAP_ID_14, TRUE);
        GF_ASSERT(data != NULL);
        resources->images[i].character = DressUpGfx_StoreUnpackedChar(gfx, data, i);
        resources->images[i].manager = gfx->graphicsManager;
    }
    data = GfGfxLoader_LoadFromOpenNarc(gfx->narc, 0, FALSE, HEAP_ID_14, TRUE);
    GF2dGfxRawResMan_AllocObj(resources->plttResMan, data, 0);
    NNS_G2dGetUnpackedPaletteData(data, &resources->palettes->palette);
    resources->palettes->manager = gfx->graphicsManager;
    resources->palettes->paletteCount = 3;
}

// backdrop i: chars member 206 + i -> hit map / image 100 + i, palette member 135 + 4i -> palette 1 + i
void DressUp_LoadBackdropCharsAndPalettes(DressUpGfx *gfx, DressUpGfxResources *resources) {
    int fileId;
    DressUpGfxResources *out = resources;
    int i = 0;
    fileId = 135;
    for (; i < DRESS_UP_BACKDROP_NUM; i++) {
        void *data = GfGfxLoader_LoadFromOpenNarc(gfx->narc, i + 206, FALSE, HEAP_ID_14, TRUE);
        u32 offset = (i + 100) * sizeof(GraphicsImageTemplate);
        NNSG2dCharacterData *character = DressUpGfx_StoreUnpackedChar(gfx, data, i + 100);
        ((GraphicsImageTemplate *)((u32)out->images + offset))->character = character;
        ((GraphicsImageTemplate *)((u32)out->images + offset))->manager = gfx->graphicsManager;
        int id = i + 1;
        data = GfGfxLoader_LoadFromOpenNarc(gfx->narc, fileId, FALSE, HEAP_ID_14, TRUE);
        GF2dGfxRawResMan_AllocObj(out->plttResMan, data, id);
        u32 paletteOffset = id * sizeof(GraphicsPaletteTemplate);
        NNS_G2dGetUnpackedPaletteData(data, &((GraphicsPaletteTemplate *)((u32)out->palettes + paletteOffset))->palette);
        if (!((GraphicsPaletteTemplate *)((u32)out->palettes + paletteOffset))->palette) {
            GF_AssertFail();
        }
        ((GraphicsPaletteTemplate *)((u32)out->palettes + paletteOffset))->manager = gfx->graphicsManager;
        ((GraphicsPaletteTemplate *)((u32)out->palettes + paletteOffset))->paletteCount = 1;
        fileId += 4;
    }
}

#pragma force_active reset

void ov41_02246C90(DressUpGfx *gfx, enum HeapID heapID) {
    gfx->monPicAttrs = GfGfxLoader_LoadFromOpenNarc(gfx->narc, 0xEB, FALSE, heapID, FALSE);
}

void ov41_02246CB0(DressUpGfx *gfx) {
    Heap_Free(gfx->monPicAttrs);
    gfx->monPicAttrs = NULL;
}
