#include "global.h"

#include "msgdata/msg.naix"
#include "msgdata/msg/msg_0314.h"
#include "pokeathlon/pokeathlon_record_screens.h"

#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "message_format.h"
#include "poke_overlay.h"
#include "sound_playback.h"
#include "system.h"

FS_EXTERN_OVERLAY(OVY_98);

static void PokeathlonCourseRecord_CopyArgs(PokeathlonCourseRecord *work, const PokeathlonCourseRecordArgs *args);
static void PokeathlonCourseRecord_FreeArgs(PokeathlonCourseRecordArgs *args);
static void PokeathlonCourseRecord_InitBgs(PokeathlonCourseRecord *work);
static void PokeathlonCourseRecord_FreeBgs(PokeathlonCourseRecord *work);
static void PokeathlonCourseRecord_LoadBgGraphics(PokeathlonCourseRecord *work);
static void PokeathlonCourseRecord_UpdatePageScroll(PokeathlonCourseRecord *work);
static void PokeathlonCourseRecord_ShowCoursePanel(PokeathlonCourseRecord *work, u32 course);
static void PokeathlonCourseRecord_PrintStaticText(PokeathlonCourseRecord *work);
static void PokeathlonCourseRecord_PrintPage(PokeathlonCourseRecord *work, u32 course);
static void PokeathlonCourseRecord_PrintEventScores(PokeathlonCourseRecord *work, int course);
static void PokeathlonCourseRecord_PrintMonNames(PokeathlonCourseRecord *work, BOOL show);
static void PokeathlonCourseRecord_PrintCourseName(PokeathlonCourseRecord *work, int course);
static void PokeathlonCourseRecord_PrintTotalLabel(PokeathlonCourseRecord *work);
static void PokeathlonCourseRecord_VBlankCallback(PokeathlonCourseRecord *work);
static void PokeathlonCourseRecord_LoadSpriteResources(PokeathlonCourseRecord *work);
static void PokeathlonCourseRecord_LoadMonPicResources(PokeathlonCourseRecord *work);
static void PokeathlonCourseRecord_CreateSprites(PokeathlonCourseRecord *work);
static void PokeathlonCourseRecord_SetMonSpritesVisible(PokeathlonCourseRecord *work, BOOL visible);
static void PokeathlonCourseRecord_UpdatePageSprites(PokeathlonCourseRecord *work, int course);
static BOOL PokeathlonCourseRecord_HandleInput(PokeathlonCourseRecord *work);

static const u8 sPokeathlonCourseRecordBgLayers[8] = {
    GF_BG_LYR_MAIN_0,
    GF_BG_LYR_MAIN_1,
    GF_BG_LYR_MAIN_2,
    GF_BG_LYR_MAIN_3,
    GF_BG_LYR_SUB_0,
    GF_BG_LYR_SUB_1,
    GF_BG_LYR_SUB_2,
    GF_BG_LYR_SUB_3,
};

static const GraphicsModes sPokeathlonCourseRecordGraphicsModes = {
    .dispMode = GX_DISPMODE_GRAPHICS,
    .bgMode = GX_BGMODE_0,
    .subMode = GX_BGMODE_0,
    ._2d3dMode = GX_BG0_AS_2D,
};

static const int sPokeathlonCourseRecordMonIconResIds[4] = { 7, 7, 5, 5 };

static const s16 sPokeathlonCourseRecordCursorPositions[6][2] = {
    { 208, 16  },
    { 240, 48  },
    { 208, 80  },
    { 240, 112 },
    { 208, 144 },
    { 224, 176 },
};

static const SpriteResourceCountsListUnion sPokeathlonCourseRecordSpriteResourceCounts = {
    .numChar = 10,
    .numPltt = 8,
    .numCell = 6,
    .numAnim = 6,
    .numMcel = 0,
    .numManm = 0,
};

static const u32 sPokeathlonCourseRecordScoreWindows[6] = { 5, 8, 6, 9, 7, 10 };

static const TouchscreenHitbox sPokeathlonCourseRecordHitboxes[] = {
    { .circle = { TOUCHSCREEN_CIRCLE_SENTINEL, 208, 16, 16 } },
    { .circle = { TOUCHSCREEN_CIRCLE_SENTINEL, 240, 48, 16 } },
    { .circle = { TOUCHSCREEN_CIRCLE_SENTINEL, 208, 80, 16 } },
    { .circle = { TOUCHSCREEN_CIRCLE_SENTINEL, 240, 112, 16 } },
    { .circle = { TOUCHSCREEN_CIRCLE_SENTINEL, 208, 144, 16 } },
    { .rect = { 160, 192, 192, 0 } },
    { .rect = { TOUCHSCREEN_RECTLIST_END } },
};

static const u32 sPokeathlonCourseRecordDigitAnims[10] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

static const s32 sPokeathlonCourseRecordScreenFiles[5][2] = {
    { 63, 69 },
    { 64, 70 },
    { 65, 71 },
    { 66, 72 },
    { 67, 73 },
};

static const u8 sPokeathlonCourseRecordCourseEvents[10][4] = {
    { 0, 1, 6, 10 },
    { 3, 2, 9, 10 },
    { 8, 9, 1, 10 },
    { 7, 6, 3, 10 },
    { 5, 4, 0, 10 },
    { 0, 5, 4, 6  },
    { 5, 4, 1, 8  },
    { 9, 8, 1, 3  },
    { 3, 2, 9, 7  },
    { 6, 7, 0, 2  },
};

static const int sPokeathlonCourseRecordMonPicResIds[4][4] = {
    { 3, 4, 4, 4 },
    { 4, 5, 4, 4 },
    { 5, 6, 4, 4 },
    { 0, 0, 0, 0 },
};

static const PokeathlonCourseRecordSpritePos sPokeathlonCourseRecordScrollSpritePositions[6] = {
    { 11, 96,  64  },
    { 12, 56,  104 },
    { 13, 136, 104 },
    { 14, 120, 152 },
    { 15, 104, 152 },
    { 16, 88,  152 },
};

