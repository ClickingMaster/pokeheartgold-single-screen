#include "pokeathlon/pokeathlon_event_intro.h"

#include "global.h"

#include "constants/flags.h"
#include "constants/sndseq.h"

#include "msgdata/msg.naix"
#include "msgdata/msg/msg_0309.h"
#include "pokeathlon/pokeathlon_save.h"

#include "assert.h"
#include "font.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "palette.h"
#include "poke_overlay.h"
#include "save_vars_flags.h"
#include "screen_fade.h"
#include "sound.h"
#include "sound_gb.h"
#include "sound_playback.h"
#include "sound_system.h"
#include "system.h"
#include "touchscreen.h"
#include "wireless_status_icon.h"

enum {
    EVENT_INTRO_STEP_APPEAR,
    EVENT_INTRO_STEP_DETAILS,
    EVENT_INTRO_STEP_RULES,
    EVENT_INTRO_STEP_CONTROLS,
    EVENT_INTRO_STEP_FADE_OUT,
};

enum PokeathlonEventIntroInitState {
    EVENT_INTRO_INIT_CREATE,
    EVENT_INTRO_INIT_GRAPHICS,
};

enum PokeathlonEventIntroAppearState {
    EVENT_INTRO_APPEAR_PLAY_BGM,
    EVENT_INTRO_APPEAR_FADE_IN_SUB,
    EVENT_INTRO_APPEAR_START_ZOOM,
    EVENT_INTRO_APPEAR_ZOOM,
    EVENT_INTRO_APPEAR_WAIT_FADE,
    EVENT_INTRO_APPEAR_LINK_WAIT,
};

enum PokeathlonEventIntroDetailsState {
    EVENT_INTRO_DETAILS_INPUT,
    EVENT_INTRO_DETAILS_WAIT_FADE,
};

enum PokeathlonEventIntroPageState {
    EVENT_INTRO_PAGE_STATE_FADE_IN,
    EVENT_INTRO_PAGE_STATE_INPUT,
    EVENT_INTRO_PAGE_STATE_WAIT_FADE,
};

enum PokeathlonEventIntroFadeOutState {
    EVENT_INTRO_FADE_OUT_START,
    EVENT_INTRO_FADE_OUT_WAIT,
};

FS_EXTERN_OVERLAY(OVY_98);

void SetBothScreensBrightnessByColor(u16 color);

typedef struct EventIntroBgTemplates {
    BgTemplate templates[7];
} EventIntroBgTemplates;

static void PokeathlonEventIntro_Create(PokeathlonCourseData *course);
static void PokeathlonEventIntro_InitBgs(BgConfig *bgConfig, enum HeapID heapId);
static void PokeathlonEventIntro_LoadGraphics(PokeathlonEventIntro *intro);
static void PokeathlonEventIntro_PrintDetails(PokeathlonEventIntro *intro);
static void PokeathlonEventIntro_InitSprites(PokeathlonEventIntro *intro);
static void PokeathlonEventIntro_LoadSpriteResources(PokeathlonEventIntro *intro);
static void PokeathlonEventIntro_CreateSprites(PokeathlonEventIntro *intro);
static void PokeathlonEventIntro_SetMascotAnim(PokeathlonEventIntro *intro);
static void PokeathlonEventIntro_HideZoomBgs(PokeathlonEventIntro *intro);

static const u8 sPokeathlonEventIntroBgTypes[7] = { 0, 0, 0, 0, 0, 2, 2 };

static const u8 sPokeathlonEventIntroBgLayers[7] = { 0, 1, 2, 4, 5, 6, 7 };

PokeathlonCourseStepFunc gPokeathlonEventIntroSteps[5] = {
    PokeathlonEventIntro_Appear,
    PokeathlonEventIntro_DetailsPage,
    PokeathlonEventIntro_RulesPage,
    PokeathlonEventIntro_ControlsPage,
    PokeathlonEventIntro_FadeOut,
};

static ManagedSpriteTemplate sPokeathlonEventIntroSpriteTemplates[4] = {
    {
     .x = 0,
     .y = 0,
     .z = 0,
     .animation = 5,
     .drawPriority = 0,
     .pal = 0xFFFF,
     .vram = NNS_G2D_VRAM_TYPE_2DMAIN,
     .resIdList = { 0, 0, 0, 0, 0, 0 },
     .bgPriority = 0,
     .vramTransfer = 0,
     },
    {
     .x = 0,
     .y = 0,
     .z = 0,
     .animation = 4,
     .drawPriority = 0,
     .pal = 0xFFFF,
     .vram = NNS_G2D_VRAM_TYPE_2DMAIN,
     .resIdList = { 0, 0, 0, 0, 0, 0 },
     .bgPriority = 0,
     .vramTransfer = 0,
     },
    {
     .x = 0,
     .y = 0,
     .z = 0,
     .animation = 0,
     .drawPriority = 0,
     .pal = 0xFFFF,
     .vram = NNS_G2D_VRAM_TYPE_2DMAIN,
     .resIdList = { 0, 0, 0, 0, 0, 0 },
     .bgPriority = 0,
     .vramTransfer = 0,
     },
    {
     .x = 0,
     .y = 0,
     .z = 0,
     .animation = 16,
     .drawPriority = 0,
     .pal = 0xFFFF,
     .vram = NNS_G2D_VRAM_TYPE_2DMAIN,
     .resIdList = { 0, 0, 0, 0, 0, 0 },
     .bgPriority = 0,
     .vramTransfer = 0,
     },
};

static const TouchscreenHitbox sPokeathlonEventIntroControlsHitboxes[3] = {
    { 0,   40, 144, 200 },
    { 0,   40, 200, 0   },
    { 255, 0,  0,   0   }
};

static const TouchscreenHitbox sPokeathlonEventIntroDetailsHitboxes[3] = {
    { 0,   40,  184, 0 },
    { 152, 184, 192, 0 },
    { 255, 0,   0,   0 }
};

static const TouchscreenHitbox sPokeathlonEventIntroRulesHitboxes[3] = {
    { 0,   40, 144, 200 },
    { 0,   40, 200, 0   },
    { 255, 0,  0,   0   }
};

static const GraphicsModes sPokeathlonEventIntroGraphicsModes = {
    .dispMode = GX_DISPMODE_GRAPHICS,
    .bgMode = GX_BGMODE_0,
    .subMode = GX_BGMODE_5,
    ._2d3dMode = GX_BG0_AS_2D,
};

