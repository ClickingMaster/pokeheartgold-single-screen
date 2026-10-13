#include "application/plaza_news.h"

#include "global.h"

#include "bg_window.h"
#include "filesystem.h"
#include "font.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "msgdata.h"
#include "obj_char_transfer.h"
#include "obj_pltt_transfer.h"
#include "palette.h"
#include "pm_string.h"
#include "screen_fade.h"
#include "sprite.h"
#include "system.h"
#include "text.h"
#include "sound_playback.h"
#include "unk_02009D48.h"
#include "sprite_resource.h"
#include "oam_manager.h"
#include "wireless_status_icon.h"
#include "vram_transfer_manager.h"

// Hours, minutes and seconds as kept by the lobby (overlay 45), copied as one word.
typedef union PlazaNewsTime {
    u32 word;
    struct {
        u8 hour;
        u8 minute;
        u8 second;
        u8 unk_03;
    } hms;
} PlazaNewsTime;

typedef struct PlazaNewsArgs {
    void *newsData;
    const PlazaNewsTime *unk_04;
    void *lobby;
} PlazaNewsArgs;

typedef struct PlazaNewsGraphics {
    BgConfig *bgConfig;
    SpriteList *spriteList;
    G2dRenderer renderer;
    GF_2DGfxResMan *resourceManagers[4];
    NARC *narc;
} PlazaNewsGraphics;

// Screen data the clock gauge and the people grid copy their cells from.
typedef struct PlazaNewsTileset {
    void *rawData;
    NNSG2dScreenData *screenData;
} PlazaNewsTileset;

// TIME panel: a gauge of 40 cells (10 columns, 4 rows) counting down the plaza time.
typedef struct PlazaNewsClock {
    PlazaNewsTime time;
    u8 isWaiting;
    u8 needsRedraw;
    u16 blinkTimer;
    u32 secondsPerCell;
} PlazaNewsClock;

// PLAZA PEOPLE panel: a grid of 5x4 visitor icons.
typedef struct PlazaNewsPeople {
    s16 frame;
    u16 state;
} PlazaNewsPeople;

// One scrolling line of the LATEST NEWS HEADLINES panel.
typedef struct PlazaNewsTicker {
    u8 active;
    u8 paletteNum;
    u16 position;
    u16 duration;
    u16 width;
    String *text;
    Window bitmap;
    u8 nameColors[4];
} PlazaNewsTicker;

typedef struct PlazaNewsHeadlines {
    PlazaNewsTicker tickers[3];
    Window windows[3];
    String *string;
    void *paletteFile;
    NNSG2dPaletteData *palette;
} PlazaNewsHeadlines;

// Palette flash (with a sound effect) played on a panel title when its contents change.
typedef struct PlazaNewsFlash {
    u16 active;
    u16 timer;
    u16 color;
    u16 paletteOffset;
    u32 sound;
} PlazaNewsFlash;

typedef struct PlazaNewsTitles {
    Window windows[4];
    PlazaNewsFlash flashes[4];
} PlazaNewsTitles;

typedef struct PlazaNewsTitleTemplate {
    u8 left;
    u8 top;
    u8 width;
    u8 height;
    u16 baseTile;
    u8 palette;
    u8 x;
    u8 y;
    u8 alignment;
    u8 filler_0A[2];
    u32 color;
} PlazaNewsTitleTemplate;

typedef struct PlazaNewsWork {
    void *newsData;
    const PlazaNewsTime *unk_04;
    PlazaNewsGraphics graphics;
    PlazaNewsTileset tileset;
    PlazaNewsClock clock;
    PlazaNewsPeople people;
    PlazaNewsHeadlines headlines;
    PlazaNewsTitles titles;
} PlazaNewsWork;

// Lobby (overlay 45) interface.
int WiFiPlazaWork_IsClosing(void *lobby);
void ov45_0222A4A8(void *lobby);
void WiFiPlazaWork_SetLowVolume(void *lobby, int a1);
int WiFiPlazaWork_IsSpecialVisitor(void *lobby, int index);
void WiFiPlazaWork_GetSpecialVisitors(void *lobby, void *out);
u32 WiFiPlazaWork_GetOpenDuration(void *lobby);
void WiFiPlazaTime_Subtract(const PlazaNewsTime *first, const PlazaNewsTime *second, PlazaNewsTime *out);
int WiFiPlaza_HasConnectionFailed(void);
void WiFiPlazaNews_RemoveCurrentEvent(void *newsData);
int WiFiPlazaNews_GetOpenTime(void *newsData, PlazaNewsTime *out);
int WiFiPlazaNews_IsSlotOccupied(void *newsData, int index);
int WiFiPlazaNews_GetSlotGender(void *newsData, int index);
int WiFiPlazaNews_IsOwnSlot(void *newsData, int index);
int WiFiPlazaNews_IsOldestSlot(void *newsData, int index);
int WiFiPlazaNews_IsLastJoinedSlot(void *newsData, int index);
int WiFiPlazaNews_GetSlotChange(void *newsData);
BOOL WiFiPlazaNews_HasEvent(void *newsData);
void *WiFiPlazaNews_GetEventType(void *newsData);
int WiFiPlazaNews_FormatEvent(void *newsData, void *info, void *item, String *dest, enum HeapID heapID);
int WiFiPlazaNews_GetEventSlot(void *newsData, void *item, int index);

extern const u8 sPlazaNewsHeadlineRowTops[3];     // tilemap top of each headline row, followed by the row durations
extern const u32 sPlazaNewsLastHeadlineRowDuration[]; // duration of the last headline row (240); rows 0-1 come before it
extern const ObjCharTransferTemplate sPlazaNewsObjCharTransfer;
extern const int sPlazaNewsClockBlinkTiles[4];
extern const GraphicsModes sPlazaNewsGraphicsModes;
extern const u32 sPlazaNewsBgIds[5];
extern const GraphicsBanks sPlazaNewsBanks;
extern const PlazaNewsTitleTemplate sPlazaNewsTitleTemplates[4];
extern const BgTemplate sPlazaNewsBgTemplates[5];
extern const u16 sPlazaNewsTitlePaletteOffsets[4];
extern const u32 sPlazaNewsTitleSEs[4];