static const WindowTemplate sPokeathlonCourseRecordWindowTemplates[17] = {
    { GF_BG_LYR_MAIN_0, 0,  0,  14, 2, 12, 1   },
    { GF_BG_LYR_MAIN_0, 0,  22, 14, 2, 12, 29  },
    { GF_BG_LYR_MAIN_0, 25, 21, 6,  2, 12, 57  },
    { GF_BG_LYR_SUB_2,  7,  0,  18, 2, 12, 1   },
    { GF_BG_LYR_SUB_2,  2,  2,  28, 2, 12, 37  },
    { GF_BG_LYR_SUB_2,  3,  6,  14, 2, 12, 93  },
    { GF_BG_LYR_SUB_2,  3,  8,  14, 2, 12, 121 },
    { GF_BG_LYR_SUB_2,  3,  10, 14, 2, 12, 149 },
    { GF_BG_LYR_SUB_2,  22, 6,  8,  2, 12, 177 },
    { GF_BG_LYR_SUB_2,  22, 8,  8,  2, 12, 193 },
    { GF_BG_LYR_SUB_2,  22, 10, 8,  2, 12, 209 },
    { GF_BG_LYR_SUB_2,  7,  12, 14, 2, 12, 225 },
    { GF_BG_LYR_SUB_2,  22, 12, 8,  2, 12, 253 },
    { GF_BG_LYR_SUB_2,  26, 22, 6,  2, 12, 269 },
    { GF_BG_LYR_SUB_2,  4,  16, 12, 2, 12, 281 },
    { GF_BG_LYR_SUB_2,  5,  19, 12, 2, 12, 305 },
    { GF_BG_LYR_SUB_2,  6,  22, 12, 2, 12, 329 },
};

static const PokeathlonCourseRecordBgTemplates sPokeathlonCourseRecordBgTemplates = {
    {
     {
            .x = 0,
            .y = 0,
            .bufferSize = GF_BG_BUF_SIZE_256x256_4BPP,
            .baseTile = 0,
            .size = GF_BG_SCR_SIZE_256x256,
            .colorMode = GX_BG_COLORMODE_16,
            .screenBase = GX_BG_SCRBASE_0x0000,
            .charBase = GX_BG_CHARBASE_0x04000,
            .bgExtPltt = GX_BG_EXTPLTT_01,
            .priority = 0,
            .areaOver = GX_BG_AREAOVER_XLU,
            .mosaic = 0,
        },
     {
            .x = 0,
            .y = 0,
            .bufferSize = GF_BG_BUF_SIZE_256x256_4BPP,
            .baseTile = 0,
            .size = GF_BG_SCR_SIZE_256x256,
            .colorMode = GX_BG_COLORMODE_16,
            .screenBase = GX_BG_SCRBASE_0x0800,
            .charBase = GX_BG_CHARBASE_0x08000,
            .bgExtPltt = GX_BG_EXTPLTT_01,
            .priority = 3,
            .areaOver = GX_BG_AREAOVER_XLU,
            .mosaic = 0,
        },
     {
            .x = 0,
            .y = 0,
            .bufferSize = GF_BG_BUF_SIZE_512x256_4BPP,
            .baseTile = 0,
            .size = GF_BG_SCR_SIZE_512x256,
            .colorMode = GX_BG_COLORMODE_16,
            .screenBase = GX_BG_SCRBASE_0x1000,
            .charBase = GX_BG_CHARBASE_0x0c000,
            .bgExtPltt = GX_BG_EXTPLTT_01,
            .priority = 2,
            .areaOver = GX_BG_AREAOVER_XLU,
            .mosaic = 0,
        },
     {
            .x = 0,
            .y = 0,
            .bufferSize = GF_BG_BUF_SIZE_256x256_4BPP,
            .baseTile = 0,
            .size = GF_BG_SCR_SIZE_256x256,
            .colorMode = GX_BG_COLORMODE_16,
            .screenBase = GX_BG_SCRBASE_0x2000,
            .charBase = GX_BG_CHARBASE_0x14000,
            .bgExtPltt = GX_BG_EXTPLTT_01,
            .priority = 1,
            .areaOver = GX_BG_AREAOVER_XLU,
            .mosaic = 0,
        },
     {
            .x = 0,
            .y = 0,
            .bufferSize = GF_BG_BUF_SIZE_256x256_4BPP,
            .baseTile = 0,
            .size = GF_BG_SCR_SIZE_256x256,
            .colorMode = GX_BG_COLORMODE_16,
            .screenBase = GX_BG_SCRBASE_0xf800,
            .charBase = GX_BG_CHARBASE_0x00000,
            .bgExtPltt = GX_BG_EXTPLTT_01,
            .priority = 3,
            .areaOver = GX_BG_AREAOVER_XLU,
            .mosaic = 0,
        },
     {
            .x = 256,
            .y = 0,
            .bufferSize = GF_BG_BUF_SIZE_512x256_4BPP,
            .baseTile = 0,
            .size = GF_BG_SCR_SIZE_512x256,
            .colorMode = GX_BG_COLORMODE_16,
            .screenBase = GX_BG_SCRBASE_0xe800,
            .charBase = GX_BG_CHARBASE_0x04000,
            .bgExtPltt = GX_BG_EXTPLTT_01,
            .priority = 2,
            .areaOver = GX_BG_AREAOVER_XLU,
            .mosaic = 0,
        },
     {
            .x = 0,
            .y = 0,
            .bufferSize = GF_BG_BUF_SIZE_256x256_4BPP,
            .baseTile = 0,
            .size = GF_BG_SCR_SIZE_256x256,
            .colorMode = GX_BG_COLORMODE_16,
            .screenBase = GX_BG_SCRBASE_0xe000,
            .charBase = GX_BG_CHARBASE_0x08000,
            .bgExtPltt = GX_BG_EXTPLTT_01,
            .priority = 0,
            .areaOver = GX_BG_AREAOVER_XLU,
            .mosaic = 0,
        },
     {
            .x = 0,
            .y = 0,
            .bufferSize = GF_BG_BUF_SIZE_256x256_4BPP,
            .baseTile = 0,
            .size = GF_BG_SCR_SIZE_256x256,
            .colorMode = GX_BG_COLORMODE_16,
            .screenBase = GX_BG_SCRBASE_0xd800,
            .charBase = GX_BG_CHARBASE_0x0c000,
            .bgExtPltt = GX_BG_EXTPLTT_01,
            .priority = 1,
            .areaOver = GX_BG_AREAOVER_XLU,
            .mosaic = 0,
        },
     },
};