static const OamCharTransferParam sPokeathlonEventIntroOamCharTransferParam = {
    .maxTasks = 0,
    .sizeMain = 0x20000,
    .sizeSub = 0x4000,
    .charModeMain = GX_OBJVRAMMODE_CHAR_1D_64K,
    .charModeSub = GX_OBJVRAMMODE_CHAR_1D_256K,
};

static const SpriteResourceCountsListUnion sPokeathlonEventIntroSpriteResourceCounts = { 1, 1, 1, 1, 0, 0 };

static const s16 sPokeathlonEventIntroSpritePositions[8][2] = {
    { 32,  24  },
    { 32,  112 },
    { 208, 112 },
    { 226, 180 },
    { 16,  16  },
    { 16,  16  },
    { 200, 160 },
    { 226, 180 }
};

static const OamManagerParam sPokeathlonEventIntroOamManagerParam = {
    .fromOBJmain = 0,
    .numOBJmain = 126,
    .fromAffineMain = 0,
    .numAffineMain = 32,
    .fromOBJsub = 1,
    .numOBJsub = 126,
    .fromAffineSub = 0,
    .numAffineSub = 32,
};

static const GraphicsBanks sPokeathlonEventIntroGraphicsBanks = {
    .bg = GX_VRAM_BG_128_A,
    .bgextpltt = GX_VRAM_BGEXTPLTT_NONE,
    .subbg = GX_VRAM_SUB_BG_128_C,
    .subbgextpltt = GX_VRAM_SUB_BGEXTPLTT_0123_H,
    .obj = GX_VRAM_OBJ_128_B,
    .objextpltt = GX_VRAM_OBJEXTPLTT_NONE,
    .subobj = GX_VRAM_SUB_OBJ_16_I,
    .subobjextpltt = GX_VRAM_SUB_OBJEXTPLTT_NONE,
    .tex = GX_VRAM_TEX_NONE,
    .texpltt = GX_VRAM_TEXPLTT_NONE,
};

static const int sPokeathlonEventIntroMascotAnims[10][2] = {
    { 6,  6  },
    { 7,  7  },
    { 8,  8  },
    { 9,  9  },
    { 10, 10 },
    { 11, 11 },
    { 12, 12 },
    { 13, 13 },
    { 14, 14 },
    { 15, 15 }
};

static const WindowTemplate sPokeathlonEventIntroWindowTemplates[12] = {
    {
     .bgId = GF_BG_LYR_MAIN_2,
     .left = 7,
     .top = 2,
     .width = 14,
     .height = 2,
     .palette = 12,
     .baseTile = 1,
     },
    {
     .bgId = GF_BG_LYR_MAIN_2,
     .left = 4,
     .top = 6,
     .width = 24,
     .height = 2,
     .palette = 12,
     .baseTile = 0x1D,
     },
    {
     .bgId = GF_BG_LYR_MAIN_2,
     .left = 7,
     .top = 13,
     .width = 14,
     .height = 2,
     .palette = 12,
     .baseTile = 0x4D,
     },
    {
     .bgId = GF_BG_LYR_MAIN_2,
     .left = 4,
     .top = 17,
     .width = 24,
     .height = 6,
     .palette = 12,
     .baseTile = 0x69,
     },
    {
     .bgId = GF_BG_LYR_MAIN_2,
     .left = 24,
     .top = 1,
     .width = 7,
     .height = 2,
     .palette = 3,
     .baseTile = 0xF9,
     },
    {
     .bgId = GF_BG_LYR_SUB_1,
     .left = 9,
     .top = 3,
     .width = 14,
     .height = 2,
     .palette = 12,
     .baseTile = 1,
     },
    {
     .bgId = GF_BG_LYR_SUB_1,
     .left = 5,
     .top = 18,
     .width = 22,
     .height = 2,
     .palette = 12,
     .baseTile = 0x1D,
     },
    {
     .bgId = GF_BG_LYR_SUB_1,
     .left = 5,
     .top = 20,
     .width = 22,
     .height = 2,
     .palette = 12,
     .baseTile = 0x49,
     },
    {
     .bgId = GF_BG_LYR_MAIN_2,
     .left = 5,
     .top = 1,
     .width = 12,
     .height = 2,
     .palette = 12,
     .baseTile = 1,
     },
    {
     .bgId = GF_BG_LYR_MAIN_2,
     .left = 19,
     .top = 1,
     .width = 5,
     .height = 2,
     .palette = 3,
     .baseTile = 0x19,
     },
    {
     .bgId = GF_BG_LYR_MAIN_2,
     .left = 26,
     .top = 1,
     .width = 5,
     .height = 2,
     .palette = 3,
     .baseTile = 0x23,
     },
    {
     .bgId = GF_BG_LYR_MAIN_2,
     .left = 2,
     .top = 5,
     .width = 28,
     .height = 18,
     .palette = 12,
     .baseTile = 0x2D,
     },
};