void PlazaNews_VBlankCallback(void *data);
void PlazaNews_Update(PlazaNewsWork *work, void *lobby);
void PlazaNewsGraphics_Init(PlazaNewsGraphics *graphics, enum HeapID heapID);
void PlazaNewsGraphics_Exit(PlazaNewsGraphics *graphics);
void PlazaNewsGraphics_DrawSprites(PlazaNewsGraphics *graphics);
void PlazaNewsGraphics_VBlank(PlazaNewsGraphics *graphics);
void PlazaNewsGraphics_InitBgs(PlazaNewsGraphics *graphics, enum HeapID heapID);
void PlazaNewsGraphics_FreeBgs(PlazaNewsGraphics *graphics);
void PlazaNewsGraphics_InitSprites(PlazaNewsGraphics *graphics, enum HeapID heapID);
void PlazaNewsGraphics_FreeSprites(PlazaNewsGraphics *graphics);
void PlazaNewsTileset_Load(PlazaNewsTileset *tileset, PlazaNewsGraphics *graphics, enum HeapID heapID);
void PlazaNewsTileset_Free(PlazaNewsTileset *tileset);
void PlazaNewsClock_DrawCell(PlazaNewsTileset *tileset, PlazaNewsGraphics *graphics, u32 tile, u8 col, u8 row);
void PlazaNewsPeople_DrawIcon(PlazaNewsTileset *tileset, PlazaNewsGraphics *graphics, u32 col, u32 row, u32 tile, u32 altRow, u32 usePalette3, u32 altPage, u32 usePalette4);
void PlazaNewsClock_Init(PlazaNewsClock *clock, PlazaNewsGraphics *graphics, PlazaNewsTileset *tileset, void *newsData, const PlazaNewsTime *a4, void *lobby, enum HeapID heapID);
void PlazaNewsClock_Free(PlazaNewsClock *clock);
BOOL PlazaNewsClock_UpdateTime(PlazaNewsClock *clock, void *newsData, const PlazaNewsTime *a2);
void PlazaNewsClock_Draw(PlazaNewsClock *clock, PlazaNewsTileset *tileset, PlazaNewsGraphics *graphics);
void PlazaNewsClock_DrawWaiting(PlazaNewsClock *clock, PlazaNewsTileset *tileset, PlazaNewsGraphics *graphics);
void PlazaNewsClock_DrawGauge(PlazaNewsClock *clock, PlazaNewsTileset *tileset, PlazaNewsGraphics *graphics);
void PlazaNewsPeople_Init(PlazaNewsPeople *people, PlazaNewsGraphics *graphics, PlazaNewsTileset *tileset, void *newsData, void *lobby, enum HeapID heapID);
void PlazaNewsPeople_Free(PlazaNewsPeople *people);
BOOL PlazaNewsPeople_UpdateState(PlazaNewsPeople *people, void *newsData);
void PlazaNewsPeople_Draw(PlazaNewsPeople *people, PlazaNewsTileset *tileset, PlazaNewsGraphics *graphics, void *newsData, void *lobby);
void PlazaNewsPeople_DrawGrid(PlazaNewsTileset *tileset, PlazaNewsGraphics *graphics, void *newsData, void *lobby, int mode);
void PlazaNewsPeople_BlinkMode2(PlazaNewsTileset *tileset, PlazaNewsGraphics *graphics, void *newsData, void *lobby, u32 frame);
void PlazaNewsPeople_BlinkMode1(PlazaNewsTileset *tileset, PlazaNewsGraphics *graphics, void *newsData, void *lobby, u32 frame);
void PlazaNewsHeadlines_GetNameColors(u8 *colors, void *newsData, void *item, void *lobby);
void PlazaNewsHeadlines_LoadNameColors(const u8 *colors, NNSG2dPaletteData *palette, int paletteNum);
void PlazaNewsHeadlines_Init(PlazaNewsHeadlines *headlines, PlazaNewsGraphics *graphics, enum HeapID heapID);
void PlazaNewsHeadlines_Free(PlazaNewsHeadlines *headlines);
BOOL PlazaNewsHeadlines_Update(PlazaNewsHeadlines *headlines, void *newsData, void *lobby, enum HeapID heapID);
void PlazaNewsHeadlines_Draw(PlazaNewsHeadlines *headlines);
void PlazaNewsTicker_Init(PlazaNewsTicker *ticker, PlazaNewsGraphics *graphics, int row, enum HeapID heapID);
void PlazaNewsTicker_Free(PlazaNewsTicker *ticker);
void PlazaNewsTicker_SetText(PlazaNewsTicker *ticker, String *text, u32 duration, u8 *nameColors, NNSG2dPaletteData *palette);
void PlazaNewsTicker_Stop(PlazaNewsTicker *ticker);
BOOL PlazaNewsTicker_Advance(PlazaNewsTicker *ticker);
void PlazaNewsTicker_Draw(PlazaNewsTicker *ticker, Window *window);
void PlazaNewsTitles_Init(PlazaNewsTitles *titles, PlazaNewsGraphics *graphics, enum HeapID heapID);
void PlazaNewsTitles_Free(PlazaNewsTitles *titles);
void PlazaNewsTitles_Update(PlazaNewsTitles *titles);
void PlazaNewsTitles_StartFlash(PlazaNewsTitles *titles, u32 index);
void PlazaNewsFlash_Init(PlazaNewsFlash *flash, u16 paletteOffset, u32 sound);
void PlazaNewsFlash_Clear(PlazaNewsFlash *flash);
void PlazaNewsFlash_Start(PlazaNewsFlash *flash);
void PlazaNewsFlash_Update(PlazaNewsFlash *flash);