static const ManagedSpriteTemplate sPokeathlonCourseRecordSpriteTemplates[26] = {
    {
     .x = 208,
     .y = 16,
     .z = 0,
     .animation = 8,
     .drawPriority = 2,
     .pal = 0,
     .vram = NNS_G2D_VRAM_TYPE_2DMAIN,
     .resIdList = { 1, 1, 1, 1, 0, 0 },
     .bgPriority = 3,
     .vramTransfer = 0,
     },
    {
     .x = 208,
     .y = 16,
     .z = 0,
     .animation = 0,
     .drawPriority = 2,
     .pal = 0,
     .vram = NNS_G2D_VRAM_TYPE_2DMAIN,
     .resIdList = { 2, 2, 2, 2, 0, 0 },
     .bgPriority = 3,
     .vramTransfer = 0,
     },
    {
     .x = 240,
     .y = 48,
     .z = 0,
     .animation = 1,
     .drawPriority = 2,
     .pal = 1,
     .vram = NNS_G2D_VRAM_TYPE_2DMAIN,
     .resIdList = { 2, 2, 2, 2, 0, 0 },
     .bgPriority = 3,
     .vramTransfer = 0,
     },
    {
     .x = 208,
     .y = 80,
     .z = 0,
     .animation = 2,
     .drawPriority = 2,
     .pal = 2,
     .vram = NNS_G2D_VRAM_TYPE_2DMAIN,
     .resIdList = { 2, 2, 2, 2, 0, 0 },
     .bgPriority = 3,
     .vramTransfer = 0,
     },
    {
     .x = 240,
     .y = 112,
     .z = 0,
     .animation = 3,
     .drawPriority = 2,
     .pal = 3,
     .vram = NNS_G2D_VRAM_TYPE_2DMAIN,
     .resIdList = { 2, 2, 2, 2, 0, 0 },
     .bgPriority = 3,
     .vramTransfer = 0,
     },
    {
     .x = 208,
     .y = 144,
     .z = 0,
     .animation = 4,
     .drawPriority = 2,
     .pal = 4,
     .vram = NNS_G2D_VRAM_TYPE_2DMAIN,
     .resIdList = { 2, 2, 2, 2, 0, 0 },
     .bgPriority = 3,
     .vramTransfer = 0,
     },
    {
     .x = 208,
     .y = 16,
     .z = 0,
     .animation = 2,
     .drawPriority = 1,
     .pal = 0,
     .vram = NNS_G2D_VRAM_TYPE_2DMAIN,
     .resIdList = { 1, 1, 1, 1, 0, 0 },
     .bgPriority = 3,
     .vramTransfer = 0,
     },
    {
     .x = 240,
     .y = 48,
     .z = 0,
     .animation = 3,
     .drawPriority = 1,
     .pal = 0,
     .vram = NNS_G2D_VRAM_TYPE_2DMAIN,
     .resIdList = { 1, 1, 1, 1, 0, 0 },
     .bgPriority = 3,
     .vramTransfer = 0,
     },
    {
     .x = 208,
     .y = 80,
     .z = 0,
     .animation = 2,
     .drawPriority = 1,
     .pal = 0,
     .vram = NNS_G2D_VRAM_TYPE_2DMAIN,
     .resIdList = { 1, 1, 1, 1, 0, 0 },
     .bgPriority = 3,
     .vramTransfer = 0,
     },
    {
     .x = 240,
     .y = 112,
     .z = 0,
     .animation = 3,
     .drawPriority = 1,
     .pal = 0,
     .vram = NNS_G2D_VRAM_TYPE_2DMAIN,
     .resIdList = { 1, 1, 1, 1, 0, 0 },
     .bgPriority = 3,
     .vramTransfer = 0,
     },
    {
     .x = 208,
     .y = 144,
     .z = 0,
     .animation = 2,
     .drawPriority = 1,
     .pal = 0,
     .vram = NNS_G2D_VRAM_TYPE_2DMAIN,
     .resIdList = { 1, 1, 1, 1, 0, 0 },
     .bgPriority = 3,
     .vramTransfer = 0,
     },
    {
     .x = 96,
     .y = 64,
     .z = 0,
     .animation = 0,
     .drawPriority = 2,
     .pal = 0,
     .vram = NNS_G2D_VRAM_TYPE_2DMAIN,
     .resIdList = { 3, 4, 4, 4, 0, 0 },
     .bgPriority = 0,
     .vramTransfer = 0,
     },
    {
     .x = 56,
     .y = 104,
     .z = 0,
     .animation = 0,
     .drawPriority = 1,
     .pal = 0,
     .vram = NNS_G2D_VRAM_TYPE_2DMAIN,
     .resIdList = { 4, 5, 4, 4, 0, 0 },
     .bgPriority = 0,
     .vramTransfer = 0,
     },
    {
     .x = 136,
     .y = 104,
     .z = 0,
     .animation = 0,
     .drawPriority = 1,
     .pal = 0,
     .vram = NNS_G2D_VRAM_TYPE_2DMAIN,
     .resIdList = { 5, 6, 4, 4, 0, 0 },
     .bgPriority = 0,
     .vramTransfer = 0,
     },
    {
     .x = 120,
     .y = 152,
     .z = 0,
     .animation = 0,
     .drawPriority = 1,
     .pal = 0,
     .vram = NNS_G2D_VRAM_TYPE_2DMAIN,
     .resIdList = { 0, 0, 0, 0, 0, 0 },
     .bgPriority = 0,
     .vramTransfer = 0,
     },
    {
     .x = 104,
     .y = 152,
     .z = 0,
     .animation = 1,
     .drawPriority = 1,
     .pal = 0,
     .vram = NNS_G2D_VRAM_TYPE_2DMAIN,
     .resIdList = { 0, 0, 0, 0, 0, 0 },
     .bgPriority = 0,
     .vramTransfer = 0,
     },
    {
     .x = 88,
     .y = 152,
     .z = 0,
     .animation = 2,
     .drawPriority = 1,
     .pal = 0,
     .vram = NNS_G2D_VRAM_TYPE_2DMAIN,
     .resIdList = { 0, 0, 0, 0, 0, 0 },
     .bgPriority = 0,
     .vramTransfer = 0,
     },
    {
     .x = 128,
     .y = 56,
     .z = 0,
     .animation = 10,
     .drawPriority = 0,
     .pal = 0,
     .vram = NNS_G2D_VRAM_TYPE_2DMAIN,
     .resIdList = { 0, 0, 0, 0, 0, 0 },
     .bgPriority = 0,
     .vramTransfer = 0,
     },
    {
     .x = 40,
     .y = 96,
     .z = 0,
     .animation = 11,
     .drawPriority = 0,
     .pal = 0,
     .vram = NNS_G2D_VRAM_TYPE_2DMAIN,
     .resIdList = { 0, 0, 0, 0, 0, 0 },
     .bgPriority = 0,
     .vramTransfer = 0,
     },
    {
     .x = 128,
     .y = 152,
     .z = 0,
     .animation = 12,
     .drawPriority = 0,
     .pal = 0,
     .vram = NNS_G2D_VRAM_TYPE_2DMAIN,
     .resIdList = { 0, 0, 0, 0, 0, 0 },
     .bgPriority = 0,
     .vramTransfer = 0,
     },
    {
     .x = 208,
     .y = 149,
     .z = 0,
     .animation = 0,
     .drawPriority = 1,
     .pal = 0,
     .vram = NNS_G2D_VRAM_TYPE_2DSUB,
     .resIdList = { 6, 3, 3, 3, 0, 0 },
     .bgPriority = 0,
     .vramTransfer = 0,
     },
    {
     .x = 176,
     .y = 149,
     .z = 0,
     .animation = 1,
     .drawPriority = 1,
     .pal = 0,
     .vram = NNS_G2D_VRAM_TYPE_2DSUB,
     .resIdList = { 6, 3, 3, 3, 0, 0 },
     .bgPriority = 0,
     .vramTransfer = 0,
     },
    {
     .x = 144,
     .y = 149,
     .z = 0,
     .animation = 2,
     .drawPriority = 1,
     .pal = 0,
     .vram = NNS_G2D_VRAM_TYPE_2DSUB,
     .resIdList = { 6, 3, 3, 3, 0, 0 },
     .bgPriority = 0,
     .vramTransfer = 0,
     },
    {
     .x = 16,
     .y = 128,
     .z = 0,
     .animation = 0,
     .drawPriority = 1,
     .pal = 0,
     .vram = NNS_G2D_VRAM_TYPE_2DSUB,
     .resIdList = { 7, 7, 5, 5, 0, 0 },
     .bgPriority = 0,
     .vramTransfer = 0,
     },
    {
     .x = 24,
     .y = 152,
     .z = 0,
     .animation = 0,
     .drawPriority = 1,
     .pal = 0,
     .vram = NNS_G2D_VRAM_TYPE_2DSUB,
     .resIdList = { 8, 7, 5, 5, 0, 0 },
     .bgPriority = 0,
     .vramTransfer = 0,
     },
    {
     .x = 32,
     .y = 176,
     .z = 0,
     .animation = 0,
     .drawPriority = 1,
     .pal = 0,
     .vram = NNS_G2D_VRAM_TYPE_2DSUB,
     .resIdList = { 9, 7, 5, 5, 0, 0 },
     .bgPriority = 0,
     .vramTransfer = 0,
     },
};