static const EventIntroBgTemplates sPokeathlonEventIntroBgTemplates = {
    .templates = {
                  {
            .x = 0,
            .y = 0,
            .bufferSize = 0x800,
            .baseTile = 0,
            .size = GF_BG_SCR_SIZE_256x256,
            .colorMode = GX_BG_COLORMODE_16,
            .screenBase = GX_BG_SCRBASE_0x0000,
            .charBase = GX_BG_CHARBASE_0x04000,
            .bgExtPltt = GX_BG_EXTPLTT_01,
            .priority = 2,
            .areaOver = GX_BG_AREAOVER_XLU,
            .mosaic = FALSE,
        },
                  {
            .x = 0,
            .y = 0,
            .bufferSize = 0x800,
            .baseTile = 0,
            .size = GF_BG_SCR_SIZE_256x256,
            .colorMode = GX_BG_COLORMODE_16,
            .screenBase = GX_BG_SCRBASE_0x0800,
            .charBase = GX_BG_CHARBASE_0x08000,
            .bgExtPltt = GX_BG_EXTPLTT_01,
            .priority = 1,
            .areaOver = GX_BG_AREAOVER_XLU,
            .mosaic = FALSE,
        },
                  {
            .x = 0,
            .y = 0,
            .bufferSize = 0x800,
            .baseTile = 0,
            .size = GF_BG_SCR_SIZE_256x256,
            .colorMode = GX_BG_COLORMODE_16,
            .screenBase = GX_BG_SCRBASE_0x1000,
            .charBase = GX_BG_CHARBASE_0x0c000,
            .bgExtPltt = GX_BG_EXTPLTT_01,
            .priority = 0,
            .areaOver = GX_BG_AREAOVER_XLU,
            .mosaic = FALSE,
        },
                  {
            .x = 0,
            .y = 0,
            .bufferSize = 0x800,
            .baseTile = 0,
            .size = GF_BG_SCR_SIZE_256x256,
            .colorMode = GX_BG_COLORMODE_16,
            .screenBase = GX_BG_SCRBASE_0x0000,
            .charBase = GX_BG_CHARBASE_0x04000,
            .bgExtPltt = GX_BG_EXTPLTT_01,
            .priority = 3,
            .areaOver = GX_BG_AREAOVER_XLU,
            .mosaic = FALSE,
        },
                  {
            .x = 0,
            .y = 0,
            .bufferSize = 0x800,
            .baseTile = 0,
            .size = GF_BG_SCR_SIZE_256x256,
            .colorMode = GX_BG_COLORMODE_16,
            .screenBase = GX_BG_SCRBASE_0x0800,
            .charBase = GX_BG_CHARBASE_0x08000,
            .bgExtPltt = GX_BG_EXTPLTT_01,
            .priority = 2,
            .areaOver = GX_BG_AREAOVER_XLU,
            .mosaic = FALSE,
        },
                  {
            .x = 0,
            .y = 0,
            .bufferSize = 0x800,
            .baseTile = 0,
            .size = GF_BG_SCR_SIZE_256x256,
            .colorMode = GX_BG_COLORMODE_256,
            .screenBase = GX_BG_SCRBASE_0x1000,
            .charBase = GX_BG_CHARBASE_0x10000,
            .bgExtPltt = GX_BG_EXTPLTT_01,
            .priority = 0,
            .areaOver = GX_BG_AREAOVER_XLU,
            .mosaic = FALSE,
        },
                  {
            .x = 0,
            .y = 0,
            .bufferSize = 0x800,
            .baseTile = 0,
            .size = GF_BG_SCR_SIZE_256x256,
            .colorMode = GX_BG_COLORMODE_256,
            .screenBase = GX_BG_SCRBASE_0x1800,
            .charBase = GX_BG_CHARBASE_0x14000,
            .bgExtPltt = GX_BG_EXTPLTT_01,
            .priority = 1,
            .areaOver = GX_BG_AREAOVER_XLU,
            .mosaic = FALSE,
        },
                  },
};

BOOL PokeathlonEventIntro_Init(PokeathlonCourseData *course) {
    PokeathlonEventIntro *intro = PokeathlonCourse_GetWork(course);
    switch (PokeathlonCourse_GetEventInitState(course)) {
    case EVENT_INTRO_INIT_CREATE:
        PokeathlonEventIntro_Create(course);
        PokeathlonCourse_AdvanceEventInitState(course);
        break;
    case EVENT_INTRO_INIT_GRAPHICS:
        PokeathlonEventIntro_InitSprites(intro);
        PokeathlonEventIntro_InitBgs(intro->bgConfig, intro->heapId);
        intro->banner = PokeathlonEventBanner_New(intro->event, intro->heapId);
        PokeathlonEventBanner_AddWindows(intro->banner, intro->bgConfig);
        PokeathlonEventIntro_LoadGraphics(intro);
        PokeathlonEventBanner_LoadGraphics(intro->banner);
        PokeathlonEventIntro_HideZoomBgs(intro);
        WirelessStatusIcon_CreateOnScreenIfConnected(2);
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
        GfGfx_EngineBTogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
        PokeathlonCourse_SetEventDrawEnabled(course, 1);
        PokeathlonCourse_SetEventInitState(course, 0);
        return TRUE;
    default:
        GF_AssertFail();
        break;
    }
    return FALSE;
}

BOOL PokeathlonEventIntro_Main(PokeathlonCourseData *course) {
    PokeathlonEventIntro *intro = PokeathlonCourse_GetWork(course);
    SpriteSystem_DrawSprites(intro->spriteManager);
    return TRUE;
}

BOOL PokeathlonEventIntro_Exit(PokeathlonCourseData *course) {
    PokeathlonEventIntro *intro = PokeathlonCourse_GetWork(course);
    int i;

    NARC_Delete(intro->narc);
    WirelessStatusIcon_Destroy();
    PokeathlonEventBanner_Free(intro->banner);
    PokeathlonUtil_FreeTextContext(intro->text, 12);
    for (i = 0; i < 7; i++) {
        FreeBgTilemapBuffer(intro->bgConfig, sPokeathlonEventIntroBgLayers[i]);
    }
    Heap_Free(intro->bgConfig);
    for (i = 0; i < 4; i++) {
        if (intro->sprites[i] != NULL) {
            Sprite_DeleteAndFreeResources(intro->sprites[i]);
            intro->sprites[i] = NULL;
        }
    }
    SpriteSystem_FreeResourcesAndManager(intro->spriteSystem, intro->spriteManager);
    SpriteSystem_Free(intro->spriteSystem);
    Main_SetVBlankIntrCB(NULL, NULL);
    Main_SetHBlankIntrCB(NULL, NULL);
    PokeathlonCourse_FreeWork(course);
    gSystem.screensFlipped = FALSE;
    GfGfx_SwapDisplay();
    UnloadOverlayByID(FS_OVERLAY_ID(OVY_98));
    Heap_Destroy(HEAP_ID_136);
    return TRUE;
}

static void PokeathlonEventIntro_VBlankCallback(PokeathlonEventIntro *intro) {
    SpriteSystem_TransferOam();
    DoScheduledBgGpuUpdates(intro->bgConfig);
    OS_SetIrqCheckFlag(OS_IE_V_BLANK);
}

static void PokeathlonEventIntro_SetBanks(void) {
    GraphicsBanks banks = sPokeathlonEventIntroGraphicsBanks;
    GfGfx_SetBanks(&banks);
}

// Whether the sequence variable of the playing BGM has reached value (the screen follows the music).
static BOOL PokeathlonEventIntro_IsBgmAt(u32 variable, s16 value) {
    s16 current;
    GF_SndReadSeqVariable(GF_GetCurrentPlayingBGM(), variable, &current);
    return current >= value;
}

static u8 PokeathlonEventIntro_GetEvent(PokeathlonCourseData *course) {
    return PokeathlonCourse_GetCurrentEvent(course);
}