BOOL PlazaNews_Init(OverlayManager *man, int *state) {
    PlazaNewsArgs *args;
    PlazaNewsWork *work;
#pragma unused(state)
    args = OverlayManager_GetArgs(man);
    Heap_Create(HEAP_ID_3, HEAP_ID_114, 5 << 16);
    work = OverlayManager_CreateAndGetData(man, sizeof(PlazaNewsWork), HEAP_ID_114);
    memset(work, 0, sizeof(PlazaNewsWork));
    work->newsData = args->newsData;
    work->unk_04 = args->unk_04;
    PlazaNewsGraphics_Init(&work->graphics, HEAP_ID_114);
    PlazaNewsTileset_Load(&work->tileset, &work->graphics, HEAP_ID_114);
    PlazaNewsClock_Init(&work->clock, &work->graphics, &work->tileset, work->newsData, work->unk_04, args->lobby, HEAP_ID_114);
    PlazaNewsPeople_Init(&work->people, &work->graphics, &work->tileset, work->newsData, args->lobby, HEAP_ID_114);
    PlazaNewsHeadlines_Init(&work->headlines, &work->graphics, HEAP_ID_114);
    PlazaNewsTitles_Init(&work->titles, &work->graphics, HEAP_ID_114);
    Main_SetVBlankIntrCB(PlazaNews_VBlankCallback, work);
    HBlankInterruptDisable();
    return TRUE;
}

BOOL PlazaNews_Main(OverlayManager *man, u32 *state) {
    PlazaNewsWork *work = OverlayManager_GetData(man);
    PlazaNewsArgs *args = OverlayManager_GetArgs(man);
    switch (*state) {
    case 0:
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, RGB_BLACK, 6, 1, HEAP_ID_114);
        WiFiPlazaWork_SetLowVolume(args->lobby, 1);
        (*state)++;
        break;
    case 1:
        if (IsPaletteFadeFinished() == TRUE) {
            (*state)++;
        }
        break;
    case 2:
        if ((gSystem.newKeys & PAD_BUTTON_B) || WiFiPlazaWork_IsClosing(args->lobby) == 1 || WiFiPlaza_HasConnectionFailed() == 1) {
            if (WiFiPlazaWork_IsClosing(args->lobby) == 1) {
                ov45_0222A4A8(args->lobby);
            }
            *state = 3;
        } else {
            if (PlazaNewsClock_UpdateTime(&work->clock, work->newsData, work->unk_04) == TRUE) {
                PlazaNewsTitles_StartFlash(&work->titles, 1);
            }
            if (PlazaNewsPeople_UpdateState(&work->people, work->newsData) == TRUE) {
                PlazaNewsTitles_StartFlash(&work->titles, 2);
            }
            if (PlazaNewsHeadlines_Update(&work->headlines, work->newsData, args->lobby, HEAP_ID_114) == TRUE) {
                PlazaNewsTitles_StartFlash(&work->titles, 3);
            }
        }
        break;
    case 3:
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, RGB_BLACK, 6, 1, HEAP_ID_114);
        (*state)++;
        break;
    case 4:
        if (IsPaletteFadeFinished() == TRUE) {
            return TRUE;
        }
        break;
    }
    PlazaNews_Update(work, args->lobby);
    return FALSE;
}

BOOL PlazaNews_Exit(OverlayManager *man, int *state) {
    PlazaNewsWork *work;
#pragma unused(state)
    work = OverlayManager_GetData(man);
    OverlayManager_GetArgs(man);
    PlazaNewsTitles_Free(&work->titles);
    PlazaNewsClock_Free(&work->clock);
    PlazaNewsPeople_Free(&work->people);
    PlazaNewsHeadlines_Free(&work->headlines);
    PlazaNewsTileset_Free(&work->tileset);
    Main_SetVBlankIntrCB(NULL, NULL);
    HBlankInterruptDisable();
    PlazaNewsGraphics_Exit(&work->graphics);
    OverlayManager_FreeData(man);
    Heap_Destroy(HEAP_ID_114);
    return TRUE;
}

void PlazaNews_VBlankCallback(void *data) {
    PlazaNewsWork *work = data;
    PlazaNewsGraphics_VBlank(&work->graphics);
}

void PlazaNews_Update(PlazaNewsWork *work, void *lobby) {
    PlazaNewsClock_Draw(&work->clock, &work->tileset, &work->graphics);
    PlazaNewsPeople_Draw(&work->people, &work->tileset, &work->graphics, work->newsData, lobby);
    PlazaNewsHeadlines_Draw(&work->headlines);
    PlazaNewsTitles_Update(&work->titles);
    PlazaNewsGraphics_DrawSprites(&work->graphics);
}

void PlazaNewsGraphics_Init(PlazaNewsGraphics *graphics, enum HeapID heapID) {
    graphics->narc = NARC_New(NARC_a_1_9_6, heapID);
    GF_CreateVramTransferManager(0x30, heapID);
    GfGfx_SetBanks(&sPlazaNewsBanks);
    PlazaNewsGraphics_InitBgs(graphics, heapID);
    PlazaNewsGraphics_InitSprites(graphics, heapID);
}

void PlazaNewsGraphics_Exit(PlazaNewsGraphics *graphics) {
    NARC_Delete(graphics->narc);
    GF_DestroyVramTransferManager();
    PlazaNewsGraphics_FreeBgs(graphics);
    PlazaNewsGraphics_FreeSprites(graphics);
}

void PlazaNewsGraphics_DrawSprites(PlazaNewsGraphics *graphics) {
    SpriteList_RenderAndAnimateSprites(graphics->spriteList);
}