// The first 0xDC bytes of PokeathlonSave hold the five course records.
static inline PokeathlonCourseRecordEntry *PokeathlonCourseRecord_GetRecords(PokeathlonCourseRecordArgs *args) {
    return (PokeathlonCourseRecordEntry *)args->pokeathlon;
}

BOOL PokeathlonCourseRecord_Init(OverlayManager *man, int *state) {
    PokeathlonCourseRecord *work;
    PokeathlonCourseRecordArgs *args;
    SpriteResourceCountsListUnion counts;

    HandleLoadOverlay(FS_OVERLAY_ID(OVY_98), OVY_LOAD_ASYNC);
    args = OverlayManager_GetArgs(man);
    GF_ASSERT(args != NULL);
    Heap_Create(HEAP_ID_3, HEAP_ID_132, 0x30000);
    work = OverlayManager_CreateAndGetData(man, sizeof(PokeathlonCourseRecord), HEAP_ID_132);
    MI_CpuFill8(work, 0, sizeof(PokeathlonCourseRecord));
    work->heapId = HEAP_ID_132;
    work->selection.scrolling = FALSE;
    work->selection.pageChanged = TRUE;
    work->selection.scrollForward = TRUE;
    PokeathlonCourseRecord_CopyArgs(work, args);
    PokeathlonUtil_ResetDisplay();
    PokeathlonCourseRecord_InitBgs(work);
    PokeathlonCourseRecord_LoadBgGraphics(work);
    work->textContext = PokeathlonUtil_CreateTextContext(work->heapId, work->bgConfig, 17, sPokeathlonCourseRecordWindowTemplates, NARC_msg_msg_0314_bin);
    counts = sPokeathlonCourseRecordSpriteResourceCounts;
    work->spriteContext = PokeathlonUtil_CreateSpriteContext(work->heapId, &counts, 26);
    PokeathlonCourseRecord_LoadSpriteResources(work);
    PokeathlonCourseRecord_CreateSprites(work);
    PokeathlonCourseRecord_SetMonSpritesVisible(work, FALSE);
    PokeathlonCourseRecord_ShowCoursePanel(work, 0);
    PokeathlonCourseRecord_PrintPage(work, 0);
    PokeathlonCourseRecord_UpdatePageSprites(work, 0);
    PokeathlonCourseRecord_PrintStaticText(work);
    SetKeyRepeatTimers(2, 4);
    ResetVisibleHardwareWindows(PM_LCD_TOP);
    ResetVisibleHardwareWindows(PM_LCD_BOTTOM);
    Main_SetVBlankIntrCB((GFIntrCB)PokeathlonCourseRecord_VBlankCallback, work);
    return TRUE;
}

BOOL PokeathlonCourseRecord_Exit(OverlayManager *man, int *state) {
    PokeathlonCourseRecord *work = OverlayManager_GetData(man);

    SetKeyRepeatTimers(4, 8);
    PokeathlonCourseRecord_FreeArgs(work->args);
    OverlayManager_FreeData(man);
    Heap_Destroy(HEAP_ID_132);
    UnloadOverlayByID(FS_OVERLAY_ID(OVY_98));
    return TRUE;
}

BOOL PokeathlonCourseRecord_Main(OverlayManager *man, int *state) {
    PokeathlonCourseRecord *work = OverlayManager_GetData(man);

    PokeathlonCourseRecord_UpdatePageScroll(work);
    if (work->selection.pageChanged == TRUE) {
        PokeathlonCourseRecord_ShowCoursePanel(work, work->selection.page);
        PokeathlonCourseRecord_PrintPage(work, work->selection.page);
        PokeathlonCourseRecord_UpdatePageSprites(work, work->selection.page);
        work->selection.pageChanged = FALSE;
        work->selection.scrolling = TRUE;
    }
    switch (*state) {
    case RECORD_SCREEN_STATE_FADE_IN:
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, RGB_BLACK, 6, 1, work->heapId);
        *state = RECORD_SCREEN_STATE_WAIT_FADE_IN;
        break;
    case RECORD_SCREEN_STATE_WAIT_FADE_IN:
        if (IsPaletteFadeFinished()) {
            *state = RECORD_SCREEN_STATE_INPUT;
        }
        break;
    case RECORD_SCREEN_STATE_INPUT:
        if (PokeathlonCourseRecord_HandleInput(work) && work->selection.scrolling == FALSE) {
            PlaySE(SEQ_SE_DP_SELECT);
            BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, RGB_BLACK, 6, 1, work->heapId);
            *state = RECORD_SCREEN_STATE_WAIT_FADE_OUT;
        }
        break;
    case RECORD_SCREEN_STATE_WAIT_FADE_OUT:
        if (IsPaletteFadeFinished()) {
            *state = RECORD_SCREEN_STATE_EXIT;
        }
        break;
    case RECORD_SCREEN_STATE_EXIT:
        PokeathlonUtil_FreeTextContext(work->textContext, 17);
        PokeathlonCourseRecord_FreeBgs(work);
        PokeathlonUtil_FreeSpriteContext(work->spriteContext, work->sprites, 26);
        PokeathlonUtil_DisableDisplay();
        return TRUE;
    default:
        GF_ASSERT(!"seq error!");
        break;
    }
    PokeathlonUtil_DrawSprites(work->spriteContext);
    return FALSE;
}