static BOOL PokeathlonEventIntro_HasFlag0EF(PokeathlonCourseData *course) {
    SaveData *saveData = PokeathlonCourse_GetSaveData(course);
    SaveVarsFlags *varsFlags = Save_VarsFlags_Get(saveData);
    return Save_VarsFlags_CheckFlagInArray(varsFlags, FLAG_UNK_0EF);
}

#pragma opt_propagation off

static void PokeathlonEventIntro_Create(PokeathlonCourseData *course) {
    PokeathlonEventIntro *intro;
    enum HeapID heapId;
    PokeathlonCourse_GetWork(course);
    Heap_Create(HEAP_ID_POKEATHLON, HEAP_ID_136, 0x40000);
    heapId = HEAP_ID_136;
    HandleLoadOverlay(FS_OVERLAY_ID(OVY_98), OVY_LOAD_ASYNC);
    Main_SetVBlankIntrCB(NULL, NULL);
    Main_SetHBlankIntrCB(NULL, NULL);
    GfGfx_DisableEngineAPlanes();
    GfGfx_DisableEngineBPlanes();
    reg_GX_DISPCNT &= 0xFFFFE0FF;
    reg_GXS_DB_DISPCNT &= 0xFFFFE0FF;
    PokeathlonEventIntro_SetBanks();
    intro = PokeathlonCourse_AllocWork(course, sizeof(PokeathlonEventIntro));
    MI_CpuFill8(intro, 0, sizeof(PokeathlonEventIntro));
    intro->narc = NARC_New(NARC_a_2_1_9, heapId);
    intro->bgConfig = BgConfig_Alloc(heapId);
    intro->event = PokeathlonEventIntro_GetEvent(course);
    intro->round = PokeathlonCourse_GetRound(course);
    intro->linkMode = PokeathlonCourse_GetMode(course) == POKEATHLON_COURSE_MODE_LINK ? TRUE : FALSE;
    intro->course = course;
    intro->heapId = HEAP_ID_136;
    Main_SetVBlankIntrCB((GFIntrCB)PokeathlonEventIntro_VBlankCallback, intro);
    gSystem.screensFlipped = TRUE;
    GfGfx_SwapDisplay();
    SetBothScreensBrightnessByColor(0x7FFF);
    if (intro->linkMode) {
        intro->showRecord = FALSE;
    } else {
        intro->showRecord = PokeathlonEventIntro_HasFlag0EF(course);
    }
}

#pragma opt_propagation reset

static void PokeathlonEventIntro_LoadDetailsBg(PokeathlonEventIntro *intro) {
    LoadFontPal0(GF_PAL_LOCATION_SUB_BGEXT, (enum GFPalSlotOffset)0x4000, intro->heapId);
    PokeathlonEventBanner_LoadGraphics(intro->banner);
    GfGfxLoader_GXLoadPalFromOpenNarc(intro->narc, 4, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_0_OFFSET, 0, intro->heapId);
    GfGfxLoader_LoadCharDataFromOpenNarc(intro->narc, 5, intro->bgConfig, GF_BG_LYR_SUB_0, 0, 0, TRUE, intro->heapId);
    if (intro->showRecord != FALSE) {
        GfGfxLoader_LoadScrnDataFromOpenNarc(intro->narc, 10, intro->bgConfig, GF_BG_LYR_SUB_0, 0, 0, TRUE, intro->heapId);
    } else {
        GfGfxLoader_LoadScrnDataFromOpenNarc(intro->narc, 9, intro->bgConfig, GF_BG_LYR_SUB_0, 0, 0, TRUE, intro->heapId);
    }
}

// Picture of a rules / controls page of the event on sub BG 3.
static void PokeathlonEventIntro_LoadPagePicture(PokeathlonEventIntro *intro, int event) {
    MtxFx22 matrix;

    GF_ASSERT(event < 10);
    GfGfxLoader_GXLoadPalFromOpenNarc(intro->narc, event + 32, GF_PAL_LOCATION_SUB_BGEXT, (enum GFPalSlotOffset)0x6000, 0, intro->heapId);
    GfGfxLoader_LoadCharDataFromOpenNarc(intro->narc, event + 42, intro->bgConfig, GF_BG_LYR_SUB_3, 0, 0, TRUE, intro->heapId);
    GfGfxLoader_LoadScrnDataFromOpenNarc(intro->narc, event + 52, intro->bgConfig, GF_BG_LYR_SUB_3, 0, 0, TRUE, intro->heapId);
    BG_SetMaskColor(GF_BG_LYR_SUB_0, 0);
    matrix._00 = FX32_ONE;
    matrix._01 = 0;
    matrix._10 = 0;
    matrix._11 = FX32_ONE;
    OS_WaitVBlankIntr();
    SetBgAffine(intro->bgConfig, GF_BG_LYR_SUB_3, &matrix, 0, 0);
    Bg_SetTextDimAndAffineParams(intro->bgConfig, GF_BG_LYR_SUB_3, BG_POS_OP_SET_Y, 0, &matrix, 0, 0);
}

static void PokeathlonEventIntro_InitBgs(BgConfig *bgConfig, enum HeapID heapId) {
    EventIntroBgTemplates templates;
    GraphicsModes modes;
    int i;

    modes = sPokeathlonEventIntroGraphicsModes;
    SetBothScreensModesAndDisable(&modes);
    templates = sPokeathlonEventIntroBgTemplates;
    for (i = 0; i < 7; i++) {
        InitBgFromTemplate(bgConfig, sPokeathlonEventIntroBgLayers[i], &templates.templates[i], sPokeathlonEventIntroBgTypes[i]);
        BgClearTilemapBufferAndCommit(bgConfig, sPokeathlonEventIntroBgLayers[i]);
        BG_ClearCharDataRange(sPokeathlonEventIntroBgLayers[i], 0x40, 0, heapId);
    }
}

