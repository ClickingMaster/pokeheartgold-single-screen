#ifndef POKEHEARTGOLD_APPLICATION_DRESS_UP_DRESS_UP_INTERNAL_H
#define POKEHEARTGOLD_APPLICATION_DRESS_UP_DRESS_UP_INTERNAL_H

#include "global.h"

#include "bg_window.h"
#include "filesystem.h"
#include "gf_3d_loader.h"
#include "graphics_object_manager.h"
#include "heap.h"
#include "pokepic.h"
#include "sprite.h"
#include "touchscreen.h"
#include "unk_02009D48.h"
#include "unk_0200A090.h"

#define DRESS_UP_IMAGE_MAX     0x76 // 100 accessories + 18 backdrops
#define DRESS_UP_PALETTE_MAX   0x13 // the accessory palette + 18 backdrop palettes
#define DRESS_UP_HIT_MAP_MAX   0x77 // images + the mon
#define DRESS_UP_ACCESSORY_NUM 100
#define DRESS_UP_BACKDROP_NUM  18

// graphics core shared by the Dress-Up editor and the Accessory Portrait renderer (offset 0 of both works)
typedef struct DressUpGfx {
    GraphicsManager *graphicsManager; // 0x000
    GraphicsImage **images;           // 0x004
    int imageMax;                     // 0x008
    int imageCount;                   // 0x00C
    GraphicsPalette **palettes;       // 0x010
    int paletteMax;                   // 0x014
    int paletteCount;                 // 0x018
    BOOL graphicsManagerActive;       // 0x01C
    PokepicManager *pokepicManager;   // 0x020
    NNSGfdTexKey texKey;              // 0x024
    NNSGfdPlttKey plttKey;            // 0x028
    BOOL pokepicActive;               // 0x02C
    void *monPicAttrs;                // 0x030, NARC_a_0_2_6 member 0xEB
    GF_2DGfxRawResMan *hitMapResMan;  // 0x034
    NNSG2dCharacterData **hitMaps;    // 0x038, [accessory], [100 + backdrop], [0x76] mon
    int hitMapCount;                  // 0x03C
    BgConfig *bgConfig;               // 0x040
    SpriteList *spriteList;           // 0x044
    GF_2DGfxResMan *charResMan;       // 0x048
    GF_2DGfxResMan *plttResMan;       // 0x04C
    GF_2DGfxResMan *cellResMan;       // 0x050
    GF_2DGfxResMan *animResMan;       // 0x054
    G2dRenderer renderer;             // 0x058
    NARC *narc;                       // 0x180
} DressUpGfx;                         // size 0x184

// image and palette templates loaded into the graphics manager
typedef struct DressUpGfxResources {
    GraphicsImageTemplate *images;     // 0x00
    int imageCount;                    // 0x04
    GraphicsPaletteTemplate *palettes; // 0x08
    int paletteCount;                  // 0x0C
    GF_2DGfxRawResMan *charResMan;     // 0x10
    GF_2DGfxRawResMan *plttResMan;     // 0x14
} DressUpGfxResources;                 // size 0x18

typedef struct DressUpSlot {
    u32 id;
    GraphicsObject *object;
} DressUpSlot;

typedef struct DressUpSlotList {
    DressUpSlot *slots;
    int count;
} DressUpSlotList;

typedef struct DressUpSlotTemplate {
    DressUpSlotList *list;            // 0x00
    GraphicsManager *graphicsManager; // 0x04
    GraphicsImage *image;             // 0x08
    GraphicsPalette *palette;         // 0x0C
    int x;                            // 0x10
    int y;                            // 0x14
    u32 id;                           // 0x18
    u32 paletteSlot;                  // 0x1C
} DressUpSlotTemplate;

typedef struct DressUpScale {
    fx32 x;
    fx32 y;
    fx32 z;
} DressUpScale;

