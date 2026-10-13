#include "application/dress_up/accessory_portrait.h"

#include "global.h"

#include "constants/sndseq.h"

#include "application/dress_up/dress_up_internal.h"
#include "msgdata/msg.naix"
#include "msgdata/msg/msg_0215.h"

#include "assert.h"
#include "bg_window.h"
#include "fashion_case.h"
#include "filesystem.h"
#include "font.h"
#include "gf_3d_loader.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "message_format.h"
#include "msgdata.h"
#include "overlay_41.h"
#include "overlay_manager.h"
#include "pm_string.h"
#include "pokemon.h"
#include "screen_fade.h"
#include "sound_playback.h"
#include "sprite.h"
#include "system.h"
#include "text.h"
#include "touchscreen.h"
#include "swap_3d_buffers.h"

// Accessory Portrait: the renderer of a saved dress-up design (also used by the VS Recorder) and the app that shows
// one design with its caption until a key or the touch screen is pressed.

// save_fashion.c
typedef struct SaveFashionMon SaveFashionMon;
BOOL FashionEntry_IsItemPlaced(SaveFashionDataSub *entry, int index);
SaveFashionMon *FashionEntry_GetMon(SaveFashionDataSub *entry);
u8 *FashionEntry_GetItem(SaveFashionDataSub *entry, int index);
void FashionEntry_GetMonSecondText(SaveFashionDataSub *entry, String *string);
u8 FashionEntry_GetBackdrop(SaveFashionDataSub *entry);
u16 FashionEntry_GetMailMessageWord(SaveFashionDataSub *entry);
s8 FashionMon_GetZ(SaveFashionMon *mon);
u8 FashionMon_GetX(SaveFashionMon *mon);
u8 FashionMon_GetY(SaveFashionMon *mon);
void FashionMon_ToPokemon(SaveFashionMon *mon, Pokemon *pokemon);
u8 FashionItem_GetId(u8 *data);
u8 FashionItem_GetX(u8 *data);
u8 FashionItem_GetY(u8 *data);
s8 FashionItem_GetZ(s8 *data);

AccessoryPortrait *AccessoryPortrait_Create(const AccessoryPortraitConfig *config, SaveFashionDataSub *fashionData) {
    AccessoryPortraitArgs args;
    AccessoryPortrait_CopyConfig(&args, config);
    AccessoryPortrait_ReadFashionData(&args, fashionData);
    return AccessoryPortrait_CreateFromArgs(&args);
}

void AccessoryPortrait_Draw(AccessoryPortrait *portrait) {
    if (portrait->bgEnabled == TRUE) {
        DressUp_UpdateBackdropAnim(&portrait->backdropAnim);
    }
    if (portrait->monVisible == TRUE) {
        AccessoryPortrait_DrawMon(portrait);
    }
}

void AccessoryPortrait_Destroy(AccessoryPortrait *portrait) {
    if (portrait->bgEnabled != FALSE) {
        AccessoryPortrait_ClearBackground(portrait);
    }
    AccessoryPortrait_FreeCanvas(portrait);
    DressUpGfx_FreeSlotList(portrait->slotList);
    portrait->slotList = NULL;
    DressUpNodePool_Free(&portrait->nodePool);
    AccessoryPortrait_FreeGfx(portrait);
    Heap_Free(portrait->mon);
    Heap_Free(portrait);
}

void AccessoryPortrait_VBlank(AccessoryPortrait *portrait) {
    DressUpGfx_HandlePokepicLoad(&portrait->gfx);
}

void AccessoryPortrait_SetMonVisible(AccessoryPortrait *portrait, BOOL visible) {
    portrait->monVisible = visible;
}

void AccessoryPortrait_SetPosition(AccessoryPortrait *portrait, int x, int y) {
    DressUpCanvas_SetOrigin(&portrait->canvas, portrait->x, portrait->y, x, y);
    if (portrait->bgEnabled != FALSE) {
        BgSetPosTextAndCommit(portrait->gfx.bgConfig, GF_BG_LYR_MAIN_2, BG_POS_OP_SET_X, 0x88 - x);
        BgSetPosTextAndCommit(portrait->gfx.bgConfig, GF_BG_LYR_MAIN_2, BG_POS_OP_SET_Y, 0x10 - y);
    }
    portrait->x = x;
    portrait->y = y;
}