static void PokeathlonEventIntro_LoadGraphics(PokeathlonEventIntro *intro) {
    GfGfxLoader_GXLoadPalFromOpenNarc(intro->narc, 4, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_0_OFFSET, 0, intro->heapId);
    GfGfxLoader_LoadCharDataFromOpenNarc(intro->narc, 5, intro->bgConfig, GF_BG_LYR_MAIN_0, 0, 0, TRUE, intro->heapId);
    GfGfxLoader_LoadScrnDataFromOpenNarc(intro->narc, 6, intro->bgConfig, GF_BG_LYR_MAIN_0, 0, 0, TRUE, intro->heapId);
    if (intro->linkMode == FALSE) {
        GfGfxLoader_LoadCharDataFromOpenNarc(intro->narc, 5, intro->bgConfig, GF_BG_LYR_MAIN_1, 0, 0, TRUE, intro->heapId);
        GfGfxLoader_LoadScrnDataFromOpenNarc(intro->narc, 8, intro->bgConfig, GF_BG_LYR_MAIN_1, 0, 0, TRUE, intro->heapId);
    }
    PokeathlonEventIntro_LoadDetailsBg(intro);
    intro->text = PokeathlonUtil_CreateTextContext(intro->heapId, intro->bgConfig, 12, sPokeathlonEventIntroWindowTemplates, NARC_msg_msg_0309_bin);
    PokeathlonEventIntro_PrintDetails(intro);
}

// Best solo record of the event.
static u16 PokeathlonEventIntro_GetRecord(PokeathlonEventIntro *intro, int event) {
    Pokeathlon_RecordsSolo *records = PokeathlonSave_GetRecordsSolo2(Save_Pokeathlon_Get(PokeathlonCourse_GetSaveData(intro->course)));
    return records[event].entries[0].score;
}

static void PokeathlonEventIntro_PrintDetails(PokeathlonEventIntro *intro) {
    int event = intro->event;
    u8 number = intro->round + 1;
    if (!intro->showRecord) {
        PokeathlonUtil_SetWindowY(intro->text, 5, 5);
    }
    PokeathlonUtil_PrintAlignedMessage(intro->text, 0, msg_0309_00197, FALSE);
    PokeathlonUtil_PrintAlignedMessage(intro->text, 2, msg_0309_00208, FALSE);
    PokeathlonUtil_PrintMessageAtXY(intro->text, 1, event + msg_0309_00198, 0, 0);
    PokeathlonUtil_PrintMessageAtXY(intro->text, 3, event * 3 + msg_0309_00209, 0, 0);
    PokeathlonUtil_PrintMessageWithNumber(intro->text, 5, msg_0309_00239, number, 1, 0);
    if (!intro->linkMode) {
        PokeathlonUtil_PrintAlignedMessageAtY(intro->text, 4, msg_0309_00240, FALSE, 0, 4);
    }
    PokeathlonEventBanner_PrintTexts(intro->banner);
    if (intro->showRecord) {
        int record = PokeathlonEventIntro_GetRecord(intro, intro->event);
        if (record == 0xFFFF) {
            record = 0;
        }
        if (intro->event == 0) {
            // seconds and tenths (30 frames a second)
            MessageFormat *msgFormat = PokeathlonUtil_GetMessageFormat(intro->text);
            BufferIntegerAsString(msgFormat, 0, record / 30, 3, PRINTING_MODE_LEFT_ALIGN, TRUE);
            BufferIntegerAsString(msgFormat, 1, (record % 30) * 10 / 30, 1, PRINTING_MODE_LEFT_ALIGN, TRUE);
        } else if (intro->event == 6) {
            // fx22.10
            PokeathlonUtil_BufferNumber(intro->text, (u32)record >> 10, 2, 0);
            PokeathlonUtil_BufferNumber(intro->text, (record % 1024) * 10 / 1024, 1, 1);
        } else {
            PokeathlonUtil_BufferNumber(intro->text, record, 3, 0);
        }
        PokeathlonUtil_PrintAlignedMessage(intro->text, 6, msg_0309_00176, FALSE);
        PokeathlonUtil_PrintMessage(intro->text, 7, event + msg_0309_00177);
    }
}

static void PokeathlonEventIntro_PrintRules(PokeathlonEventIntro *intro) {
    int msgId = intro->event * 3 + 210;
    PokeathlonUtil_PrintAlignedMessage(intro->text, 8, msg_0309_00244, FALSE);
    PokeathlonUtil_PrintAlignedMessageAtY(intro->text, 9, msg_0309_00242, FALSE, 0, 4);
    PokeathlonUtil_PrintAlignedMessageAtY(intro->text, 10, msg_0309_00243, FALSE, 0, 4);
    PokeathlonUtil_PrintMessageAtXY(intro->text, 11, msgId, 0, 0);
}

static void PokeathlonEventIntro_PrintControls(PokeathlonEventIntro *intro) {
    int msgId = intro->event * 3 + 211;
    PokeathlonUtil_PrintAlignedMessage(intro->text, 8, msg_0309_00245, FALSE);
    PokeathlonUtil_PrintAlignedMessageAtY(intro->text, 9, msg_0309_00241, FALSE, 0, 4);
    PokeathlonUtil_PrintAlignedMessageAtY(intro->text, 10, msg_0309_00243, FALSE, 0, 4);
    PokeathlonUtil_PrintMessageAtXY(intro->text, 11, msgId, 0, 0);
}

static void PokeathlonEventIntro_InitSprites(PokeathlonEventIntro *intro) {
    SpriteResourceCountsListUnion counts = sPokeathlonEventIntroSpriteResourceCounts;
    OamManagerParam oamManagerParam = sPokeathlonEventIntroOamManagerParam;
    OamCharTransferParam charTransferParam = sPokeathlonEventIntroOamCharTransferParam;
    charTransferParam.maxTasks = 4;
    intro->spriteSystem = SpriteSystem_Alloc(intro->heapId);
    intro->spriteManager = SpriteManager_New(intro->spriteSystem);
    SpriteSystem_Init(intro->spriteSystem, &oamManagerParam, &charTransferParam, 32);
    SpriteSystem_InitSprites(intro->spriteSystem, intro->spriteManager, 4);
    SpriteSystem_InitManagerWithCapacities(intro->spriteSystem, intro->spriteManager, &counts);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
    G2dRenderer_SetSubSurfaceCoords(SpriteSystem_GetRenderer(intro->spriteSystem), 0, 2 << 20);
    PokeathlonEventIntro_LoadSpriteResources(intro);
    PokeathlonEventIntro_CreateSprites(intro);
}