DressUpSlotList *DressUpGfx_CreateSlotList(int count, enum HeapID heapID);
void DressUpGfx_FreeSlotList(DressUpSlotList *list);
DressUpSlot *DressUpGfx_AddSlotObject(DressUpSlotTemplate *template);
BOOL DressUpGfx_IsTouchHeldOnSlot(DressUpSlot *slot);
BOOL DressUpGfx_HitTestSlotPixels(DressUpSlot *slot, int *x, int *y, NNSG2dCharacterData *hitMap);
void DressUpGfx_SetSlotPos(DressUpSlot *slot, s16 x, s16 y);
void DressUpGfx_GetSlotPos(DressUpSlot *slot, s32 *x, s32 *y);
void DressUpGfx_GetSlotSize(DressUpSlot *slot, s32 *width, s32 *height);
void DressUpGfx_SetSlotVisible(DressUpSlot *slot, BOOL visible);
void DressUpGfx_SetSlotPriority(DressUpSlot *slot, u32 depth);
void ov41_02246020(DressUpSlot *slot, int *x, int *y);
DressUpSlot *DressUpGfx_FindFreeSlot(DressUpSlotList *list);
GraphicsObject *DressUpGfx_CreateSlotGraphicsObject(const DressUpSlotTemplate *template);
void DressUpGfx_GetSlotHitbox(DressUpSlot *slot, TouchscreenHitbox *hitbox);
void DressUpGfx_InitDisplay(void);
void DressUpGfx_ResetDisplay(void);
void DressUp_InitGfxCore(DressUpGfx *gfx);
void DressUp_FreeGfxCore(DressUpGfx *gfx);
void DressUp_DrawFrame3D(DressUpGfx *gfx);
void DressUpGfx_UploadRawResTables(DressUpGfx *gfx, DressUpGfxResources *resources);
void DressUpGfx_FreeAllTexAndPltt(DressUpGfx *gfx);
Sprite *DressUpGfx_CreateSprite(DressUpGfx *gfx, int id, int x, int y, u32 priority, NNS_G2D_VRAM_TYPE whichScreen);
void DressUpGfx_LoadCharRes(DressUpGfx *gfx, NARC *narc, int fileId, BOOL compressed, int vram, int id);
void DressUpGfx_LoadPlttRes(DressUpGfx *gfx, NARC *narc, int fileId, BOOL compressed, int vram, int plttNum, int id);
void DressUpGfx_LoadCellRes(DressUpGfx *gfx, NARC *narc, int fileId, BOOL compressed, int id);
void DressUpGfx_LoadAnimRes(DressUpGfx *gfx, NARC *narc, int fileId, BOOL compressed, int id);
void DressUpGfx_FreeCharRes(DressUpGfx *gfx, int id);
void DressUpGfx_FreePlttRes(DressUpGfx *gfx, int id);
void DressUpGfx_FreeCellRes(DressUpGfx *gfx, int id);
void DressUpGfx_FreeAnimRes(DressUpGfx *gfx, int id);
void DressUp_LoadRawResTables(DressUpGfx *gfx, DressUpGfxResources *resources);
void DressUpGfx_DestroyRawResTables(DressUpGfxResources *resources);
NNSG2dCharacterData *DressUpGfx_StoreUnpackedChar(DressUpGfx *gfx, void *data, int index);
void DressUp_DisableDragWindow(void);
void DressUp_EnableDragWindow(void);
void DressUp_VBlank(DressUpGfx *gfx);
void AccessoryPortrait_AllocRawResTables(DressUpGfxResources *resources, enum HeapID heapID);
int DressUpGfx_CharPixelEquals(NNSG2dCharacterData *character, int x, int y, int value);
void AccessoryPortrait_InitGfxCore(DressUpGfx *gfx, GraphicsManagerTemplate *template, enum HeapID heapID);
void AccessoryPortrait_InitBg(DressUpGfx *gfx, BgConfig *bgConfig, enum HeapID heapID);
void AccessoryPortrait_FreeGfxCore(DressUpGfx *gfx);
void AccessoryPortrait_FreeBg(DressUpGfx *gfx);
void DressUpGfx_HandlePokepicLoad(DressUpGfx *gfx);
void AccessoryPortrait_DrawTransformed(DressUpGfx *gfx, int x, int y, u16 angle, const DressUpScale *scale);
void AccessoryPortrait_InitAppGfx(DressUpGfx *gfx);
void AccessoryPortrait_FreeAppGfx(DressUpGfx *gfx);
void AccessoryPortrait_AppVBlank(DressUpGfx *gfx);
void AccessoryPortrait_RenderSprites(DressUpGfx *gfx);
void DressUpGfx_SetVramBanks(void);
void DressUpGfx_Init3D(void);
void DressUpGfx_Init2DModes(void);
void DressUpGfx_ResetFrameVram(void);
void DressUpGfx_DisablePlanes(void);
void DressUpGfx_CreateGraphicsManager(DressUpGfx *gfx, GraphicsManagerTemplate *template);
void DressUpGfx_DestroyGraphicsManager(DressUpGfx *gfx);
void DressUpGfx_DrawGraphicsManager(DressUpGfx *gfx);
void DressUpGfx_AddImages(DressUpGfx *gfx, GraphicsImageTemplate *templates, int count);
void DressUpGfx_AddPalettes(DressUpGfx *gfx, GraphicsPaletteTemplate *templates, int count);
void DressUpGfx_InitBgs(DressUpGfx *gfx);
void DressUpGfx_FreeBgs(DressUpGfx *gfx);
void DressUp_CreateCharRawResMan(DressUpGfx *gfx);
void DressUp_DestroyCharRawResMan(DressUpGfx *gfx);
void DressUpGfx_InitSpriteSystem(DressUpGfx *gfx);
void DressUpGfx_FreeSpriteSystem(DressUpGfx *gfx);
void DressUpGfx_RenderSprites(DressUpGfx *gfx);
void DressUp_LoadAccessoryChars(DressUpGfx *gfx, DressUpGfxResources *resources);
void DressUp_LoadBackdropCharsAndPalettes(DressUpGfx *gfx, DressUpGfxResources *resources);
void ov41_02246C90(DressUpGfx *gfx, enum HeapID heapID);
void ov41_02246CB0(DressUpGfx *gfx);
void DressUpGfx_CreatePokepicManager(DressUpGfx *gfx, enum HeapID heapID, u32 texSize, u32 plttSize);
void DressUpGfx_DestroyPokepicManager(DressUpGfx *gfx);
void DressUpGfx_AllocRawResTables(DressUpGfxResources *resources, int imageCount, int paletteCount, enum HeapID heapID);
void DressUpGfx_FreeRawResTables(DressUpGfxResources *resources);

#endif // POKEHEARTGOLD_APPLICATION_DRESS_UP_DRESS_UP_INTERNAL_H
