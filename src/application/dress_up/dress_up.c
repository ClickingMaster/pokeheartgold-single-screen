#include <nnsys/gfd_internal.h>

#include "global.h"

#include "constants/pokemon.h"
#include "constants/sndseq.h"

#include "application/dress_up/dress_up_internal.h"
#include "msgdata/msg.naix"
#include "msgdata/msg/msg_0215.h"

#include "assert.h"
#include "bg_window.h"
#include "brightness.h"
#include "fashion_case.h"
#include "filesystem.h"
#include "font.h"
#include "font_sprite.h"
#include "game_stats.h"
#include "gf_3d_loader.h"
#include "gf_3d_vramman.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "graphics_object_manager.h"
#include "heap.h"
#include "launch_application_internal.h"
#include "math_util.h"
#include "menu_input_state.h"
#include "message_format.h"
#include "msgdata.h"
#include "oam_manager.h"
#include "obj_char_transfer.h"
#include "obj_pltt_transfer.h"
#include "options.h"
#include "overlay_manager.h"
#include "player_data.h"
#include "pm_string.h"
#include "pokemon.h"
#include "pokepic.h"
#include "render_text.h"
#include "render_window.h"
#include "screen_fade_legacy.h"
#include "sound_playback.h"
#include "sound_system.h"
#include "sprite.h"
#include "sprite_resource.h"
#include "sprite_resource_header.h"
#include "sprite_transfer.h"
#include "swap_3d_buffers.h"
#include "sys_task_api.h"
#include "systask_environment.h"
#include "system.h"
#include "text.h"
#include "touch_hitbox_controller.h"
#include "touchpad.h"
#include "touchscreen.h"
#include "yes_no_prompt.h"

// Pokemon Dress-Up: the editor (ScrCmd_OpenDressUp) and the Accessory Portrait renderer.
//
// Graphics core shared by both: slot objects (3D textured quads of the graphics manager), Pokepic manager, 2D
// sprite system, BGs and the accessory/backdrop textures.

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

// fashion save entry accessors (src/save_fashion.c)
void FashionEntry_MarkSaved(SaveFashionDataSub *entry);
void FashionEntry_Clear(SaveFashionDataSub *entry);
void FashionEntry_SetMon(SaveFashionDataSub *entry, Pokemon *mon, DressUpMonPic *monPic);
void FashionEntry_PlaceItem(SaveFashionDataSub *entry, DressUpSlot *slot, int index);
void FashionEntry_SetBackdrop(SaveFashionDataSub *entry, int value);
void FashionEntry_SetMonSecondText(SaveFashionDataSub *entry, String *string, u32 gender); // u8 in save_fashion.c; the caller does not truncate

typedef struct DressUpTransition {
    DressUpWork *work;
    u32 *done;
    int counter;
    int state;
} DressUpTransition;

enum {
    DRESS_UP_STATE_INIT,
    DRESS_UP_STATE_FADE_IN,
    DRESS_UP_STATE_WAIT_FADE_IN,
    DRESS_UP_STATE_TUTORIAL,
    DRESS_UP_STATE_WAIT_TUTORIAL_MSG,
    DRESS_UP_STATE_WAIT_TUTORIAL_INPUT,
    DRESS_UP_STATE_EDIT,
    DRESS_UP_STATE_WAIT_FINISH_TRANSITION,
    DRESS_UP_STATE_DRESSED_UP_PROMPT,
    DRESS_UP_STATE_SAVE_PROMPT,
    DRESS_UP_STATE_WAIT_RETURN_TRANSITION,
    DRESS_UP_STATE_FADE_OUT,
    DRESS_UP_STATE_WAIT_FADE_OUT,
};

void DressUp_FinishTransitionTask(SysTask *task, DressUpTransition *env);
void DressUp_ReturnTransitionTask(SysTask *task, DressUpTransition *env);
BOOL DressUp_SlideCanvasStep(DressUpTransition *env, int dx, int dy, int frames);

// page swap task
typedef struct DressUpPageSwap {
    DressUpTray *tray;       // 0x00
    int fromGroup;           // 0x04
    int fromPage;            // 0x08
    int toGroup;             // 0x0C
    int toPage;              // 0x10
    BOOL tabSlideDone;       // 0x14
    int counter;             // 0x18
    int state;               // 0x1C
    int fromLeft;            // 0x20, objects of the old page still to slide out
    int toLeft;              // 0x24, objects of the new page still to slide in
    DressUpSlideAnim *anims; // 0x28
    int animCount;           // 0x2C
} DressUpPageSwap;

void DressUpTray_PageSwapTask(SysTask *task, DressUpPageSwap *swap);

int FashionCase_GetWallpaperOrder(FashionCase *fashionCase, int index); // u8 in save_fashion.c; the caller does not truncate

// object tween back into the tray
typedef struct DressUpTrayTween {
    DressUpTray *tray; // 0x00
    DressUpNode *node; // 0x04
    int stepX;         // 0x08
    int stepY;         // 0x0C
    int destX;         // 0x10
    int destY;         // 0x14
    int mode;          // 0x18
    int page;          // 0x1C
    int frames;        // 0x20
    BOOL *paused;      // 0x24, DressUpTray.pageSwapActive
    BOOL *dragging;    // 0x28, DressUpDrag.dragging
} DressUpTrayTween;

void DressUp_TweenToTrayTask(SysTask *task, DressUpTrayTween *tween);

// BG slide task
typedef struct DressUpBgSlide {
    DressUpBgScreen *screen;          // 0x00
    DressUpBgScreenTemplate template; // 0x04, loaded at the end
    BOOL *done;                       // 0x34
    int frames;                       // 0x38
    int stepX;                        // 0x3C
    int stepY;                        // 0x40
    int startX;                       // 0x44
    int startY;                       // 0x48
} DressUpBgSlide;                     // size 0x4C

typedef struct DressUpCharUpload {
    BgConfig *bgConfig;            // 0x00
    NNSG2dCharacterData *charData; // 0x04
    void *buffer;                  // 0x08
    u32 layer;                     // 0x0C
    u32 tileStart;                 // 0x10
} DressUpCharUpload;

typedef struct DressUpPlttUpload {
    NNSG2dPaletteData *plttData; // 0x00
    void *buffer;                // 0x04
    u32 location;                // 0x08, 0 main, 4 sub
    u32 offset;                  // 0x0C
    u32 size;                    // 0x10
} DressUpPlttUpload;

void DressUp_BgSlideTask(SysTask *task, DressUpBgSlide *slide);
void DressUpBgScreen_LoadCharsTask(SysTask *task, DressUpCharUpload *upload);
void DressUpBgScreen_LoadPlttTask(SysTask *task, DressUpPlttUpload *upload);

extern const u8 sDressUpKeyPressTouchStates[2];

void TextOBJ_UpdatePosition(TextOBJ *object);

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

void DressUpGfx_GetSlotMargins(DressUpSlot *slot, int *x, int *y) {
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
    DressUpGfx_LoadMonPicAttrs(gfx, HEAP_ID_13);
    DressUpGfx_InitSpriteSystem(gfx);
    gfx->bgConfig = BgConfig_Alloc(HEAP_ID_14);
    DressUpGfx_InitBgs(gfx);
}