static void PokeathlonCourseRecord_CopyArgs(PokeathlonCourseRecord *work, const PokeathlonCourseRecordArgs *args) {
    PokeathlonCourseRecordArgs *copy = Heap_Alloc(work->heapId, sizeof(PokeathlonCourseRecordArgs));
    copy->pokeathlon = args->pokeathlon;
    copy->allGoalsMet = args->allGoalsMet;
    work->args = copy;
}

static BOOL PokeathlonCourseRecord_AreFlags0EFTo0F1Set(PokeathlonCourseRecordArgs *args) {
    return args->allGoalsMet;
}

static void PokeathlonCourseRecord_FreeArgs(PokeathlonCourseRecordArgs *args) {
    Heap_Free(args);
}

static PokeathlonRecordMon *PokeathlonCourseRecord_GetMon(PokeathlonCourseRecordArgs *args, int course, int slot) {
    return &PokeathlonCourseRecord_GetRecords(args)[course].mons[slot];
}

static u16 PokeathlonCourseRecord_GetTotalScore(PokeathlonCourseRecordArgs *args, int course) {
    GF_ASSERT(args != NULL);
    GF_ASSERT(course < 5);
    return PokeathlonCourseRecord_GetRecords(args)[course].totalScore;
}

static BOOL PokeathlonCourseRecord_HasRecord(PokeathlonCourseRecordArgs *args, int course) {
    GF_ASSERT(args != NULL);
    return PokeathlonCourseRecord_GetTotalScore(args, course) != 0;
}

static u16 PokeathlonCourseRecord_GetScore(PokeathlonCourseRecordArgs *args, int course, int event) {
    GF_ASSERT(args != NULL);
    if (event == 3) {
        u16 bonus = PokeathlonCourseRecord_GetRecords(args)[course].totalScore - PokeathlonCourseRecord_GetRecords(args)[course].eventScores[0];
        bonus -= PokeathlonCourseRecord_GetRecords(args)[course].eventScores[1];
        bonus -= PokeathlonCourseRecord_GetRecords(args)[course].eventScores[2];
        return bonus;
    }
    return PokeathlonCourseRecord_GetRecords(args)[course].eventScores[event];
}

static int PokeathlonCourseRecord_GetEventId(PokeathlonCourseRecordArgs *args, s32 course, u32 event) {
    const u8 *events;

    GF_ASSERT(args != NULL);
    GF_ASSERT(course < 10);
    GF_ASSERT(event < 4);
    events = sPokeathlonCourseRecordCourseEvents[course];
    return events[event];
}

static u16 PokeathlonCourseRecord_GetMonSpecies(PokeathlonCourseRecordArgs *args, u32 course, u32 slot) {
    GF_ASSERT(args != NULL);
    GF_ASSERT(slot < 3);
    GF_ASSERT(course < 5);
    return PokeathlonCourseRecord_GetRecords(args)[course].mons[slot].info.species;
}

static void PokeathlonCourseRecord_InitBgs(PokeathlonCourseRecord *work) {
    GraphicsModes modes;
    PokeathlonCourseRecordBgTemplates templates;
    int i;

    work->bgConfig = BgConfig_Alloc(work->heapId);
    modes = sPokeathlonCourseRecordGraphicsModes;
    SetBothScreensModesAndDisable(&modes);
    templates = sPokeathlonCourseRecordBgTemplates;
    for (i = 0; i < 8; i++) {
        InitBgFromTemplate(work->bgConfig, sPokeathlonCourseRecordBgLayers[i], &templates.templates[i], GF_BG_TYPE_TEXT);
        BgClearTilemapBufferAndCommit(work->bgConfig, sPokeathlonCourseRecordBgLayers[i]);
        BG_ClearCharDataRange(sPokeathlonCourseRecordBgLayers[i], 32, 0, work->heapId);
    }
    G2_SetBlendAlpha(GX_BLEND_PLANEMASK_BG3, GX_BLEND_PLANEMASK_BG2, 15, 0);
    G2S_SetBlendAlpha(GX_BLEND_PLANEMASK_BG3, GX_BLEND_PLANEMASK_BG1, 15, 0);
    work->narc = NARC_New(NARC_a_1_7_7, work->heapId);
}

static void PokeathlonCourseRecord_FreeBgs(PokeathlonCourseRecord *work) {
    u8 i;

    NARC_Delete(work->narc);
    for (i = 0; i < 8; i++) {
        FreeBgTilemapBuffer(work->bgConfig, sPokeathlonCourseRecordBgLayers[i]);
    }
    Heap_Free(work->bgConfig);
}

static void PokeathlonCourseRecord_LoadBgGraphics(PokeathlonCourseRecord *work) {
    GfGfxLoader_GXLoadPalFromOpenNarc(work->narc, 30, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_0_OFFSET, 0, work->heapId);
    GfGfxLoader_LoadCharDataFromOpenNarc(work->narc, 31, work->bgConfig, GF_BG_LYR_MAIN_1, 0, 0, TRUE, work->heapId);
    GfGfxLoader_LoadScrnDataFromOpenNarc(work->narc, 32, work->bgConfig, GF_BG_LYR_MAIN_1, 0, 0, TRUE, work->heapId);
    GfGfxLoader_LoadCharDataFromOpenNarc(work->narc, 62, work->bgConfig, GF_BG_LYR_MAIN_2, 0, 0, TRUE, work->heapId);
    GfGfxLoader_GXLoadPalFromOpenNarc(work->narc, 36, GF_PAL_LOCATION_MAIN_BG, (enum GFPalSlotOffset)0x1E0, 0x20, work->heapId);
    GfGfxLoader_LoadCharDataFromOpenNarc(work->narc, 37, work->bgConfig, GF_BG_LYR_MAIN_3, 0, 0, TRUE, work->heapId);
    GfGfxLoader_LoadScrnDataFromOpenNarc(work->narc, 38, work->bgConfig, GF_BG_LYR_MAIN_3, 0, 0, TRUE, work->heapId);
    GfGfxLoader_GXLoadPalFromOpenNarc(work->narc, 33, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_0_OFFSET, 0, work->heapId);
    GfGfxLoader_LoadCharDataFromOpenNarc(work->narc, 34, work->bgConfig, GF_BG_LYR_SUB_0, 0, 0, TRUE, work->heapId);
    GfGfxLoader_LoadScrnDataFromOpenNarc(work->narc, 35, work->bgConfig, GF_BG_LYR_SUB_0, 0, 0, TRUE, work->heapId);
    GfGfxLoader_LoadCharDataFromOpenNarc(work->narc, 68, work->bgConfig, GF_BG_LYR_SUB_1, 0, 0, TRUE, work->heapId);
    GfGfxLoader_GXLoadPalFromOpenNarc(work->narc, 39, GF_PAL_LOCATION_SUB_BG, (enum GFPalSlotOffset)0x1E0, 0x20, work->heapId);
    GfGfxLoader_LoadCharDataFromOpenNarc(work->narc, 40, work->bgConfig, GF_BG_LYR_SUB_3, 0, 0, TRUE, work->heapId);
    GfGfxLoader_LoadScrnDataFromOpenNarc(work->narc, 41, work->bgConfig, GF_BG_LYR_SUB_3, 0, 0, TRUE, work->heapId);
}