AccessoryPortrait *AccessoryPortrait_CreateFromArgs(AccessoryPortraitArgs *args) {
    AccessoryPortrait *portrait = Heap_Alloc(args->heapID, sizeof(AccessoryPortrait));
    memset(portrait, 0, sizeof(AccessoryPortrait));
    portrait->heapID = args->heapID;
    portrait->mon = AllocMonZeroed(args->heapID);
    FashionMon_ToPokemon(args->mon, portrait->mon);
    AccessoryPortrait_InitGfx(portrait, args);
    AccessoryPortrait_InitBg(&portrait->gfx, args->bgConfig, args->heapID);
    DressUpNodePool_Init(&portrait->nodePool, 0x15, args->heapID);
    portrait->slotList = DressUpGfx_CreateSlotList(0x14, args->heapID);
    AccessoryPortrait_InitCanvas(portrait, args);
    AccessoryPortrait_SetBackground(portrait, args);
    DressUp_StartBackdropAnim(&portrait->backdropAnim, &portrait->canvas, args->heapID);
    portrait->bgEnabled = TRUE;
    portrait->monVisible = TRUE;
    AccessoryPortrait_InitTransform(portrait, args);
    return portrait;
}

void AccessoryPortrait_InitGfx(AccessoryPortrait *portrait, AccessoryPortraitArgs *args) {
    DressUpGfxResources resources;
    GraphicsManagerTemplate template;
    portrait->gfx.narc = NARC_New(NARC_a_0_2_6, args->heapID);
    template.objectCount = 0x2CE;
    template.imageCount = DRESS_UP_IMAGE_MAX;
    template.paletteCount = DRESS_UP_PALETTE_MAX;
    template.heapId = args->heapID;
    AccessoryPortrait_InitGfxCore(&portrait->gfx, &template, args->heapID);
    AccessoryPortrait_LoadAccessoryGfx(portrait, &resources, args, args->heapID);
    DressUpGfx_UploadRawResTables(&portrait->gfx, &resources);
    DressUpGfx_DestroyRawResTables(&resources);
}

void AccessoryPortrait_DrawMon(AccessoryPortrait *portrait) {
    AccessoryPortrait_DrawTransformed(&portrait->gfx, portrait->x + portrait->monOffsetX, portrait->y + portrait->monOffsetY, portrait->rotation, &portrait->scale);
}

void AccessoryPortrait_FreeGfx(AccessoryPortrait *portrait) {
    DressUpGfx_FreeAllTexAndPltt(&portrait->gfx);
    AccessoryPortrait_FreeGfxCore(&portrait->gfx);
    if (portrait->bgEnabled != FALSE) {
        AccessoryPortrait_FreeBg(&portrait->gfx);
    }
    NARC_Delete(portrait->gfx.narc);
}

// the mon and every saved accessory at their saved positions
void AccessoryPortrait_InitCanvas(AccessoryPortrait *portrait, AccessoryPortraitArgs *args) {
    DressUpCanvasTemplate template = { 0 };
    PokepicTemplate pokepicTemplate;
    int i;

    template.graphicsManager = portrait->gfx.graphicsManager;
    template.images = portrait->gfx.images;
    template.palettes = portrait->gfx.palettes;
    template.paletteSlots = portrait->gfx.monPicAttrs;
    template.pokepicManager = portrait->gfx.pokepicManager;
    template.bgConfig = portrait->gfx.bgConfig;
    template.slotList = portrait->slotList;
    template.nodePool = &portrait->nodePool;
    template.limit = 0x15;
    DressUpCanvas_Init(&portrait->canvas, &template);
    AccessoryPortrait_CreateCanvasMonPic(&portrait->canvas, portrait->mon, &pokepicTemplate, FashionMon_GetX(args->mon), FashionMon_GetY(args->mon), FashionMon_GetZ(args->mon), args->heapID);
    for (i = 0; i < args->count; i++) {
        AccessoryPortrait_PlaceCanvasAccessory(&portrait->canvas, FashionItem_GetId(args->items[i]), FashionItem_GetX(args->items[i]), FashionItem_GetY(args->items[i]), FashionItem_GetZ((s8 *)args->items[i]));
    }
}

void AccessoryPortrait_SetBackground(AccessoryPortrait *portrait, AccessoryPortraitArgs *args) {
    DressUpCanvas_SetBackdrop(&portrait->canvas, args->background, args->heapID);
}

void AccessoryPortrait_FreeCanvas(AccessoryPortrait *portrait) {
    DressUpCanvas_ClearObjects(&portrait->canvas);
    DressUpCanvas_Destroy(&portrait->canvas);
}

void AccessoryPortrait_ClearBackground(AccessoryPortrait *portrait) {
    DressUpCanvas_FreeBackdrop(&portrait->canvas);
}

#pragma force_active on