static void PokeathlonEventIntro_LoadSpriteResources(PokeathlonEventIntro *intro) {
    SpriteSystem *spriteSystem = intro->spriteSystem;
    SpriteManager *spriteManager = intro->spriteManager;

    SpriteSystem_LoadPlttResObjFromOpenNarc(spriteSystem, spriteManager, intro->narc, 0, FALSE, 16, NNS_G2D_VRAM_TYPE_2DMAIN, 0);
    SpriteSystem_LoadCharResObjFromOpenNarc(spriteSystem, spriteManager, intro->narc, 1, TRUE, NNS_G2D_VRAM_TYPE_2DMAIN, 0);
    SpriteSystem_LoadCellResObjFromOpenNarc(spriteSystem, spriteManager, intro->narc, 2, TRUE, 0);
    SpriteSystem_LoadAnimResObjFromOpenNarc(spriteSystem, spriteManager, intro->narc, 3, TRUE, 0);
}

static void PokeathlonEventIntro_UpdateMascot(PokeathlonEventIntro *intro) {
    if (!ManagedSprite_IsAnimated(intro->sprites[2])) {
        intro->animStep++;
        if (intro->animStep >= 2) {
            intro->animStep = 0;
        }
        PokeathlonEventIntro_SetMascotAnim(intro);
    }
}

static void PokeathlonEventIntro_PlaceSprites(PokeathlonEventIntro *intro, int layout) {
    int i = 0;
    const s16(*position)[2] = sPokeathlonEventIntroSpritePositions + layout * 4;
    do {
        ManagedSprite_SetPositionXYWithSubscreenOffset(intro->sprites[i], (*position)[0], (*position)[1], 2 << 20);
        position++;
    } while (++i < 4);
}

static void PokeathlonEventIntro_ResetMascot(PokeathlonEventIntro *intro) {
    intro->animStep = 0;
    PokeathlonEventIntro_SetMascotAnim(intro);
}

// Places the templates at the positions of layout 0 and creates the sprites.

static void PokeathlonEventIntro_CreateSprites(PokeathlonEventIntro *intro) {
    const s16(*position)[2] = sPokeathlonEventIntroSpritePositions;
    int i = 0;
    s16 *coords = &sPokeathlonEventIntroSpriteTemplates[0].x;
    PokeathlonEventIntro *cursor = intro;
    ManagedSpriteTemplate *template = sPokeathlonEventIntroSpriteTemplates;
    for (; i < 4; i++) {
        SpriteSystem *spriteSystem = intro->spriteSystem;
        SpriteManager *spriteManager = intro->spriteManager;
        coords[0] = (*position)[0];
        coords[1] = (*position)[1];
        cursor->sprites[0] = SpriteSystem_NewSpriteWithYOffset(spriteSystem, spriteManager, template, 2 << 20);
        ManagedSprite_SetAnimateFlag(cursor->sprites[0], TRUE);
        position++;
        coords += sizeof(ManagedSpriteTemplate) / sizeof(s16);
        template ++;
        cursor = (PokeathlonEventIntro *)((u8 *)cursor + sizeof(ManagedSprite *));
    }
    ManagedSprite_SetAnimateFlag(intro->sprites[2], FALSE);
    PokeathlonEventIntro_SetMascotAnim(intro);
}

static void PokeathlonEventIntro_SetMascotAnim(PokeathlonEventIntro *intro) {
    ManagedSprite_SetAnim(intro->sprites[2], sPokeathlonEventIntroMascotAnims[intro->event][intro->animStep]);
}

static void PokeathlonEventIntro_HideZoomBgs(PokeathlonEventIntro *intro) {
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_OFF);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG3, GF_PLANE_TOGGLE_OFF);
}

static void PokeathlonEventIntro_StartZoom(PokeathlonEventIntro *intro) {
    int offset = 0;
    MtxFx22 matrix;

    if (intro->showRecord == FALSE) {
        offset = 32;
    }
    matrix._00 = 0x125;
    matrix._01 = 0;
    matrix._10 = 0;
    matrix._11 = 0x125;
    OS_WaitVBlankIntr();
    SetBgAffine(intro->bgConfig, GF_BG_LYR_SUB_3, &matrix, 128, 96);
    SetBgAffine(intro->bgConfig, GF_BG_LYR_SUB_2, &matrix, 128, 96);
    Bg_SetTextDimAndAffineParams(intro->bgConfig, GF_BG_LYR_SUB_3, BG_POS_OP_SUB_Y, offset, &matrix, 128, 96);
    Bg_SetTextDimAndAffineParams(intro->bgConfig, GF_BG_LYR_SUB_2, BG_POS_OP_SUB_Y, offset, &matrix, 128, 96);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_ON);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG3, GF_PLANE_TOGGLE_ON);
    intro->timer = 0;
    intro->zooming = TRUE;
}

static void PokeathlonEventIntro_UpdateZoom(PokeathlonEventIntro *intro) {
    MtxFx22 matrix;
    fx32 scale = (++intro->timer << FX32_SHIFT) / 14;

    matrix._00 = scale;
    matrix._01 = 0;
    matrix._10 = 0;
    matrix._11 = scale;
    OS_WaitVBlankIntr();
    SetBgAffine(intro->bgConfig, GF_BG_LYR_SUB_3, &matrix, 128, 96);
    SetBgAffine(intro->bgConfig, GF_BG_LYR_SUB_2, &matrix, 128, 96);
    if (intro->timer >= 14) {
        intro->timer = 0;
        intro->zooming = FALSE;
    }
}

static void PokeathlonEventIntro_FadeToPage(PokeathlonCourseData *course, int page) {
    PokeathlonEventIntro *intro = PokeathlonCourse_GetWork(course);

    BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, 0x7FFF, 4, 1, intro->heapId);
    intro->page = page;
}

static BOOL PokeathlonEventIntro_IsFadeFinished(PokeathlonCourseData *course) {
    return IsPaletteFadeFinished();
}