void DressUp_FreeGfxCore(DressUpGfx *gfx) {
    DressUpGfx_FreeMonPicAttrs(gfx);
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
    DressUpGfx_LoadMonPicAttrs(gfx, heapID);
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
    DressUpGfx_FreeMonPicAttrs(gfx);
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

void DressUpGfx_LoadMonPicAttrs(DressUpGfx *gfx, enum HeapID heapID) {
    gfx->monPicAttrs = GfGfxLoader_LoadFromOpenNarc(gfx->narc, 0xEB, FALSE, heapID, FALSE);
}

void DressUpGfx_FreeMonPicAttrs(DressUpGfx *gfx) {
    Heap_Free(gfx->monPicAttrs);
    gfx->monPicAttrs = NULL;
}

// The editor app: Init/Main/Exit, the editor's graphics, canvas, tray, frame BG, buttons and HUD set-up, the DONE /
// return transitions, the two yes/no prompts and saving the design.

void DressUpGfx_CreatePokepicManager(DressUpGfx *gfx, enum HeapID heapID, u32 texSize, u32 plttSize) {
    u32 key;

    gfx->pokepicManager = PokepicManager_Create(heapID);
    gfx->texKey = NNS_GfdDefaultFuncAllocTexVram(texSize, FALSE, 0);
    gfx->plttKey = NNS_GfdDefaultFuncAllocPlttVram(plttSize, FALSE, 1);

    key = gfx->texKey;
    PokepicManager_SetCharBaseAddrAndSize(gfx->pokepicManager, (key << 16) >> 13, ((key & 0x7FFF0000) >> 16) << 4);
    key = gfx->plttKey;
    PokepicManager_SetPlttBaseAddrAndSize(gfx->pokepicManager, (key << 16) >> 13, ((key & 0xFFFF0000) >> 16) << 3);
    gfx->pokepicActive = TRUE;
}

void DressUpGfx_DestroyPokepicManager(DressUpGfx *gfx) {
    PokepicManager_Delete(gfx->pokepicManager);
    NNS_GfdDefaultFuncFreeTexVram(gfx->texKey);
    NNS_GfdDefaultFuncFreePlttVram(gfx->plttKey);
    gfx->pokepicActive = FALSE;
}

void DressUpGfx_AllocRawResTables(DressUpGfxResources *resources, int imageCount, int paletteCount, enum HeapID heapID) {
    u32 size = imageCount * sizeof(GraphicsImageTemplate);
    resources->images = Heap_Alloc(heapID, size);
    memset(resources->images, 0, size);
    resources->charResMan = GF2dGfxRawResMan_Create(imageCount, heapID);
    resources->imageCount = imageCount;

    size = paletteCount * sizeof(GraphicsPaletteTemplate);
    resources->palettes = Heap_Alloc(heapID, size);
    memset(resources->palettes, 0, size);
    resources->plttResMan = GF2dGfxRawResMan_Create(paletteCount, heapID);
    resources->paletteCount = paletteCount;
}

void DressUpGfx_FreeRawResTables(DressUpGfxResources *resources) {
    if (resources->charResMan != NULL) {
        GF2dGfxRawResObj_Destroy(resources->charResMan);
        resources->charResMan = NULL;
    }
    if (resources->plttResMan != NULL) {
        GF2dGfxRawResObj_Destroy(resources->plttResMan);
        resources->plttResMan = NULL;
    }
    Heap_Free(resources->images);
    resources->images = NULL;
    Heap_Free(resources->palettes);
    resources->palettes = NULL;
}

BOOL DressUp_Init(OverlayManager *man, int *state) {
    DressUpWork *work;
    DressUpArgs *args;

    Heap_Create(HEAP_ID_3, HEAP_ID_13, 0x20000);
    Heap_Create(HEAP_ID_3, HEAP_ID_14, 0x40000);
    work = OverlayManager_CreateAndGetData(man, sizeof(DressUpWork), HEAP_ID_13);
    memset(work, 0, sizeof(DressUpWork));
    Main_SetVBlankIntrCB(DressUp_VBlankCB, work);
    HBlankInterruptDisable();
    args = OverlayManager_GetArgs(man);
    work->options = args->options;
    if (args->menuInputStatePtr != NULL) {
        work->menuInputState = MenuInputStateMgr_GetState(args->menuInputStatePtr);
    } else {
        work->menuInputState = MENU_INPUT_STATE_BUTTONS;
    }
    GF_TouchpadResetSampling();
    GF_TouchpadStartSampling(4);
    DressUp_LoadFashionCaseTable(args->fashionCase, work->caseTable);
    DressUp_InitGraphics(work);
    DressUpNodePool_Init(&work->nodePool, 0x2CF, HEAP_ID_13);
    work->slotList = DressUpGfx_CreateSlotList(0x2BC, HEAP_ID_13);
    DressUp_InitCanvas(work, args->mon, 10, FALSE);
    DressUp_InitTray(work);
    DressUp_LoadFrameBg(work, 0);
    DressUp_InitButtons(work);
    DressUp_InitHud(work, args->options);
    DressUpDrag_Init(&work->touch, &work->canvas, &work->tray, &work->gfx, &work->hud, TRUE);
    work->yesNoPrompt = YesNoPrompt_Create(HEAP_ID_13);
    work->msgWindow = AllocWindows(HEAP_ID_13, 1);
    work->uiState = DRESS_UP_UI_ACCESSORIES;
    Sound_SetSceneAndPlayBGM(0x35, 0, 0);
    return TRUE;
}

BOOL DressUp_Main(OverlayManager *man, int *state) {
    DressUpWork *work = OverlayManager_GetData(man);
    BOOL done = FALSE;
    DressUpArgs *args = OverlayManager_GetArgs(man);

    switch (*state) {
    case DRESS_UP_STATE_INIT:
    case DRESS_UP_STATE_FADE_IN:
        BeginNormalPaletteFade(1, 5, 5, RGB_BLACK, 6, 1, HEAP_ID_13);
        *state = DRESS_UP_STATE_WAIT_FADE_IN;
        break;
    case DRESS_UP_STATE_WAIT_FADE_IN:
        if (IsPaletteFadeFinished()) {
            (*state)++;
        }
        break;
    case DRESS_UP_STATE_TUTORIAL:
        // tutorial
        if (args->showTutorial == 1) {
            TextFlags_SetCanTouchSpeedUpPrint(TRUE);
            work->printerId = DressUpHud_PrintMsg(&work->hud, NARC_msgdata_msg, NARC_msg_msg_0215_bin, msg_0215_00047);
            (*state)++;
        } else {
            *state = DRESS_UP_STATE_EDIT;
        }
        break;
    case DRESS_UP_STATE_WAIT_TUTORIAL_MSG:
        if (!TextPrinterCheckActive(work->printerId)) {
            DressUpHud_FreeMsgString(&work->hud);
            (*state)++;
        }
        break;
    case DRESS_UP_STATE_WAIT_TUTORIAL_INPUT:
        if (((gSystem.newKeys & (PAD_BUTTON_A | PAD_BUTTON_B)) | gSystem.touchNew) != 0) {
            DressUpHud_PrintMsgInstant(&work->hud, NARC_msgdata_msg, NARC_msg_msg_0215_bin, msg_0215_00048);
            TextFlags_SetCanTouchSpeedUpPrint(FALSE);
            (*state)++;
        }
        break;
    case DRESS_UP_STATE_EDIT:
        // editing
        if (work->uiState == DRESS_UP_UI_DONE) {
            DressUp_StartFinishTransition(work, &work->transitionDone);
            *state = DRESS_UP_STATE_WAIT_FINISH_TRANSITION;
        }
        DressUpTouch_Update(&work->touch);
        DressUp_UpdateCountMeter(work);
        DressUpHud_UpdateCountdown(&work->hud);
        DressUp_UpdateButtons(work);
        break;
    case DRESS_UP_STATE_WAIT_FINISH_TRANSITION:
        if (work->transitionDone) {
            work->transitionDone = FALSE;
            *state = DRESS_UP_STATE_DRESSED_UP_PROMPT;
            work->uiState = DRESS_UP_UI_DRESSED_UP;
            DressUp_StartBackdropAnim(&work->backdropAnim, &work->canvas, HEAP_ID_14);
        }
        break;
    case DRESS_UP_STATE_DRESSED_UP_PROMPT:
        if (work->uiState == DRESS_UP_UI_DRESSED_NO) {
            DressUp_StartReturnTransition(work, &work->transitionDone);
            *state = DRESS_UP_STATE_WAIT_RETURN_TRANSITION;
        } else if (work->uiState == DRESS_UP_UI_DRESSED_YES) {
            *state = DRESS_UP_STATE_SAVE_PROMPT;
            work->uiState = DRESS_UP_UI_SAVE;
            DressUp_ShowSavePrompt(work);
        } else {
            work->uiState = DressUp_HandleDressedUpPrompt(work);
            DressUp_UpdateBackdropAnim(&work->backdropAnim);
        }
        break;
    case DRESS_UP_STATE_SAVE_PROMPT:
        if (work->uiState == DRESS_UP_UI_SAVE_YES) {
            work->saveRequested = TRUE;
            *state = DRESS_UP_STATE_FADE_OUT;
        } else if (work->uiState == DRESS_UP_UI_SAVE_NO) {
            work->saveRequested = FALSE;
            *state = DRESS_UP_STATE_FADE_OUT;
        } else {
            work->uiState = DressUp_HandleSavePrompt(work);
            DressUp_UpdateBackdropAnim(&work->backdropAnim);
        }
        break;
    case DRESS_UP_STATE_WAIT_RETURN_TRANSITION:
        if (work->transitionDone) {
            work->transitionDone = FALSE;
            *state = DRESS_UP_STATE_EDIT;
            work->uiState = DRESS_UP_UI_ACCESSORIES;
            DressUp_StopBackdropAnim(&work->backdropAnim);
        }
        break;
    case DRESS_UP_STATE_FADE_OUT:
        BeginNormalPaletteFade(1, 0, 0, RGB_BLACK, 6, 1, HEAP_ID_13);
        (*state)++;
        break;
    case DRESS_UP_STATE_WAIT_FADE_OUT:
        if (IsPaletteFadeFinished()) {
            *state = DRESS_UP_STATE_INIT;
            work->uiState = DRESS_UP_UI_EXIT;
            done = TRUE;
            DressUp_StopBackdropAnim(&work->backdropAnim);
        }
        break;
    }
    DressUp_DrawFrame(work);
    return done;
}

BOOL DressUp_Exit(OverlayManager *man, int *state) {
    DressUpWork *work = OverlayManager_GetData(man);
    DressUpArgs *args = OverlayManager_GetArgs(man);

    if (work->saveRequested == TRUE) {
        GameStats_AddScore(args->gameStats, SCORE_EVENT_POKEMON_DRESSED);
        DressUp_SaveDesign(args->fashionEntry, &work->canvas, args->profile);
    }
    if (args->savedResult != NULL) {
        if (work->saveRequested == TRUE) {
            *args->savedResult = TRUE;
        } else {
            *args->savedResult = FALSE;
        }
    }
    if (args->menuInputStatePtr != NULL) {
        MenuInputStateMgr_SetState(args->menuInputStatePtr, work->menuInputState);
    }
    YesNoPrompt_Destroy(work->yesNoPrompt);
    WindowArray_Delete(work->msgWindow, 1);
    DressUp_FreeHud(work);
    DressUpDrag_Destroy(&work->touch);
    DressUp_FreeButtons(work);
    DressUp_FreeFrameBg(work);
    DressUp_FreeCanvas(work);
    DressUp_FreeTray(work);
    DressUpGfx_FreeSlotList(work->slotList);
    work->slotList = NULL;
    DressUpNodePool_Free(&work->nodePool);
    DressUp_FreeGraphics(work);
    Main_SetVBlankIntrCB(NULL, NULL);
    HBlankInterruptDisable();
    if (GF_TouchpadStopSampling() != TRUE) {
        GF_AssertFail();
    }
    OverlayManager_FreeData(man);
    Heap_Destroy(HEAP_ID_13);
    Heap_Destroy(HEAP_ID_14);
    return TRUE;
}

void DressUp_InitGraphics(DressUpWork *work) {
    DressUpGfxResources resources;

    DressUpGfx_InitDisplay();
    DressUp_InitGfxCore(&work->gfx);
    DressUp_LoadRawResTables(&work->gfx, &resources);
    DressUpGfx_UploadRawResTables(&work->gfx, &resources);
    DressUpGfx_DestroyRawResTables(&resources);
}

void DressUp_DrawFrame(DressUpWork *work) {
    DressUp_DrawFrame3D(&work->gfx);
}

void DressUp_FreeGraphics(DressUpWork *work) {
    DressUpGfx_FreeAllTexAndPltt(&work->gfx);
    DressUp_FreeGfxCore(&work->gfx);
    DressUpGfx_ResetDisplay();
}

void DressUp_InitCanvas(DressUpWork *work, Pokemon *mon, int limit, BOOL altMonPic) {
    DressUpCanvasTemplate template;
    PokepicTemplate pokepicTemplate;

    template.graphicsManager = work->gfx.graphicsManager;
    template.images = work->gfx.images;
    template.palettes = work->gfx.palettes;
    template.paletteSlots = work->gfx.monPicAttrs;
    template.pokepicManager = work->gfx.pokepicManager;
    {
        // bgConfig is loaded before limit is stored
        BgConfig *bgConfig = work->gfx.bgConfig;
        template.limit = limit;
        template.bgConfig = bgConfig;
    }
    template.slotList = work->slotList;
    template.nodePool = &work->nodePool;
    DressUpCanvas_Init(&work->canvas, &template);
    if (altMonPic == FALSE) {
        DressUpCanvas_CreateMonPic(&work->canvas, mon, &pokepicTemplate, HEAP_ID_14);
    } else {
        DressUpCanvas_CreateMonPicEx(&work->canvas, mon, &pokepicTemplate, HEAP_ID_14);
    }
    DressUp_LoadMonHitMap(&work->gfx, &pokepicTemplate);
    DressUpCanvas_RefreshPriorities(&work->canvas);
    DressUpCanvas_SetBackdrop(&work->canvas, 0, HEAP_ID_14);
}

void DressUp_FreeCanvas(DressUpWork *work) {
    DressUpCanvas_FreeBackdrop(&work->canvas);
    DressUpCanvas_ClearObjects(&work->canvas);
    DressUpCanvas_Destroy(&work->canvas);
}

// fills the tray with every owned accessory (once per item owned) and every owned backdrop
void DressUp_InitTray(DressUpWork *work) {
    DressUpTrayTemplate template;
    int i, j, n;

    template.graphicsManager = work->gfx.graphicsManager;
    template.images = work->gfx.images;
    template.palettes = work->gfx.palettes;
    template.paletteSlots = work->gfx.monPicAttrs;
    template.bgConfig = work->gfx.bgConfig;
    template.caseTable = work->caseTable;
    template.slotList = work->slotList;
    template.nodePool = &work->nodePool;
    template.pageCounts[0] = 14;
    template.pageCounts[1] = 2;
    template.pageCounts[2] = 1;
    DressUpTray_Init(&work->tray, &template);
    DressUpTray_SetPageImmediate(&work->tray, 0, 0);
    for (i = 0; i < DRESS_UP_ACCESSORY_NUM; i++) {
        n = DressUp_GetAccessoryCount(work->caseTable, i);
        for (j = 0; j < n; j++) {
            DressUpTray_AddItem(&work->tray, 0, i);
        }
    }
    for (i = 0; i < DRESS_UP_BACKDROP_NUM; i++) {
        n = DressUp_GetBackdropByOrder(work->caseTable, i);
        if (n < DRESS_UP_BACKDROP_NUM) {
            DressUpTray_AddItem(&work->tray, 1, n);
        }
    }
    DressUpTray_RefreshPriorities(&work->tray);
}

void DressUp_FreeTray(DressUpWork *work) {
    DressUpTray_FreeTabBg(&work->tray);
    DressUpTray_ClearObjects(&work->tray);
    DressUpTray_Destroy(&work->tray);
}

void DressUp_RestoreEditBgLayout(DressUpWork *work) {
    G2_SetBG0Priority(1);
    G2_SetBG1Priority(0);
    G2_SetBG2Priority(2);
    G2_SetBG3Priority(3);
    BgSetPosTextAndCommit(work->gfx.bgConfig, GF_BG_LYR_MAIN_3, BG_POS_OP_SET_Y, -16);
    DressUpTray_LoadTabBg(&work->tray, 0);
    DressUpTray_SlideInTabBg(&work->tray, 0, 2, 0, NULL);
}

void DressUp_VBlankCB(void *arg) {
    DressUp_VBlank(arg);
}

// frame BG: 0 while editing, 1 once DONE is pressed
void DressUp_LoadFrameBg(DressUpWork *work, int frame) {
    DressUpBgScreenTemplate template;

    template.bgConfig = work->gfx.bgConfig;
    template.narcId = NARC_a_0_2_6;
    template.charFileId = 2 * frame + 0x79;
    template.plttFileId = 0x85;
    template.screenFileId = 2 * frame + 0x7A;
    template.x = 0;
    template.y = 0;
    template.layer = GF_BG_LYR_MAIN_1;
    template.plttCount = 1;
    template.plttSlot = 2;
    template.charOffset = 0;
    template.heapID = HEAP_ID_14;
    DressUpBgScreen_LoadFromTemplate(&work->frameBg, &template);
}

void DressUp_FreeFrameBg(DressUpWork *work) {
    DressUpBgScreen_Unload(&work->frameBg);
}

void DressUp_InitButtons(DressUpWork *work) {
    DressUpButtonPanel_Create(&work->buttonPanel, &work->gfx, &work->menuInputState);
    DressUpButtonPanel_SetCallback(&work->buttonPanel, 0, DressUp_OnNextPageButton, work, 0);
    DressUpButtonPanel_SetCallback(&work->buttonPanel, 1, DressUp_OnPrevPageButton, work, 0);
    DressUpButtonPanel_SetCallback(&work->buttonPanel, 2, DressUp_OnAccessoryTabButton, work, 0);
    DressUpButtonPanel_SetCallback(&work->buttonPanel, 3, DressUp_OnBackdropTabButton, work, 0);
    DressUpButtonPanel_SetCallback(&work->buttonPanel, 4, DressUp_OnDoneButton, work, 0);
}

void DressUp_FreeButtons(DressUpWork *work) {
    DressUpButtonPanel_Destroy(&work->buttonPanel, &work->gfx);
}

void DressUp_UpdateButtons(DressUpWork *work) {
    DressUpButtonPanel_Update(&work->buttonPanel);
}

void DressUp_ResetButtons(DressUpWork *work) {
    DressUpButtonPanel_ResetButtons(&work->buttonPanel);
}

void DressUp_OnDoneButton(void *button, void *arg) {
    DressUpWork *work = arg;
    if (DressUpTray_IsIdle(&work->tray)) {
        work->uiState = DRESS_UP_UI_DONE;
    }
}

void DressUp_OnNextPageButton(void *button, void *arg) {
    DressUpWork *work = arg;
    int group = DressUpTray_GetGroup(&work->tray);
    DressUpTray_CyclePage(&work->tray, group, FALSE);
}

void DressUp_OnPrevPageButton(void *button, void *arg) {
    DressUpWork *work = arg;
    int group = DressUpTray_GetGroup(&work->tray);
    DressUpTray_CyclePage(&work->tray, group, TRUE);
}

void DressUp_OnAccessoryTabButton(void *button, void *arg) {
    DressUpWork *work = arg;
    int page;

    if (work->uiState != DRESS_UP_UI_ACCESSORIES) {
        page = DressUpTray_GetGroupPage(&work->tray, 0);
        DressUpTray_SetPageImmediate(&work->tray, 0, page);
        work->uiState = DRESS_UP_UI_ACCESSORIES;
    }
}

void DressUp_OnBackdropTabButton(void *button, void *arg) {
    DressUpWork *work = arg;
    int page;

    if (work->uiState != DRESS_UP_UI_BACKDROPS) {
        page = DressUpTray_GetGroupPage(&work->tray, 1);
        DressUpTray_SetPageImmediate(&work->tray, 1, page);
        work->uiState = DRESS_UP_UI_BACKDROPS;
    }
}

void DressUp_InitHud(DressUpWork *work, Options *options) {
    DressUpHudTemplate template = { 0 };

    template.bgConfig = work->gfx.bgConfig;
    template.spriteList = work->gfx.spriteList;
    template.resMans = &work->gfx.charResMan;
    template.options = options;
    template.countMax = 10;
    template.narc = work->gfx.narc;
    DressUpHud_Init(&work->hud, &template, 15);
}

void DressUp_FreeHud(DressUpWork *work) {
    DressUpHud_Free(&work->hud);
}

void DressUp_StartFinishTransition(DressUpWork *work, u32 *done) {
    DressUpTransition *env = SysTask_GetData(CreateSysTaskAndEnvironment((SysTaskFunc)DressUp_FinishTransitionTask, sizeof(DressUpTransition), 10, HEAP_ID_13));
    env->work = work;
    env->done = done;
    env->counter = 0;
    env->state = 0;
}

// DONE: tray away, fade, slide the canvas into the middle, "Is your Pokemon all dressed up?"
void DressUp_FinishTransitionTask(SysTask *task, DressUpTransition *env) {
    switch (env->state) {
    case 0:
        if (!DressUpTray_AnimateToPage(&env->work->tray, 3, 0)) {
            GF_AssertFail();
        }
        env->state++;
        break;
    case 1:
        if (DressUpTray_IsIdle(&env->work->tray)) {
            env->state++;
        }
        break;
    case 2:
        StartBrightnessTransition(8, -16, 0, (GXBlendPlaneMask)(GX_BLEND_PLANEMASK_BG1 | GX_BLEND_PLANEMASK_BG3), SCREEN_MASK_MAIN);
        env->state++;
        break;
    case 3:
        if (IsBrightnessTransitionActive(SCREEN_MASK_MAIN)) {
            env->state++;
        }
        break;
    case 4:
        DressUpButtonPanel_MoveBy(&env->work->buttonPanel, 0, 8);
        env->counter++;
        if (env->counter >= 8) {
            env->counter = 0;
            env->state++;
        }
        break;
    case 5:
        if (DressUp_SlideCanvasStep(env, -8, 5, 8)) {
            env->counter = 0;
            env->state++;
        }
        break;
    case 6:
        DressUp_LoadFrameBg(env->work, 1);
        ScheduleSetBgPosText(env->work->gfx.bgConfig, GF_BG_LYR_MAIN_1, BG_POS_OP_SET_Y, 0);
        DressUp_ShowDressedUpPrompt(env->work);
        env->state++;
        break;
    case 7:
        StartBrightnessTransition(8, 0, -16, (GXBlendPlaneMask)(GX_BLEND_PLANEMASK_BG1 | GX_BLEND_PLANEMASK_BG3), SCREEN_MASK_MAIN);
        env->state++;
        break;
    case 8:
        if (IsBrightnessTransitionActive(SCREEN_MASK_MAIN)) {
            env->state++;
        }
        break;
    case 9:
        *env->done = TRUE;
        DestroySysTaskAndEnvironment(task);
        break;
    }
}

void DressUp_StartReturnTransition(DressUpWork *work, u32 *done) {
    DressUpTransition *env = SysTask_GetData(CreateSysTaskAndEnvironment((SysTaskFunc)DressUp_ReturnTransitionTask, sizeof(DressUpTransition), 10, HEAP_ID_13));
    env->work = work;
    env->done = done;
    env->counter = 0;
    env->state = 0;
}

// "no" to "all dressed up?": back to editing (reverse of the finish transition)
void DressUp_ReturnTransitionTask(SysTask *task, DressUpTransition *env) {
    switch (env->state) {
    case 0:
        StartBrightnessTransition(8, -16, 0, (GXBlendPlaneMask)(GX_BLEND_PLANEMASK_BG1 | GX_BLEND_PLANEMASK_BG3), SCREEN_MASK_MAIN);
        env->state++;
        break;
    case 1:
        if (IsBrightnessTransitionActive(SCREEN_MASK_MAIN)) {
            env->state++;
        }
        break;
    case 2:
        DressUp_HidePrompt(env->work);
        DressUp_RestoreEditBgLayout(env->work);
        DressUp_ResetButtons(env->work);
        DressUp_LoadFrameBg(env->work, 0);
        ScheduleSetBgPosText(env->work->gfx.bgConfig, GF_BG_LYR_MAIN_1, BG_POS_OP_SET_Y, -40);
        env->state++;
        break;
    case 3:
        if (DressUp_SlideCanvasStep(env, 8, -5, 8)) {
            env->counter = 0;
            env->state++;
        }
        break;
    case 4:
        DressUpButtonPanel_MoveBy(&env->work->buttonPanel, 0, -8);
        env->counter++;
        if (env->counter >= 8) {
            env->counter = 0;
            env->state++;
        }
        break;
    case 5:
        StartBrightnessTransition(8, 0, -16, (GXBlendPlaneMask)(GX_BLEND_PLANEMASK_BG1 | GX_BLEND_PLANEMASK_BG3), SCREEN_MASK_MAIN);
        env->state++;
        break;
    case 6:
        if (IsBrightnessTransitionActive(SCREEN_MASK_MAIN)) {
            env->state++;
        }
        break;
    case 7:
        if (!DressUpTray_AnimateToPage(&env->work->tray, 0, 0)) {
            GF_AssertFail();
        }
        env->state++;
        break;
    case 8:
        if (DressUpTray_IsIdle(&env->work->tray)) {
            env->state++;
        }
        break;
    case 9:
        *env->done = TRUE;
        DestroySysTaskAndEnvironment(task);
        break;
    }
}

// writes the mon, the trainer, the accessories (those on the mon first) and the backdrop to the fashion entry
void DressUp_SaveDesign(SaveFashionDataSub *entry, DressUpCanvas *canvas, PlayerProfile *profile) {
    DressUpNode *list;
    DressUpNode *node;
    int count;

    FashionEntry_Clear(entry);
    FashionEntry_SetMon(entry, canvas->monPic.mon, &canvas->monPic);
    if (profile != NULL) {
        String *name = PlayerProfile_GetPlayerName_NewString(profile, HEAP_ID_13);
        FashionEntry_SetMonSecondText(entry, name, PlayerProfile_GetTrainerGender(profile));
        String_Delete(name);
    }
    count = 0;
    node = canvas->monList.next;
    list = &canvas->monList;
    for (; node != list; node = node->next) {
        if (node->mode == 0) {
            FashionEntry_PlaceItem(entry, node->object, count);
            count++;
        }
    }
    list = &canvas->looseList;
    for (node = canvas->looseList.next; node != list; node = node->next) {
        if (node->mode == 0) {
            FashionEntry_PlaceItem(entry, node->object, count);
            count++;
        }
    }
    FashionEntry_SetBackdrop(entry, (u8)canvas->backdropId);
    FashionEntry_MarkSaved(entry);
}

// moves the canvas objects and its two BGs by (dx, dy); TRUE after `frames` steps
BOOL DressUp_SlideCanvasStep(DressUpTransition *env, int dx, int dy, int frames) {
    if (env->counter < 8) {
        DressUpCanvas_MoveAll(&env->work->canvas, dx, dy);
    }
    if (env->counter >= 1) {
        ScheduleSetBgPosText(env->work->gfx.bgConfig, GF_BG_LYR_MAIN_2, BG_POS_OP_SUB_X, dx);
        ScheduleSetBgPosText(env->work->gfx.bgConfig, GF_BG_LYR_MAIN_1, BG_POS_OP_SUB_X, dx);
        ScheduleSetBgPosText(env->work->gfx.bgConfig, GF_BG_LYR_MAIN_2, BG_POS_OP_SUB_Y, dy);
        ScheduleSetBgPosText(env->work->gfx.bgConfig, GF_BG_LYR_MAIN_1, BG_POS_OP_SUB_Y, dy);
    }
    env->counter++;
    if (env->counter > frames) {
        return TRUE;
    }
    return FALSE;
}

void DressUp_ShowDressedUpPrompt(DressUpWork *work) {
    YesNoPromptTemplate template;

    BgClearTilemapBufferAndCommit(work->gfx.bgConfig, GF_BG_LYR_MAIN_3);
    BgSetPosTextAndCommit(work->gfx.bgConfig, GF_BG_LYR_MAIN_3, BG_POS_OP_SET_X, 0);
    BgSetPosTextAndCommit(work->gfx.bgConfig, GF_BG_LYR_MAIN_3, BG_POS_OP_SET_Y, 0);
    MI_CpuFill8(&template, 0, sizeof(YesNoPromptTemplate));
    template.bgConfig = work->gfx.bgConfig;
    template.bgId = GF_BG_LYR_MAIN_3;
    template.tileStart = 0;
    template.plttSlot = 5;
    template.x = 25;
    template.y = 4;
    template.ignoreTouchFlag = work->menuInputState;
    YesNoPrompt_InitFromTemplate(work->yesNoPrompt, &template);
    DressUp_CreatePromptMsgWindow(work, msg_0215_00001);
    G2_SetBG0Priority(2);
    G2_SetBG1Priority(1);
    G2_SetBG2Priority(3);
    G2_SetBG3Priority(0);
}

void DressUp_HidePrompt(DressUpWork *work) {
    YesNoPrompt_Reset(work->yesNoPrompt);
    DressUp_RemovePromptMsgWindow(work);
    BgClearTilemapBufferAndCommit(work->gfx.bgConfig, GF_BG_LYR_MAIN_3);
}

int DressUp_HandleDressedUpPrompt(DressUpWork *work) {
    int result = YesNoPrompt_HandleInput(work->yesNoPrompt);
    switch (result) {
    case YESNORESPONSE_WAIT:
        return DRESS_UP_UI_DRESSED_UP;
    case YESNORESPONSE_YES:
        result = DRESS_UP_UI_DRESSED_YES;
        break;
    case YESNORESPONSE_NO:
        result = DRESS_UP_UI_DRESSED_NO;
        break;
    }
    work->menuInputState = YesNoPrompt_IsInTouchMode(work->yesNoPrompt);
    return result;
}

void DressUp_CreateMsgWindow(DressUpWork *work, int msgId, u8 x, u8 y, u8 width, u8 height) {
    MsgData *msgData;
    String *string;
    u32 frame = Options_GetFrame(work->options);
    LoadFontPal1(GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_7_OFFSET, HEAP_ID_14);
    AddWindowParameterized(work->gfx.bgConfig, work->msgWindow, GF_BG_LYR_MAIN_3, x, y, width, height, 7, 0x5A);
    FillWindowPixelBuffer(work->msgWindow, 15);
    LoadUserFrameGfx2(work->gfx.bgConfig, GF_BG_LYR_MAIN_3, 0x3C, 8, frame, HEAP_ID_14);
    DrawFrameAndWindow2(work->msgWindow, FALSE, 0x3C, 8);
    msgData = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, NARC_msg_msg_0215_bin, HEAP_ID_13);
    string = NewString_ReadMsgData(msgData, msgId);
    AddTextPrinterParameterizedWithColor(work->msgWindow, 1, string, 0, 0, TEXT_SPEED_INSTANT, MAKE_TEXT_COLOR(1, 2, 15), NULL);
    String_Delete(string);
    DestroyMsgData(msgData);
    CopyWindowToVram(work->msgWindow);
}

void DressUp_PrintMsgWindow(DressUpWork *work, int msgId) {
    MsgData *msgData;
    String *raw;
    String *expanded;
    FillWindowPixelBuffer(work->msgWindow, 15);
    msgData = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, NARC_msg_msg_0215_bin, HEAP_ID_13);
    raw = NewString_ReadMsgData(msgData, msgId);
    expanded = String_New(0x100, HEAP_ID_13);
    StringExpandPlaceholders(work->msgFormat, expanded, raw);
    AddTextPrinterParameterizedWithColor(work->msgWindow, 1, expanded, 0, 0, TEXT_SPEED_INSTANT, MAKE_TEXT_COLOR(1, 2, 15), NULL);
    String_Delete(expanded);
    String_Delete(raw);
    DestroyMsgData(msgData);
    CopyWindowToVram(work->msgWindow);
}

void DressUp_RemoveMsgWindow(DressUpWork *work) {
    ClearWindowTilemapAndCopyToVram(work->msgWindow);
    RemoveWindow(work->msgWindow);
}

void DressUp_CreatePromptMsgWindow(DressUpWork *work, int msgId) {
    DressUp_CreateMsgWindow(work, msgId, 2, 1, 27, 2);
}