void PlazaNewsGraphics_VBlank(PlazaNewsGraphics *graphics) {
    DoScheduledBgGpuUpdates(graphics->bgConfig);
    OamManager_ApplyAndResetBuffers();
    GF_RunVramTransferTasks();
}

void PlazaNewsGraphics_InitBgs(PlazaNewsGraphics *graphics, enum HeapID heapID) {
    int i;

    SetBothScreensModesAndDisable(&sPlazaNewsGraphicsModes);
    graphics->bgConfig = BgConfig_Alloc(heapID);
    gSystem.screensFlipped = FALSE;
    GfGfx_SwapDisplay();
    for (i = 0; i < 5; i++) {
        InitBgFromTemplate(graphics->bgConfig, (u8)sPlazaNewsBgIds[i], &sPlazaNewsBgTemplates[i], 0);
        BG_ClearCharDataRange((u8)sPlazaNewsBgIds[i], 0x20, 0, heapID);
        BgClearTilemapBufferAndCommit(graphics->bgConfig, (u8)sPlazaNewsBgIds[i]);
    }
    GfGfxLoader_GXLoadPalFromOpenNarc(graphics->narc, 0, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_0_OFFSET, 0, heapID);
    LoadFontPal1(GF_PAL_LOCATION_MAIN_BG, (enum GFPalSlotOffset)0x140, heapID);
    GfGfxLoader_LoadCharDataFromOpenNarc(graphics->narc, 1, graphics->bgConfig, GF_BG_LYR_MAIN_1, 0, 0, FALSE, heapID);
    GfGfxLoader_LoadScrnDataFromOpenNarc(graphics->narc, 3, graphics->bgConfig, GF_BG_LYR_MAIN_1, 0, 0, FALSE, heapID);
    GfGfxLoader_LoadScrnDataFromOpenNarc(graphics->narc, 4, graphics->bgConfig, GF_BG_LYR_MAIN_2, 0, 0, FALSE, heapID);
    GfGfxLoader_GXLoadPalFromOpenNarc(graphics->narc, 0, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_0_OFFSET, 0, heapID);
    GfGfxLoader_LoadCharDataFromOpenNarc(graphics->narc, 2, graphics->bgConfig, GF_BG_LYR_SUB_0, 0, 0, FALSE, heapID);
    GfGfxLoader_LoadScrnDataFromOpenNarc(graphics->narc, 6, graphics->bgConfig, GF_BG_LYR_SUB_0, 0, 0, FALSE, heapID);
}

void PlazaNewsGraphics_FreeBgs(PlazaNewsGraphics *graphics) {
    int i;

    for (i = 0; i < 5; i++) {
        FreeBgTilemapBuffer(graphics->bgConfig, (u8)sPlazaNewsBgIds[i]);
    }
    Heap_Free(graphics->bgConfig);
    gSystem.screensFlipped = FALSE;
    GfGfx_SwapDisplay();
}

void PlazaNewsGraphics_InitSprites(PlazaNewsGraphics *graphics, enum HeapID heapID) {
    int i;
    u32 mode = 0x00200010;

    NNS_G2dInitOamManagerModule();
    OamManager_Create(0, 0x7E, 0, 0x1F, 0, 0x7E, 0, 0x1F, heapID);
    ObjCharTransfer_InitEx((ObjCharTransferTemplate *)&sPlazaNewsObjCharTransfer, (GXOBJVRamModeChar)mode, (GXOBJVRamModeChar)(u8)mode);
    ObjPlttTransfer_Init(0x20, heapID);
    ObjCharTransfer_ClearBuffers();
    ObjPlttTransfer_Reset();
    G2dRenderer_SetObjCharTransferReservedRegion(NNS_G2D_VRAM_TYPE_2DMAIN, (GXOBJVRamModeChar)mode);
    G2dRenderer_SetPlttTransferReservedRegion(NNS_G2D_VRAM_TYPE_2DMAIN);
    graphics->spriteList = G2dRenderer_Init(0x20, &graphics->renderer, heapID);
    G2dRenderer_SetSubSurfaceCoords(&graphics->renderer, 0, 1 << 20);
    for (i = 0; i < 4; i++) {
        graphics->resourceManagers[i] = Create2DGfxResObjMan(0x20, (GfGfxResType)i, heapID);
    }
    WirelessStatusIcon_Create();
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, TRUE);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_OBJ, TRUE);
}

void PlazaNewsGraphics_FreeSprites(PlazaNewsGraphics *graphics) {
    int i;

    SpriteList_Delete(graphics->spriteList);
    for (i = 0; i < 4; i++) {
        Destroy2DGfxResObjMan(graphics->resourceManagers[i]);
    }
    ObjCharTransfer_Destroy();
    ObjPlttTransfer_Destroy();
    OamManager_Free();
}

void PlazaNewsTileset_Load(PlazaNewsTileset *tileset, PlazaNewsGraphics *graphics, enum HeapID heapID) {
    tileset->rawData = GfGfxLoader_GetScrnDataFromOpenNarc(graphics->narc, 5, FALSE, &tileset->screenData, heapID);
}

void PlazaNewsTileset_Free(PlazaNewsTileset *tileset) {
    Heap_Free(tileset->rawData);
}

void PlazaNewsClock_DrawCell(PlazaNewsTileset *tileset, PlazaNewsGraphics *graphics, u32 tile, u8 col, u8 row) {
    NNSG2dScreenData *screen = tileset->screenData;
    CopyToBgTilemapRect(graphics->bgConfig, 3, (u8)(col + 5), (u8)(row * 2 + 4), 1, 2, screen->rawData, (u8)(tile + 6), 0, (u8)(screen->screenWidth >> 3), (u8)(screen->screenHeight >> 3));
    ScheduleBgTilemapBufferTransfer(graphics->bgConfig, 3);
}