static void PokeathlonEventIntro_ShowDetails(PokeathlonEventIntro *intro) {
    MtxFx22 matrix;
    int y;

    GfGfxLoader_LoadScrnDataFromOpenNarc(intro->narc, 6, intro->bgConfig, GF_BG_LYR_MAIN_0, 0, 0, TRUE, intro->heapId);
    PokeathlonEventIntro_LoadDetailsBg(intro);
    PokeathlonEventIntro_PlaceSprites(intro, 0);
    ManagedSprite_SetDrawFlag(intro->sprites[0], TRUE);
    ManagedSprite_SetDrawFlag(intro->sprites[1], TRUE);
    ManagedSprite_SetDrawFlag(intro->sprites[2], TRUE);
    ManagedSprite_SetDrawFlag(intro->sprites[3], TRUE);
    ToggleBgLayer(GF_BG_LYR_SUB_1, GF_PLANE_TOGGLE_ON);
    ToggleBgLayer(GF_BG_LYR_SUB_2, GF_PLANE_TOGGLE_ON);
    ToggleBgLayer(GF_BG_LYR_SUB_0, GF_PLANE_TOGGLE_ON);
    ToggleBgLayer(GF_BG_LYR_MAIN_1, GF_PLANE_TOGGLE_ON);
    PokeathlonEventIntro_PrintDetails(intro);
    y = 0;
    if (intro->showRecord == FALSE) {
        y = -32;
    }
    matrix._00 = FX32_ONE;
    matrix._01 = 0;
    matrix._10 = 0;
    matrix._11 = FX32_ONE;
    OS_WaitVBlankIntr();
    SetBgAffine(intro->bgConfig, GF_BG_LYR_SUB_3, &matrix, 0, 0);
    Bg_SetTextDimAndAffineParams(intro->bgConfig, GF_BG_LYR_SUB_3, BG_POS_OP_SET_Y, y, &matrix, 0, 0);
}

static void PokeathlonEventIntro_ShowRules(PokeathlonEventIntro *intro) {
    PokeathlonEventIntro_LoadPagePicture(intro, intro->event);
    GfGfxLoader_LoadScrnDataFromOpenNarc(intro->narc, 7, intro->bgConfig, GF_BG_LYR_MAIN_0, 0, 0, TRUE, intro->heapId);
    PokeathlonEventIntro_PlaceSprites(intro, 1);
    ManagedSprite_SetDrawFlag(intro->sprites[0], TRUE);
    ManagedSprite_SetDrawFlag(intro->sprites[1], FALSE);
    ManagedSprite_SetDrawFlag(intro->sprites[2], FALSE);
    ManagedSprite_SetDrawFlag(intro->sprites[3], FALSE);
    ToggleBgLayer(GF_BG_LYR_SUB_1, GF_PLANE_TOGGLE_OFF);
    ToggleBgLayer(GF_BG_LYR_SUB_2, GF_PLANE_TOGGLE_OFF);
    ToggleBgLayer(GF_BG_LYR_SUB_0, GF_PLANE_TOGGLE_OFF);
    ToggleBgLayer(GF_BG_LYR_MAIN_1, GF_PLANE_TOGGLE_OFF);
    PokeathlonEventIntro_PrintRules(intro);
}

static void PokeathlonEventIntro_ShowControls(PokeathlonEventIntro *intro) {
    PokeathlonEventIntro_LoadPagePicture(intro, intro->event);
    GfGfxLoader_LoadScrnDataFromOpenNarc(intro->narc, 7, intro->bgConfig, GF_BG_LYR_MAIN_0, 0, 0, TRUE, intro->heapId);
    PokeathlonEventIntro_PlaceSprites(intro, 1);
    ManagedSprite_SetDrawFlag(intro->sprites[0], FALSE);
    ManagedSprite_SetDrawFlag(intro->sprites[1], TRUE);
    ManagedSprite_SetDrawFlag(intro->sprites[2], TRUE);
    ManagedSprite_SetDrawFlag(intro->sprites[3], FALSE);
    ToggleBgLayer(GF_BG_LYR_SUB_1, GF_PLANE_TOGGLE_OFF);
    ToggleBgLayer(GF_BG_LYR_SUB_2, GF_PLANE_TOGGLE_OFF);
    ToggleBgLayer(GF_BG_LYR_SUB_0, GF_PLANE_TOGGLE_OFF);
    ToggleBgLayer(GF_BG_LYR_MAIN_1, GF_PLANE_TOGGLE_OFF);
    PokeathlonEventIntro_PrintControls(intro);
}

static void PokeathlonEventIntro_ShowPage(PokeathlonCourseData *course) {
    PokeathlonEventIntro *intro = PokeathlonCourse_GetWork(course);

    BgClearTilemapBufferAndCommit(intro->bgConfig, GF_BG_LYR_MAIN_0);
    BgClearTilemapBufferAndCommit(intro->bgConfig, GF_BG_LYR_MAIN_2);
    BgClearTilemapBufferAndCommit(intro->bgConfig, GF_BG_LYR_SUB_0);
    BgClearTilemapBufferAndCommit(intro->bgConfig, GF_BG_LYR_SUB_1);
    BgClearTilemapBufferAndCommit(intro->bgConfig, GF_BG_LYR_SUB_2);
    BgClearTilemapBufferAndCommit(intro->bgConfig, GF_BG_LYR_SUB_3);
    PokeathlonEventIntro_ResetMascot(intro);
    switch (intro->page) {
    case EVENT_INTRO_PAGE_DETAILS:
        PokeathlonEventIntro_ShowDetails(intro);
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, 0x7FFF, 4, 1, intro->heapId);
        break;
    case EVENT_INTRO_PAGE_RULES:
        PokeathlonEventIntro_ShowRules(intro);
        break;
    case EVENT_INTRO_PAGE_CONTROLS:
        PokeathlonEventIntro_ShowControls(intro);
        break;
    default:
        GF_AssertFail();
        break;
    }
    ((void (*)(PokeathlonCourseData *, u32))PokeathlonCourse_SetNextState)(course, intro->page);
}