void DressUp_PrintPromptMsgWindow(DressUpWork *work, int msgId) {
    DressUp_PrintMsgWindow(work, msgId);
}

void DressUp_RemovePromptMsgWindow(DressUpWork *work) {
    DressUp_RemoveMsgWindow(work);
}

void DressUp_UpdateCountMeter(DressUpWork *work) {
    int count = DressUpCanvas_GetCount(&work->canvas);
    DressUpHud_SetAccessoryCount(&work->hud, count);
}

// The editor's save prompt, then the canvas (the stage) shared by the Dress-Up editor and the Accessory Portrait:
// backdrop, the mon and the accessories on it, kept in two lists (accessories touching the mon move with it).

void DressUp_ShowSavePrompt(DressUpWork *work) {
    YesNoPromptTemplate template;

    MI_CpuFill8(&template, 0, sizeof(YesNoPromptTemplate));
    template.bgConfig = work->gfx.bgConfig;
    template.bgId = GF_BG_LYR_MAIN_3;
    template.tileStart = 0;
    template.plttSlot = 5;
    template.x = 25;
    template.y = 4;
    template.ignoreTouchFlag = work->menuInputState;
    YesNoPrompt_Reset(work->yesNoPrompt);
    YesNoPrompt_InitFromTemplate(work->yesNoPrompt, &template);
    DressUp_PrintPromptMsgWindow(work, msg_0215_00002);
    G2_SetBG0Priority(2);
    G2_SetBG1Priority(1);
    G2_SetBG2Priority(3);
    G2_SetBG3Priority(0);
}

int DressUp_HandleSavePrompt(DressUpWork *work) {
    int result = YesNoPrompt_HandleInput(work->yesNoPrompt);
    switch (result) {
    case YESNORESPONSE_WAIT:
        return DRESS_UP_UI_SAVE;
    case YESNORESPONSE_YES:
        result = DRESS_UP_UI_SAVE_YES;
        break;
    case YESNORESPONSE_NO:
        result = DRESS_UP_UI_SAVE_NO;
        break;
    }
    work->menuInputState = YesNoPrompt_IsInTouchMode(work->yesNoPrompt);
    return result;
}

// point `index` of a 4x4 grid over the rectangle, column by column
void DressUpCanvas_GetRectSamplePoint(u32 top, u32 bottom, u32 left, u32 right, u32 index, u32 *outX, u32 *outY) {
    u32 dx = (right - left) / 3;
    u32 x1 = left + dx;
    u32 x2 = left + dx * 2;
    u32 dy = (bottom - top) / 3;
    u32 y1 = top + dy;
    u32 y2 = top + dy * 2;
    switch (index) {
    case 0:
        *outX = left;
        *outY = top;
        return;
    case 1:
        *outX = left;
        *outY = y1;
        return;
    case 2:
        *outX = left;
        *outY = y2;
        return;
    case 3:
        *outX = left;
        *outY = bottom;
        return;
    case 4:
        *outX = x1;
        *outY = top;
        return;
    case 5:
        *outX = x1;
        *outY = y1;
        return;
    case 6:
        *outX = x1;
        *outY = y2;
        return;
    case 7:
        *outX = x1;
        *outY = bottom;
        return;
    case 8:
        *outX = x2;
        *outY = top;
        return;
    case 9:
        *outX = x2;
        *outY = y1;
        return;
    case 10:
        *outX = x2;
        *outY = y2;
        return;
    case 11:
        *outX = x2;
        *outY = bottom;
        return;
    case 12:
        *outX = right;
        *outY = top;
        return;
    case 13:
        *outX = right;
        *outY = y1;
        return;
    case 14:
        *outX = right;
        *outY = y2;
        return;
    case 15:
        *outX = right;
        *outY = bottom;
        return;
    }
}

void DressUpCanvas_Init(DressUpCanvas *canvas, const DressUpCanvasTemplate *template) {
    canvas->graphicsManager = template->graphicsManager;
    canvas->images = template->images;
    canvas->palettes = template->palettes;
    canvas->paletteSlots = template->paletteSlots;
    canvas->pokepicManager = template->pokepicManager;
    canvas->bgConfig = template->bgConfig;
    canvas->slotList = template->slotList;
    canvas->backdropId = 0;
    canvas->nodePool = template->nodePool;
    canvas->looseList.next = &canvas->looseList;
    canvas->looseList.prev = &canvas->looseList;
    canvas->monList.next = &canvas->monList;
    canvas->monList.prev = &canvas->monList;
    canvas->count = 0;
    canvas->limit = template->limit;
    DressUpList_InsertAfter(DressUpNodePool_Acquire(canvas->nodePool, &canvas->monPic, 3), &canvas->monList);
}

void DressUpCanvas_Destroy(DressUpCanvas *canvas) {
    DressUpCanvas_FreeMonPic(canvas);
    DressUpCanvas_ClearObjects(canvas);
    memset(canvas, 0, sizeof(DressUpCanvas));
}

void AccessoryPortrait_CreateCanvasMonPic(DressUpCanvas *canvas, Pokemon *mon, PokepicTemplate *template, int x, int y, int priority, enum HeapID heapID) {
    DressUpMon_Create(&canvas->monPic, canvas->pokepicManager, mon, template, heapID);
    DressUpMon_SetPriority(&canvas->monPic, priority);
    DressUpMon_SetCenterPos(&canvas->monPic, x, y);
}

void DressUpCanvas_CreateMonPic(DressUpCanvas *canvas, Pokemon *mon, PokepicTemplate *template, enum HeapID heapID) {
    DressUpMon_Create(&canvas->monPic, canvas->pokepicManager, mon, template, heapID);
}

void DressUpCanvas_CreateMonPicEx(DressUpCanvas *canvas, Pokemon *mon, PokepicTemplate *template, enum HeapID heapID) {
    DressUpMon_CreateEx(&canvas->monPic, canvas->pokepicManager, mon, template, heapID, 1);
}

void DressUpCanvas_AddToMonList(DressUpCanvas *canvas, DressUpNode *node) {
    DressUpList_InsertAfter(node, canvas->monList.prev);
}

void DressUpCanvas_RemoveNode(DressUpNode *node) {
    DressUpList_Remove(node);
}

void DressUpCanvas_FreeMonPic(DressUpCanvas *canvas) {
    DressUpMon_Delete(&canvas->monPic);
}

BOOL AccessoryPortrait_PlaceCanvasAccessory(DressUpCanvas *canvas, int accessory, int x, int y, int priority) {
    if (canvas->count < canvas->limit) {
        DressUpSlotTemplate template;
        DressUpSlot *slot;
        template.list = canvas->slotList;
        template.graphicsManager = canvas->graphicsManager;
        template.image = canvas->images[accessory];
        template.palette = *canvas->palettes;
        template.x = x;
        template.y = y;
        template.id = accessory;
        template.paletteSlot = canvas->paletteSlots[accessory];
        slot = DressUpGfx_AddSlotObject(&template);
        DressUpGfx_SetSlotPriority(slot, priority);
        DressUpList_InsertAfter(DressUpNodePool_Acquire(canvas->nodePool, slot, 0), &canvas->looseList);
        canvas->count++;
        return TRUE;
    }
    return FALSE;
}

BOOL DressUpCanvas_TryAddAccessory(DressUpCanvas *canvas, DressUpNode *node, NNSG2dCharacterData **hitMaps) {
    if (canvas->count < canvas->limit) {
        DressUpCanvas_LinkByMonOverlap(canvas, node, TRUE, hitMaps);
        canvas->count++;
        return TRUE;
    }
    return FALSE;
}

void DressUpCanvas_RemoveAccessory(DressUpCanvas *canvas, DressUpNode *node) {
    DressUpList_Remove(node);
    canvas->count--;
    if (canvas->count < 0) {
        GF_AssertFail();
    }
}

void DressUpCanvas_ClearObjects(DressUpCanvas *canvas) {
    DressUpList_Clear(&canvas->looseList);
    DressUpList_Clear(&canvas->monList);
    canvas->count = 0;
}

void DressUpCanvas_MoveAll(DressUpCanvas *canvas, int dx, int dy) {
    DressUpObj_MoveListBy(&canvas->looseList, dx, dy);
    DressUpObj_MoveListBy(&canvas->monList, dx, dy);
}

void DressUpCanvas_MoveMonGroup(DressUpCanvas *canvas, int dx, int dy) {
    DressUpObj_MoveListBy(&canvas->monList, dx, dy);
}

// moves everything from origin (x, y) to (newX, newY)
void DressUpCanvas_SetOrigin(DressUpCanvas *canvas, int x, int y, int newX, int newY) {
    int negY = -y;
    int negX = -x;
    DressUpObj_MoveListBy(&canvas->looseList, negX, negY);
    DressUpObj_MoveListBy(&canvas->looseList, newX, newY);
    DressUpObj_MoveListBy(&canvas->monList, negX, negY);
    DressUpObj_MoveListBy(&canvas->monList, newX, newY);
}

void DressUpCanvas_RefreshPriorities(DressUpCanvas *canvas) {
    DressUpCanvas_SetPriorities(canvas, -1);
}

// priorities count down from `priority`: the mon's list first, then (8 lower) the loose accessories
void DressUpCanvas_SetPriorities(DressUpCanvas *canvas, int priority) {
    DressUpNode *node = canvas->monList.next;
    DressUpNode *end = &canvas->monList;
    while (node != end) {
        if (node->mode == 0) {
            DressUpGfx_SetSlotPriority(node->object, priority);
        } else {
            DressUpMon_SetPriority(node->object, priority);
        }
        node = node->next;
        priority--;
    }

    node = canvas->looseList.next;
    end = &canvas->looseList;
    priority -= 8;
    while (node != end) {
        if (node->mode == 0) {
            DressUpGfx_SetSlotPriority(node->object, priority);
        } else {
            DressUpMon_SetPriority(node->object, priority);
        }
        node = node->next;
        priority--;
    }
}

BOOL DressUpCanvas_IsTouchHeldIn(DressUpCanvas *canvas) {
    TouchscreenHitbox hitbox;
    hitbox.rect.top = 18;
    hitbox.rect.bottom = 143;
    hitbox.rect.left = 138;
    hitbox.rect.right = 246;
    return TouchscreenHitbox_TouchHeldIsIn(&hitbox);
}

BOOL DressUpCanvas_IsPointIn(DressUpCanvas *canvas, u32 x, u32 y) {
    TouchscreenHitbox hitbox;
    hitbox.rect.top = 18;
    hitbox.rect.bottom = 143;
    hitbox.rect.left = 138;
    hitbox.rect.right = 246;
    return TouchscreenHitbox_PointIsIn(&hitbox, x, y);
}

// topmost object at (x, y): the mon's list first, then the loose accessories
DressUpNode *DressUpCanvas_FindObjectAt(DressUpCanvas *canvas, int *x, int *y, NNSG2dCharacterData **hitMaps) {
    DressUpNode *node = canvas->monList.next;

    struct {
        DressUpCanvas *canvas;
        DressUpNode *end;
    } it = { canvas, (DressUpNode *)canvas }; // codegen: a two-word local

    DressUpNode *end;
    it.end = &canvas->monList;
    if (node != it.end) {
        do {
            if (DressUpObj_HitTest(node, x, y, hitMaps) == TRUE) {
                return node;
            }
            node = node->next;
        } while (node != it.end);
    }
    node = canvas->looseList.next;
    end = &canvas->looseList;
    while (node != end) {
        if (DressUpObj_HitTest(node, x, y, hitMaps) == TRUE) {
            return node;
        }
        node = node->next;
    }
    return NULL;
}

void DressUpCanvas_SetBackdrop(DressUpCanvas *canvas, int backdrop, enum HeapID heapID) {
    DressUpBgScreenTemplate template;
    template.bgConfig = canvas->bgConfig;
    template.narcId = NARC_a_0_2_6;
    template.charFileId = backdrop * 4 + 0x86;
    template.plttFileId = backdrop * 4 + 0x87;
    template.screenFileId = backdrop * 4 + 0x88;
    template.x = 136;
    template.y = 16;
    template.layer = GF_BG_LYR_MAIN_2;
    template.plttCount = 1;
    template.plttSlot = 13;
    template.charOffset = 0;
    template.heapID = heapID;
    DressUpBgScreen_LoadFromTemplate(&canvas->backdrop, &template);
    canvas->backdropId = backdrop;
}

void DressUpCanvas_FreeBackdrop(DressUpCanvas *canvas) {
    DressUpBgScreen_Unload(&canvas->backdrop);
}

int DressUpCanvas_GetCount(DressUpCanvas *canvas) {
    return canvas->count;
}

// largest pushback (by magnitude) any object on the mon needs to get back inside the canvas
void DressUpCanvas_GetMaxOverflow(DressUpCanvas *canvas, int *outX, int *outY) {
    DressUpNode *node = canvas->monList.next;
    int maxX = 0;
    int maxY = 0;
    DressUpNode *end = &canvas->monList;

    while (node != end) {
        int x;
        int y;
        DressUpCanvas_GetPushback(node, &x, &y);
        if ((x < 0 ? -x : x) > (maxX < 0 ? -maxX : maxX)) {
            maxX = x;
        }
        if ((y < 0 ? -y : y) > (maxY < 0 ? -maxY : maxY)) {
            maxY = y;
        }
        node = node->next;
    }
    *outX = maxX;
    *outY = maxY;
}

#pragma opt_propagation off

// links the node into the mon's list if any of 16 sample points of its opaque area hits the mon, else into the
// loose list; at the front of the list or at the back
void DressUpCanvas_LinkByMonOverlap(DressUpCanvas *canvas, DressUpNode *node, BOOL atFront, NNSG2dCharacterData **hitMaps) {
    int x, y;
    int width, height;
    int left, top;
    u32 sampleX, sampleY;
    int right, bottom;
    DressUpCanvas *area;
    int marginTop;
    int y0;
    NNSG2dCharacterData *monHitMap;
    int x1;
    int y1;
    int x0;
    int marginLeft;
    int i;
    int hit;

    DressUpObj_GetPos(node, &x, &y);
    DressUpObj_GetSize(node, &width, &height);
    DressUpObj_GetMargins(node, &left, &right, &top, &bottom);
    monHitMap = hitMaps[DRESS_UP_IMAGE_MAX];
    hit = 0;
    x0 = x;
    marginLeft = left;
    x1 = x0 + width - right;
    y0 = y;
    y1 = y0 + height - bottom;
    marginTop = top;
    area = canvas;
    i = 0;
    do {
        DressUpCanvas_GetRectSamplePoint(y0 + marginTop, y1, x0 + marginLeft, x1, i, &sampleX, &sampleY);
        hit |= DressUpMon_HitTestPoint(&area->monPic, sampleX, sampleY, monHitMap);
    } while (++i < 16);
    if (hit) {
        if (atFront) {
            DressUpList_InsertAfter(node, &canvas->monList);
        } else {
            DressUpList_InsertAfter(node, canvas->monList.prev);
        }
    } else {
        if (atFront) {
            DressUpList_InsertAfter(node, &canvas->looseList);
        } else {
            DressUpList_InsertAfter(node, canvas->looseList.prev);
        }
    }
}

#pragma opt_propagation reset

// The tray of the Dress-Up editor: owned accessories (14 pages by category) and backdrops (2 pages of 9) as objects
// in per-page lists, the tab BG, and the page swap (old page slides out, new page slides in).

// how far the object sticks out of the canvas (0 if inside)
void DressUpCanvas_GetPushback(DressUpNode *node, int *outX, int *outY) {
    int width;
    int height;
    int x;
    int y;
    int left;
    int top;
    int right;
    int bottom;
    int overLeft;
    int overRight;
    int overTop;
    int overBottom;

    DressUpObj_GetSize(node, &width, &height);
    DressUpObj_GetPos(node, &x, &y);
    DressUpObj_GetMargins(node, &left, &right, &top, &bottom);
    overLeft = 138 - (x + left);
    overRight = x + width - right - 246;
    overTop = 18 - (y + top);
    overBottom = y + height - bottom - 143;
    if (overLeft > 0) {
        *outX = overLeft;
    } else if (overRight > 0) {
        *outX = -overRight;
    } else {
        *outX = 0;
    }
    if (overTop > 0) {
        *outY = overTop;
    } else if (overBottom > 0) {
        *outY = -overBottom;
    } else {
        *outY = 0;
    }
}

void DressUpTray_Init(DressUpTray *tray, const DressUpTrayTemplate *template) {
    tray->graphicsManager = template->graphicsManager;
    tray->images = template->images;
    tray->palettes = template->palettes;
    tray->paletteSlots = template->paletteSlots;
    tray->bgConfig = template->bgConfig;
    tray->slotList = template->slotList;
    tray->nodePool = template->nodePool;
    tray->caseTable = template->caseTable;
    DressUpTray_LoadTabBg(tray, 0);
    DressUpTray_InitPageLists(tray, template);
}

void DressUpTray_Destroy(DressUpTray *tray) {
    int i;
    DressUpTrayGroup *group;

    DressUpTray_ClearObjects(tray);
    i = 0;
    group = tray->groups;
    while (i < 4) {
        DressUpTray_FreePageTable(group);
        i++;
        group++;
    }
    memset(tray, 0, sizeof(DressUpTray));
}

// page of an item: accessories by category, backdrops 9 per page in fashion case order
int DressUpTray_GetItemPage(int mode, int id, u32 *caseTable) {
    switch (mode) {
    case 0:
        if (id <= 5) {
            return 0;
        }
        if (id <= 11) {
            return 1;
        }
        if (id <= 17) {
            return 2;
        }
        if (id <= 21) {
            return 3;
        }
        if (id <= 28) {
            return 4;
        }
        if (id <= 33) {
            return 5;
        }
        if (id <= 38) {
            return 6;
        }
        if (id <= 42) {
            return 7;
        }
        if (id <= 49) {
            return 8;
        }
        if (id <= 55) {
            return 9;
        }
        if (id <= 60) {
            return 10;
        }
        if (id <= 71) {
            return 11;
        }
        if (id <= 91) {
            return 12;
        }
        if (id <= 99) {
            return 13;
        }
        break;
    case 1:
        return DressUp_GetBackdropOrder(caseTable, id) / 9;
    case 2:
        mode = DressUp_GetBackdropOrder(caseTable, id) / 9;
        break;
    }
    return mode;
}

// accessories land at a random spot of the tray, backdrops on a 3x3 grid
void DressUpTray_GetItemSpawnPos(int mode, int id, int *outX, int *outY, int width, int height, u32 *caseTable) {
    switch (mode) {
    case 0:
        *outX = 10;
        *outY = 18;
        *outX += MTRandom() % (108 - width);
        *outY += MTRandom() % (125 - height);
        break;
    case 1:
    case 2:
        DressUpTray_GetGridPos(DressUp_GetBackdropOrder(caseTable, id), outX, outY);
        break;
    }
}

void DressUpTray_AddItem(DressUpTray *tray, int mode, int id) {
    DressUpSlotTemplate template;
    int x, y;
    int width, height;
    DressUpSlot *slot;
    DressUpNode *node;
    int imageIndex;
    int paletteIndex;
    int page;

    template.list = tray->slotList;
    template.id = id;
    template.graphicsManager = tray->graphicsManager;
    template.x = 0;
    template.y = 0;
    switch (mode) {
    case 0:
        imageIndex = id;
        paletteIndex = 0;
        template.paletteSlot = tray->paletteSlots[id];
        break;
    case 1:
        imageIndex = id;
        imageIndex += DRESS_UP_ACCESSORY_NUM;
        paletteIndex = id + 1;
        template.paletteSlot = 0;
        break;
    case 2:
        imageIndex = id;
        imageIndex += DRESS_UP_ACCESSORY_NUM;
        paletteIndex = id + 1;
        template.paletteSlot = 0;
        break;
    default:
        GF_AssertFail();
        break;
    }
    template.image = tray->images[imageIndex];
    template.palette = tray->palettes[paletteIndex];
    GF_ASSERT(template.image != NULL);
    GF_ASSERT(template.palette != NULL);
    page = DressUpTray_GetItemPage(mode, id, tray->caseTable);
    slot = DressUpGfx_AddSlotObject(&template);
    node = DressUpNodePool_Acquire(tray->nodePool, slot, mode);
    DressUpList_InsertAfter(node, tray->groups[mode].pages[page].prev);
    DressUpObj_GetSize(node, &width, &height);
    DressUpTray_GetItemSpawnPos(mode, id, &x, &y, width, height, tray->caseTable);
    DressUpObj_SetPos(node, x, y);
    DressUpTray_SyncObjectVisible(tray, slot, mode, page);
}