void PlazaNewsPeople_DrawIcon(PlazaNewsTileset *tileset, PlazaNewsGraphics *graphics, u32 col, u32 row, u32 tile, u32 altRow, u32 usePalette3, u32 altPage, u32 usePalette4) {
    NNSG2dScreenData *screen;
    u32 srcX = tile % 3;
    u32 srcY = tile / 3;
    u32 y;
    u32 x;
    if (altPage != 0) {
        srcY += 2;
    }
    if (altRow == 1) {
        srcY++;
    }
    screen = tileset->screenData;
    y = row * 2 + 4;
    x = col * 2 + 0x15;
    CopyToBgTilemapRect(graphics->bgConfig, 3, (u8)x, (u8)y, 2, 2, screen->rawData, (u8)(srcX * 2), (u8)(srcY * 2), (u8)(screen->screenWidth >> 3), (u8)(screen->screenHeight >> 3));
    if (usePalette4 != 0) {
        BgTilemapRectChangePalette(graphics->bgConfig, 3, (u8)x, (u8)y, 2, 2, 4);
    } else if (usePalette3 != 0) {
        BgTilemapRectChangePalette(graphics->bgConfig, 3, (u8)x, (u8)y, 2, 2, 3);
    }
    ScheduleBgTilemapBufferTransfer(graphics->bgConfig, 3);
}

void PlazaNewsClock_Init(PlazaNewsClock *clock, PlazaNewsGraphics *graphics, PlazaNewsTileset *tileset, void *newsData, const PlazaNewsTime *a4, void *lobby, enum HeapID heapID) {
    u32 seconds = WiFiPlazaWork_GetOpenDuration(lobby);
    clock->time.hms.hour = 0;
    clock->time.hms.minute = seconds / 60 + 1;
    clock->time.hms.second = 0;
    clock->secondsPerCell = seconds / 40;
    clock->isWaiting = TRUE;
    clock->blinkTimer = 0;
    clock->needsRedraw = FALSE;
    PlazaNewsClock_UpdateTime(clock, newsData, a4);
    PlazaNewsClock_Draw(clock, tileset, graphics);
}

void PlazaNewsClock_Free(PlazaNewsClock *clock) {
}

#pragma force_active on
BOOL PlazaNewsClock_UpdateTime(PlazaNewsClock *clock, void *newsData, const PlazaNewsTime *a2) {
    PlazaNewsTime now;
    PlazaNewsTime remaining;
    PlazaNewsTime diff;
    int changed = FALSE;
    if (WiFiPlazaNews_GetOpenTime(newsData, &now) == 0) {
        return changed;
    }
    if (clock->isWaiting == TRUE) {
        clock->isWaiting = FALSE;
        changed = TRUE;
        clock->needsRedraw = changed;
    }
    WiFiPlazaTime_Subtract(a2, &now, &remaining);
    WiFiPlazaTime_Subtract(&remaining, &clock->time, &diff);
    if (diff.hms.second >= clock->secondsPerCell || diff.hms.minute != 0 || diff.hms.hour != 0) {
        changed = TRUE;
        clock->needsRedraw = changed;
        clock->time = remaining;
    }
    return changed;
}
#pragma force_active reset

void PlazaNewsClock_Draw(PlazaNewsClock *clock, PlazaNewsTileset *tileset, PlazaNewsGraphics *graphics) {
    if (clock->isWaiting == TRUE) {
        PlazaNewsClock_DrawWaiting(clock, tileset, graphics);
    } else {
        PlazaNewsClock_DrawGauge(clock, tileset, graphics);
    }
}

void PlazaNewsClock_DrawWaiting(PlazaNewsClock *clock, PlazaNewsTileset *tileset, PlazaNewsGraphics *graphics) {
    clock->blinkTimer++;
    if (clock->blinkTimer > 16) {
        clock->blinkTimer = 0;
    }
}

#pragma force_active on
void PlazaNewsClock_DrawGauge(PlazaNewsClock *clock, PlazaNewsTileset *tileset, PlazaNewsGraphics *graphics) {
    int i;
    int cell = (clock->time.hms.minute * 60 + clock->time.hms.second + clock->time.hms.hour * 3600) / clock->secondsPerCell;
    if (clock->needsRedraw) {
        int j;
        clock->blinkTimer = 0;
        clock->needsRedraw = FALSE;
        FillBgTilemapRect(graphics->bgConfig, 3, 0, 5, 4, 10, 8, 0);
        ScheduleBgTilemapBufferTransfer(graphics->bgConfig, 3);
        for (i = 0; i < 4; i++) {
            int row = 3 - i;
            for (j = 0; j < 10; j++) {
                int col = 9 - j;
                u32 index = col + row * 10;
                if (index >= cell) {
                    int tile;
                    if (index == cell) {
                        tile = 1;
                    } else if (row == 3) {
                        tile = 2;
                    } else {
                        tile = 0;
                    }
                    PlazaNewsClock_DrawCell(tileset, graphics, tile, col, row);
                }
            }
        }
    } else if ((u32)cell < 40) {
        int timer;
        int row;
        int col;
        int phase;
        clock->blinkTimer++;
        timer = clock->blinkTimer;
        phase = timer % 32;
        col = (u32)cell % 10;
        row = (u32)cell / 10;
        if (phase == 0) {
            u32 step = (30 * clock->secondsPerCell) / 4;
            int tile = 6;
            for (int k = 0; k < 4; k++) {
                if (step * (k + 1) > (u32)timer) {
                    tile = sPlazaNewsClockBlinkTiles[k];
                    break;
                }
            }
            PlazaNewsClock_DrawCell(tileset, graphics, tile, col, row);
        } else if (phase == 16) {
            PlazaNewsClock_DrawCell(tileset, graphics, 3, col, row);
        }
    }
}
#pragma force_active reset