static void PokeathlonCourseRecord_StartPageScroll(PokeathlonCourseRecord *work, int course, BOOL forward) {
    s16 offset;
    u8 i;

    if (forward) {
        offset = -256;
    } else {
        offset = 256;
    }
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_OFF);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG1, GF_PLANE_TOGGLE_OFF);
    GfGfxLoader_LoadScrnDataFromOpenNarc(work->narc, sPokeathlonCourseRecordScreenFiles[course][0], work->bgConfig, GF_BG_LYR_MAIN_2, 0, 0, TRUE, work->heapId);
    GfGfxLoader_LoadScrnDataFromOpenNarc(work->narc, sPokeathlonCourseRecordScreenFiles[course][1], work->bgConfig, GF_BG_LYR_SUB_1, 0, 0, TRUE, work->heapId);
    BgSetPosTextAndCommit(work->bgConfig, GF_BG_LYR_MAIN_2, BG_POS_OP_SET_X, offset);
    BgSetPosTextAndCommit(work->bgConfig, GF_BG_LYR_SUB_1, BG_POS_OP_SET_X, offset);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_ON);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG1, GF_PLANE_TOGGLE_ON);
    for (i = 0; i < 6; i++) {
        ManagedSprite_OffsetPositionXY(work->sprites[sPokeathlonCourseRecordScrollSpritePositions[i].sprite], -offset, 0);
    }
}

static void PokeathlonCourseRecord_UpdatePageScroll(PokeathlonCourseRecord *work) {
    s32 xpos;
    s32 x;
    u8 i;

    if (work->selection.scrolling == FALSE) {
        return;
    }
    xpos = Bg_GetXpos(work->bgConfig, GF_BG_LYR_MAIN_2);
    x = xpos >> 2;
    if (xpos == x) {
        x = 0;
    }
    ScheduleSetBgPosText(work->bgConfig, GF_BG_LYR_MAIN_2, BG_POS_OP_SET_X, x);
    ScheduleSetBgPosText(work->bgConfig, GF_BG_LYR_SUB_1, BG_POS_OP_SET_X, x);
    for (i = 0; i < 6; i++) {
        const PokeathlonCourseRecordSpritePos *pos = &sPokeathlonCourseRecordScrollSpritePositions[i];
        ManagedSprite_SetPositionXY(work->sprites[pos->sprite], pos->x - x, pos->y);
    }
    if (x == 0) {
        work->selection.scrolling = FALSE;
    }
}

static void PokeathlonCourseRecord_ShowCoursePanel(PokeathlonCourseRecord *work, u32 course) {
    if (PokeathlonCourseRecord_HasRecord(work->args, course)) {
        PokeathlonCourseRecord_StartPageScroll(work, course, work->selection.scrollForward);
    } else {
        BgClearTilemapBufferAndCommit(work->bgConfig, GF_BG_LYR_MAIN_2);
        BgClearTilemapBufferAndCommit(work->bgConfig, GF_BG_LYR_SUB_1);
    }
}

static void PokeathlonCourseRecord_PrintStaticText(PokeathlonCourseRecord *work) {
    PokeathlonUtil_PrintMessageAtXY(work->textContext, 0, msg_0314_00002, 1, 0);
    PokeathlonUtil_PrintAlignedMessageAtY(work->textContext, 2, msg_0314_00000, TRUE, 0, 4);
    PokeathlonUtil_PrintAlignedMessage(work->textContext, 11, msg_0314_00007, FALSE);
}

static void PokeathlonCourseRecord_PrintPage(PokeathlonCourseRecord *work, u32 course) {
    PokeathlonCourseRecord_PrintCourseName(work, course);
    PokeathlonCourseRecord_PrintEventScores(work, course);
    PokeathlonCourseRecord_PrintMonNames(work, PokeathlonCourseRecord_HasRecord(work->args, course));
    PokeathlonCourseRecord_PrintTotalLabel(work);
}

static void PokeathlonCourseRecord_PrintEventScores(PokeathlonCourseRecord *work, int course) {
    MessageFormat *msgFormat = PokeathlonUtil_GetMessageFormat(work->textContext);
    int i;
    const u32 *windows = sPokeathlonCourseRecordScoreWindows;

    for (i = 0; i < 3; i++, windows += 2) {
        BufferPokeathlonEventName(msgFormat, 1, (u8)PokeathlonCourseRecord_GetEventId(work->args, course, (u8)i));
        PokeathlonUtil_PrintMessageWithNumber(work->textContext, windows[0], msg_0314_00010, i + 1, 1, 0);
        if (PokeathlonCourseRecord_HasRecord(work->args, course)) {
            PokeathlonUtil_PrintMessageWithNumber(work->textContext, windows[1], msg_0314_00008, PokeathlonCourseRecord_GetScore(work->args, course, (u16)i), 3, 0);
        } else {
            PokeathlonUtil_PrintAlignedMessage(work->textContext, windows[1], msg_0314_00009, FALSE);
        }
    }
    if (PokeathlonCourseRecord_HasRecord(work->args, course)) {
        PokeathlonUtil_PrintMessageWithNumber(work->textContext, 12, msg_0314_00008, PokeathlonCourseRecord_GetScore(work->args, course, 3), 3, 0);
    } else {
        PokeathlonUtil_PrintAlignedMessage(work->textContext, 12, msg_0314_00009, FALSE);
    }
}

static void PokeathlonCourseRecord_PrintMonNames(PokeathlonCourseRecord *work, BOOL show) {
    s32 i;
    PokeathlonTextContext *textContext = work->textContext;

    for (i = 0; i < 3; i++) {
        if (show) {
            PokeathlonUtil_PrintSpeciesName(textContext, i + 14, PokeathlonCourseRecord_GetMonSpecies(work->args, (u8)work->selection.page, (u8)i));
        } else {
            PokeathlonUtil_ClearAndCopyWindow(textContext, i + 14);
        }
    }
}