void DressUpTray_LinkObjectToPage(DressUpTray *tray, int mode, int page, DressUpNode *node) {
    DressUpList_InsertAfter(node, &tray->groups[mode].pages[page]);
    DressUpTray_SyncObjectVisible(tray, node->object, mode, page);
}

void DressUpTray_RemoveNode(DressUpNode *node) {
    DressUpList_Remove(node);
}

void DressUpTray_ClearObjects(DressUpTray *tray) {
    int i;
    int j;
    for (i = 0; i < 3; i++) {
        for (j = 0; j < tray->groups[i].pageCount; j++) {
            DressUpList_Clear(&tray->groups[i].pages[j]);
        }
    }
}

void DressUpTray_RefreshPriorities(DressUpTray *tray) {
    DressUpNode *node;
    int priority = -1;
    DressUpNode *head = DressUpTray_GetCurPageHead(tray);
    node = head->next;
    while (node != head) {
        if (node->mode <= 2) {
            DressUpGfx_SetSlotPriority(node->object, priority);
        }
        node = node->next;
        priority--;
    }
}

BOOL DressUpTray_AnimateToPage(DressUpTray *tray, int group, int page) {
    if (tray->pageSwapActive == FALSE) {
        int oldGroup = tray->group;
        DressUpTray_StartPageSwapTask(tray, oldGroup, tray->groups[oldGroup].curPage, group, page);
        tray->group = group;
        tray->groups[group].curPage = page;
        DressUpTray_RefreshPriorities(tray);
        return TRUE;
    }
    return FALSE;
}

// next / previous page of the group that has objects on it
int DressUpTray_CyclePage(DressUpTray *tray, int group, BOOL reverse) {
    int start = DressUpTray_GetGroupPage(tray, group);
    // codegen: the page count is re-read every time, through the tray shifted by the group
    volatile DressUpTray *shifted = (volatile DressUpTray *)((u8 *)tray + group * sizeof(DressUpTrayGroup));
    int i;
    for (i = 1; i < shifted->groups[0].pageCount; i++) {
        int page;
        if (reverse == FALSE) {
            page = (i + start) % shifted->groups[0].pageCount;
        } else {
            page = start - i;
            if (page < 0) {
                page += shifted->groups[0].pageCount;
            }
        }
        {
            DressUpNode *head = DressUpTray_GetPageHead(tray, group, page);
            if (head->next != head) {
                return DressUpTray_AnimateToPage(tray, group, page);
            }
        }
    }
    return 0;
}

void DressUpTray_SetPageImmediate(DressUpTray *tray, int group, int page) {
    DressUpTray_SetCurPageVisible(tray, FALSE);
    tray->group = group;
    tray->groups[group].curPage = page;
    DressUpTray_SetCurPageVisible(tray, TRUE);
    DressUpTray_RefreshPriorities(tray);
}

BOOL DressUpTray_IsTouchHeldIn(DressUpTray *tray) {
    TouchscreenHitbox hitbox;
    hitbox.rect.top = 18;
    hitbox.rect.bottom = 143;
    hitbox.rect.left = 10;
    hitbox.rect.right = 118;
    return TouchscreenHitbox_TouchHeldIsIn(&hitbox);
}

BOOL DressUpTray_IsPointIn(DressUpTray *tray, u32 x, u32 y) {
    TouchscreenHitbox hitbox;
    hitbox.rect.top = 18;
    hitbox.rect.bottom = 143;
    hitbox.rect.left = 10;
    hitbox.rect.right = 118;
    return TouchscreenHitbox_PointIsIn(&hitbox, x, y);
}

DressUpNode *DressUpTray_FindObjectAt(DressUpTray *tray, int *x, int *y, NNSG2dCharacterData **hitMaps) {
    DressUpNode *node;
    DressUpNode *head;
    head = DressUpTray_GetCurPageHead(tray);
    node = head->next;
    while (node != head) {
        if (DressUpObj_HitTest(node, x, y, hitMaps) == TRUE) {
            return node;
        }
        node = node->next;
    }
    return NULL;
}

void DressUpTray_LoadTabBg(DressUpTray *tray, int tab) {
    DressUpBgScreenTemplate template;
    template.bgConfig = tray->bgConfig;
    template.narcId = NARC_a_0_2_6;
    template.charFileId = tab * 2 + 0x81;
    template.plttFileId = 0x85;
    template.screenFileId = tab * 2 + 0x82;
    template.x = 8;
    template.y = 129;
    template.layer = GF_BG_LYR_MAIN_3;
    template.plttCount = 1;
    template.plttSlot = 2;
    template.charOffset = 0;
    template.heapID = HEAP_ID_14;
    DressUpBgScreen_LoadFromTemplate(&tray->tabBg, &template);
    tray->tabBgIndex = tab;
}

// loads the tab BG and slides it in; flags: 1 from the side, 2 from below
void DressUpTray_SlideInTabBg(DressUpTray *tray, int tab, u32 flags, int frames, BOOL *done) {
    DressUpBgScreenTemplate template;
    int dx, dy;
    template.bgConfig = tray->bgConfig;
    template.narcId = NARC_a_0_2_6;
    template.charFileId = tab * 2 + 0x81;
    template.plttFileId = 0x85;
    template.screenFileId = tab * 2 + 0x82;
    template.x = 8;
    template.y = 129;
    template.layer = GF_BG_LYR_MAIN_3;
    template.plttCount = 1;
    template.plttSlot = 2;
    template.charOffset = 0;
    template.heapID = HEAP_ID_14;
    dx = 0;
    if (flags & 1) {
        dx = 112;
    }
    if (flags & 2) {
        dy = 129;
    } else {
        dy = 0;
    }
    DressUp_StartBgSlide(&tray->tabBg, &template, dx, dy, frames, done);
    tray->tabBgIndex = tab;
}

void DressUpTray_FreeTabBg(DressUpTray *tray) {
    DressUpBgScreen_Unload(&tray->tabBg);
}

int DressUpTray_GetGroup(DressUpTray *tray) {
    if (tray == NULL) {
        GF_AssertFail();
    }
    return tray->group;
}

int DressUpTray_GetGroupPage(DressUpTray *tray, int group) {
    DressUpTrayGroup pages;
    if (tray == NULL) {
        GF_AssertFail();
    }
    pages = tray->groups[group];
    return pages.curPage;
}

void DressUpTray_MovePageObjects(DressUpTray *tray, int group, int page, int dx, int dy) {
    DressUpNode *head = DressUpTray_GetPageHead(tray, group, page);
    DressUpObj_MoveListBy(head, dx, dy);
}

BOOL DressUpTray_IsIdle(DressUpTray *tray) {
    return tray->pageSwapActive == FALSE;
}

// page lists per group: accessories, backdrops, group 2, group 3 (the empty tray while DONE)
void DressUpTray_InitPageLists(DressUpTray *tray, const DressUpTrayTemplate *template) {
    DressUpTray_AllocPageTable(&tray->groups[0], template->pageCounts[0]);
    tray->group = 0;
    DressUpTray_SetListVisible(tray->groups[0].pages, TRUE);
    DressUpTray_AllocPageTable(&tray->groups[1], template->pageCounts[1]);
    DressUpTray_AllocPageTable(&tray->groups[2], template->pageCounts[2]);
    DressUpTray_AllocPageTable(&tray->groups[3], 1);
}

void DressUpTray_SetListVisible(DressUpNode *head, BOOL visible) {
    DressUpNode *node = head->next;
    while (node != head) {
        if (node->mode <= 2) {
            DressUpGfx_SetSlotVisible(node->object, visible);
        }
        node = node->next;
    }
}

void DressUpTray_SetPageVisible(DressUpTray *tray, int group, int page, BOOL visible) {
    DressUpNode *head = DressUpTray_GetPageHead(tray, group, page);
    DressUpTray_SetListVisible(head, visible);
}

void DressUpTray_SetCurPageVisible(DressUpTray *tray, BOOL visible) {
    DressUpNode *head = DressUpTray_GetCurPageHead(tray);
    DressUpTray_SetListVisible(head, visible);
}

void DressUpTray_AllocPageTable(DressUpTrayGroup *group, int count) {
    int i;
    group->pages = Heap_Alloc(HEAP_ID_14, count * sizeof(DressUpNode));
    group->pageCount = count;
    group->curPage = 0;
    for (i = 0; i < group->pageCount; i++) {
        group->pages[i].next = &group->pages[i];
        group->pages[i].prev = &group->pages[i];
        DressUpTray_SetListVisible(&group->pages[i], FALSE);
    }
}

void DressUpTray_FreePageTable(DressUpTrayGroup *group) {
    int zero;
    u8 *bytes = (u8 *)group;
    Heap_Free(group->pages);
    zero = 0;
    group->pages = NULL;
    // clears the group byte by byte
    bytes[0] = zero;
    bytes[1] = zero;
    bytes[2] = zero;
    bytes[3] = zero;
    bytes[4] = zero;
    bytes[5] = zero;
    bytes[6] = zero;
    bytes[7] = zero;
    bytes[8] = zero;
    bytes[9] = zero;
    bytes[10] = zero;
    bytes[11] = zero;
}

DressUpNode *DressUpTray_GetCurPageHead(DressUpTray *tray) {
    DressUpTrayGroup pages = tray->groups[tray->group];
    return &pages.pages[pages.curPage];
}

DressUpNode *DressUpTray_GetPageHead(DressUpTray *tray, int group, int page) {
    DressUpTrayGroup pages = tray->groups[group];
    return &pages.pages[page];
}

int DressUpTray_CountPageObjects(DressUpTray *tray, int group, int page) {
    int count = 0;
    DressUpNode *head = DressUpTray_GetPageHead(tray, group, page);
    DressUpNode *node = head->next;
    while (node != head) {
        node = node->next;
        count++;
    }
    return count;
}

DressUpNode *DressUpTray_GetPageObject(DressUpTray *tray, int group, int page, int index) {
    DressUpNode *node;
    int i = 0;
    DressUpNode *head = DressUpTray_GetPageHead(tray, group, page);
    node = head->next;
    while (node != head) {
        if (i == index) {
            return node;
        }
        node = node->next;
        i++;
    }
    return NULL;
}

// an object is visible only on the shown page
void DressUpTray_SyncObjectVisible(DressUpTray *tray, DressUpSlot *slot, int group, int page) {
    if (tray->group != group || page != tray->groups[group].curPage) {
        DressUpGfx_SetSlotVisible(slot, FALSE);
    } else {
        DressUpGfx_SetSlotVisible(slot, TRUE);
    }
}

// backdrop grid: 3x3 cells of 24x32 with 8 pixel gaps
void DressUpTray_GetGridPos(int index, int *x, int *y) {
    int cell;
    int row;
    int column;
    cell = index % 9;
    row = cell / 3;
    column = cell % 3;
    *y = (row + 1) * 8 + row * 32;
    *y += 16;
    *x = (column + 1) * 8 + column * 24;
    *x += 8;
}

void DressUpTray_StartPageSwapTask(DressUpTray *tray, int fromGroup, int fromPage, int toGroup, int toPage) {
    DressUpPageSwap *swap = SysTask_GetData(CreateSysTaskAndEnvironment((SysTaskFunc)DressUpTray_PageSwapTask, sizeof(DressUpPageSwap), 0, HEAP_ID_13));
    swap->tray = tray;
    swap->fromGroup = fromGroup;
    swap->fromPage = fromPage;
    swap->toGroup = toGroup;
    swap->toPage = toPage;
    swap->state = 0;
    swap->fromLeft = DressUpTray_CountPageObjects(tray, fromGroup, fromPage);
    swap->toLeft = DressUpTray_CountPageObjects(tray, toGroup, toPage);
    swap->animCount = swap->fromLeft + swap->toLeft;
    swap->anims = Heap_Alloc(HEAP_ID_13, sizeof(DressUpSlideAnim) * swap->animCount);
    if (!swap->anims) {
        GF_AssertFail();
    }
    memset(swap->anims, 0, sizeof(DressUpSlideAnim) * swap->animCount);
    tray->pageSwapActive = TRUE;
}

// the new page waits 132 pixels up; the old page drops out one object per frame, then the new page drops in
// (two per frame) while the tab BG slides to the other tab
void DressUpTray_PageSwapTask(SysTask *task, DressUpPageSwap *swap) {
    int i;
    switch (swap->state) {
    case 0:
        DressUpTray_MovePageObjects(swap->tray, swap->toGroup, swap->toPage, 0, -132);
        DressUpTray_SetPageVisible(swap->tray, swap->toGroup, swap->toPage, TRUE);
        swap->tabSlideDone = FALSE;
        DressUpTray_SlideInTabBg(swap->tray, (swap->tray->tabBgIndex + 1) % 2, 2, 5, &swap->tabSlideDone);
        swap->counter = DressUp_DivFloorPlusOne(swap->fromLeft, 1);
        swap->state++;
        break;
    case 1:
        for (i = 0; i < swap->counter; i++) {
            if (swap->fromLeft - 1 >= 0) {
                swap->fromLeft--;
                DressUp_StartSlideAnim(DressUpTray_GetPageObject(swap->tray, swap->fromGroup, swap->fromPage, swap->fromLeft), swap->anims, swap->animCount);
            }
        }
        if (swap->fromLeft == 0) {
            swap->counter = DressUp_DivFloorPlusOne(swap->toLeft, 2);
            swap->state++;
        }
        break;
    case 2:
        for (i = 0; i < swap->counter; i++) {
            if (swap->toLeft - 1 >= 0) {
                swap->toLeft--;
                DressUp_StartSlideAnim(DressUpTray_GetPageObject(swap->tray, swap->toGroup, swap->toPage, swap->toLeft), swap->anims, swap->animCount);
            }
        }
        if (swap->toLeft == 0) {
            swap->state++;
            swap->counter = 0;
        }
        break;
    case 3:
        swap->counter++;
        if (swap->counter > 3 && swap->tabSlideDone) {
            swap->state++;
        }
        break;
    case 4:
        DressUpTray_SetPageVisible(swap->tray, swap->fromGroup, swap->fromPage, FALSE);
        DressUpTray_MovePageObjects(swap->tray, swap->fromGroup, swap->fromPage, 0, -132);
        swap->tray->pageSwapActive = FALSE;
        Heap_Free(swap->anims);
        DestroySysTaskAndEnvironment(task);
        return;
    default:
        GF_AssertFail();
        break;
    }
    DressUp_TickSlideAnims(swap->anims, swap->animCount);
}

void DressUp_StartSlideAnim(DressUpNode *node, DressUpSlideAnim *anims, int count) {
    DressUpSlideAnim *anim = DressUp_FindFreeSlideAnim(anims, count);
    anim->node = node;
    anim->remaining = 3;
    anim->delta = 44;
}

DressUpSlideAnim *DressUp_FindFreeSlideAnim(DressUpSlideAnim *anims, int count) {
    int i;
    i = 0;
    if (count > 0) {
        DressUpSlideAnim *anim = anims;
        do {
            if (anim->node == NULL) {
                return &anims[i];
            }
            i++;
            anim++;
        } while (i < count);
    }
    return NULL;
}

void DressUp_TickSlideAnims(DressUpSlideAnim *anims, int count) {
    int i;
    for (i = 0; i < count; i++) {
        if (anims->node != NULL) {
            DressUp_TickSlideAnim(anims);
        }
        anims++;
    }
}

void DressUp_TickSlideAnim(DressUpSlideAnim *anim) {
    int x, y;
    DressUpObj_GetPos(anim->node, &x, &y);
    y += anim->delta;
    DressUpObj_SetPos(anim->node, x, y);
    if (--anim->remaining <= 0) {
        // clears the slot byte by byte
        u8 *bytes = (u8 *)anim;
        bytes[0] = 0;
        bytes[1] = 0;
        bytes[2] = 0;
        bytes[3] = 0;
        bytes[4] = 0;
        bytes[5] = 0;
        bytes[6] = 0;
        bytes[7] = 0;
        bytes[8] = 0;
        bytes[9] = 0;
        bytes[10] = 0;
        bytes[11] = 0;
    }
}

// Touch handling of the Dress-Up editor: the touch dispatcher (new touch / held / released), the fashion case
// table, and the drag controller that picks objects up from the canvas or the tray and drops them again.

// ceil(value / divisor) for positive values that are not multiples, value / divisor + 1 otherwise
int DressUp_DivFloorPlusOne(int value, int divisor) {
    int d = divisor;
    int v = value;
    int rem = v % d;
    return (d - rem + v) / d;
}

void DressUpTouch_Reset(DressUpTouch *touch) {
    u8 *bytes = (u8 *)touch;
    u32 n = sizeof(DressUpTouch);
    while (n != 0) {
        *bytes++ = 0;
        n--;
    }
    touch->onTouch = DressUpTouch_DefaultHandler;
    touch->onRelease = DressUpTouch_DefaultHandler;
    touch->onHeld = DressUpTouch_DefaultHandler;
}

void DressUpTouch_Update(DressUpTouch *touch) {
    if (gSystem.touchNew != 0) {
        touch->onTouch(touch);
    } else if (gSystem.touchHeld != 0) {
        touch->onHeld(touch);
    } else if (touch->wasHeld != 0) {
        touch->onRelease(touch);
    }
    touch->lastX = gSystem.touchX;
    touch->lastY = gSystem.touchY;
    touch->wasHeld = gSystem.touchHeld;
}

void DressUpTouch_DefaultHandler(DressUpTouch *touch) {
    return;
}

// owned count per accessory, then the order of each owned backdrop (18 = not owned)
void DressUp_LoadFashionCaseTable(FashionCase *fashionCase, u32 *caseTable) {
    int i;
    for (i = 0; i < DRESS_UP_ACCESSORY_NUM; i++) {
        caseTable[i] = FashionCase_GetAccessoryCount(fashionCase, i);
    }
    for (i = 0; i < DRESS_UP_BACKDROP_NUM; i++) {
        caseTable[DRESS_UP_ACCESSORY_NUM + i] = DRESS_UP_BACKDROP_NUM;
    }
    for (i = 0; i < DRESS_UP_BACKDROP_NUM; i++) {
        int backdrop = FashionCase_GetWallpaperOrder(fashionCase, i);
        if (backdrop != DRESS_UP_BACKDROP_NUM) {
            caseTable[DRESS_UP_ACCESSORY_NUM + backdrop] = i;
        }
    }
}

u32 DressUp_GetAccessoryCount(u32 *caseTable, u32 accessory) {
    if (accessory >= DRESS_UP_ACCESSORY_NUM) {
        GF_AssertFail();
    }
    return caseTable[accessory];
}

u32 DressUp_GetBackdropByOrder(u32 *caseTable, u32 order) {
    u32 *entry = &caseTable[order];
    return entry[DRESS_UP_ACCESSORY_NUM];
}

int DressUp_GetBackdropOrder(u32 *caseTable, u32 backdrop) {
    int i;
    i = 0;
    do {
        if (backdrop == caseTable[DRESS_UP_ACCESSORY_NUM]) {
            return i;
        }
        i++;
        caseTable++;
    } while (i < DRESS_UP_BACKDROP_NUM);
    return i;
}