#pragma force_active on
void PlazaNewsPeople_Init(PlazaNewsPeople *people, PlazaNewsGraphics *graphics, PlazaNewsTileset *tileset, void *newsData, void *lobby, enum HeapID heapID) {
    PlazaNewsPeople_DrawGrid(tileset, graphics, newsData, lobby, 0);
}
#pragma force_active reset

void PlazaNewsPeople_Free(PlazaNewsPeople *people) {
}

BOOL PlazaNewsPeople_UpdateState(PlazaNewsPeople *people, void *newsData) {
    int ret = FALSE;
    switch (WiFiPlazaNews_GetSlotChange(newsData)) {
    case 0:
        break;
    case 1:
        ret = TRUE;
        people->state = ret;
        break;
    case 2:
        people->state = 3;
        ret = TRUE;
        break;
    default:
        break;
    }
    return ret;
}

void PlazaNewsPeople_Draw(PlazaNewsPeople *people, PlazaNewsTileset *tileset, PlazaNewsGraphics *graphics, void *newsData, void *lobby) {
    switch (people->state) {
    case 1:
        people->state = 2;
        people->frame = 0;
        return;
    case 2:
        PlazaNewsPeople_BlinkMode2(tileset, graphics, newsData, lobby, people->frame);
        people->frame++;
        if (people->frame >= 32) {
            people->state = 0;
            return;
        }
        break;
    case 3:
        people->state = 4;
        people->frame = 0;
        return;
    case 4:
        PlazaNewsPeople_BlinkMode1(tileset, graphics, newsData, lobby, people->frame);
        people->frame++;
        if (people->frame >= 32) {
            people->state = 0;
        }
        break;
    }
}

#pragma force_active on
void PlazaNewsPeople_DrawGrid(PlazaNewsTileset *tileset, PlazaNewsGraphics *graphics, void *newsData, void *lobby, int mode) {
    struct {
        volatile int altPage;
        volatile int usePalette3;
        volatile int altRow;
        volatile int row;
        volatile int unk_10;
        int rowU8;
    } f;
    int col;
    int base;
    int nextRow;
    FillBgTilemapRect(graphics->bgConfig, 3, 0, 21, 4, 10, 8, 0);
    base = 0;
    f.row = 0;
    do {
        f.rowU8 = (u8)f.row;
        col = 0;
        do {
            if (WiFiPlazaNews_IsSlotOccupied(newsData, col + base)) {
                int highlight;
                int tile;
                f.altRow = WiFiPlazaNews_GetSlotGender(newsData, col + base);
                f.usePalette3 = WiFiPlazaNews_IsOwnSlot(newsData, col + base);
                f.altPage = WiFiPlazaNews_IsOldestSlot(newsData, col + base);
                f.unk_10 = WiFiPlazaNews_IsLastJoinedSlot(newsData, col + base);
                highlight = WiFiPlazaWork_IsSpecialVisitor(lobby, col + base);
                if (mode == 2 && f.unk_10 == 1) {
                    tile = 0;
                } else {
                    tile = mode;
                }
                PlazaNewsPeople_DrawIcon(tileset, graphics, (u8)col, f.rowU8, tile, f.altRow, f.usePalette3, f.altPage, highlight);
            }
            col++;
        } while (col < 5);
        nextRow = f.row + 1;
        base += 5;
        f.row = nextRow;
    } while (nextRow < 4);
}
#pragma force_active reset

void PlazaNewsPeople_BlinkMode2(PlazaNewsTileset *tileset, PlazaNewsGraphics *graphics, void *newsData, void *lobby, u32 frame) {
    int mode;
    if ((frame & 7) != 0) {
        return;
    }
    frame >>= 3;
    if ((frame & 1) == 0) {
        mode = 2;
    } else {
        mode = 0;
    }
    PlazaNewsPeople_DrawGrid(tileset, graphics, newsData, lobby, mode);
}

#pragma opt_propagation off
#pragma force_active on
void PlazaNewsPeople_BlinkMode1(PlazaNewsTileset *tileset, PlazaNewsGraphics *graphics, void *newsData, void *lobby, u32 frame) {
    if (!(frame & 7)) {
        u32 blink = frame >> 3;
        int mode = 1;
        if (1 & blink) {
            mode = 0;
        }
        PlazaNewsPeople_DrawGrid(tileset, graphics, newsData, lobby, mode);
    }
}
#pragma force_active reset
#pragma opt_propagation reset

void PlazaNewsHeadlines_GetNameColors(u8 *colors, void *newsData, void *item, void *lobby) {
    int i;
    for (i = 0; i < 4; i++) {
        int person = WiFiPlazaNews_GetEventSlot(newsData, item, i);
        if (person != 20) {
            if (WiFiPlazaWork_IsSpecialVisitor(lobby, person) == 1) {
                colors[i] = 9;
            } else {
                colors[i] = 5;
            }
        }
    }
}

#pragma force_active on
void PlazaNewsHeadlines_LoadNameColors(const u8 *colors, NNSG2dPaletteData *palette, int paletteNum) {
    int i = 0;
    int base = paletteNum * 32;
    u8 *data = palette->pRawData;
    int offset = i;
    base += 10;
    for (; i < 4; i++, offset += 4) {
        if (!GF_CreateNewVramTransferTask(NNS_GFD_DST_2D_BG_PLTT_MAIN, base + offset, data + (colors[i] * 2 + 224), 4)) {
            GF_AssertFail();
        }
    }
}
#pragma force_active reset