static void PokeathlonCourseRecord_PrintCourseName(PokeathlonCourseRecord *work, int course) {
    PokeathlonTextContext *textContext = work->textContext;
    MessageFormat *msgFormat = PokeathlonUtil_GetMessageFormat(textContext);
    int medal;

    BufferPokeathlonMedalName(msgFormat, 0, (u8)course);
    PokeathlonUtil_PrintMessageAtXY(work->textContext, 1, msg_0314_00004, 1, 0);
    BufferPokeathlonCourseName(msgFormat, 0, course & 0xFF);
    PokeathlonUtil_PrintAlignedMessage(work->textContext, 3, msg_0314_00003, TRUE);
    if (PokeathlonCourseRecord_AreFlags0EFTo0F1Set(work->args)) {
        medal = PokeathlonUtil_GetCourseMedal(PokeathlonCourseRecord_GetTotalScore(work->args, course));
        if (medal == 0) {
            PokeathlonUtil_PrintAlignedMessage(work->textContext, 4, msg_0314_00005, TRUE);
        } else if (medal == 1) {
            PokeathlonUtil_PrintAlignedMessage(work->textContext, 4, msg_0314_00006, TRUE);
        } else {
            FillWindowPixelBuffer(&textContext->windows[4], 0);
            ScheduleWindowCopyToVram(&textContext->windows[4]);
        }
    }
}

static void PokeathlonCourseRecord_PrintTotalLabel(PokeathlonCourseRecord *work) {
    PokeathlonTextContext *textContext = work->textContext;

    if (PokeathlonCourseRecord_HasRecord(work->args, work->selection.page)) {
        PokeathlonUtil_PrintAlignedMessage(work->textContext, 13, msg_0314_00001, FALSE);
    } else {
        FillWindowPixelBuffer(&textContext->windows[13], 0);
        ScheduleWindowCopyToVram(&textContext->windows[13]);
    }
}

static void PokeathlonCourseRecord_VBlankCallback(PokeathlonCourseRecord *work) {
    vu8 *dtcm = (vu8 *)0x027E0000;

    SpriteSystem_TransferOam();
    DoScheduledBgGpuUpdates(work->bgConfig);
    *(vu32 *)(dtcm + 0x3FF8) |= OS_IE_V_BLANK; // OS_SetIrqCheckFlag(OS_IE_V_BLANK), DTCM address as a literal
}

static void PokeathlonCourseRecord_LoadSpriteResources(PokeathlonCourseRecord *work) {
    SpriteSystem *spriteSystem = PokeathlonUtil_GetSpriteSystem(work->spriteContext);
    SpriteManager *spriteManager = PokeathlonUtil_GetSpriteManager(work->spriteContext);

    SpriteSystem_LoadCharResObjFromOpenNarc(spriteSystem, spriteManager, work->narc, 21, TRUE, NNS_G2D_VRAM_TYPE_2DMAIN, 0);
    SpriteSystem_LoadPlttResObjFromOpenNarc(spriteSystem, spriteManager, work->narc, 20, FALSE, 1, NNS_G2D_VRAM_TYPE_2DMAIN, 0);
    SpriteSystem_LoadCellResObjFromOpenNarc(spriteSystem, spriteManager, work->narc, 22, TRUE, 0);
    SpriteSystem_LoadAnimResObjFromOpenNarc(spriteSystem, spriteManager, work->narc, 23, TRUE, 0);

    SpriteSystem_LoadCharResObjFromOpenNarc(spriteSystem, spriteManager, work->narc, 1, TRUE, NNS_G2D_VRAM_TYPE_2DMAIN, 1);
    SpriteSystem_LoadPlttResObjFromOpenNarc(spriteSystem, spriteManager, work->narc, 0, FALSE, 1, NNS_G2D_VRAM_TYPE_2DMAIN, 1);
    SpriteSystem_LoadCellResObjFromOpenNarc(spriteSystem, spriteManager, work->narc, 2, TRUE, 1);
    SpriteSystem_LoadAnimResObjFromOpenNarc(spriteSystem, spriteManager, work->narc, 3, TRUE, 1);

    SpriteSystem_LoadCharResObjFromOpenNarc(spriteSystem, spriteManager, work->narc, 5, TRUE, NNS_G2D_VRAM_TYPE_2DMAIN, 2);
    SpriteSystem_LoadPlttResObjFromOpenNarc(spriteSystem, spriteManager, work->narc, 4, FALSE, 5, NNS_G2D_VRAM_TYPE_2DMAIN, 2);
    SpriteSystem_LoadCellResObjFromOpenNarc(spriteSystem, spriteManager, work->narc, 6, TRUE, 2);
    SpriteSystem_LoadAnimResObjFromOpenNarc(spriteSystem, spriteManager, work->narc, 7, TRUE, 2);

    SpriteSystem_LoadCharResObjFromOpenNarc(spriteSystem, spriteManager, work->narc, 9, TRUE, NNS_G2D_VRAM_TYPE_2DSUB, 6);
    SpriteSystem_LoadPlttResObjFromOpenNarc(spriteSystem, spriteManager, work->narc, 8, FALSE, 1, NNS_G2D_VRAM_TYPE_2DSUB, 3);
    SpriteSystem_LoadCellResObjFromOpenNarc(spriteSystem, spriteManager, work->narc, 10, TRUE, 3);
    SpriteSystem_LoadAnimResObjFromOpenNarc(spriteSystem, spriteManager, work->narc, 11, TRUE, 3);

    PokeathlonCourseRecord_LoadMonPicResources(work);
    PokeathlonUtil_LoadMonIconResources(work->spriteContext, sPokeathlonCourseRecordMonIconResIds, 3, 0, TRUE);
}

static void PokeathlonCourseRecord_LoadMonPicResources(PokeathlonCourseRecord *work) {
    u8 i;
    SpriteSystem *spriteSystem = PokeathlonUtil_GetSpriteSystem(work->spriteContext);
    SpriteManager *spriteManager = PokeathlonUtil_GetSpriteManager(work->spriteContext);

    for (i = 0; i < 3; i++) {
        PokeathlonUtil_LoadMonPicSpriteResources(spriteSystem, spriteManager, sPokeathlonCourseRecordMonPicResIds[i], NNS_G2D_VRAM_TYPE_2DMAIN);
    }
}