void AccessoryPortrait_InitTransform(AccessoryPortrait *portrait, AccessoryPortraitArgs *args) {
    portrait->x = 72;
    portrait->y = 56;
    portrait->monOffsetX = 56;
    portrait->monOffsetY = 64;
    portrait->scale.x = FX32_ONE;
    portrait->scale.y = FX32_ONE;
    portrait->scale.z = FX32_ONE;
    portrait->rotation = 0;
    AccessoryPortrait_SetPosition(portrait, args->x, args->y);
    portrait->unk_25C = 0x7FFF;
}

#pragma force_active reset

void AccessoryPortrait_CopyConfig(AccessoryPortraitArgs *args, const AccessoryPortraitConfig *config) {
    args->bgConfig = config->bgConfig;
    args->x = config->x;
    args->y = config->y;
    args->heapID = config->heapID;
}

void AccessoryPortrait_ReadFashionData(AccessoryPortraitArgs *args, SaveFashionDataSub *fashionData) {
    int i;
    args->mon = FashionEntry_GetMon(fashionData);
    args->count = 0;
    for (i = 0; i < 10; i++) {
        if (FashionEntry_IsItemPlaced(fashionData, i)) {
            args->items[args->count] = FashionEntry_GetItem(fashionData, i);
            args->count++;
        }
    }
    args->background = FashionEntry_GetBackdrop(fashionData);
}

void AccessoryPortrait_LoadAccessoryGfx(AccessoryPortrait *portrait, DressUpGfxResources *resources, AccessoryPortraitArgs *args, enum HeapID heapID) {
    AccessoryPortrait_AllocRawResTables(resources, heapID);
    AccessoryPortrait_LoadAccessoryCharsAndPltt(portrait, resources, args, heapID);
}

// chars of the placed accessories only (NARC member id + 1), then the shared accessory palette (member 0)
void AccessoryPortrait_LoadAccessoryCharsAndPltt(AccessoryPortrait *portrait, DressUpGfxResources *resources, AccessoryPortraitArgs *args, enum HeapID heapID) {
    int i;
    int id;
    void *data;
    for (i = 0; i < args->count; i++) {
        id = FashionItem_GetId(args->items[i]);
        if (GF2dGfxRawResMan_DoesNotHaveObjWithId(resources->charResMan, id) == TRUE) {
            data = GfGfxLoader_LoadFromOpenNarc(portrait->gfx.narc, id + 1, FALSE, heapID, TRUE);
            GF2dGfxRawResMan_AllocObj(resources->charResMan, data, id);
            NNS_G2dGetUnpackedCharacterData(data, &resources->images[id].character);
            resources->images[id].manager = portrait->gfx.graphicsManager;
        }
    }
    data = GfGfxLoader_LoadFromOpenNarc(portrait->gfx.narc, 0, FALSE, heapID, TRUE);
    GF2dGfxRawResMan_AllocObj(resources->plttResMan, data, 0);
    NNS_G2dGetUnpackedPaletteData(data, &resources->palettes->palette);
    resources->palettes->manager = portrait->gfx.graphicsManager;
    resources->palettes->paletteCount = 3;
}

BOOL AccessoryPortrait_Init(OverlayManager *man, int *state) {
    AccessoryPortraitAppWork *work;
    FashionAppData *args;
    AccessoryPortraitConfig config;
    Heap_Create(HEAP_ID_3, HEAP_ID_13, 0x20000);
    Heap_Create(HEAP_ID_3, HEAP_ID_14, 0x40000);
    work = OverlayManager_CreateAndGetData(man, sizeof(AccessoryPortraitAppWork), HEAP_ID_13);
    memset(work, 0, sizeof(AccessoryPortraitAppWork));
    Main_SetVBlankIntrCB(AccessoryPortrait_VBlankCB, work);
    HBlankInterruptDisable();
    args = OverlayManager_GetArgs(man);
    work->fashionData = Save_FashionData_GetEntry(args->saveFashionData, args->entryIndex);
    work->selection = args->entryIndex;
    work->mode = args->mode;
    DressUpGfx_InitDisplay();
    gSystem.screensFlipped = FALSE;
    GfGfx_SwapDisplay();
    AccessoryPortrait_InitAppGfx(&work->gfx, HEAP_ID_14);
    config.bgConfig = work->gfx.bgConfig;
    config.x = 0x48;
    config.y = 0x10;
    config.heapID = HEAP_ID_14;
    work->portrait = AccessoryPortrait_Create(&config, work->fashionData);
    AccessoryPortrait_LoadFrameBg(work);
    AccessoryPortrait_LoadSubBg(work);
    AccessoryPortrait_CreateIconSprite(work);
    AccessoryPortrait_CreateCaptionWindow(work);
    AccessoryPortrait_DrawCaption(work);
    return TRUE;
}