#pragma force_active on
void PlazaNewsHeadlines_Init(PlazaNewsHeadlines *headlines, PlazaNewsGraphics *graphics, enum HeapID heapID) {
    int i = 0;
    PlazaNewsTicker *ticker = headlines->tickers;
    int baseTile = 768;
    Window *window = &headlines->windows[0];
    const u8 *top = sPlazaNewsHeadlineRowTops;

    for (; i < 3; i++) {
        PlazaNewsTicker_Init(ticker, graphics, i, heapID);
        AddWindowParameterized(graphics->bgConfig, window, 3, 1, *top, 30, 2, i + 7, baseTile);
        FillWindowPixelBuffer(window, 0);
        ScheduleWindowCopyToVram(window);
        ticker++;
        baseTile += 60;
        top++;
        window++;
    }
    headlines->string = String_New(256, heapID);
    headlines->paletteFile = GfGfxLoader_GetPlttDataFromOpenNarc(graphics->narc, 0, &headlines->palette, heapID);
    BG_LoadPlttData(0, (u8 *)headlines->palette->pRawData + 238, 4, 250);
    BG_LoadPlttData(0, (u8 *)headlines->palette->pRawData + 238, 4, 282);
    BG_LoadPlttData(0, (u8 *)headlines->palette->pRawData + 238, 4, 314);
}
#pragma force_active reset

void PlazaNewsHeadlines_Free(PlazaNewsHeadlines *headlines) {
    int i;
    Window *window;
    PlazaNewsTicker *ticker;
    Heap_Free(headlines->paletteFile);
    String_Delete(headlines->string);
    ticker = headlines->tickers;
    i = 0;
    window = headlines->windows;
    while (i < 3) {
        RemoveWindow(window);
        PlazaNewsTicker_Free(ticker);
        i++;
        window++;
        ticker++;
    }
}

#pragma force_active on
BOOL PlazaNewsHeadlines_Update(PlazaNewsHeadlines *headlines, void *newsData, void *lobby, enum HeapID heapID) {
    u32 info[5];
    u32 nameColors;
    int i;
    PlazaNewsTicker *ticker;
    const u32 *duration;
    void *item;
    WiFiPlazaWork_GetSpecialVisitors(lobby, info);
    i = 2;
    ticker = &headlines->tickers[2];
    duration = sPlazaNewsLastHeadlineRowDuration;
    for (; i >= 0; i--) {
        if (ticker->active) {
            if (PlazaNewsTicker_Advance(ticker) == TRUE) {
                if (i + 1 < 3) {
                    PlazaNewsTicker_SetText(&headlines->tickers[i + 1], ticker->text, duration[1], ticker->nameColors, headlines->palette);
                }
                PlazaNewsTicker_Stop(ticker);
            }
        }
        ticker--;
        duration--;
    }
    if (headlines->tickers[0].active == FALSE) {
        if (WiFiPlazaNews_HasEvent(newsData) == TRUE) {
            do {
                item = WiFiPlazaNews_GetEventType(newsData);
                if (WiFiPlazaNews_FormatEvent(newsData, info, item, headlines->string, heapID) == 1) {
                    PlazaNewsHeadlines_GetNameColors((u8 *)&nameColors, newsData, item, lobby);
                    PlazaNewsTicker_SetText(&headlines->tickers[0], headlines->string, 336, (u8 *)&nameColors, headlines->palette);
                    WiFiPlazaNews_RemoveCurrentEvent(newsData);
                    return TRUE;
                }
                WiFiPlazaNews_RemoveCurrentEvent(newsData);
            } while (WiFiPlazaNews_HasEvent(newsData) == TRUE);
        }
    }
    return FALSE;
}
#pragma force_active reset

void PlazaNewsHeadlines_Draw(PlazaNewsHeadlines *headlines) {
    int i;
    PlazaNewsTicker *ticker;
    Window *window;
    PlazaNewsTicker *cur;

    ticker = headlines->tickers;
    i = 0;
    window = headlines->windows;
    cur = ticker;
    while (i < 3) {
        if (ticker->active == TRUE) {
            PlazaNewsTicker_Draw(cur, window);
        }
        i++;
        ticker++;
        window++;
        cur++;
    }
}

void PlazaNewsTicker_Init(PlazaNewsTicker *ticker, PlazaNewsGraphics *graphics, int row, enum HeapID heapID) {
    ticker->active = FALSE;
    ticker->position = 0;
    ticker->text = String_New(0x100, heapID);
    ticker->paletteNum = row + 7;
    AddWindowParameterized(graphics->bgConfig, &ticker->bitmap, 3, 0, 0, 180, 2, 7, 0);
}

void PlazaNewsTicker_Free(PlazaNewsTicker *ticker) {
    String_Delete(ticker->text);
    RemoveWindow(&ticker->bitmap);
}

#pragma force_active on
void PlazaNewsTicker_SetText(PlazaNewsTicker *ticker, String *text, u32 duration, u8 *nameColors, NNSG2dPaletteData *palette) {
    String_Copy(ticker->text, text);
    ticker->active = TRUE;
    ticker->position = 0;
    ticker->duration = duration;
    ticker->width = FontID_String_GetWidth(1, text, 0);
    ticker->width += 256;
    GF_ASSERT(ticker->width <= 1440);
    FillWindowPixelBuffer(&ticker->bitmap, 0);
    AddTextPrinterParameterized(&ticker->bitmap, 1, ticker->text, 0, 0, TEXT_SPEED_NOTRANSFER, NULL);
    ticker->nameColors[0] = nameColors[0];
    ticker->nameColors[1] = nameColors[1];
    ticker->nameColors[2] = nameColors[2];
    ticker->nameColors[3] = nameColors[3];
    PlazaNewsHeadlines_LoadNameColors(ticker->nameColors, palette, ticker->paletteNum);
}
#pragma force_active reset

void PlazaNewsTicker_Stop(PlazaNewsTicker *ticker) {
    ticker->active = FALSE;
}