static void PokeathlonCourseRecord_CreateSprites(PokeathlonCourseRecord *work) {
    int i;
    s32 course;
    const ManagedSpriteTemplate *spriteTemplate = sPokeathlonCourseRecordSpriteTemplates;

    for (i = 0; i < 26; i++) {
        work->sprites[i] = PokeathlonUtil_NewSprite(work->spriteContext, spriteTemplate);
        ManagedSprite_SetAnimateFlag(work->sprites[i], TRUE);
        spriteTemplate++;
    }
    for (course = 0; course < 5; course++) {
        int medal;
        if (!PokeathlonCourseRecord_HasRecord(work->args, course)) {
            ManagedSprite_SetDrawFlag(work->sprites[1 + course], FALSE);
        }
        medal = PokeathlonUtil_GetCourseMedal(PokeathlonCourseRecord_GetTotalScore(work->args, course));
        if (medal == 2) {
            if (course % 2 == 0) {
                ManagedSprite_SetAnim(work->sprites[6 + course], 5);
            } else {
                ManagedSprite_SetAnim(work->sprites[6 + course], 6);
            }
        } else if (medal == 1) {
            if (course % 2 == 0) {
                ManagedSprite_SetAnim(work->sprites[6 + course], 2);
            } else {
                ManagedSprite_SetAnim(work->sprites[6 + course], 3);
            }
        } else {
            ManagedSprite_SetDrawFlag(work->sprites[6 + course], FALSE);
        }
    }
}

static void PokeathlonCourseRecord_SetMonSpritesVisible(PokeathlonCourseRecord *work, BOOL visible) {
    ManagedSprite_SetDrawFlag(work->sprites[14], visible);
    ManagedSprite_SetDrawFlag(work->sprites[15], visible);
    ManagedSprite_SetDrawFlag(work->sprites[16], visible);
    ManagedSprite_SetDrawFlag(work->sprites[11], visible);
    ManagedSprite_SetDrawFlag(work->sprites[12], visible);
    ManagedSprite_SetDrawFlag(work->sprites[13], visible);
    ManagedSprite_SetDrawFlag(work->sprites[17], visible);
    ManagedSprite_SetDrawFlag(work->sprites[18], visible);
    ManagedSprite_SetDrawFlag(work->sprites[19], visible);
}

static void PokeathlonCourseRecord_UpdatePageSprites(PokeathlonCourseRecord *work, int course) {
    u32 totalScore = PokeathlonCourseRecord_GetTotalScore(work->args, course);
    u8 i;

    for (i = 0; i < 3; i++) {
        if (PokeathlonCourseRecord_HasRecord(work->args, course) == TRUE) {
            u16 digit = PokeathlonUtil_GetDecimalDigit(totalScore, (u8)(i + 1));
            ManagedSprite_SetAnim(work->sprites[20 + i], digit);
            ManagedSprite_SetAnim(work->sprites[14 + i], sPokeathlonCourseRecordDigitAnims[digit]);
            ManagedSprite_SetDrawFlag(work->sprites[14 + i], TRUE);
            ManagedSprite_SetDrawFlag(work->sprites[20 + i], TRUE);
        } else {
            ManagedSprite_SetDrawFlag(work->sprites[14 + i], FALSE);
            ManagedSprite_SetDrawFlag(work->sprites[20 + i], FALSE);
        }
    }
    for (i = 0; i < 3; i++) {
        PokeathlonRecordMon *mon = PokeathlonCourseRecord_GetMon(work->args, course, i);
        if (PokeathlonCourseRecord_HasRecord(work->args, course) == TRUE) {
            PokeathlonUtil_LoadMonPicSprite(work->sprites[11 + i], &mon->info, TRUE, FALSE, work->heapId);
            ManagedSprite_SetDrawFlag(work->sprites[11 + i], TRUE);
            PokeathlonUtil_SetMonIconSprite(work->spriteContext, work->sprites[23 + i], &mon->info, FALSE, TRUE);
            ManagedSprite_SetDrawFlag(work->sprites[23 + i], TRUE);
        } else {
            ManagedSprite_SetDrawFlag(work->sprites[23 + i], FALSE);
        }
    }
    for (i = 0; i < 3; i++) {
        ManagedSprite_SetDrawFlag(work->sprites[17 + i], PokeathlonCourseRecord_HasRecord(work->args, course));
    }
}

static BOOL PokeathlonCourseRecord_HandleInput(PokeathlonCourseRecord *work) {
    PokeathlonCourseRecordSelection *selection = &work->selection;
    int oldCursor = selection->cursor;
    int cursor;
    BOOL exit = FALSE;

    if (gSystem.newKeys & PAD_BUTTON_B) {
        return TRUE;
    }
    if (gSystem.touchNew) {
        int hit = TouchscreenHitbox_FindHitboxAtTouchNew(sPokeathlonCourseRecordHitboxes);
        if (hit != -1) {
            if (hit == 5) {
                exit = TRUE;
            } else {
                selection->scrollForward = selection->cursor < hit;
                ManagedSprite_SetAnim(work->sprites[0], 9);
                ManagedSprite_SetAnimateFlag(work->sprites[0], TRUE);
                PlaySE(SEQ_SE_DP_SELECT);
            }
            selection->cursor = hit;
        }
    } else if ((gSystem.newAndRepeatedKeys & PAD_KEY_RIGHT) || (gSystem.newAndRepeatedKeys & PAD_KEY_DOWN)) {
        if (oldCursor < 5) {
            selection->cursor++;
            selection->scrollForward = TRUE;
            PlaySE(SEQ_SE_DP_SELECT);
        }
    } else if ((gSystem.newAndRepeatedKeys & PAD_KEY_LEFT) || (gSystem.newAndRepeatedKeys & PAD_KEY_UP)) {
        if (oldCursor > 0) {
            selection->cursor--;
            selection->scrollForward = FALSE;
            PlaySE(SEQ_SE_DP_SELECT);
        }
    } else if ((gSystem.newKeys & PAD_BUTTON_A) && oldCursor == 5) {
        return TRUE;
    }
    cursor = selection->cursor;
    if (cursor > 5) {
        cursor = 5;
    } else if (cursor < 0) {
        cursor = 0;
    }
    selection->cursor = cursor;
    work->selection.page = selection->cursor;
    if (oldCursor != selection->cursor) {
        ManagedSprite_SetPositionXY(work->sprites[0], sPokeathlonCourseRecordCursorPositions[selection->cursor][0], sPokeathlonCourseRecordCursorPositions[selection->cursor][1]);
        if (selection->cursor == 5) {
            ManagedSprite_SetAnim(work->sprites[0], 12);
        } else {
            if (ManagedSprite_GetActiveAnim(work->sprites[0]) != 9) {
                ManagedSprite_SetAnim(work->sprites[0], 8);
            }
            if (oldCursor != 5 || selection->cursor != 4) {
                PokeathlonCourseRecord_SetMonSpritesVisible(work, FALSE);
                work->selection.pageChanged = TRUE;
            }
        }
    }
    return exit;
}