BOOL AccessoryPortrait_Main(OverlayManager *man, int *state) {
    AccessoryPortraitAppWork *work = OverlayManager_GetData(man);
    Thunk_G3X_Reset();
    NNS_G2dSetupSoftwareSpriteCamera();
    AccessoryPortrait_Draw(work->portrait);
    RequestSwap3DBuffers(GX_SORTMODE_AUTO, GX_BUFFERMODE_Z);
    AccessoryPortrait_RenderSprites(&work->gfx);
    switch (*state) {
    case 0:
        (*state)++;
        break;
    case 1:
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_UNK_5, FADE_TYPE_BRIGHTNESS_IN, 0, 6, 1, HEAP_ID_13);
        (*state)++;
        break;
    case 2:
        if (IsPaletteFadeFinished()) {
            (*state)++;
        }
        break;
    case 3:
        if ((gSystem.newKeys & (PAD_BUTTON_A | PAD_BUTTON_B)) || System_GetTouchNew()) {
            PlaySE(SEQ_SE_DP_DECIDE);
            (*state)++;
        }
        break;
    case 4:
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_DOWNWARD_OUT, FADE_TYPE_BRIGHTNESS_OUT, 0, 6, 1, HEAP_ID_13);
        (*state)++;
        break;
    case 5:
        if (IsPaletteFadeFinished()) {
            return TRUE;
        }
        break;
    }
    return FALSE;
}

BOOL AccessoryPortrait_Exit(OverlayManager *man, int *state) {
    AccessoryPortraitAppWork *work = OverlayManager_GetData(man);
    AccessoryPortrait_Destroy(work->portrait);
    AccessoryPortrait_FreeIconSprite(work);
    AccessoryPortrait_FreeCaptionWindow(work);
    AccessoryPortrait_FreeAppGfx(&work->gfx);
    DressUpGfx_ResetDisplay();
    Main_SetVBlankIntrCB(NULL, NULL);
    HBlankInterruptDisable();
    OverlayManager_FreeData(man);
    Heap_Destroy(HEAP_ID_13);
    Heap_Destroy(HEAP_ID_14);
    return TRUE;
}

void AccessoryPortrait_VBlankCB(void *data) {
    AccessoryPortraitAppWork *work = data;
    AccessoryPortrait_VBlank(work->portrait);
    AccessoryPortrait_AppVBlank(&work->gfx);
}

// photo frame on BG1 (NARC_a_0_2_6 pal 0x7E, chars 0x7D, screen 0x80)
void AccessoryPortrait_LoadFrameBg(AccessoryPortraitAppWork *work) {
    NNSG2dScreenData *screen;
    void *data;
    GfGfxLoader_GXLoadPalFromOpenNarc(work->gfx.narc, 0x7E, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_3_OFFSET, 0x40, HEAP_ID_14);
    GfGfxLoader_LoadCharDataFromOpenNarc(work->gfx.narc, 0x7D, work->gfx.bgConfig, GF_BG_LYR_MAIN_1, 0, 0, FALSE, HEAP_ID_14);
    data = GfGfxLoader_GetScrnData(NARC_a_0_2_6, 0x80, FALSE, &screen, HEAP_ID_14);
    LoadRectToBgTilemapRect(work->gfx.bgConfig, GF_BG_LYR_MAIN_1, screen->rawData, 0, 0, (u8)(screen->screenWidth >> 3), (u8)(screen->screenHeight >> 3));
    BgTilemapRectChangePalette(work->gfx.bgConfig, GF_BG_LYR_MAIN_1, 0, 0, (u8)(screen->screenWidth >> 3), (u8)(screen->screenHeight >> 3), 4);
    Heap_Free(data);
    ScheduleBgTilemapBufferTransfer(work->gfx.bgConfig, GF_BG_LYR_MAIN_1);
}

void AccessoryPortrait_LoadSubBg(AccessoryPortraitAppWork *work) {
    GfGfxLoader_GXLoadPal(NARC_a_2_3_7, 0, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_0_OFFSET, 0, HEAP_ID_14);
    GfGfxLoader_LoadScrnData(NARC_a_2_3_7, 9, work->gfx.bgConfig, GF_BG_LYR_SUB_0, 0, 0, FALSE, HEAP_ID_14);
    GfGfxLoader_LoadCharData(NARC_a_2_3_7, 1, work->gfx.bgConfig, GF_BG_LYR_SUB_0, 0, 0, FALSE, HEAP_ID_14);
}