void DressUpDrag_Init(DressUpTouch *touch, DressUpCanvas *canvas, DressUpTray *tray, DressUpGfx *gfx, DressUpHud *hud, BOOL allowMonDrag) {
    DressUpDrag *drag;
    DressUpTouch_Reset(touch);
    drag = Heap_Alloc(HEAP_ID_13, sizeof(DressUpDrag));
    touch->drag = drag;
    memset(drag, 0, sizeof(DressUpDrag));
    drag = touch->drag;
    drag->canvas = canvas;
    drag->tray = tray;
    drag->gfx = gfx;
    drag->hud = hud;
    drag->allowMonDrag = allowMonDrag;
    touch->onTouch = DressUpDrag_OnTouchIdle;
    touch->onRelease = DressUpDrag_OnReleaseAccessory;
    touch->onHeld = DressUpDrag_OnHeldAccessory;
    touch->destroy = DressUpDrag_Destroy;
}

void DressUpDrag_Destroy(DressUpTouch *touch) {
    Heap_Free(touch->drag);
    DressUpTouch_Reset(touch);
}

// new touch: picks up the object under the stylus (canvas first, then the tray) and sets the drag callbacks
void DressUpDrag_OnTouchIdle(DressUpTouch *touch) {
    DressUpDrag *drag = touch->drag;
    int x, y;
    DressUpNode *node;
    int priority;
    if (DressUpCanvas_IsTouchHeldIn(drag->canvas)) {
        node = DressUpCanvas_FindObjectAt(drag->canvas, &x, &y, drag->gfx->hitMaps);
        if (node && node->mode == 3 && !drag->allowMonDrag) {
            node = NULL;
        }
        if (node) {
            if (!node->mode) {
                DressUpCanvas_RemoveAccessory(drag->canvas, node);
                priority = 0;
            } else {
                DressUpCanvas_RemoveNode(node);
                priority = DressUpMon_GetPriority(node->object);
                DressUpObj_GetPos(node, &drag->monX, &drag->monY);
            }
            DressUpCanvas_RefreshPriorities(drag->canvas);
            DressUpDrag_BeginDrag(drag, node, TRUE, x, y, priority);
            DressUp_EnableDragWindow();
        }
    } else if (DressUpTray_IsTouchHeldIn(drag->tray)) {
        node = DressUpTray_FindObjectAt(drag->tray, &x, &y, drag->gfx->hitMaps);
        if (node) {
            DressUpTray_RemoveNode(node);
            DressUpTray_RefreshPriorities(drag->tray);
            DressUpDrag_BeginDrag(drag, node, FALSE, x, y, 0);
            DressUp_EnableDragWindow();
        }
    }
    if (drag->selected) {
        int se = SEQ_SE_DP_BOX02;
        switch (drag->selected->mode) {
        case 0:
            touch->onRelease = DressUpDrag_OnReleaseAccessory;
            touch->onHeld = DressUpDrag_OnHeldAccessory;
            break;
        case 1:
            touch->onRelease = DressUpDrag_OnReleaseBackdrop;
            touch->onHeld = DressUpDrag_OnHeldBackdrop;
            DressUpDrag_ApplyBackdrop(touch);
            se = SEQ_SE_DP_SYARAAN;
            break;
        case 3:
            touch->onRelease = DressUpDrag_OnReleaseMon;
            touch->onHeld = DressUpDrag_OnHeldMon;
            break;
        }
        PlaySE(se);
        DressUp_PrintSelectedItemName(drag);
    }
}

void DressUpDrag_OnHeldBackdrop(DressUpTouch *touch) {
    return;
}

// a backdrop is applied on touch; on release its tray item goes back
void DressUpDrag_OnReleaseBackdrop(DressUpTouch *touch) {
    DressUpDrag *drag = touch->drag;
    if (drag->selected != NULL) {
        GF_ASSERT(drag->selected->mode == 1);
        DressUp_StartTweenToTray(drag, 4, drag->originX, drag->originY, drag->selected->mode, drag->trayPage);
        DressUpDrag_ClearSelection(drag);
    }
}

// dropped fully inside the canvas: placed (if there is room); else back to the tray
void DressUpDrag_OnReleaseAccessory(DressUpTouch *touch) {
    DressUpDrag *drag = touch->drag;
    int top;
    int bottom;
    int left;
    int right;
    int width;
    int height;
    int x;
    int y;
    int n;
    if (drag->selected != NULL) {
        DressUpDrag_GetSelectedVisibleRect(touch, &top, &bottom, &left, &right);
        n = DressUpCanvas_IsPointIn(drag->canvas, left, top);
        n += DressUpCanvas_IsPointIn(drag->canvas, right, top);
        n += DressUpCanvas_IsPointIn(drag->canvas, left, bottom);
        if (n + DressUpCanvas_IsPointIn(drag->canvas, right, bottom) >= 4) {
            if (!DressUpCanvas_TryAddAccessory(drag->canvas, drag->selected, drag->gfx->hitMaps)) {
                x = drag->originX;
                y = drag->originY;
                PlaySE(SEQ_SE_DP_HYUN2);
                DressUpHud_PrintMsgInstant(drag->hud, NARC_msgdata_msg, NARC_msg_msg_0215_bin, msg_0215_00003);
                DressUp_StartTweenToTray(drag, 4, x, y, drag->selected->mode, drag->trayPage);
            } else {
                DressUpCanvas_RefreshPriorities(drag->canvas);
                DressUp_DisableDragWindow();
                drag->dragging = FALSE;
                PlaySE(SEQ_SE_DP_BOX01);
            }
        } else {
            DressUpSlot *slot = drag->selected->object;
            DressUpDrag_GetSelectedRect(touch, &top, &bottom, &left, &right);
            n = DressUpTray_IsPointIn(drag->tray, left, top);
            if (n + DressUpTray_IsPointIn(drag->tray, right, bottom) < 2) {
                // dropped outside the tray: a random spot for items from the canvas, else where it came from
                if (drag->fromCanvas == TRUE) {
                    DressUpObj_GetSize(drag->selected, &width, &height);
                    x = MTRandom() % (108 - width) + 10;
                    y = MTRandom() % (125 - height) + 18;
                } else {
                    x = drag->originX;
                    y = drag->originY;
                }
                PlaySE(SEQ_SE_DP_HYUN2);
            } else {
                DressUpObj_GetPos(drag->selected, &x, &y);
                PlaySE(SEQ_SE_DP_BOX02);
            }
            if (drag->fromCanvas == TRUE) {
                drag->trayPage = DressUpTray_GetItemPage(drag->selected->mode, slot->id, drag->tray->caseTable);
            }
            DressUp_StartTweenToTray(drag, 4, x, y, drag->selected->mode, drag->trayPage);
        }
        DressUpDrag_ClearSelection(drag);
    }
}

void DressUpDrag_OnHeldAccessory(DressUpTouch *touch) {
    DressUpDrag *drag = touch->drag;
    if (drag->selected != NULL && gSystem.touchX != 0xFFFF && gSystem.touchX != 0xFFFF) {
        DressUpObj_SetPos(drag->selected, (int)gSystem.touchX - drag->grabX, (int)gSystem.touchY - drag->grabY);
    }
}

void DressUpDrag_OnReleaseMon(DressUpTouch *touch) {
    DressUpDrag *drag = touch->drag;
    if (drag->selected != NULL) {
        DressUpCanvas_AddToMonList(drag->canvas, drag->selected);
        DressUpCanvas_RefreshPriorities(drag->canvas);
        DressUp_DisableDragWindow();
        drag->dragging = FALSE;
        PlaySE(SEQ_SE_DP_BOX02);
        DressUpDrag_ClearSelection(drag);
    }
}

// moves the mon (and what is on it), kept inside the canvas
void DressUpDrag_OnHeldMon(DressUpTouch *touch) {
    DressUpDrag *drag = touch->drag;
    int x;
    int y;
    int width;
    int height;
    int left;
    int top;
    int right;
    int bottom;
    int edgeTop;
    if (drag->selected != NULL && gSystem.touchX != 0xFFFF && gSystem.touchX != 0xFFFF) {
        DressUpObj_GetMargins(drag->selected, &left, &right, &top, &bottom);
        DressUpObj_GetSize(drag->selected, &width, &height);
        x = (int)gSystem.touchX - drag->grabX;
        y = (int)gSystem.touchY - drag->grabY;
        edgeTop = y + top;
        width = width - right;
        height = height - bottom;
        if (x + left <= 138) {
            x = 138 - left;
        } else if (x + width >= 246) {
            x = 246 - width;
        }
        if (edgeTop <= 18) {
            y = 18 - top;
        } else if (y + height >= 143) {
            y = 143 - height;
        }
        DressUp_MoveMonWithAccessories(touch, x, y);
        DressUpCanvas_GetMaxOverflow(drag->canvas, &left, &top);
        DressUp_MoveMonWithAccessories(touch, x + left, y + top);
    }
}

void DressUpDrag_ApplyBackdrop(DressUpTouch *touch) {
    DressUpDrag *drag = touch->drag;
    if (drag->selected != NULL) {
        DressUpSlot *slot;
        GF_ASSERT(drag->selected->mode == 1);
        slot = drag->selected->object;
        DressUpCanvas_FreeBackdrop(drag->canvas);
        DressUpCanvas_SetBackdrop(drag->canvas, slot->id, HEAP_ID_14);
    }
}

void DressUpDrag_BeginDrag(DressUpDrag *drag, DressUpNode *node, BOOL fromCanvas, int grabX, int grabY, int priority) {
    int x;
    int y;
    DressUpObj_GetPos(node, &x, &y);
    drag->selected = node;
    drag->originX = x;
    drag->originY = y;
    drag->fromCanvas = fromCanvas;
    drag->grabX = grabX;
    drag->grabY = grabY;
    if (fromCanvas == FALSE) {
        drag->trayPage = DressUpTray_GetGroupPage(drag->tray, drag->selected->mode);
    } else {
        drag->trayPage = 0;
    }
    drag->dragging = TRUE;
    DressUpObj_SetPriority(node, priority);
}

void DressUpDrag_ClearSelection(DressUpDrag *drag) {
    drag->selected = NULL;
    drag->originX = 0;
    drag->originY = 0;
    drag->fromCanvas = FALSE;
    drag->trayPage = 0;
}

void DressUpDrag_GetSelectedRect(DressUpTouch *touch, int *top, int *bottom, int *left, int *right) {
    DressUpDrag *drag = touch->drag;
    int width;
    int height;
    int x;
    int y;
    DressUpObj_GetSize(drag->selected, &width, &height);
    DressUpObj_GetPos(drag->selected, &x, &y);
    *top = y;
    *bottom = y + height;
    *left = x;
    *right = x + width;
}

void DressUpDrag_GetSelectedVisibleRect(DressUpTouch *touch, int *top, int *bottom, int *left, int *right) {
    DressUpObj_GetVisibleRect(touch->drag->selected, top, bottom, left, right);
}

void DressUp_StartTweenToTray(DressUpDrag *drag, int frames, int destX, int destY, int mode, int page) {
    int x;
    int y;
    DressUpTrayTween *tween = SysTask_GetData(CreateSysTaskAndEnvironment((SysTaskFunc)DressUp_TweenToTrayTask, sizeof(DressUpTrayTween), 0, HEAP_ID_13));
    tween->tray = drag->tray;
    tween->node = drag->selected;
    tween->frames = frames;
    tween->destX = destX;
    tween->destY = destY;
    tween->mode = mode;
    tween->page = page;
    tween->paused = &drag->tray->pageSwapActive;
    tween->dragging = &drag->dragging;
    DressUpObj_GetPos(drag->selected, &x, &y);
    tween->stepX = (tween->destX - x) / frames;
    tween->stepY = (tween->destY - y) / frames;
    drag->dragging = FALSE;
}

void DressUp_TweenToTrayTask(SysTask *task, DressUpTrayTween *tween) {
    int x;
    int y;
    if (*tween->paused != TRUE) {
        DressUpObj_GetPos(tween->node, &x, &y);
        x += tween->stepX;
        y += tween->stepY;
        tween->frames--;
        if (tween->frames < 0 || (tween->stepX == 0 && tween->stepY == 0)) {
            DressUpObj_SetPos(tween->node, tween->destX, tween->destY);
            DressUpTray_LinkObjectToPage(tween->tray, tween->mode, tween->page, tween->node);
            DressUpTray_RefreshPriorities(tween->tray);
            if (*tween->dragging == FALSE) {
                DressUp_DisableDragWindow();
            }
            DestroySysTaskAndEnvironment(task);
            return;
        }
        DressUpObj_SetPos(tween->node, x, y);
    }
}

void DressUp_PrintSelectedItemName(DressUpDrag *drag) {
    if (drag->selected->mode == 0) {
        DressUpHud_PrintMsgInstant(drag->hud, NARC_msgdata_msg, NARC_msg_msg_0216_bin, ((DressUpSlot *)drag->selected->object)->id);
    } else if (drag->selected->mode == 1) {
        DressUpHud_PrintMsgInstant(drag->hud, NARC_msgdata_msg, NARC_msg_msg_0218_bin, ((DressUpSlot *)drag->selected->object)->id);
    }
}

void DressUp_MoveMonWithAccessories(DressUpTouch *touch, int x, int y) {
    DressUpDrag *drag = touch->drag;
    DressUpObj_SetPos(drag->selected, x, y);
    DressUpCanvas_MoveMonGroup(drag->canvas, x - drag->monX, y - drag->monY);
    drag->monX = x;
    drag->monY = y;
}

// the mon's hit map: its picture, unscanned
void DressUp_LoadMonHitMap(DressUpGfx *gfx, const PokepicTemplate *template) {
    void *data = GfGfxLoader_LoadFromNarc((NarcId) template->narcID, template->charDataID, FALSE, HEAP_ID_14, FALSE);
    NNSG2dCharacterData *hitMap = DressUpGfx_StoreUnpackedChar(gfx, data, DRESS_UP_IMAGE_MAX);
    UnscanPokepic(hitMap->pRawData, (NarcId) template->narcID);
}

// Objects of the Dress-Up canvas and tray (shared with the Accessory Portrait): the mon picture with its hit box
// and margins, the node pool and lists, per-mode object helpers (slot or mon), and the BG screens (load, alternate
// screen animation, sliding, V-blank char / palette uploads).

void DressUpMon_Create(DressUpMonPic *monPic, PokepicManager *pokepicManager, Pokemon *mon, PokepicTemplate *template, enum HeapID heapID) {
    DressUpMon_CreateEx(monPic, pokepicManager, mon, template, heapID, 0);
}

// the mon's Pokepic at (192, 56) with its hit box; mode 0 measures the transparent margins of the picture, else
// only the left edge (both sides) and the mon's own top / bottom value
void DressUpMon_CreateEx(DressUpMonPic *monPic, PokepicManager *pokepicManager, Pokemon *mon, PokepicTemplate *template, enum HeapID heapID, int mode) {
    int width, height;
    u8 margin;
    void *buffer;
    NNSG2dCharacterData *character;
    (void)GetMonData(mon, MON_DATA_SPECIES, NULL);
    DP_GetBoxmonSpriteCharAndPlttNarcIds(template, &mon->box, 2);
    monPic->pokepic = PokepicManager_CreatePokepic(pokepicManager, template, 192, 56, 0, 0, NULL, NULL);
    DressUpMon_GetSize(monPic, &width, &height);
    width /= 2;
    height /= 2;
    monPic->hitbox.rect.top = 56 - height;
    monPic->hitbox.rect.bottom = 56 + height;
    monPic->hitbox.rect.left = 192 - width;
    monPic->hitbox.rect.right = 192 + width;
    DressUpMon_MakeClampedRect(&monPic->hitbox, 192, 56, width, height);
    margin = GetBoxMonPicHeight_PBR(&mon->box, 2);
    buffer = GfGfxLoader_LoadFromNarc((NarcId) template->narcID, template->charDataID, FALSE, heapID, FALSE);
    NNS_G2dGetUnpackedCharacterData(buffer, &character);
    UnscanPokepic(character->pRawData, (NarcId) template->narcID);
    if (mode == 0) {
        DressUpMon_CalcPicMargins(character->pRawData, character->W * 8, character->H * 8, monPic->margins);
    } else {
        monPic->margins[0] = DressUpMon_FindPicLeftEdge(character->pRawData, character->W * 8);
        monPic->margins[1] = monPic->margins[0];
        monPic->margins[3] = margin;
        monPic->margins[2] = margin;
    }
    Heap_Free(buffer);
    monPic->mon = mon;
}

void DressUpMon_Delete(DressUpMonPic *monPic) {
    u32 i;
    u8 *bytes = (u8 *)monPic;
    Pokepic_Delete(monPic->pokepic);
    // clears the record byte by byte
    i = sizeof(DressUpMonPic);
    do {
        *bytes = 0;
        bytes++;
    } while (--i != 0);
}

void DressUpMon_SetPriority(DressUpMonPic *monPic, int priority) {
    Pokepic_SetAttr(monPic->pokepic, POKEPIC_Z, priority);
}

int DressUpMon_GetPriority(DressUpMonPic *monPic) {
    return Pokepic_GetAttr(monPic->pokepic, POKEPIC_Z);
}

void DressUpMon_SetCenterPos(DressUpMonPic *monPic, int x, int y) {
    int halfWidth;
    int halfHeight;
    DressUpMon_GetSize(monPic, &halfWidth, &halfHeight);
    Pokepic_SetAttr(monPic->pokepic, POKEPIC_X, x);
    Pokepic_SetAttr(monPic->pokepic, POKEPIC_Y, y);
    halfHeight /= 2;
    halfWidth /= 2;
    DressUpMon_MakeClampedRect(&monPic->hitbox, x, y, halfWidth, halfHeight);
}

BOOL DressUpMon_IsTouchHeldIn(DressUpMonPic *monPic) {
    return TouchscreenHitbox_TouchHeldIsIn(&monPic->hitbox);
}

BOOL DressUpMon_IsPointIn(DressUpMonPic *monPic, u32 x, u32 y) {
    return TouchscreenHitbox_PointIsIn(&monPic->hitbox, x, y);
}

void DressUpMon_GetCenterPos(DressUpMonPic *monPic, int *x, int *y) {
    *x = Pokepic_GetAttr(monPic->pokepic, POKEPIC_X);
    *y = Pokepic_GetAttr(monPic->pokepic, POKEPIC_Y);
}

void DressUpMon_GetSize(DressUpMonPic *monPic, int *width, int *height) {
    *width = 80;
    *height = 80;
}

// the touch is on an opaque pixel of the mon; (x, y) = touch position within the picture
BOOL DressUpMon_HitTestTouch(DressUpMonPic *monPic, int *x, int *y, NNSG2dCharacterData *hitMap) {
    int width, height, left, top;
    if (DressUpMon_IsTouchHeldIn(monPic) == FALSE) {
        return FALSE;
    }
    DressUpMon_GetCenterPos(monPic, &left, &top);
    DressUpMon_GetSize(monPic, &width, &height);
    left -= width / 2;
    top -= height / 2;
    *x = gSystem.touchX - left;
    *y = gSystem.touchY - top;
    if (DressUpGfx_CharPixelEquals(hitMap, *x, *y, 0) == 0) {
        return TRUE;
    }
    return FALSE;
}

BOOL DressUpMon_HitTestPoint(DressUpMonPic *monPic, int x, int y, NNSG2dCharacterData *hitMap) {
    int width, height, left, top;
    if (!DressUpMon_IsPointIn(monPic, x, y)) {
        return FALSE;
    }
    DressUpMon_GetCenterPos(monPic, &left, &top);
    DressUpMon_GetSize(monPic, &width, &height);
    left -= width / 2;
    top -= height / 2;
    y -= top;
    x -= left;
    if (!DressUpGfx_CharPixelEquals(hitMap, x, y, 0)) {
        return TRUE;
    }
    return FALSE;
}