// The banner zooms in with the music; in link mode the screen goes on by itself after 3 seconds.
BOOL PokeathlonEventIntro_Appear(PokeathlonCourseData *course, u8 *state) {
    u8 *step = state;
    PokeathlonCourseData *data = course;
    PokeathlonEventIntro *intro = PokeathlonCourse_GetWork(data);
    PokeathlonEventIntro_UpdateMascot(intro);
    switch (*step) {
    case EVENT_INTRO_APPEAR_PLAY_BGM:
        PlayBGM(SEQ_GS_PT_TITLE);
        (*step)++;
        break;
    case EVENT_INTRO_APPEAR_FADE_IN_SUB:
        if (PokeathlonEventIntro_IsBgmAt(0, 0)) {
            BeginNormalPaletteFade(FADE_SUB_ONLY, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, 0x7FFF, 1, 1, intro->heapId);
            (*step)++;
        }
        break;
    case EVENT_INTRO_APPEAR_START_ZOOM:
        if (PokeathlonEventIntro_IsBgmAt(0, 1)) {
            PokeathlonEventIntro_StartZoom(intro);
            (*step)++;
        }
        break;
    case EVENT_INTRO_APPEAR_ZOOM:
        if (intro->zooming != FALSE) {
            PokeathlonEventIntro_UpdateZoom(intro);
        } else if (PokeathlonEventIntro_IsBgmAt(0, 3)) {
            BeginNormalPaletteFade(FADE_MAIN_ONLY, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, 0x7FFF, 12, 1, intro->heapId);
            ManagedSprite_SetAnimateFlag(intro->sprites[2], TRUE);
            ManagedSprite_SetDrawFlag(intro->sprites[3], intro->linkMode == FALSE);
            (*step)++;
        }
        break;
    case EVENT_INTRO_APPEAR_WAIT_FADE:
        if (IsPaletteFadeFinished()) {
            if (intro->linkMode != FALSE) {
                intro->timer = 0;
                (*step)++;
            } else {
                PokeathlonCourse_SetNextState(data, EVENT_INTRO_STEP_DETAILS);
            }
        }
        break;
    case EVENT_INTRO_APPEAR_LINK_WAIT:
        intro->timer++;
        if (intro->timer >= 180) {
            PokeathlonCourse_SetNextState(data, EVENT_INTRO_STEP_FADE_OUT);
        }
        break;
    default:
        GF_AssertFail();
        break;
    }
    return FALSE;
}

BOOL PokeathlonEventIntro_DetailsPage(PokeathlonCourseData *course, u8 *state) {
    PokeathlonEventIntro *intro = PokeathlonCourse_GetWork(course);
    PokeathlonEventIntro_UpdateMascot(intro);
    switch (*state) {
    case EVENT_INTRO_DETAILS_INPUT:
        if (IsPaletteFadeFinished()) {
            int button = TouchscreenHitbox_FindRectAtTouchNew(sPokeathlonEventIntroDetailsHitboxes);
            if (button != -1) {
                switch (button) {
                case 0:
                    PlaySE(SEQ_SE_DP_SELECT);
                    PokeathlonEventIntro_FadeToPage(course, EVENT_INTRO_PAGE_RULES);
                    break;
                case 1:
                    PokeathlonCourse_SetNextState(course, EVENT_INTRO_STEP_FADE_OUT);
                    break;
                default:
                    GF_AssertFail();
                    break;
                }
                (*state)++;
            }
        }
        break;
    case EVENT_INTRO_DETAILS_WAIT_FADE:
        if (PokeathlonEventIntro_IsFadeFinished(course)) {
            PokeathlonEventIntro_ShowPage(course);
        }
        break;
    default:
        GF_AssertFail();
        break;
    }
    return FALSE;
}

BOOL PokeathlonEventIntro_RulesPage(PokeathlonCourseData *course, u8 *state) {
    PokeathlonEventIntro *intro = PokeathlonCourse_GetWork(course);
    PokeathlonEventIntro_UpdateMascot(intro);
    switch (*state) {
    case EVENT_INTRO_PAGE_STATE_FADE_IN:
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, 0x7FFF, 4, 1, intro->heapId);
        (*state)++;
        // fallthrough
    case EVENT_INTRO_PAGE_STATE_INPUT:
        if (IsPaletteFadeFinished()) {
            int button = TouchscreenHitbox_FindRectAtTouchNew(sPokeathlonEventIntroRulesHitboxes);
            if (button == 0) {
                PlaySE(SEQ_SE_DP_SELECT);
                PokeathlonEventIntro_FadeToPage(course, EVENT_INTRO_PAGE_CONTROLS);
                (*state)++;
            } else if (button == 1) {
                PlaySE(SEQ_SE_DP_SELECT);
                PokeathlonEventIntro_FadeToPage(course, EVENT_INTRO_PAGE_DETAILS);
                (*state)++;
            }
        }
        break;
    case EVENT_INTRO_PAGE_STATE_WAIT_FADE:
        if (PokeathlonEventIntro_IsFadeFinished(course)) {
            PokeathlonEventIntro_ShowPage(course);
        }
        break;
    default:
        GF_AssertFail();
        break;
    }
    return FALSE;
}

BOOL PokeathlonEventIntro_ControlsPage(PokeathlonCourseData *course, u8 *state) {
    PokeathlonEventIntro *intro = PokeathlonCourse_GetWork(course);
    PokeathlonEventIntro_UpdateMascot(intro);
    switch (*state) {
    case EVENT_INTRO_PAGE_STATE_FADE_IN:
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, 0x7FFF, 4, 1, intro->heapId);
        (*state)++;
        // fallthrough
    case EVENT_INTRO_PAGE_STATE_INPUT:
        if (IsPaletteFadeFinished()) {
            int button = TouchscreenHitbox_FindRectAtTouchNew(sPokeathlonEventIntroControlsHitboxes);
            if (button == 0) {
                PlaySE(SEQ_SE_DP_SELECT);
                PokeathlonEventIntro_FadeToPage(course, EVENT_INTRO_PAGE_RULES);
                (*state)++;
            } else if (button == 1) {
                PlaySE(SEQ_SE_DP_SELECT);
                PokeathlonEventIntro_FadeToPage(course, EVENT_INTRO_PAGE_DETAILS);
                (*state)++;
            }
        }
        break;
    case EVENT_INTRO_PAGE_STATE_WAIT_FADE:
        if (PokeathlonEventIntro_IsFadeFinished(course)) {
            PokeathlonEventIntro_ShowPage(course);
        }
        break;
    default:
        GF_AssertFail();
        break;
    }
    return FALSE;
}

BOOL PokeathlonEventIntro_FadeOut(PokeathlonCourseData *course, u8 *state) {
    PokeathlonEventIntro *intro = PokeathlonCourse_GetWork(course);
    PokeathlonEventIntro_UpdateMascot(intro);
    switch (*state) {
    case EVENT_INTRO_FADE_OUT_START:
        if (PokeathlonCourse_GetRound(course) == 0) {
            PlaySE(SEQ_SE_GS_GAMENFADEOPED);
        } else {
            PlaySE(SEQ_SE_GS_GAMENFADE);
        }
        GF_SndStartFadeOutBGM(0, 24);
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, 0x7FFF, 24, 1, intro->heapId);
        (*state)++;
        break;
    case EVENT_INTRO_FADE_OUT_WAIT:
        if (IsPaletteFadeFinished()) {
            return TRUE;
        }
        break;
    }
    return FALSE;
}