void AccessoryPortrait_CreateIconSprite(AccessoryPortraitAppWork *work) {
    DressUpGfx_LoadCharRes(&work->gfx, work->gfx.narc, 0xE9, FALSE, 1, 1000);
    DressUpGfx_LoadPlttRes(&work->gfx, work->gfx.narc, 0xEA, FALSE, 1, 6, 1000);
    DressUpGfx_LoadCellRes(&work->gfx, work->gfx.narc, 0xE8, FALSE, 1000);
    DressUpGfx_LoadAnimRes(&work->gfx, work->gfx.narc, 0xE7, FALSE, 1000);
    work->icon = DressUpGfx_CreateSprite(&work->gfx, 1000, 0, 0x90, 0x64, NNS_G2D_VRAM_TYPE_2DMAIN);
    Sprite_SetPriority(work->icon, 1);
}

void AccessoryPortrait_FreeIconSprite(AccessoryPortraitAppWork *work) {
    DressUpGfx_FreeCharRes(&work->gfx, 1000);
    DressUpGfx_FreePlttRes(&work->gfx, 1000);
    DressUpGfx_FreeCellRes(&work->gfx, 1000);
    DressUpGfx_FreeAnimRes(&work->gfx, 1000);
    Sprite_Delete(work->icon);
}

void AccessoryPortrait_CreateCaptionWindow(AccessoryPortraitAppWork *work) {
    work->captionWindow = AllocWindows(HEAP_ID_14, 1);
    AddWindowParameterized(work->gfx.bgConfig, work->captionWindow, GF_BG_LYR_MAIN_3, 0, 0x12, 0x20, 6, 5, 1);
    LoadFontPal0(GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_5_OFFSET, HEAP_ID_14);
    SetBgPriority(GF_BG_LYR_MAIN_3, 0);
    SetBgPriority(GF_BG_LYR_MAIN_0, 2);
    SetBgPriority(GF_BG_LYR_MAIN_1, 1);
    BgSetPosTextAndCommit(work->gfx.bgConfig, GF_BG_LYR_MAIN_3, BG_POS_OP_SET_Y, 0);
}

void AccessoryPortrait_FreeCaptionWindow(AccessoryPortraitAppWork *work) {
    ClearWindowTilemapAndCopyToVram(work->captionWindow);
    RemoveWindow(work->captionWindow);
    WindowArray_Delete(work->captionWindow, 1);
}

void AccessoryPortrait_DrawCaption(AccessoryPortraitAppWork *work) {
    FillWindowPixelBuffer(work->captionWindow, 0);
    AccessoryPortrait_PrintCaption(work);
    CopyWindowToVram(work->captionWindow);
}

// the mon's name and the saved word ("{STRVAR_1 28}"), both centred
void AccessoryPortrait_PrintCaption(AccessoryPortraitAppWork *work) {
    MsgData *msgData = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, NARC_msg_msg_0215_bin, HEAP_ID_13);
    MessageFormat *msgFormat;
    String *string;
    String *template;
    VecFx32 pos;
    int width;
    GF_ASSERT(msgData != NULL);
    msgFormat = MessageFormat_New(HEAP_ID_13);
    Sprite_SetAnimCtrlSeq(work->icon, 5);
    pos.x = 48 * FX32_ONE;
    pos.y = 144 * FX32_ONE;
    pos.z = 0;
    Sprite_SetMatrix(work->icon, &pos);
    string = String_New(12, HEAP_ID_13);
    FashionEntry_GetMonSecondText(work->fashionData, string);
    width = FontID_String_GetWidth(0, string, 0);
    AddTextPrinterParameterizedWithColor(work->captionWindow, 0, string, 0x80 - width / 2, 7, TEXT_SPEED_INSTANT, MAKE_TEXT_COLOR(1, 2, 0), NULL);
    String_Delete(string);
    BufferECWord(msgFormat, 0, FashionEntry_GetMailMessageWord(work->fashionData));
    string = String_New(200, HEAP_ID_13);
    template = NewString_ReadMsgData(msgData, msg_0215_00045);
    StringExpandPlaceholders(msgFormat, string, template);
    width = FontID_String_GetWidth(0, string, 0);
    AddTextPrinterParameterizedWithColor(work->captionWindow, 0, string, 0x80 - width / 2, 0x1B, TEXT_SPEED_INSTANT, MAKE_TEXT_COLOR(1, 2, 0), NULL);
    String_Delete(string);
    String_Delete(template);
    DestroyMsgData(msgData);
    MessageFormat_Delete(msgFormat);
}