void DressUpMon_GetMargins(DressUpMonPic *monPic, u8 *margins) {
    u8 *src = (u8 *)monPic;
    u8 *dst = margins;
    dst[0] = src[8];
    dst[1] = src[9];
    dst[2] = src[10];
    dst[3] = src[11];
}

// first column with an opaque pixel of an 80x80 4bpp picture (80 if none)
int DressUpMon_FindPicLeftEdge(const s8 *pixels, int stride) {
    int x, y;
    for (x = 0; x < 80; x++) {
        for (y = 0; y < 80; y++) {
            int index = x + y * stride;
            u8 mask = 15 << ((index % 2) * 4);
            if (pixels[index / 2] & mask) {
                return x;
            }
        }
    }
    return 80;
}

// transparent margins (left, right, top, bottom) of an 80x80 4bpp picture, at most 40
void DressUpMon_CalcPicMargins(s8 *pixels, int stride, int height, u8 *margins) {
    int x, y, row;
    margins[0] = 40;
    margins[1] = 40;
    margins[2] = 40;
    margins[3] = 40;
    for (x = 0; x < 80; x++) {
        int right;
        y = 0;
        right = 80 - x;
        row = y;
        for (; y < 80; y++) {
            int index = x + row;
            u8 mask = 15 << ((index % 2) * 4);
            if (pixels[index / 2] & mask) {
                if (margins[0] > x) {
                    margins[0] = x;
                }
                if (margins[1] > right) {
                    margins[1] = right;
                }
                if (margins[2] > y) {
                    margins[2] = y;
                }
                if (margins[3] > 80 - y) {
                    margins[3] = 80 - y;
                }
            }
            row += stride;
        }
    }
}

// hit box rect around (x, y), clamped to the screen
void DressUpMon_MakeClampedRect(TouchscreenHitbox *hitbox, int x, int y, int halfWidth, int halfHeight) {
    if (y - halfHeight >= 0) {
        hitbox->rect.top = y - halfHeight;
    } else {
        hitbox->rect.top = 0;
    }
    if (y + halfHeight <= 191) {
        hitbox->rect.bottom = y + halfHeight;
    } else {
        hitbox->rect.bottom = 191;
    }
    if (x - halfWidth >= 0) {
        hitbox->rect.left = x - halfWidth;
    } else {
        hitbox->rect.left = 0;
    }
    if (x + halfWidth <= 255) {
        hitbox->rect.right = x + halfWidth;
    } else {
        hitbox->rect.right = 255;
    }
}

void DressUpNodePool_Init(DressUpNodePool *pool, int count, enum HeapID heapID) {
    u32 size = count * sizeof(DressUpNode);
    pool->nodes = Heap_Alloc(heapID, size);
    if (pool->nodes == NULL) {
        GF_AssertFail();
    }
    memset(pool->nodes, 0, size);
    pool->count = count;
}

void DressUpNodePool_Free(DressUpNodePool *pool) {
    Heap_Free(pool->nodes);
    pool->nodes = NULL;
    pool->count = 0;
}

DressUpNode *DressUpNodePool_Acquire(DressUpNodePool *pool, void *object, u32 mode) {
    int i;
    if (pool->nodes == NULL) {
        GF_AssertFail();
    }
    if (pool->count == 0) {
        GF_AssertFail();
    }
    for (i = 0; i < pool->count; i++) {
        if (pool->nodes[i].object == NULL) {
            break;
        }
    }
    if (pool->count <= i) {
        GF_AssertFail();
    }
    pool->nodes[i].object = object;
    pool->nodes[i].mode = mode;
    return &pool->nodes[i];
}

void DressUpList_InitNode(DressUpNode *node) {
    memset(node, 0, sizeof(DressUpNode));
}

void DressUpList_InsertAfter(DressUpNode *node, DressUpNode *at) {
    node->next = at->next;
    at->next->prev = node;
    node->prev = at;
    at->next = node;
}

void DressUpList_Remove(DressUpNode *node) {
    node->prev->next = node->next;
    node->next->prev = node->prev;
}

// unlinks and frees every node of the list
void DressUpList_Clear(DressUpNode *head) {
    DressUpNode *node = head->next;
    while (node != head) {
        DressUpNode *next = node->next;
        DressUpList_Remove(node);
        DressUpList_InitNode(node);
        node = next;
    }
}

void DressUpObj_SetPriority(DressUpNode *node, int priority) {
    if ((int)node->mode < 3) { // signed compare here (unsigned in the tray)
        DressUpGfx_SetSlotPriority(node->object, priority);
    } else {
        DressUpMon_SetPriority(node->object, priority);
    }
}

// hit maps: [accessory], [100 + backdrop], [DRESS_UP_IMAGE_MAX] the mon
BOOL DressUpObj_HitTest(DressUpNode *node, int *x, int *y, NNSG2dCharacterData **hitMaps) {
    if (node->mode == 0) {
        return DressUpGfx_HitTestSlotPixels(node->object, x, y, hitMaps[((DressUpSlot *)node->object)->id]);
    }
    if (node->mode == 1) {
        DressUpSlot *slot = node->object;
        u32 index = slot->id;
        index += DRESS_UP_ACCESSORY_NUM;
        return DressUpGfx_HitTestSlotPixels(slot, x, y, hitMaps[index]);
    }
    if (node->mode == 2) {
        DressUpSlot *slot = node->object;
        u32 index = slot->id;
        index += DRESS_UP_ACCESSORY_NUM;
        return DressUpGfx_HitTestSlotPixels(slot, x, y, hitMaps[index]);
    }
    return DressUpMon_HitTestTouch(node->object, x, y, hitMaps[DRESS_UP_IMAGE_MAX]);
}

// top-left position (the mon is positioned by its centre)
void DressUpObj_SetPos(DressUpNode *node, int x, int y) {
    int width;
    int height;
    if ((int)node->mode < 3) { // signed compare here (unsigned in the tray)
        DressUpGfx_SetSlotPos(node->object, x, y);
        return;
    }
    {
        DressUpMonPic *monPic = node->object;
        DressUpMon_GetSize(monPic, &width, &height);
        width /= 2;
        height /= 2;
        DressUpMon_SetCenterPos(monPic, x + width, y + height);
    }
}

void DressUpObj_GetPos(DressUpNode *node, int *x, int *y) {
    int width;
    int height;
    if ((int)node->mode < 3) { // signed compare here (unsigned in the tray)
        DressUpGfx_GetSlotPos(node->object, x, y);
        return;
    }
    {
        DressUpMonPic *monPic = node->object;
        DressUpMon_GetCenterPos(monPic, x, y);
        DressUpMon_GetSize(monPic, &width, &height);
        width /= 2;
        height /= 2;
        *x -= width;
        *y -= height;
    }
}

void DressUpObj_GetSize(DressUpNode *node, int *width, int *height) {
    if ((int)node->mode < 3) { // signed compare here (unsigned in the tray)
        DressUpGfx_GetSlotSize(node->object, width, height);
    } else {
        DressUpMon_GetSize(node->object, width, height);
    }
}

void DressUpObj_GetMargins(DressUpNode *node, int *left, int *right, int *top, int *bottom) {
    u8 margins[4];
    if ((int)node->mode < 3) { // signed compare here (unsigned in the tray)
        DressUpGfx_GetSlotMargins(node->object, left, top);
        *right = *left;
        *bottom = *top;
    } else {
        DressUpMon_GetMargins(node->object, margins);
        *left = margins[0];
        *right = margins[1];
        *top = margins[2];
        *bottom = margins[3];
    }
}

void DressUpObj_MoveListBy(DressUpNode *head, int dx, int dy) {
    DressUpNode *node = head->next;
    while (node != head) {
        int x;
        int y;
        DressUpObj_GetPos(node, &x, &y);
        x += dx;
        y += dy;
        DressUpObj_SetPos(node, x, y);
        node = node->next;
    }
}

void DressUpObj_GetVisibleRect(DressUpNode *node, int *top, int *bottom, int *left, int *right) {
    int width;
    int height;
    int x;
    int y;
    int marginLeft;
    int marginTop;
    int marginRight;
    int marginBottom;
    DressUpObj_GetSize(node, &width, &height);
    DressUpObj_GetPos(node, &x, &y);
    DressUpObj_GetMargins(node, &marginLeft, &marginRight, &marginTop, &marginBottom);
    *top = y + marginTop;
    *bottom = y + height - marginBottom;
    *left = x + marginLeft;
    *right = x + width - marginRight;
}

void DressUpBgScreen_LoadFromTemplate(DressUpBgScreen *screen, const DressUpBgScreenTemplate *template) {
    screen->bgConfig = template->bgConfig;
    screen->xTile = template->x / 8;
    screen->yTile = template->y / 8;
    screen->layer = template->layer;
    screen->plttSlot = template->plttSlot;
    screen->plttCount = template->plttCount;
    screen->charOffset = template->charOffset;
    screen->narcId = template->narcId;
    screen->screenFileId = template->screenFileId;
    DressUpBgScreen_Load(template, &screen->widthTiles, &screen->heightTiles);
}

void DressUpBgScreen_Unload(DressUpBgScreen *screen) {
    BgClearTilemapBufferAndCommit(screen->bgConfig, (u8)screen->layer);
    memset(screen, 0, sizeof(DressUpBgScreen));
}

void DressUpBgScreen_InitAltAnim(DressUpBgScreenAnim *anim, const DressUpBgScreenAnimTemplate *template) {
    anim->screen = template->screen;
    anim->altScreenFileId = template->altScreenFileId;
    anim->delay = template->delay;
    anim->ticks = 0;
    anim->showingAlt = FALSE;
    anim->heapID = template->heapID;
}

// every `delay` ticks shows the other screen (alternate / base)
void DressUpBgScreen_UpdateAltAnim(DressUpBgScreenAnim *anim, int ticks) {
    anim->ticks += ticks;
    if (anim->ticks > anim->delay) {
        int fileId;
        NNSG2dScreenData *screenData;
        void *buffer;
        DressUpBgScreen *screen;
        anim->ticks = 0;
        if (anim->showingAlt == FALSE) {
            fileId = anim->altScreenFileId;
            anim->showingAlt = TRUE;
        } else {
            fileId = anim->screen->screenFileId;
            anim->showingAlt = FALSE;
        }
        buffer = GfGfxLoader_GetScrnData(anim->screen->narcId, fileId, FALSE, &screenData, anim->heapID);
        screen = anim->screen;
        DressUpBgScreen_CopyToTilemap(screen->bgConfig, screen->layer, screenData, screen->widthTiles, screen->heightTiles, screen->xTile, screen->yTile, screen->charOffset, screen->plttSlot);
        Heap_Free(buffer);
    }
}

void DressUpBgScreen_StopAltAnim(DressUpBgScreenAnim *anim) {
    NNSG2dScreenData *screenData;
    void *buffer = GfGfxLoader_GetScrnData(anim->screen->narcId, anim->screen->screenFileId, FALSE, &screenData, anim->heapID);
    DressUpBgScreen *screen = anim->screen;
    DressUpBgScreen_CopyToTilemap(screen->bgConfig, screen->layer, screenData, screen->widthTiles, screen->heightTiles, screen->xTile, screen->yTile, screen->charOffset, screen->plttSlot);
    Heap_Free(buffer);
    memset(anim, 0, sizeof(DressUpBgScreenAnim));
}

// scrolls the screen by (dx, dy) over `frames` frames, then loads the template there (the new screen is first
// drawn at the offset position with chars at 128 and palette 5)
void DressUp_StartBgSlide(DressUpBgScreen *screen, DressUpBgScreenTemplate *template, int dx, int dy, int frames, BOOL *done) {
    DressUpBgSlide *slide = SysTask_GetData(CreateSysTaskAndEnvironment((SysTaskFunc)DressUp_BgSlideTask, sizeof(DressUpBgSlide), 0, HEAP_ID_13));
    slide->screen = screen;
    slide->template = *template;
    slide->done = done;
    slide->frames = frames;
    slide->stepX = dx / frames;
    slide->stepY = dy / frames;
    slide->startX = Bg_GetXpos(screen->bgConfig, screen->layer);
    slide->startY = Bg_GetYpos(screen->bgConfig, screen->layer);
    template->charOffset = 128;
    template->plttSlot = 5;
    template->x -= dx;
    template->y -= dy;
    template->heapID = HEAP_ID_14;
    DressUpBgScreen_Load(template, NULL, NULL);
}

void DressUpBgScreen_AddCharOffset(NNSG2dScreenData *screenData, int charOffset) {
    int i;
    int count = screenData->szByte >> 1;
    u16 *tiles = (u16 *)screenData->rawData;
    for (i = 0; i < count; i++) {
        tiles[i] += charOffset;
    }
}

// queues the chars and the palette for V-blank and copies the screen into the tilemap buffer
void DressUpBgScreen_Load(const DressUpBgScreenTemplate *template, int *outWidth, int *outHeight) {
    struct {
        int y;
        int x;
    } tile;

    NNSG2dScreenData *screenData;
    void *buffer;
    int width, height;
    tile.x = template->x / 8;
    tile.y = template->y / 8;
    DressUpBgScreen_QueueLoadChars(template->narcId, template->charFileId, template->bgConfig, template->layer, template->charOffset, template->heapID);
    DressUpBgScreen_QueueLoadPltt(template->narcId, template->plttFileId, template->layer < GF_BG_LYR_SUB_0 ? 0 : 4, template->plttSlot * 32, template->plttCount * 32, template->heapID);
    buffer = GfGfxLoader_GetScrnData(template->narcId, template->screenFileId, FALSE, &screenData, template->heapID);
    width = screenData->screenWidth / 8;
    height = screenData->screenHeight / 8;
    DressUpBgScreen_CopyToTilemap(template->bgConfig, template->layer, screenData, width, height, tile.x, tile.y, template->charOffset, template->plttSlot);
    Heap_Free(buffer);
    if (outWidth) {
        *outWidth = width;
    }
    if (outHeight) {
        *outHeight = height;
    }
}

void DressUp_BgSlideTask(SysTask *task, DressUpBgSlide *slide) {
    slide->frames--;
    if (slide->frames >= 0) {
        ScheduleSetBgPosText(slide->screen->bgConfig, slide->screen->layer, BG_POS_OP_SUB_X, slide->stepX);
        ScheduleSetBgPosText(slide->screen->bgConfig, slide->screen->layer, BG_POS_OP_SUB_Y, slide->stepY);
    } else {
        ScheduleSetBgPosText(slide->screen->bgConfig, slide->screen->layer, BG_POS_OP_SET_X, slide->startX);
        ScheduleSetBgPosText(slide->screen->bgConfig, slide->screen->layer, BG_POS_OP_SET_Y, slide->startY);
        DressUpBgScreen_LoadFromTemplate(slide->screen, &slide->template);
        if (slide->done) {
            *slide->done = TRUE;
        }
        DestroySysTaskAndEnvironment(task);
    }
}

void DressUpBgScreen_CopyToTilemap(BgConfig *bgConfig, int layer, NNSG2dScreenData *screenData, int width, int height, int x, int y, int charOffset, int palette) {
    DressUpBgScreen_AddCharOffset(screenData, charOffset);
    CopyToBgTilemapRect(bgConfig, (u8)layer, (u8)x, (u8)y, (u8)width, (u8)height, screenData->rawData, 0, 0, (u8)width, (u8)height);
    BgTilemapRectChangePalette(bgConfig, (u8)layer, (u8)x, (u8)y, (u8)width, (u8)height, (u8)palette);
    ScheduleBgTilemapBufferTransfer(bgConfig, (u8)layer);
}

SysTask *DressUpBgScreen_QueueLoadChars(NarcId narcId, int fileId, BgConfig *bgConfig, u32 layer, u32 tileStart, enum HeapID heapID) {
    DressUpCharUpload *upload = Heap_Alloc(heapID, sizeof(DressUpCharUpload));
    memset(upload, 0, sizeof(DressUpCharUpload));
    upload->buffer = GfGfxLoader_GetCharData(narcId, fileId, FALSE, &upload->charData, heapID);
    upload->bgConfig = bgConfig;
    upload->layer = layer;
    upload->tileStart = tileStart;
    return SysTask_CreateOnVWaitQueue((SysTaskFunc)DressUpBgScreen_LoadCharsTask, upload, 128);
}

void DressUpBgScreen_QueueLoadPltt(NarcId narcId, int fileId, u32 location, u32 offset, u32 size, enum HeapID heapID) {
    DressUpPlttUpload *upload = Heap_Alloc(heapID, sizeof(DressUpPlttUpload));
    memset(upload, 0, sizeof(DressUpPlttUpload));
    upload->buffer = GfGfxLoader_GetPlttData(narcId, fileId, &upload->plttData, heapID);
    upload->location = location;
    upload->offset = offset;
    upload->size = size;
    SysTask_CreateOnVWaitQueue((SysTaskFunc)DressUpBgScreen_LoadPlttTask, upload, 128);
}

void DressUpBgScreen_LoadCharsTask(SysTask *task, DressUpCharUpload *upload) {
    DC_FlushRange(upload->charData->pRawData, upload->charData->szByte);
    BG_LoadCharTilesData(upload->bgConfig, upload->layer, upload->charData->pRawData, upload->charData->szByte, upload->tileStart);
    SysTask_Destroy(task);
    Heap_Free(upload->buffer);
    Heap_Free(upload);
}

void DressUpBgScreen_LoadPlttTask(SysTask *task, DressUpPlttUpload *upload) {
    DC_FlushRange(upload->plttData->pRawData, upload->size);
    if (upload->location == 0) {
        GX_LoadBGPltt(upload->plttData->pRawData, upload->offset, upload->size);
    } else if (upload->location == 4) {
        GXS_LoadBGPltt(upload->plttData->pRawData, upload->offset, upload->size);
    }
    SysTask_Destroy(task);
    Heap_Free(upload->buffer);
    Heap_Free(upload);
}

// buttons of the editor's panel (affine sprite, optional text on it)
void DressUpButton_Init(DressUpButton *button, const DressUpButtonTemplate *template) {
    if (!button) {
        GF_AssertFail();
    }
    if (!template) {
        GF_AssertFail();
    }
    button->sprite = Sprite_CreateAffine(template->spriteTemplate);
    if (!button->sprite) {
        GF_AssertFail();
    }
    button->trigger = template->trigger;
    button->arg = template->arg;
    if (template->callback) {
        button->callback = template->callback;
        return;
    }
    button->callback = DressUpButton_DefaultCallback;
}

void DressUpButton_InitWithText(DressUpTextButton *button, const DressUpTextButtonTemplate *template) {
    TextOBJTemplate textTemplate;
    DressUpButton_Init(&button->button, &template->button);
    textTemplate.fontSystem = template->fontSystem;
    textTemplate.window = template->window;
    textTemplate.spriteList = template->button.spriteTemplate->spriteList;
    textTemplate.plttResourceProxy = template->plttProxy;
    textTemplate.sprite = button->button.sprite;
    textTemplate.offset = template->charOffset;
    textTemplate.x = template->x;
    textTemplate.y = template->y;
    textTemplate.bgPriority = 0;
    textTemplate.drawPriority = 0;
    textTemplate.vram = template->button.spriteTemplate->whichScreen;
    textTemplate.heapID = template->button.spriteTemplate->heapID;
    button->textObj = FontOAM_Create(&textTemplate);
}

// Button panel of the Dress-Up editor: next / previous page, the accessory and backdrop tabs (a radio pair) and
// DONE (a text button), driven by touch or by the keys.

void DressUpButton_Delete(DressUpButton *button) {
    Sprite_Delete(button->sprite);
    memset(button, 0, sizeof(DressUpButton));
}

void DressUpButton_DeleteWithText(DressUpTextButton *button) {
    DressUpButton_Delete(&button->button);
    FontOAM_Delete(button->textObj);
    memset(button, 0, sizeof(DressUpTextButton));
}