BOOL PlazaNewsTicker_Advance(PlazaNewsTicker *ticker) {
    BOOL finished = TRUE;
    int position = ticker->position + 1;
    if (position <= ticker->duration) {
        ticker->position = position;
        finished = FALSE;
    }
    return finished;
}

#pragma force_active on
void PlazaNewsTicker_Draw(PlazaNewsTicker *ticker, Window *window) {
    volatile int width = ticker->width;
    int scroll = *(volatile u16 *)&ticker->position * width / ticker->duration;
    int x;
    int srcX;
    if (scroll < 256) {
        x = 255 - scroll;
        srcX = 0;
    } else {
        srcX = scroll - 256;
        x = 0;
        scroll = width - scroll;
        if (scroll > 255) {
            scroll = 255;
        }
    }
    FillWindowPixelRect(window, 0, 0, 0, 255, 16);
    BlitBitmapRect(window, ticker->bitmap.pixelBuffer, (u16)srcX, 0, 1440, 16, (u16)x, 0, (u16)scroll, 16, 15);
    ScheduleWindowCopyToVram(window);
}
#pragma force_active reset

void PlazaNewsTitles_Init(PlazaNewsTitles *titles, PlazaNewsGraphics *graphics, enum HeapID heapID) {
    int messageNo;
    MsgData *msgData;
    String *message;
    u32 lineCount;
    int x;
    const u32 *sound;
    const u16 *paletteOffset;
    PlazaNewsFlash *flash;
    Window *window;
    String *line;
    const PlazaNewsTitleTemplate *titleTemplate;

    memset(titles, 0, sizeof(PlazaNewsTitles));
    msgData = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, 0x2F5, heapID);
    message = String_New(0x80, heapID);
    messageNo = 0;
    sound = sPlazaNewsTitleSEs;
    titleTemplate = sPlazaNewsTitleTemplates;
    paletteOffset = sPlazaNewsTitlePaletteOffsets;
    window = titles->windows;
    flash = titles->flashes;

    do {
        u32 i;
        int y;

        PlazaNewsFlash_Init(flash, *paletteOffset, *sound);
        AddWindowParameterized(graphics->bgConfig, window, 0, titleTemplate->left, titleTemplate->top, titleTemplate->width, titleTemplate->height, titleTemplate->palette, titleTemplate->baseTile);
        FillWindowPixelBuffer(window, 0);
        ReadMsgDataIntoString(msgData, messageNo, message);
        lineCount = String_CountLines(message);
        y = titleTemplate->y;
        line = String_New(String_GetLength(message) + 1, heapID);
        for (i = 0; i < lineCount; i++) {
            String_GetLineN(line, message, i);
            switch (titleTemplate->alignment) {
            case 0:
                x = titleTemplate->x;
                break;
            case 1:
                x = titleTemplate->x - ((FontID_String_GetWidthMultiline(0, line, 0) + 1) >> 1);
                break;
            case 2:
                x = titleTemplate->x - FontID_String_GetWidthMultiline(0, line, 0);
                break;
            default:
                break;
            }
            AddTextPrinterParameterizedWithColor(window, 0, line, x, y, TEXT_SPEED_NOTRANSFER, titleTemplate->color, NULL);
            y += 0x10;
        }
        String_Delete(line);
        ScheduleWindowCopyToVram(window);
        sound++;
        titleTemplate++;
        paletteOffset++;
        flash++;
        window++;
        messageNo++;
    } while (messageNo < 4);

    String_Delete(message);
    DestroyMsgData(msgData);
}

void PlazaNewsTitles_Free(PlazaNewsTitles *titles) {
    Window *window;
    PlazaNewsFlash *flash;
    int i;

    window = titles->windows;
    i = 0;
    flash = titles->flashes;
    do {
        RemoveWindow(window);
        PlazaNewsFlash_Clear(flash);
        i++;
        window++;
        flash++;
    } while (i < 4);
}

void PlazaNewsTitles_Update(PlazaNewsTitles *titles) {
    int i;
    PlazaNewsFlash *flash;

    i = 0;
    flash = titles->flashes;
    do {
        PlazaNewsFlash_Update(flash);
        i++;
        flash++;
    } while (i < 4);
}

void PlazaNewsTitles_StartFlash(PlazaNewsTitles *titles, u32 index) {
    if (index >= 4) {
        GF_AssertFail();
    }
    if (index == 0) {
        GF_AssertFail();
    }
    PlazaNewsFlash_Start(&titles->flashes[index]);
}

void PlazaNewsFlash_Init(PlazaNewsFlash *flash, u16 paletteOffset, u32 sound) {
    memset(flash, 0, sizeof(PlazaNewsFlash));
    flash->paletteOffset = paletteOffset;
    flash->sound = sound;
}

void PlazaNewsFlash_Clear(PlazaNewsFlash *flash) {
    memset(flash, 0, sizeof(PlazaNewsFlash));
}

void PlazaNewsFlash_Start(PlazaNewsFlash *flash) {
    flash->active = TRUE;
    flash->timer = 0;
}

#pragma force_active on
void PlazaNewsFlash_Update(PlazaNewsFlash *flash) {
    if (flash->timer > 28) {
        flash->active = FALSE;
    }
    if (flash->active) {
        int amount;
        u16 color;
        if (flash->timer == 0) {
            PlaySE((u16)flash->sound);
        }
        if (flash->timer < 2) {
            amount = (flash->timer * 16) / 2;
        } else if (flash->timer < 14) {
            amount = 16;
        } else {
            amount = 16 - ((flash->timer - 14) * 16) / 14;
        }
        color = 14;
        BlendPalette(&color, &flash->color, 1, (u8)amount, 25);
        DC_FlushRange(&flash->color, 2);
        GX_LoadBGPltt(&flash->color, flash->paletteOffset, 2);
        flash->timer++;
    }
}
#pragma force_active reset