void DressUpButton_TryCallback(DressUpButton *button, u32 state) {
    if (state == button->trigger) {
        button->callback(button, button->arg);
    }
}

// the two tab buttons (2, 3): the selected one shows pressed (or selected), the other normal
void DressUp_UpdateCategoryTabs(DressUpButtonPanel *panel, int selection, int state) {
    int i = 2;
    DressUpButton *button = &panel->buttons[2];
    for (; i <= 3; i++, button++) {
        if (i != selection) {
            DressUpButton_SetAnimNormal(button);
        } else if (state == 0) {
            DressUpButton_SetAnimPressed(button);
            PlaySE(SEQ_SE_DP_PASO);
        } else if (state == 2) {
            DressUpButton_SetAnimSelected(button);
        } else {
            DressUpButton_SetAnimPressed(button);
        }
    }
}

void DressUpButton_SetCallback(DressUpButton *button, DressUpButtonCallback callback, void *arg, u32 trigger) {
    if (button == NULL) {
        GF_AssertFail();
    }
    button->trigger = trigger;
    button->arg = arg;
    button->callback = callback;
}

void DressUpButton_DefaultCallback(void *button, void *arg) {
}

void DressUpButton_SetAnimSelected(DressUpButton *button) {
    Sprite_SetAnimationFrame(button->sprite, 2);
}

void DressUpButton_SetAnimNormal(DressUpButton *button) {
    Sprite_SetAnimationFrame(button->sprite, 0);
}

void DressUpButton_SetAnimPressed(DressUpButton *button) {
    Sprite_SetAnimationFrame(button->sprite, 1);
}

void DressUpButtonPanel_Create(DressUpButtonPanel *panel, DressUpGfx *gfx, MenuInputState *inputState) {
    panel->inputState = inputState;
    panel->fontSystem = FontSystem_NewInit(1, HEAP_ID_13);
    DressUpButtonPanel_LoadResources(gfx);
    DressUpButton_CreateSprite(&panel->buttons[0], 0, gfx, 48, 144, 40, 32);
    DressUp_SetHitboxRect(panel->hitboxes, 0, 48, 152, 40, 24);
    DressUpButton_CreateSprite(&panel->buttons[1], 1, gfx, 8, 144, 40, 32);
    DressUp_SetHitboxRect(panel->hitboxes, 1, 8, 152, 40, 24);
    DressUpButton_CreateSprite(&panel->buttons[2], 2, gfx, 96, 144, 40, 42);
    DressUp_SetHitboxRect(panel->hitboxes, 2, 96, 156, 40, 34);
    DressUpButton_CreateSprite(&panel->buttons[3], 3, gfx, 136, 144, 40, 42);
    DressUp_SetHitboxRect(panel->hitboxes, 3, 136, 156, 40, 34);
    FontID_Alloc(2, HEAP_ID_14);
    {
        Window *label = DressUpButton_CreateTextBitmap(gfx, NARC_msgdata_msg, NARC_msg_msg_0215_bin, msg_0215_00000, 9, 5);
        DressUpButton_CreateTextSprite(&panel->done, 4, gfx, panel->fontSystem, label, 184, 144, 72, 42);
        DressUp_SetHitboxRect(panel->hitboxes, 4, 184, 156, 72, 34);
        DressUpButton_DeleteTextBitmap(label);
    }
    FontID_Release(2);
    panel->visible = TRUE;
    DressUpButtonPanel_FreeResources(gfx);
    DressUpButton_SetAnimSelected(&panel->buttons[2]);
    panel->controller = TouchHitboxController_Create(panel->hitboxes, 5, DressUpButtonPanel_OnTouch, panel, HEAP_ID_13);
}

void DressUpButtonPanel_Destroy(DressUpButtonPanel *panel, DressUpGfx *gfx) {
    int i;
    for (i = 0; i < 5; i++) {
        DressUpGfx_FreeCellRes(gfx, i);
        DressUpGfx_FreeAnimRes(gfx, i);
        ObjCharTransfer_ResetTransferTasksByResID(i);
    }
    ObjPlttTransfer_FreeTaskByID(0);
    ObjPlttTransfer_FreeTaskByID(1);
    i = 0;
    {
        DressUpButton *button = panel->buttons;
        do {
            DressUpButton_Delete(button);
            i++;
            button++;
        } while (i < 4);
    }
    ObjCharTransfer_FreeCharSpace(&panel->done.charAlloc);
    DressUpButton_DeleteWithText(&panel->done);
    FontSystem_Delete(panel->fontSystem);
    TouchHitboxController_Destroy(panel->controller);
    panel->controller = NULL;
}

// A / B press DONE, up / down the page buttons, left / right the tabs; a pressed key shows for 2 frames
void DressUpButtonPanel_HandleKeys(DressUpButtonPanel *panel) {
    int key = 0xFF;
    u32 keys;
    if (!panel->controller) {
        GF_AssertFail();
    }
    if (panel->keyRepeat) {
        panel->keyRepeat--;
        DressUpButtonPanel_OnTouch(panel->key, sDressUpKeyPressTouchStates[panel->keyRepeat ^ 1], panel);
        return;
    }
    keys = gSystem.newKeys;
    if (keys & (PAD_BUTTON_A | PAD_BUTTON_B)) {
        key = 4;
    } else if (keys & PAD_KEY_UP) {
        key = 1;
    } else if (keys & PAD_KEY_DOWN) {
        key = 0;
    } else if (keys & PAD_KEY_LEFT) {
        key = 2;
    } else if (keys & PAD_KEY_RIGHT) {
        key = 3;
    }
    if (key != 0xFF) {
        panel->key = key;
        panel->keyRepeat = 2;
        DressUpButtonPanel_OnTouch(panel->key, 0, panel);
    }
}

// switches the shared input mode between touch and keys
int DressUpButtonPanel_UpdateInputMode(DressUpButtonPanel *panel) {
    if (panel->keyRepeat) {
        return 0;
    }
    if (*panel->inputState == MENU_INPUT_STATE_TOUCH) {
        if (System_GetTouchHeld()) {
            return 0;
        }
        if (gSystem.heldKeys) {
            *panel->inputState = MENU_INPUT_STATE_BUTTONS;
        }
    } else {
        if (gSystem.heldKeys) {
            return 0;
        }
        if (System_GetTouchHeld()) {
            *panel->inputState = MENU_INPUT_STATE_TOUCH;
        }
    }
    return 0;
}

void DressUpButtonPanel_Update(DressUpButtonPanel *panel) {
    if (!panel->controller) {
        GF_AssertFail();
    }
    if (DressUpButtonPanel_UpdateInputMode(panel) == 0) {
        if (*panel->inputState == MENU_INPUT_STATE_BUTTONS) {
            DressUpButtonPanel_HandleKeys(panel);
            return;
        }
        TouchHitboxController_IsTriggered(panel->controller);
    }
}

void DressUpButtonPanel_ResetButtons(DressUpButtonPanel *panel) {
    DressUpButton_SetAnimByTouchState(&panel->done.button, 3);
    DressUpButton_SetTextByState(&panel->done, 3);
    DressUp_UpdateCategoryTabs(panel, 2, 3);
}

void DressUpButtonPanel_MoveBy(DressUpButtonPanel *panel, int dx, int dy) {
    DressUpButtonPanel *base[1] = { panel };
    int i = 0;
    DressUpButton *button = panel->buttons;
    do {
        DressUpButton_MoveBy(button, dx, dy);
        i++;
        button++;
    } while (i < 4);
    DressUpButton_MoveTextBy(&base[0]->done, dx, dy);
}

void DressUpButtonPanel_SetCallback(DressUpButtonPanel *panel, int button, DressUpButtonCallback callback, void *arg, u32 trigger) {
    if (!panel) {
        GF_AssertFail();
    }
    if (button < 4) {
        DressUpButton_SetCallback(&panel->buttons[button], callback, arg, trigger);
    } else {
        DressUpButton_SetCallback(&panel->done.button, callback, arg, trigger);
    }
}

// touch controller callback (also used for keys): button `which` in touch state `state`
void DressUpButtonPanel_OnTouch(u32 which, u32 state, void *arg) {
    DressUpButtonPanel *panel = arg;
    DressUpButton *button = (DressUpButton *)panel;
    switch (which) {
    case 0:
        DressUpButton_SetAnimByTouchState(button, state);
        DressUpButton_PlaySE(button, SEQ_SE_DP_MAZYO3, 0, state);
        DressUpButton_TryCallback(button, state);
        break;
    case 1:
        button++;
        DressUpButton_SetAnimByTouchState(button, state);
        DressUpButton_PlaySE(button, SEQ_SE_DP_MAZYO3, 0, state);
        DressUpButton_TryCallback(button, state);
        break;
    case 2:
    case 3:
        DressUp_UpdateCategoryTabs(panel, which, state);
        DressUpButton_TryCallback(button + which, state);
        break;
    case 4:
        if (panel->visible == TRUE) {
            DressUpButton_SetAnimByTouchState(&panel->done.button, state);
            DressUpButton_SetTextByState(&panel->done, state);
            DressUpButton_PlaySE(&panel->done.button, SEQ_SE_DP_PIRORIRO, 0, state);
            DressUpButton_TryCallback(&panel->done.button, state);
        }
        break;
    default:
        GF_AssertFail();
        break;
    }
}

void DressUpButton_CreateSprite(DressUpButton *button, int resId, DressUpGfx *gfx, int x, int y, int width, int height) {
    DressUpButtonTemplate template;
    SpriteResourcesHeader header;
    SpriteTemplate spriteTemplate;
    CreateSpriteResourcesHeader(&header, resId, 0, resId, resId, -1, -1, 0, 0, gfx->charResMan, gfx->plttResMan, gfx->cellResMan, gfx->animResMan, NULL, NULL);
    spriteTemplate.spriteList = gfx->spriteList;
    spriteTemplate.header = &header;
    spriteTemplate.position.x = (u32)x << FX32_SHIFT;
    spriteTemplate.position.y = (u32)y << FX32_SHIFT;
    spriteTemplate.position.z = 0;
    spriteTemplate.drawPriority = 2;
    spriteTemplate.whichScreen = NNS_G2D_VRAM_TYPE_2DMAIN;
    spriteTemplate.heapID = HEAP_ID_14;
    template.spriteTemplate = &spriteTemplate;
    template.trigger = 1;
    template.callback = NULL;
    template.arg = NULL;
    DressUpButton_Init(button, &template);
}

void DressUpButton_CreateTextSprite(DressUpTextButton *button, int resId, DressUpGfx *gfx, FontSystem *fontSystem, Window *label, int x, int y, int width, int height) {
    DressUpTextButtonTemplate template;
    SpriteResourcesHeader header;
    SpriteTemplate spriteTemplate;
    CreateSpriteResourcesHeader(&header, resId, 0, resId, resId, -1, -1, 0, 0, gfx->charResMan, gfx->plttResMan, gfx->cellResMan, gfx->animResMan, NULL, NULL);
    spriteTemplate.spriteList = gfx->spriteList;
    spriteTemplate.header = &header;
    spriteTemplate.position.x = x * FX32_ONE;
    spriteTemplate.position.y = y * FX32_ONE;
    spriteTemplate.position.z = 0;
    spriteTemplate.drawPriority = 2;
    spriteTemplate.whichScreen = NNS_G2D_VRAM_TYPE_2DMAIN;
    spriteTemplate.heapID = HEAP_ID_14;
    template.button.spriteTemplate = &spriteTemplate;
    template.button.callback = NULL;
    template.button.arg = NULL;
    template.button.trigger = 1;
    template.fontSystem = fontSystem;
    template.window = label;
    template.x = 0;
    template.y = 19;
    template.plttProxy = SpriteTransfer_GetPaletteProxy(SpriteResourceCollection_Find(gfx->plttResMan, 1), NULL);
    GF_ASSERT(ObjCharTransfer_ReserveCharSpace(FontOAM_GetCharSize(label, NNS_G2D_VRAM_TYPE_2DMAIN, HEAP_ID_13), TRUE, NNS_G2D_VRAM_TYPE_2DMAIN, &button->charAlloc));
    template.charOffset = button->charAlloc.offset;
    DressUpButton_InitWithText(button, &template);
}

// hit box rect i = (x, y, width, height)
void DressUp_SetHitboxRect(TouchscreenHitbox *hitboxes, int i, int x, int y, int width, int height) {
    u8 *rect = (u8 *)hitboxes;
    i *= 4;
    rect[i] = y;
    rect += i;
    rect[2] = x;
    rect[1] = y + height;
    rect[3] = x + width;
}

void DressUpButtonPanel_LoadResources(DressUpGfx *gfx) {
    int i = 0;
    int charFileId = 0x6B, cellFileId = 0x6A, animFileId = 0x69;
    do {
        DressUpGfx_LoadCharRes(gfx, gfx->narc, charFileId, FALSE, 1, i);
        DressUpGfx_LoadCellRes(gfx, gfx->narc, cellFileId, FALSE, i);
        DressUpGfx_LoadAnimRes(gfx, gfx->narc, animFileId, FALSE, i);
        charFileId += 3;
        cellFileId += 3;
        animFileId += 3;
        i++;
    } while (i < 5);
    DressUpGfx_LoadPlttRes(gfx, gfx->narc, 0x68, FALSE, 1, 3, 0);
    DressUpGfx_LoadPlttRes(gfx, gfx->narc, 0x78, FALSE, 1, 2, 1);
}

void DressUpButtonPanel_FreeResources(DressUpGfx *gfx) {
    int i;
    for (i = 0; i < 5; i++) {
        DressUpGfx_FreeCharRes(gfx, i);
    }
    DressUpGfx_FreePlttRes(gfx, 0);
    DressUpGfx_FreePlttRes(gfx, 1);
}

void DressUpButton_SetAnimByTouchState(DressUpButton *button, u32 state) {
    if (state == 0) {
        DressUpButton_SetAnimPressed(button);
    } else if (state == 2) {
        DressUpButton_SetAnimSelected(button);
    } else if (state == 1 || state == 3) {
        DressUpButton_SetAnimNormal(button);
    }
}

void DressUpButton_SetTextByState(DressUpTextButton *button, u32 state) {
    if (state == 0) {
        TextOBJ_SetPosition(button->textObj, 0, 15);
        TextOBJ_SetPaletteNum(button->textObj, 4);
    } else {
        if (state == 1) {
            TextOBJ_SetPosition(button->textObj, 0, 19);
        }
        if (state == 3) {
            TextOBJ_SetPosition(button->textObj, 0, 19);
            TextOBJ_SetPaletteNum(button->textObj, 3);
        }
    }
}

// Editor sub screen (message box, accessory count meter, an unused countdown with its pulse tween), message and
// sprite resource helpers shared by the buttons, and the backdrop animation of the finished design.

void DressUpButton_PlaySE(DressUpButton *button, u32 se, u32 trigger, u32 state) {
    if (state == trigger) {
        PlaySE((u16)se);
    }
}

// message centred in an off-screen window, for a text button
Window *DressUpButton_CreateTextBitmap(DressUpGfx *gfx, NarcId narcId, int bank, int msgId, int width, int height) {
    MsgData *msgData = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, narcId, bank, HEAP_ID_13);
    String *string;
    Window *window;
    int x;
    GF_ASSERT(msgData != NULL);
    string = NewString_ReadMsgData(msgData, msgId);
    window = AllocWindows(HEAP_ID_14, 1);
    InitWindow(window);
    AddTextWindowTopLeftCorner(gfx->bgConfig, window, width, height, 0, 0);
    x = FontID_String_GetCenterAlignmentX(2, string, 0, width * 8);
    AddTextPrinterParameterizedWithColor(window, 2, string, x, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 3), NULL);
    String_Delete(string);
    DestroyMsgData(msgData);
    return window;
}

void DressUpButton_DeleteTextBitmap(Window *window) {
    WindowArray_Delete(window, 1);
}

void DressUpButton_MoveBy(DressUpButton *button, int dx, int dy) {
    VecFx32 pos = *Sprite_GetMatrixPtr(button->sprite);
    pos.x += dx << FX32_SHIFT;
    pos.y += dy << FX32_SHIFT;
    Sprite_SetMatrix(button->sprite, &pos);
}

void DressUpButton_MoveTextBy(DressUpTextButton *button, int dx, int dy) {
    DressUpButton_MoveBy(&button->button, dx, dy);
    TextOBJ_UpdatePosition(button->textObj);
}

void DressUpHud_Init(DressUpHud *hud, const DressUpHudTemplate *template, u32 flags) {
    if (flags & DRESS_UP_HUD_CLEAR) {
        memset(hud, 0, sizeof(DressUpHud));
    }
    if (flags & DRESS_UP_HUD_SUB_BG) {
        DressUpHud_LoadSubBg(hud, template->bgConfig);
    }
    if (flags & DRESS_UP_HUD_MSG_WINDOW) {
        DressUpHud_AddWindow(&hud->msgWindow, template->bgConfig, 2, 19, 27, 4, 31, TRUE);
    }
    if (flags & DRESS_UP_HUD_COUNT_ICONS) {
        DressUpHud_CreateCountIcons(&hud->countIcons, template->spriteList, template->resMans, template->countMax, template->narc);
    }
    if (flags & DRESS_UP_HUD_COUNTDOWN) {
        DressUpCountdown_Create(&hud->countdown, template->spriteList, template->resMans, template->countdownSeconds, template->bgConfig, template->countdownOutput, template->narc);
    }
    if (flags & DRESS_UP_HUD_INFO_WINDOW) {
        DressUpHud_AddWindow(&hud->infoWindow, template->bgConfig, 2, 1, 27, 2, 139, TRUE);
        FillWindowPixelBuffer(hud->infoWindow, 15);
        DressUp_PrintMsg(hud->infoWindow, NARC_msgdata_msg, template->infoMsgBank, 6, 0, 0, MAKE_TEXT_COLOR(1, 2, 15), TEXT_SPEED_NOTRANSFER);
        DressUp_PrintMsg(hud->infoWindow, NARC_msgdata_msg, template->infoMsgBank, template->infoMsgId, 72, 0, MAKE_TEXT_COLOR(1, 2, 15), TEXT_SPEED_NOTRANSFER);
        DrawFrameAndWindow2(hud->infoWindow, FALSE, 1, 1);
    }
    hud->spriteList = template->spriteList;
    hud->resMans = template->resMans;
    DressUpHud_LoadFrameAndFontPal(template->bgConfig, template->options);
    hud->frame = Options_GetFrame(template->options);
    hud->textFrameDelay = Options_GetTextFrameDelay(template->options);
    hud->flags |= flags;
}

void DressUpHud_Free(DressUpHud *hud) {
    if (hud->flags & DRESS_UP_HUD_SUB_BG) {
        DressUpHud_UnloadSubBg(hud);
        hud->flags &= ~DRESS_UP_HUD_SUB_BG;
    }
    if (hud->flags & DRESS_UP_HUD_MSG_WINDOW) {
        DressUpHud_DeleteWindow(hud->msgWindow);
        hud->flags &= ~DRESS_UP_HUD_MSG_WINDOW;
    }
    if (hud->flags & DRESS_UP_HUD_COUNT_ICONS) {
        DressUpHud_DestroyCountIcons(&hud->countIcons, hud->resMans);
        hud->flags &= ~DRESS_UP_HUD_COUNT_ICONS;
    }
    if (hud->flags & DRESS_UP_HUD_COUNTDOWN) {
        DressUpCountdown_Destroy(&hud->countdown, hud->resMans);
        hud->flags &= ~DRESS_UP_HUD_COUNTDOWN;
    }
    if (hud->flags & DRESS_UP_HUD_INFO_WINDOW) {
        DressUpHud_DeleteWindow(hud->infoWindow);
        hud->flags &= ~DRESS_UP_HUD_INFO_WINDOW;
    }
    memset(hud, 0, sizeof(DressUpHud));
}

void DressUpHud_UpdateCountdown(DressUpHud *hud) {
    if (hud->flags & DRESS_UP_HUD_COUNTDOWN) {
        DressUpCountdown_Update(&hud->countdown);
    }
}

void DressUpHud_PrintMsgInstant(DressUpHud *hud, NarcId narcId, int bank, int msgId) {
    GF_ASSERT(hud->flags & DRESS_UP_HUD_MSG_WINDOW);
    DressUp_PrintMsgFramed(hud->msgWindow, narcId, bank, msgId, 0, 0, TEXT_SPEED_NOTRANSFER);
}

u8 DressUpHud_PrintMsg(DressUpHud *hud, NarcId narcId, int bank, int msgId) {
    GF_ASSERT(hud->flags & DRESS_UP_HUD_MSG_WINDOW);
    return DressUp_PrintMsgFramedKeepString(hud->msgWindow, narcId, bank, msgId, 0, 0, hud->textFrameDelay, &hud->msgString);
}

void DressUpHud_FreeMsgString(DressUpHud *hud) {
    String_Delete(hud->msgString);
    hud->msgString = NULL;
}

void DressUpHud_SetAccessoryCount(DressUpHud *hud, int count) {
    DressUpHud_SetCountIcons(&hud->countIcons, count);
}

void DressUpHud_LoadSubBg(DressUpHud *hud, BgConfig *bgConfig) {
    DressUpBgScreenTemplate template;

    template.bgConfig = bgConfig;
    template.narcId = NARC_a_0_2_6;
    template.charFileId = 0xE0;
    template.plttFileId = 0xE1;
    template.screenFileId = 0xE2;
    template.x = 0;
    template.y = 0;
    template.layer = GF_BG_LYR_SUB_0;
    template.plttCount = 1;
    template.plttSlot = 0;
    template.charOffset = 0;
    template.heapID = HEAP_ID_14;
    DressUpBgScreen_LoadFromTemplate(&hud->subBg, &template);
}

void DressUpHud_LoadFrameAndFontPal(BgConfig *bgConfig, Options *options) {
    u8 frame = Options_GetFrame(options);

    LoadUserFrameGfx2(bgConfig, GF_BG_LYR_SUB_1, 1, 1, frame, HEAP_ID_14);
    LoadFontPal1(GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_2_OFFSET, HEAP_ID_14);
}

void DressUpHud_AddWindow(Window **window, BgConfig *bgConfig, int x, int y, int width, int height, int baseTile, BOOL drawFrame) {
    *window = AllocWindows(HEAP_ID_14, 1);
    InitWindow(*window);
    AddWindowParameterized(bgConfig, *window, GF_BG_LYR_SUB_1, x, y, width, height, 2, baseTile);
    FillWindowPixelBuffer(*window, 15);
    if (drawFrame != FALSE) {
        DrawFrameAndWindow2(*window, FALSE, 1, 1);
    }
    CopyWindowToVram(*window);
}

void DressUpHud_UnloadSubBg(DressUpHud *hud) {
    DressUpBgScreen_Unload(&hud->subBg);
}

void DressUpHud_DeleteWindow(Window *window) {
    WindowArray_Delete(window, 1);
}

u8 DressUp_PrintMsgFramed(Window *window, NarcId narcId, int bank, int msgId, int x, int y, int textSpeed) {
    FillWindowPixelBuffer(window, 15);
    u8 printerId = DressUp_PrintMsg(window, narcId, bank, msgId, x, y, MAKE_TEXT_COLOR(1, 2, 15), textSpeed);
    DrawFrameAndWindow2(window, FALSE, 1, 1);
    return printerId;
}

u8 DressUp_PrintMsgFramedKeepString(Window *window, NarcId narcId, int bank, int msgId, int x, int y, int textSpeed, String **string) {
    FillWindowPixelBuffer(window, 15);
    u8 printerId = DressUp_PrintMsgKeepString(window, narcId, bank, msgId, x, y, MAKE_TEXT_COLOR(1, 2, 15), textSpeed, string);
    DrawFrameAndWindow2(window, FALSE, 1, 1);
    return printerId;
}

u8 DressUp_PrintMsg(Window *window, NarcId narcId, int bank, int msgId, int x, int y, u32 color, int textSpeed) {
    MsgData *msgData = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, narcId, bank, HEAP_ID_13);
    GF_ASSERT(msgData);
    String *string = NewString_ReadMsgData(msgData, msgId);
    u8 printerId = AddTextPrinterParameterizedWithColor(window, 1, string, x, y, textSpeed, color, NULL);
    String_Delete(string);
    DestroyMsgData(msgData);
    return printerId;
}

// as DressUp_PrintMsg, but the String is kept in *string until the printer is done with it
u8 DressUp_PrintMsgKeepString(Window *window, NarcId narcId, int bank, int msgId, int x, int y, u32 color, int textSpeed, String **string) {
    GF_ASSERT(!*string);
    MsgData *msgData = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, narcId, bank, HEAP_ID_13);
    GF_ASSERT(msgData);
    *string = NewString_ReadMsgData(msgData, msgId);
    u8 printerId = AddTextPrinterParameterizedWithColor(window, 1, *string, x, y, textSpeed, color, NULL);
    DestroyMsgData(msgData);
    return printerId;
}

void DressUpHud_CreateCountIcons(DressUpCountIcons *icons, SpriteList *spriteList, GF_2DGfxResMan **resMans, int count, NARC *narc) {
    SpriteResourcesHeader header;
    SimpleSpriteTemplate template;

    struct {
        int y, row;
    } loop;

    int col, index, x;

    DressUp_LoadSpriteRes(icons->resources, resMans, HEAP_ID_14, narc, 103, 225, 102, 101, 2, 2000);
    DressUp_MakeSpriteHeader(icons->resources, resMans, &header, 0);
    template.header = &header;
    template.whichScreen = NNS_G2D_VRAM_TYPE_2DSUB;
    loop.row = 0;
    template.priority = 0;
    template.heapID = HEAP_ID_14;
    template.spriteList = spriteList;
    loop.y = 104;
    index = loop.row;
    for (; loop.row < 2; loop.y += 18, loop.row++) {
        template.position.y = loop.y;
        template.position.y = (s32)((u32) template.position.y << FX32_SHIFT);
        template.position.y += 512 * FX32_ONE; // sub screen
        for (col = 0, x = 38; col < 10; x += 18, col++) {
            template.position.x = x;
            template.position.x = (s32)((u32) template.position.x << FX32_SHIFT);
            icons->sprites[index + col] = Sprite_Create(&template);
            Sprite_SetAnimCtrlSeq(icons->sprites[index + col], 1);
            if (col + index >= count) {
                Sprite_SetDrawFlag(icons->sprites[index + col], FALSE);
            }
        }
        index += 10;
    }
}

#pragma push
#pragma opt_lifetimes off

// icons below count show as used (seq 0), the others as free (seq 1)
void DressUpHud_SetCountIcons(DressUpCountIcons *icons, int count) {
    int i = icons->count;
    if (i < count) {
        for (; i < count; i++) {
            Sprite_SetAnimCtrlSeq(icons->sprites[i], 0);
        }
    } else if (i > count) {
        for (int j = i - 1; j >= count; j--) {
            Sprite_SetAnimCtrlSeq(icons->sprites[j], 1);
        }
    }
    icons->count = count;
}

#pragma pop

void DressUpHud_DestroyCountIcons(DressUpCountIcons *icons, GF_2DGfxResMan **resMans) {
    int i = 0;
    DressUpCountIcons *cursor = icons;
    do {
        Sprite_Delete(cursor->sprites[0]);
        i++;
        cursor = (DressUpCountIcons *)((u8 *)cursor + sizeof(Sprite *));
    } while (i < DRESS_UP_COUNT_ICON_NUM);
    DressUp_FreeSpriteRes(icons->resources, resMans);
}

void DressUp_LoadSpriteRes(SpriteResource **resources, GF_2DGfxResMan **resMans, enum HeapID heapID, NARC *narc, int charFileId, int plttFileId, int cellFileId, int animFileId, int plttCount, int baseId) {
    resources[GF_GFX_RES_TYPE_CHAR] = AddCharResObjFromOpenNarc(resMans[GF_GFX_RES_TYPE_CHAR], narc, charFileId, FALSE, baseId + charFileId, NNS_G2D_VRAM_TYPE_2DSUB, heapID);
    SpriteTransfer_CreateCharTransferTask_AllocAtEnd(resources[GF_GFX_RES_TYPE_CHAR]);
    GF2DGfxResObj_FreeData(resources[GF_GFX_RES_TYPE_CHAR]);

    resources[GF_GFX_RES_TYPE_PLTT] = AddPlttResObjFromOpenNarc(resMans[GF_GFX_RES_TYPE_PLTT], narc, plttFileId, FALSE, baseId + plttFileId, NNS_G2D_VRAM_TYPE_2DSUB, plttCount, heapID);
    SpriteTransfer_CreatePlttTransferTask(resources[GF_GFX_RES_TYPE_PLTT]);
    GF2DGfxResObj_FreeData(resources[GF_GFX_RES_TYPE_PLTT]);

    resources[GF_GFX_RES_TYPE_CELL] = AddCellOrAnimResObjFromOpenNarc(resMans[GF_GFX_RES_TYPE_CELL], narc, cellFileId, FALSE, baseId + cellFileId, GF_GFX_RES_TYPE_CELL, heapID);
    resources[GF_GFX_RES_TYPE_ANIM] = AddCellOrAnimResObjFromOpenNarc(resMans[GF_GFX_RES_TYPE_ANIM], narc, animFileId, FALSE, baseId + animFileId, GF_GFX_RES_TYPE_ANIM, heapID);
}

void DressUp_FreeSpriteRes(SpriteResource **resources, GF_2DGfxResMan **resMans) {
    SpriteTransfer_DeleteCharTransferTask(resources[GF_GFX_RES_TYPE_CHAR]);
    SpriteTransfer_DeletePlttTransferTask(resources[GF_GFX_RES_TYPE_PLTT]);
    DestroySingle2DGfxResObj(resMans[GF_GFX_RES_TYPE_CHAR], resources[GF_GFX_RES_TYPE_CHAR]);
    DestroySingle2DGfxResObj(resMans[GF_GFX_RES_TYPE_PLTT], resources[GF_GFX_RES_TYPE_PLTT]);
    DestroySingle2DGfxResObj(resMans[GF_GFX_RES_TYPE_CELL], resources[GF_GFX_RES_TYPE_CELL]);
    DestroySingle2DGfxResObj(resMans[GF_GFX_RES_TYPE_ANIM], resources[GF_GFX_RES_TYPE_ANIM]);
}

void DressUp_MakeSpriteHeader(SpriteResource **resources, GF_2DGfxResMan **resMans, SpriteResourcesHeader *header, int priority) {
    int charId = GF2DGfxResObj_GetResID(resources[GF_GFX_RES_TYPE_CHAR]);
    int plttId = GF2DGfxResObj_GetResID(resources[GF_GFX_RES_TYPE_PLTT]);
    int cellId = GF2DGfxResObj_GetResID(resources[GF_GFX_RES_TYPE_CELL]);
    CreateSpriteResourcesHeader(header, charId, plttId, cellId, GF2DGfxResObj_GetResID(resources[GF_GFX_RES_TYPE_ANIM]), -1, -1, FALSE, priority, resMans[GF_GFX_RES_TYPE_CHAR], resMans[GF_GFX_RES_TYPE_PLTT], resMans[GF_GFX_RES_TYPE_CELL], resMans[GF_GFX_RES_TYPE_ANIM], NULL, NULL);
}

void DressUpCountdown_Create(DressUpCountdown *countdown, SpriteList *spriteList, GF_2DGfxResMan **resMans, int seconds, BgConfig *bgConfig, int *output, NARC *narc) {
    SpriteResourcesHeader header;
    SimpleSpriteTemplate template;
    int i, x;

    DressUp_LoadSpriteRes(countdown->resources, resMans, HEAP_ID_14, narc, 229, 230, 228, 227, 2, 3000);
    DressUp_MakeSpriteHeader(countdown->resources, resMans, &header, 0);
    template.spriteList = spriteList;
    template.header = &header;
    template.whichScreen = NNS_G2D_VRAM_TYPE_2DSUB;
    template.heapID = HEAP_ID_14;
    template.position.y = 58 << FX32_SHIFT;
    template.priority = 0;
    template.position.y += 512 * FX32_ONE; // sub screen
    for (i = 0, x = 103; i < 2; x += 24, i++) {
        template.position.x = x;
        template.position.x <<= FX32_SHIFT;
        countdown->digits[i] = Sprite_Create(&template);
    }
    countdown->value = seconds;
    countdown->frames = seconds * 30;
    countdown->output = output;
    countdown->output[0] = seconds;
    countdown->output[2] = seconds;
    countdown->warning = FALSE;
    DressUpCountdown_DrawDigits(countdown);
    DressUpHud_AddWindow(&countdown->window, bgConfig, 10, 8, 14, 4, 193, FALSE);
    FillWindowPixelBuffer(countdown->window, 0);
    DressUp_PrintMsg(countdown->window, NARC_msgdata_msg, NARC_msg_msg_0215_bin, msg_0215_00004, 0, 4, MAKE_TEXT_COLOR(1, 2, 0), TEXT_SPEED_NOTRANSFER);
    DressUp_PrintMsg(countdown->window, NARC_msgdata_msg, NARC_msg_msg_0215_bin, msg_0215_00005, 72, 4, MAKE_TEXT_COLOR(1, 2, 0), TEXT_SPEED_NOTRANSFER);
    CopyWindowToVram(countdown->window);
}

void DressUpCountdown_Destroy(DressUpCountdown *countdown, GF_2DGfxResMan **resMans) {
    if (countdown->tasks[0]) {
        SysTask_Destroy(countdown->tasks[0]);
    }
    if (countdown->tasks[1]) {
        SysTask_Destroy(countdown->tasks[1]);
    }
    DressUp_FreeSpriteRes(countdown->resources, resMans);
    DressUpHud_DeleteWindow(countdown->window);
    memset(countdown, 0, sizeof(DressUpCountdown));
}

void DressUpCountdown_Update(DressUpCountdown *countdown) {
    DressUpCountdown_DecFrames(countdown);
    DressUpCountdown_Sync(countdown);
    DressUpCountdown_DrawDigits(countdown);
    DressUpCountdown_UpdatePulse(&countdown->pulse);
}

// follows the owner's seconds counter; each change in the last ten seconds pulses the digits
void DressUpCountdown_Sync(DressUpCountdown *countdown) {
    s32 seconds = countdown->output[0];
    if (countdown->value != seconds) {
        countdown->value = seconds;
        if (seconds <= 10) {
            DressUpCountdown_StartPulse(countdown, &countdown->pulse);
            PlaySE(SEQ_SE_DP_HYUN2);
        }
    }
}

#pragma push
#pragma opt_treetransformation off

void DressUpCountdown_DrawDigits(DressUpCountdown *countdown) {
    struct {
        volatile int value, digit;
    } state;

    state.value = countdown->value;
    int divisor = 1;
    int i;
    DressUpCountdown *cursor;
    divisor *= 10;
    i = 0;
    cursor = countdown;
    for (; i < 2; i++) {
        int quotient = state.value / divisor;
        state.digit = quotient;
        if (quotient > 10) {
            GF_AssertFail();
        }
        Sprite_SetAnimCtrlSeq(cursor->digits[0], state.digit);
        u32 consumed = (u32)state.digit * (u32)divisor;
        state.value = (s32)((u32)state.value - consumed);
        divisor /= 10;
        if (countdown->warning == FALSE && countdown->value <= 10) {
            Sprite_SetPalIndexRespectVramOffset(cursor->digits[0], 1);
            if (i == 1) {
                countdown->warning = TRUE;
            }
        }
        cursor = (DressUpCountdown *)((u8 *)cursor + sizeof(Sprite *));
    }
}

#pragma pop

void DressUpCountdown_DecFrames(DressUpCountdown *countdown) {
    s32 frames = countdown->frames - 1;
    if (frames >= 0) {
        countdown->frames = frames;
    }
}

void DressUpTween_Init(DressUpTween *tween, fx32 start, fx32 end, int frames) {
    tween->value = start;
    tween->start = start;
    tween->distance = end - start;
    tween->frames = frames;
    tween->frame = 0;
}

BOOL DressUpTween_Step(DressUpTween *tween) {
    int next;
    fx32 value;
    value = FX_Div((fx32)(((s64)tween->distance * (tween->frame << FX32_SHIFT) + FX32_HALF) >> FX32_SHIFT), tween->frames << FX32_SHIFT);
    tween->value = value + tween->start;
    next = tween->frame + 1;
    if (next <= tween->frames) {
        tween->frame = next;
        return FALSE;
    }
    tween->frame = tween->frames;
    return TRUE;
}

// digits grow by 0.15 per second below ten, then shrink back over 16 frames around their centre
void DressUpCountdown_StartPulse(DressUpCountdown *countdown, DressUpCountdownPulse *pulse) {
    VecFx32 pos;
    fx32 scale;
    int i;

    pos.y = 58 << FX32_SHIFT;
    pos.y += 512 * FX32_ONE; // sub screen
    for (i = 0; i < 2; i++) {
        pulse->digits[i] = countdown->digits[i];
        Sprite_SetAffineOverwriteMode(pulse->digits[i], 2);
        pos.x = 103 + 24 * i;
        pos.x <<= FX32_SHIFT;
        pulse->positions[i] = pos;
    }

    i = 10 - countdown->value;
    if (i > 0) {
        scale = FX_Mul(i << FX32_SHIFT, 0x266) + FX32_ONE;
    } else {
        scale = FX32_ONE;
    }
    DressUpTween_Init(&pulse->scale, scale, FX32_ONE, 16);
    scale = FX_Mul(scale, 0x18000) - 0x18000;
    DressUpTween_Init(&pulse->x, scale, 0, 16);
    DressUpTween_Init(&pulse->y, scale, 0, 16);
    pulse->active = TRUE;
}

void DressUpCountdown_UpdatePulse(DressUpCountdownPulse *pulse) {
    VecFx32 scale;
    VecFx32 pos;
    BOOL done;
    if (pulse->active == FALSE) {
        return;
    }
    done = DressUpTween_Step(&pulse->scale);
    DressUpTween_Step(&pulse->x);
    DressUpTween_Step(&pulse->y);
    scale.x = pulse->scale.value;
    scale.y = pulse->scale.value;
    scale.z = pulse->scale.value;
    Sprite_SetAffineScale(pulse->digits[0], &scale);
    Sprite_SetAffineScale(pulse->digits[1], &scale);
    pos = pulse->positions[0];
    pos.x -= pulse->x.value;
    pos.y -= pulse->y.value;
    Sprite_SetMatrix(pulse->digits[0], &pos);
    pos = pulse->positions[1];
    pos.y -= pulse->y.value;
    Sprite_SetMatrix(pulse->digits[1], &pos);
    if (done) {
        pulse->active = FALSE;
    }
}

// the backdrop alternates with its second screen (file backdrop * 4 + 0x89) every 16 ticks
void DressUp_StartBackdropAnim(DressUpBackdropAnim *anim, DressUpCanvas *canvas, enum HeapID heapID) {
    DressUpBgScreenAnimTemplate template;
    anim->canvas = canvas;
    template.screen = &canvas->backdrop;
    template.altScreenFileId = canvas->backdropId * 4 + 0x89;
    template.delay = 16;
    template.heapID = heapID;
    DressUpBgScreen_InitAltAnim(&anim->anim, &template);
}

void DressUp_UpdateBackdropAnim(DressUpBackdropAnim *anim) {
    DressUpBgScreen_UpdateAltAnim(&anim->anim, 1);
}

void DressUp_StopBackdropAnim(DressUpBackdropAnim *anim) {
    DressUpBgScreen_StopAltAnim(&anim->anim);
    memset(anim, 0, sizeof(DressUpBackdropAnim));
}
