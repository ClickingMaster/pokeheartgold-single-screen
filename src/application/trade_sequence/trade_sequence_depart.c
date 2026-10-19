#include "global.h"

#include "constants/sndseq.h"

#include "application/trade_sequence/trade_sequence_internal.h"
#include "application/trade_sequence/trade_sequence_models.h"

#include "bg_window.h"
#include "camera.h"
#include "gf_3d_vramman.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "math_util.h"
#include "screen_fade_legacy.h"
#include "sound_playback.h"
#include "sprite.h"
#include "sprite_resource_header.h"
#include "swap_3d_buffers.h"
#include "sys_task.h"
#include "sys_task_api.h"
#include "text.h"

// Scene 1 of the trade animation: the Poke Ball model circles up off the top screen, rises through the bottom
// screen while light streaks fall, then the screen fades to white.

#define TRADE_SEQUENCE_DEPART_NUM_STATES  4
#define TRADE_SEQUENCE_DEPART_NUM_STREAKS 20

// Rotation speed around one axis, changing by step until it reaches target.
typedef struct TradeSequenceDepartSpin {
    s16 speed;
    s16 step;
    s16 target;
    u16 frames;
} TradeSequenceDepartSpin;

// Moves the ball model: it circles a center (an ellipse of radii radiusX/radiusZ, turning by angleSpeed every frame)
// while its height and its rotation speeds change over time.
typedef struct TradeSequenceDepartFlight {
    TradeSequenceModel *model;
    TradeSequenceModels *models;
    VecFx32 center;
    VecFx32 position;
    TradeSequenceRotation rotation;
    u8 padding_26[2];
    fx32 height;
    fx32 speed;
    fx32 acceleration;
    u8 unk_34[4];
    fx32 radiusX;
    fx32 radiusZ;
    fx32 radiusXStep;
    fx32 radiusZStep;
    fx32 radiusXTarget;
    fx32 radiusZTarget;
    int radiusFrames;
    s16 angle;
    s16 angleSpeed;
    s16 angleSpeedTarget;
    s16 angleSpeedStep;
    TradeSequenceDepartSpin spins[3]; // x, y, z
    int angleSpeedFrames;
    BOOL outOfView;
    BOOL flyingAway;
    int frames;
} TradeSequenceDepartFlight;

typedef struct TradeSequenceDepartStreak {
    Sprite *head;
    Sprite *tail;
} TradeSequenceDepartStreak;

typedef struct TradeSequenceDepart {
    TradeSequence *ts;
    u32 subState;
    int timer;
    TradeSequenceCellAnim cellAnim;
    TradeSequenceDepartStreak streaks[TRADE_SEQUENCE_DEPART_NUM_STREAKS];
    u32 savedRngSeed;
    BgConfig *bgConfig;
    TradeSequencePulses *pulses;
    int pulsesRunning;
    TradeSequenceBgZoom *bgZoom;
    int bgZoomFinished;
    TradeSequenceModels *models;
    TradeSequenceModel *ball;
    TradeSequenceDepartFlight flight;
    SysTask *flightTask;
    SysTask *scrollTask;
    BOOL swapScreens;
} TradeSequenceDepart;

// Makes the light streaks fall down both screens, revealing one more every 12 frames.
typedef struct TradeSequenceDepartStreakFall {
    SysTask **task;
    TradeSequenceDepart *depart;
    BOOL visible[TRADE_SEQUENCE_DEPART_NUM_STREAKS];
    VecFx32 positions[TRADE_SEQUENCE_DEPART_NUM_STREAKS];
    int delay;
    int revealed;
} TradeSequenceDepartStreakFall;

// Scrolls the backgrounds of both screens up faster and faster; starts the falling streaks on the way.
typedef struct TradeSequenceDepartScroll {
    TradeSequenceDepart *depart;
    TradeSequenceDepartStreakFall fall;
    BgConfig *bgConfig;
    fx32 mainY;
    fx32 subY;
    fx32 speed;
    fx32 distance;
    SysTask **task;
    SysTask *fallTask;
} TradeSequenceDepartScroll;

typedef struct TradeSequenceDepartAlphaFade {
    fx32 alpha;
    fx32 step;
    fx32 target;
    int frames;
    TradeSequenceModel *model;
} TradeSequenceDepartAlphaFade;

enum TradeSequenceDepartZoomStep {
    TRADE_DEPART_ZOOM_WAIT_FADE_IN,
    TRADE_DEPART_ZOOM_WAIT_ZOOM,
};

enum TradeSequenceDepartFlyOffStep {
    TRADE_DEPART_FLY_OFF_INIT,
    TRADE_DEPART_FLY_OFF_DELAY,
    TRADE_DEPART_FLY_OFF_WAIT_FLYING_AWAY,
    TRADE_DEPART_FLY_OFF_WAIT_OUT_OF_VIEW,
    TRADE_DEPART_FLY_OFF_SWAP_SCREENS,
};

enum TradeSequenceDepartFlyThroughStep {
    TRADE_DEPART_FLY_THROUGH_RESTART_FLIGHT,
    TRADE_DEPART_FLY_THROUGH_STOP_PULSES,
    TRADE_DEPART_FLY_THROUGH_WAIT_PULSES,
    TRADE_DEPART_FLY_THROUGH_WAIT_OUT_OF_VIEW,
    TRADE_DEPART_FLY_THROUGH_WAIT_FADE_OUT,
};

typedef BOOL (*TradeSequenceDepartStateFunc)(TradeSequenceDepart *depart, u32 *state);

static void TradeSequenceDepart_Draw(TradeSequenceDepart *depart);
static BOOL TradeSequenceDepart_SetupScreens(TradeSequenceDepart *depart, u32 *state);
static BOOL TradeSequenceDepart_WaitBgZoom(TradeSequenceDepart *depart, u32 *state);
static BOOL TradeSequenceDepart_FlyOffTopScreen(TradeSequenceDepart *depart, u32 *state);
static BOOL TradeSequenceDepart_FlyThroughBottomScreen(TradeSequenceDepart *depart, u32 *state);
static void TradeSequenceDepart_InitBgs(TradeSequenceDepart *depart);
static void TradeSequenceDepart_FreeBgs(TradeSequenceDepart *depart);
static void TradeSequenceDepart_Init3D(TradeSequenceDepart *depart);
static void TradeSequenceDepart_Free3D(TradeSequenceDepart *depart);
static void TradeSequenceDepart_CreateStreaks(TradeSequenceDepart *depart);
static void TradeSequenceDepart_DeleteStreaks(TradeSequenceDepart *depart);
static void TradeSequenceDepart_RequestScreenSwap(TradeSequenceDepart *depart);
static void TradeSequenceDepart_ScreenSwapTask(SysTask *task, void *data);
static void TradeSequenceDepart_StartStreakFall(TradeSequenceDepart *depart, SysTask **task, TradeSequenceDepartStreakFall *fall);
static void TradeSequenceDepart_StreakFallTask(SysTask *task, void *data);
static void TradeSequenceDepart_StopStreakFall(SysTask *task);
static void TradeSequenceDepart_StartScroll(TradeSequenceDepart *depart, SysTask **task);
static void TradeSequenceDepart_ScrollTask(SysTask *task, void *data);
static void TradeSequenceDepart_StopScroll(SysTask *task);
static void TradeSequenceDepart_StartAlphaFade(TradeSequenceModel *model, int start, int target, int frames);
static void TradeSequenceDepart_AlphaFadeTask(SysTask *task, void *data);
static SysTask *TradeSequenceDepart_StartFlight(TradeSequenceDepart *depart);
static void TradeSequenceDepart_SetAngleSpeed(TradeSequenceDepartFlight *flight, int target, int frames);
static void TradeSequenceDepart_SetRadius(TradeSequenceDepartFlight *flight, fx32 radiusX, fx32 radiusZ, int frames);
static void TradeSequenceDepart_SetSpin(TradeSequenceDepartFlight *flight, int axis, int target, int frames);
static void TradeSequenceDepart_UpdateFlight(TradeSequenceDepartFlight *flight);
static void TradeSequenceDepart_FlightTask_Appear(SysTask *task, void *data);
static void TradeSequenceDepart_FlightTask_FlyAway(SysTask *task, void *data);
static BOOL TradeSequenceDepart_IsFlightOutOfView(SysTask *task);
static BOOL TradeSequenceDepart_IsFlightFlyingAway(SysTask *task);
static void TradeSequenceDepart_RestartFlightFromBelow(SysTask *task);
static void TradeSequenceDepart_FlightTask_Rise(SysTask *task, void *data);
static void TradeSequenceDepart_FlightTask_Hover(SysTask *task, void *data);
static void TradeSequenceDepart_FlightTask_Sink(SysTask *task, void *data);
static void TradeSequenceDepart_FlightTask_Pause(SysTask *task, void *data);
static void TradeSequenceDepart_FlightTask_Leave(SysTask *task, void *data);

static s32 sTradeSequenceDepartWaitFrames;
static CameraAngle sTradeSequenceDepartCameraAngle;

static const u16 sTradeSequenceDepartPaletteOffsets[] = { 0x20, 0x60, 0xA0, 0xE0 };

static const TradeSequenceDepartStateFunc sTradeSequenceDepartStates[TRADE_SEQUENCE_DEPART_NUM_STATES] = {
    TradeSequenceDepart_SetupScreens,
    TradeSequenceDepart_WaitBgZoom,
    TradeSequenceDepart_FlyOffTopScreen,
    TradeSequenceDepart_FlyThroughBottomScreen,
};

static const GXRgb sTradeSequenceDepartEdgeColors[8] = {
    GX_RGB(4, 4, 4),
    GX_RGB(4, 4, 4),
    GX_RGB(4, 4, 4),
    GX_RGB(4, 4, 4),
    GX_RGB(4, 4, 4),
    GX_RGB(4, 4, 4),
    GX_RGB(4, 4, 4),
    GX_RGB(4, 4, 4),
};

static const GraphicsModes sTradeSequenceDepartGraphicsModes = {
    .dispMode = GX_DISPMODE_GRAPHICS,
    .bgMode = GX_BGMODE_4,
    .subMode = GX_BGMODE_4,
    ._2d3dMode = GX_BG0_AS_3D,
};

static const BgTemplate sTradeSequenceDepartBgTemplate_Layer2 = {
    .x = 0,
    .y = 0,
    .bufferSize = 0,
    .baseTile = 0,
    .size = GF_BG_SCR_SIZE_1024x1024,
    .colorMode = GX_BG_COLORMODE_256,
    .screenBase = GX_BG_SCRBASE_0xb000,
    .charBase = GX_BG_CHARBASE_0x00000,
    .bgExtPltt = GX_BG_EXTPLTT_01,
    .priority = 2,
    .areaOver = GX_BG_AREAOVER_REPEAT,
    .mosaic = FALSE,
};

static const BgTemplate sTradeSequenceDepartBgTemplate_Layer3 = {
    .x = 0,
    .y = 0,
    .bufferSize = 0x800,
    .baseTile = 0,
    .size = GF_BG_SCR_SIZE_256x256,
    .colorMode = GX_BG_COLORMODE_256,
    .screenBase = GX_BG_SCRBASE_0xf000,
    .charBase = GX_BG_CHARBASE_0x10000,
    .bgExtPltt = GX_BG_EXTPLTT_01,
    .priority = 3,
    .areaOver = GX_BG_AREAOVER_REPEAT,
    .mosaic = FALSE,
};

static const GraphicsBanks sTradeSequenceDepartGraphicsBanks = {
    .bg = GX_VRAM_BG_128_B,
    .bgextpltt = GX_VRAM_BGEXTPLTT_23_G,
    .subbg = GX_VRAM_SUB_BG_128_C,
    .subbgextpltt = GX_VRAM_SUB_BGEXTPLTT_0123_H,
    .obj = GX_VRAM_OBJ_16_F,
    .objextpltt = GX_VRAM_OBJEXTPLTT_NONE,
    .subobj = GX_VRAM_SUB_OBJ_16_I,
    .subobjextpltt = GX_VRAM_SUB_OBJEXTPLTT_NONE,
    .tex = GX_VRAM_TEX_0_A,
    .texpltt = GX_VRAM_TEXPLTT_0123_E,
};

void *TradeSequenceDepart_Init(TradeSequence *ts) {
    TradeSequenceDepart *depart = Heap_Alloc(HEAP_ID_57, sizeof(TradeSequenceDepart));
    if (depart != NULL) {
        depart->ts = ts;
        depart->subState = 0;
        depart->bgConfig = TradeSequence_GetBgConfig(ts);
        depart->models = NULL;
        depart->pulses = NULL;
        depart->bgZoom = NULL;
        depart->flightTask = NULL;
        depart->scrollTask = NULL;
        depart->savedRngSeed = MTRandom();
        depart->bgZoomFinished = FALSE;
        depart->swapScreens = FALSE;
        SysTask_CreateOnVWaitQueue(TradeSequenceDepart_ScreenSwapTask, depart, 0);
    }
    return depart;
}

void TradeSequenceDepart_Exit(void *data) {
    TradeSequenceDepart *depart = data;
    OSIntrMode enabled = OS_DisableInterrupts();
    if (depart != NULL) {
        TradeSequenceDepart_FreeBgs(depart);
        TradeSequenceDepart_Free3D(depart);
        TradeSequenceDepart_DeleteStreaks(depart);
        if (depart->flightTask != NULL) {
            SysTask_Destroy(depart->flightTask);
        }
        if (depart->scrollTask != NULL) {
            TradeSequenceDepart_StopScroll(depart->scrollTask);
        }
        SetMTRNGSeed(depart->savedRngSeed);
        Heap_Free(depart);
    }
    OS_RestoreInterrupts(enabled);
}

BOOL TradeSequenceDepart_Main(void *data, int *state) {
    TradeSequenceDepart *depart = data;

    if (*state < NELEMS(sTradeSequenceDepartStates)) {
        if (sTradeSequenceDepartStates[*state](depart, &depart->subState)) {
            *state = *state + 1;
            depart->subState = 0;
        }
        TradeSequenceDepart_Draw(depart);
        return FALSE;
    }
    return TRUE;
}

static void TradeSequenceDepart_Draw(TradeSequenceDepart *depart) {
    if (depart->models != NULL) {
        G3X_Reset();
        TradeSequence_DrawModels(depart->models);
        RequestSwap3DBuffers(GX_SORTMODE_AUTO, GX_BUFFERMODE_Z);
    }
}

static BOOL TradeSequenceDepart_SetupScreens(TradeSequenceDepart *depart, u32 *state) {
    TradeSequenceDepart_InitBgs(depart);
    TradeSequenceDepart_Init3D(depart);
    TradeSequenceDepart_CreateStreaks(depart);
    BeginNormalPaletteFade(0, 1, 1, RGB_BLACK, 8, 1, HEAP_ID_57);
    return TRUE;
}

static BOOL TradeSequenceDepart_WaitBgZoom(TradeSequenceDepart *depart, u32 *state) {
    switch (*state) {
    case TRADE_DEPART_ZOOM_WAIT_FADE_IN:
        if (IsPaletteFadeFinished()) {
            sTradeSequenceDepartWaitFrames = 0;
            (*state)++;
        }
        break;
    case TRADE_DEPART_ZOOM_WAIT_ZOOM:
        if (sTradeSequenceDepartWaitFrames < 30) {
            sTradeSequenceDepartWaitFrames++;
        }
        if (depart->bgZoomFinished) {
            return TRUE;
        }
        break;
    }
    return FALSE;
}

static BOOL TradeSequenceDepart_FlyOffTopScreen(TradeSequenceDepart *depart, u32 *state) {
    switch (*state) {
    case TRADE_DEPART_FLY_OFF_INIT:
        depart->timer = 0;
        (*state)++;
        break;
    case TRADE_DEPART_FLY_OFF_DELAY:
        if (++depart->timer > 8) {
            TradeSequenceDepart_StartAlphaFade(depart->ball, 0, 31, 24);
            depart->flightTask = TradeSequenceDepart_StartFlight(depart);
            TradeSequence_SetModelAlpha(depart->ball, 0);
            TradeSequence_SetModelVisible(depart->ball, TRUE);
            (*state)++;
        }
        break;
    case TRADE_DEPART_FLY_OFF_WAIT_FLYING_AWAY:
        if (TradeSequenceDepart_IsFlightFlyingAway(depart->flightTask)) {
            (*state)++;
        }
        break;
    case TRADE_DEPART_FLY_OFF_WAIT_OUT_OF_VIEW:
        if (TradeSequenceDepart_IsFlightOutOfView(depart->flightTask)) {
            (*state)++;
        }
        break;
    case TRADE_DEPART_FLY_OFF_SWAP_SCREENS:
        TradeSequenceDepart_RequestScreenSwap(depart);
        return TRUE;
    }
    return FALSE;
}

static BOOL TradeSequenceDepart_FlyThroughBottomScreen(TradeSequenceDepart *depart, u32 *state) {
    switch (*state) {
    case TRADE_DEPART_FLY_THROUGH_RESTART_FLIGHT:
        TradeSequenceDepart_RestartFlightFromBelow(depart->flightTask);
        depart->timer = 0;
        (*state)++;
        break;
    case TRADE_DEPART_FLY_THROUGH_STOP_PULSES:
        if (++depart->timer > 2) {
            depart->timer = 0;
            TradeSequence_StopPulses(depart->pulses);
            (*state)++;
        }
        break;
    case TRADE_DEPART_FLY_THROUGH_WAIT_PULSES:
        if (!depart->pulsesRunning) {
            TradeSequenceDepart_StartScroll(depart, &depart->scrollTask);
            (*state)++;
        }
        break;
    case TRADE_DEPART_FLY_THROUGH_WAIT_OUT_OF_VIEW:
        if (depart->timer < 40) {
            depart->timer++;
            if (depart->timer == 40) {
                PlaySE(SEQ_SE_DP_KOUKAN10);
            }
        }
        if (TradeSequenceDepart_IsFlightOutOfView(depart->flightTask)) {
            BeginNormalPaletteFade(0, 0, 0, RGB_WHITE, 4, 1, HEAP_ID_57);
            (*state)++;
        }
        break;
    case TRADE_DEPART_FLY_THROUGH_WAIT_FADE_OUT:
        if (IsPaletteFadeFinished()) {
            return TRUE;
        }
        break;
    }
    return FALSE;
}

static void TradeSequenceDepart_InitBgs(TradeSequenceDepart *depart) {
    OSIntrMode intrMode;
    u32 srcOffset;
    void *buf;

    GfGfx_SetBanks(&sTradeSequenceDepartGraphicsBanks);
    reg_GX_POWCNT &= ~REG_GX_POWCNT_DSEL_MASK;
    SetBothScreensModesAndDisable(&sTradeSequenceDepartGraphicsModes);
    InitBgFromTemplate(depart->bgConfig, GF_BG_LYR_MAIN_2, &sTradeSequenceDepartBgTemplate_Layer2, GF_BG_TYPE_AFFINE);
    InitBgFromTemplate(depart->bgConfig, GF_BG_LYR_SUB_2, &sTradeSequenceDepartBgTemplate_Layer2, GF_BG_TYPE_AFFINE);
    intrMode = OS_DisableInterrupts();
    InitBgFromTemplate(depart->bgConfig, GF_BG_LYR_MAIN_3, &sTradeSequenceDepartBgTemplate_Layer3, GF_BG_TYPE_256x16PLTT);
    InitBgFromTemplate(depart->bgConfig, GF_BG_LYR_SUB_3, &sTradeSequenceDepartBgTemplate_Layer3, GF_BG_TYPE_256x16PLTT);
    OS_RestoreInterrupts(intrMode);
    GfGfxLoader_LoadCharData(NARC_a_0_8_9, 2, depart->bgConfig, GF_BG_LYR_MAIN_2, 0, 0, TRUE, HEAP_ID_57);
    GfGfxLoader_LoadCharData(NARC_a_0_8_9, 2, depart->bgConfig, GF_BG_LYR_SUB_2, 0, 0, TRUE, HEAP_ID_57);
    GfGfxLoader_LoadScrnData(NARC_a_0_8_9, 0, depart->bgConfig, GF_BG_LYR_MAIN_2, 0, 0, TRUE, HEAP_ID_57);
    GfGfxLoader_LoadScrnData(NARC_a_0_8_9, 0, depart->bgConfig, GF_BG_LYR_SUB_2, 0, 0, TRUE, HEAP_ID_57);
    srcOffset = TradeSequence_GetTimeOfDay(depart->ts);
    GfGfxLoader_GXLoadPal(NARC_a_0_8_9, 3, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_0_OFFSET, 0x20, HEAP_ID_57);
    GfGfxLoader_GXLoadPal(NARC_a_0_8_9, 3, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_0_OFFSET, 0x20, HEAP_ID_57);
    srcOffset = sTradeSequenceDepartPaletteOffsets[srcOffset];
    GfGfxLoader_GXLoadPalWithSrcOffset(NARC_a_0_8_9, 3, GF_PAL_LOCATION_MAIN_BG, srcOffset, GF_PAL_SLOT_1_OFFSET, 0x40, HEAP_ID_57);
    GfGfxLoader_GXLoadPalWithSrcOffset(NARC_a_0_8_9, 3, GF_PAL_LOCATION_SUB_BG, srcOffset, GF_PAL_SLOT_1_OFFSET, 0x40, HEAP_ID_57);
    GfGfxLoader_LoadCharData(NARC_a_0_8_9, 5, depart->bgConfig, GF_BG_LYR_MAIN_3, 0, 0, TRUE, HEAP_ID_57);
    GfGfxLoader_LoadCharData(NARC_a_0_8_9, 5, depart->bgConfig, GF_BG_LYR_SUB_3, 0, 0, TRUE, HEAP_ID_57);
    GfGfxLoader_LoadScrnData(NARC_a_0_8_9, 4, depart->bgConfig, GF_BG_LYR_MAIN_3, 0, 0, TRUE, HEAP_ID_57);
    GfGfxLoader_LoadScrnData(NARC_a_0_8_9, 4, depart->bgConfig, GF_BG_LYR_SUB_3, 0, 0, TRUE, HEAP_ID_57);
    buf = Heap_Alloc(HEAP_ID_57, 0x60);
    if (buf != NULL) {
        MI_CpuClear32(buf, 0x60);
        DC_FlushRange(buf, 0x60);
        GX_BeginLoadBGExtPltt();
        GXS_BeginLoadBGExtPltt();
        GX_LoadBGExtPltt(buf, 0x6000, 0x60);
        GXS_LoadBGExtPltt(buf, 0x6000, 0x60);
        GX_EndLoadBGExtPltt();
        GXS_EndLoadBGExtPltt();
        Heap_Free(buf);
    }
    BgSetPosTextAndCommit(depart->bgConfig, GF_BG_LYR_SUB_2, BG_POS_OP_SET_Y, -256);
    ToggleBgLayer(GF_BG_LYR_SUB_3, GF_PLANE_TOGGLE_OFF);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
    BgSetPosTextAndCommit(depart->bgConfig, GF_BG_LYR_MAIN_3, BG_POS_OP_SET_Y, 0);
    depart->pulses = TradeSequence_CreatePulses(&depart->pulsesRunning, 1);
    depart->bgZoom = TradeSequence_StartBgZoom(depart->bgConfig, 0x480, FX32_ONE, 1, 0x128, 0, &depart->bgZoomFinished);
}

static void TradeSequenceDepart_FreeBgs(TradeSequenceDepart *depart) {
    if (depart->pulsesRunning) {
        TradeSequence_DestroyPulses(depart->pulses);
    }
    FreeBgTilemapBuffer(depart->bgConfig, GF_BG_LYR_MAIN_2);
    FreeBgTilemapBuffer(depart->bgConfig, GF_BG_LYR_SUB_2);
    FreeBgTilemapBuffer(depart->bgConfig, GF_BG_LYR_MAIN_3);
    FreeBgTilemapBuffer(depart->bgConfig, GF_BG_LYR_SUB_3);
}

static void TradeSequenceDepart_Init3D(TradeSequenceDepart *depart) {
    NNS_G3dInit();
    G3X_InitMtxStack();
    G3X_SetShading(GX_SHADING_TOON);
    G3X_AntiAlias(TRUE);
    G3X_AlphaTest(FALSE, 0);
    G3X_AlphaBlend(TRUE);
    G3X_EdgeMarking(TRUE);
    G3X_SetEdgeColorTable(sTradeSequenceDepartEdgeColors);
    G3X_SetClearColor(RGB(31, 31, 16), 0, 0x7FFF, 63, FALSE);
    G3_SwapBuffers(GX_SORTMODE_AUTO, GX_BUFFERMODE_W);
    G3_ViewPort(0, 0, 255, 191);
    GF_3DVramMan_InitFrameTexVramManager(1, TRUE);
    GF_3DVramMan_InitFramePlttVramManager(0x4000, TRUE);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG0, GF_PLANE_TOGGLE_ON);
    G2_SetBG0Priority(0);
    depart->models = TradeSequence_CreateModels(1, 0, 0, 0);
    depart->ball = TradeSequence_LoadModel(depart->models, 0, NARC_a_0_8_9, 27, 0, -0x37800, 0, FALSE);
    sTradeSequenceDepartCameraAngle.x = 0xEBE0;
    sTradeSequenceDepartCameraAngle.y = 0;
    sTradeSequenceDepartCameraAngle.z = 0;
    TradeSequence_SetCameraAngle(depart->models, &sTradeSequenceDepartCameraAngle);
    TradeSequence_SetModelScale(depart->ball, 0x1C00);
}

static void TradeSequenceDepart_Free3D(TradeSequenceDepart *depart) {
    if (depart->models != NULL) {
        TradeSequence_DestroyModels(depart->models);
    }
}

static void TradeSequenceDepart_CreateStreaks(TradeSequenceDepart *depart) {
    NNSG2dImagePaletteProxy palette;
    NNSG2dImageProxy image;
    SpriteResourcesHeader header;
    int i, x, y;
    TradeSequenceDepart *p;

    TradeSequence_LoadCellAnim(&depart->cellAnim, NARC_a_0_8_9, 11, 12);
    NNS_G2dInitImagePaletteProxy(&palette);
    NNS_G2dInitImageProxy(&image);
    GfGfxLoader_LoadImageMapping(NARC_a_0_8_9, 13, TRUE, GF_BG_LYR_MAIN_0, 0, NNS_G2D_VRAM_TYPE_2DMAIN, 0, HEAP_ID_57, &image);
    GfGfxLoader_LoadImageMapping(NARC_a_0_8_9, 13, TRUE, GF_BG_LYR_MAIN_0, 0, NNS_G2D_VRAM_TYPE_2DSUB, 0, HEAP_ID_57, &image);
    GfGfxLoader_PartiallyLoadPalette(NARC_a_0_8_9, 14, NNS_G2D_VRAM_TYPE_2DMAIN, 0, HEAP_ID_57, &palette);
    GfGfxLoader_PartiallyLoadPalette(NARC_a_0_8_9, 14, NNS_G2D_VRAM_TYPE_2DSUB, 0, HEAP_ID_57, &palette);
    TradeSequence_InitSpriteHeader(&header, &depart->cellAnim, &image, &palette, 1);
    SetMTRNGSeed(0x35947D1);
    i = 0;
    p = depart;
    do {
        x = MTRandom() % 232 + 12;
        y = (int)(MTRandom() % 452) - 28;
        p->streaks[0].head = TradeSequence_CreateSprite(depart->ts, &header, x, y, 0, NNS_G2D_VRAM_TYPE_2DMAIN);
        y += 56;
        p->streaks[0].tail = TradeSequence_CreateSprite(depart->ts, &header, x, y, 0, NNS_G2D_VRAM_TYPE_2DMAIN);
        Sprite_SetAnimCtrlSeq(p->streaks[0].head, 0);
        Sprite_SetAnimCtrlSeq(p->streaks[0].tail, 1);
        Sprite_SetDrawFlag(p->streaks[0].head, FALSE);
        Sprite_SetDrawFlag(p->streaks[0].tail, FALSE);
        p = (TradeSequenceDepart *)((u8 *)p + sizeof(TradeSequenceDepartStreak));
    } while (++i < TRADE_SEQUENCE_DEPART_NUM_STREAKS);
}

static void TradeSequenceDepart_DeleteStreaks(TradeSequenceDepart *depart) {
    s32 i;
    TradeSequenceDepart *p;

    i = 0;
    p = depart;
    do {
        if (p->streaks[0].head != NULL) {
            Sprite_Delete(p->streaks[0].head);
        }
        if (p->streaks[0].tail != NULL) {
            Sprite_Delete(p->streaks[0].tail);
        }
        i++;
        p = (TradeSequenceDepart *)((u8 *)p + sizeof(TradeSequenceDepartStreak));
    } while (i < TRADE_SEQUENCE_DEPART_NUM_STREAKS);
    TradeSequence_FreeCellAnim(&depart->cellAnim);
}

static void TradeSequenceDepart_RequestScreenSwap(TradeSequenceDepart *depart) {
    depart->swapScreens = TRUE;
}

// Once requested (on the next V-wait): moves the layer 2 backgrounds, shows layer 3 on the sub engine instead of the main
// one and swaps the screens, so that the ball, which left the top screen, comes into view on the bottom one.
static void TradeSequenceDepart_ScreenSwapTask(SysTask *task, void *data) {
    TradeSequenceDepart *depart = data;

    if (depart->swapScreens) {
        depart->swapScreens = FALSE;
        BgSetPosTextAndCommit(depart->bgConfig, GF_BG_LYR_MAIN_2, BG_POS_OP_SET_Y, -256);
        BgSetPosTextAndCommit(depart->bgConfig, GF_BG_LYR_SUB_2, BG_POS_OP_SET_Y, 0);
        ToggleBgLayer(GF_BG_LYR_MAIN_3, GF_PLANE_TOGGLE_OFF);
        ToggleBgLayer(GF_BG_LYR_SUB_3, GF_PLANE_TOGGLE_ON);
        reg_GX_POWCNT |= REG_GX_POWCNT_DSEL_MASK;
        SysTask_Destroy(task);
    }
}

static void TradeSequenceDepart_StartStreakFall(TradeSequenceDepart *depart, SysTask **task, TradeSequenceDepartStreakFall *fall) {
    int i;

    fall->depart = depart;
    fall->task = task;
    fall->delay = 0;
    fall->revealed = 0;
    for (i = 0; i < TRADE_SEQUENCE_DEPART_NUM_STREAKS; i++) {
        fall->visible[i] = FALSE;
        fall->positions[i] = *Sprite_GetMatrixPtr(depart->streaks[i].head);
    }
    *fall->task = SysTask_CreateOnMainQueue(TradeSequenceDepart_StreakFallTask, fall, 0);
    if (*fall->task == NULL) {
        Heap_Free(fall);
    }
}

static void TradeSequenceDepart_StreakFallTask(SysTask *task, void *data) {
    TradeSequenceDepartStreakFall *fall = data;
    int i;

    fall->delay++;
    if (fall->delay > 12) {
        fall->delay = 0;
        if (fall->revealed < TRADE_SEQUENCE_DEPART_NUM_STREAKS) {
            fall->visible[fall->revealed] = TRUE;
            fall->revealed++;
        }
    }

    for (i = 0; i < TRADE_SEQUENCE_DEPART_NUM_STREAKS; i++) {
        VecFx32 tail;

        fall->positions[i].y += FX32_CONST(33);
        if (fall->positions[i].y >= FX32_CONST(424)) {
            fall->positions[i].y -= FX32_CONST(452);
            if (fall->visible[i]) {
                Sprite_SetDrawFlag(fall->depart->streaks[i].head, TRUE);
                Sprite_SetDrawFlag(fall->depart->streaks[i].tail, TRUE);
            }
        }
        Sprite_SetMatrix(fall->depart->streaks[i].head, &fall->positions[i]);
        tail = fall->positions[i];
        tail.y += FX32_CONST(56);
        Sprite_SetMatrix(fall->depart->streaks[i].tail, &tail);
    }
}

static void TradeSequenceDepart_StopStreakFall(SysTask *task) {
    if (task != NULL) {
        *((TradeSequenceDepartStreakFall *)SysTask_GetData(task))->task = NULL;
        SysTask_Destroy(task);
    }
}

static void TradeSequenceDepart_StartScroll(TradeSequenceDepart *depart, SysTask **task) {
    TradeSequenceDepartScroll *scroll = Heap_Alloc(HEAP_ID_57, sizeof(TradeSequenceDepartScroll));
    if (scroll) {
        scroll->depart = depart;
        scroll->bgConfig = depart->bgConfig;
        scroll->mainY = Bg_GetYpos(scroll->bgConfig, GF_BG_LYR_MAIN_2) << FX32_SHIFT;
        scroll->subY = Bg_GetYpos(scroll->bgConfig, GF_BG_LYR_SUB_2) << FX32_SHIFT;
        scroll->speed = 0;
        scroll->distance = 0;
        scroll->task = task;
        scroll->fallTask = NULL;
        *task = SysTask_CreateOnVBlankQueue(TradeSequenceDepart_ScrollTask, scroll, 0);
        if (*task == NULL) {
            Heap_Free(scroll);
        }
    }
}

static void TradeSequenceDepart_ScrollTask(SysTask *task, void *data) {
    TradeSequenceDepartScroll *scroll = data;

    if (scroll->distance < FX32_CONST(512)) {
        scroll->speed += 0x120;
        scroll->distance += scroll->speed;
        BgSetPosTextAndCommit(scroll->bgConfig, GF_BG_LYR_MAIN_2, BG_POS_OP_SET_Y, (scroll->mainY - scroll->distance) >> FX32_SHIFT);
        BgSetPosTextAndCommit(scroll->bgConfig, GF_BG_LYR_SUB_2, BG_POS_OP_SET_Y, (scroll->subY - scroll->distance) >> FX32_SHIFT);
        if (scroll->distance >= FX32_CONST(128) && scroll->fallTask == NULL) {
            TradeSequenceDepart_StartStreakFall(scroll->depart, &scroll->fallTask, &scroll->fall);
        }
    }
}

static void TradeSequenceDepart_StopScroll(SysTask *task) {
    TradeSequenceDepartScroll *scroll;
    if (task != NULL) {
        scroll = SysTask_GetData(task);
        *scroll->task = NULL;
        TradeSequenceDepart_StopStreakFall(scroll->fallTask);
        Heap_Free(scroll);
        SysTask_Destroy(task);
    }
}

static void TradeSequenceDepart_StartAlphaFade(TradeSequenceModel *model, int start, int target, int frames) {
    TradeSequenceDepartAlphaFade *fade = Heap_Alloc(HEAP_ID_57, sizeof(TradeSequenceDepartAlphaFade));
    if (fade != NULL) {
        fade->alpha = start << FX32_SHIFT;
        fade->target = target << FX32_SHIFT;
        fade->step = (fade->target - fade->alpha) / frames;
        fade->frames = frames;
        fade->model = model;
        TradeSequence_SetModelAlpha(model, start);
        SysTask_CreateOnMainQueue(TradeSequenceDepart_AlphaFadeTask, fade, 0);
    }
}

static void TradeSequenceDepart_AlphaFadeTask(SysTask *task, void *data) {
    TradeSequenceDepartAlphaFade *fade = data;

    fade->frames--;
    if (fade->frames > 0) {
        fade->alpha += fade->step;
        TradeSequence_SetModelAlpha(fade->model, fade->alpha >> FX32_SHIFT);
        return;
    }
    TradeSequence_SetModelAlpha(fade->model, fade->target >> FX32_SHIFT);
    Heap_Free(fade);
    SysTask_Destroy(task);
}

static SysTask *TradeSequenceDepart_StartFlight(TradeSequenceDepart *depart) {
    TradeSequenceDepartFlight *flight = &depart->flight;
    int i;

    TradeSequence_GetModelPosition(depart->ball, &flight->center);
    TradeSequence_GetModelRotation(depart->ball, &flight->rotation);
    flight->position = flight->center;
    flight->models = depart->models;
    flight->model = depart->ball;
    flight->height = flight->center.y;
    flight->speed = (-0x8000 - flight->center.y) / 30;
    flight->frames = 30;
    flight->radiusX = 0;
    flight->radiusZ = 0;
    flight->radiusXStep = 0;
    flight->radiusZStep = 0;
    flight->angle = 0;
    flight->angleSpeed = 0x4000;
    flight->angleSpeedStep = 0;
    flight->flyingAway = FALSE;
    for (i = 0; i < 3; i++) {
        flight->spins[i].speed = 0;
        flight->spins[i].step = 0;
        flight->spins[i].target = 0;
        flight->spins[i].frames = 0;
    }
    flight->outOfView = FALSE;
    TradeSequenceDepart_SetAngleSpeed(flight, 0xD00, 30);
    TradeSequenceDepart_SetRadius(flight, 0x29000, 0x14000, 30);
    return SysTask_CreateOnMainQueue(TradeSequenceDepart_FlightTask_Appear, flight, 0);
}

static void TradeSequenceDepart_SetAngleSpeed(TradeSequenceDepartFlight *flight, int target, int frames) {
    if (frames != 0) {
        flight->angleSpeedStep = (target - flight->angleSpeed) / frames;
        flight->angleSpeed += flight->angleSpeedStep;
    } else {
        flight->angleSpeed = target;
    }
    flight->angleSpeedTarget = target;
    flight->angleSpeedFrames = frames;
}

static void TradeSequenceDepart_SetRadius(TradeSequenceDepartFlight *flight, fx32 radiusX, fx32 radiusZ, int frames) {
    if (frames != 0) {
        flight->radiusXStep = (radiusX - flight->radiusX) / frames;
        flight->radiusZStep = (radiusZ - flight->radiusZ) / frames;
        flight->radiusX += flight->radiusXStep;
        flight->radiusZ += flight->radiusZStep;
        flight->radiusXTarget = radiusX;
        flight->radiusZTarget = radiusZ;
    }
    flight->radiusFrames = frames;
}

static void TradeSequenceDepart_SetSpin(TradeSequenceDepartFlight *flight, int axis, int target, int frames) {
    if (frames != 0) {
        flight->spins[axis].step = (target - flight->spins[axis].speed) / frames;
    }
    flight->spins[axis].target = target;
    flight->spins[axis].frames = frames;
}

static void TradeSequenceDepart_UpdateFlight(TradeSequenceDepartFlight *flight) {
    s64 product;
    s32 radius;
    s32 angle;
    int i;

    if (flight->angleSpeedFrames != 0) {
        flight->angleSpeedFrames--;
        if (flight->angleSpeedFrames != 0) {
            flight->angleSpeed += flight->angleSpeedStep;
        } else {
            flight->angleSpeed = flight->angleSpeedTarget;
        }
    }
    flight->angle += flight->angleSpeed;
    angle = (u16)flight->angle / 182; // to degrees
    if (angle >= 360) {
        angle -= 360;
    }
    if (flight->radiusFrames != 0) {
        flight->radiusX += flight->radiusXStep;
        flight->radiusZ += flight->radiusZStep;
        flight->radiusFrames--;
        if (flight->radiusFrames == 0) {
            flight->radiusX = flight->radiusXTarget;
            flight->radiusZ = flight->radiusZTarget;
        }
    }
    radius = flight->radiusX;
    product = GF_CosDegNoWrap(angle);
    product *= radius;
    flight->position.x = flight->center.x + (s32)((product + 0x800) >> FX32_SHIFT);
    radius = flight->radiusZ;
    product = GF_SinDegNoWrap(angle);
    product *= radius;
    flight->position.z = flight->center.z + (s32)((product + 0x800) >> FX32_SHIFT);
    flight->rotation.y += flight->angleSpeed;
    for (i = 0; i < 3; i++) {
        if (flight->spins[i].frames != 0) {
            flight->spins[i].frames--;
            if (flight->spins[i].frames == 0) {
                flight->spins[i].speed = flight->spins[i].target;
            } else {
                flight->spins[i].speed += flight->spins[i].step;
            }
        }
    }
    flight->rotation.x += flight->spins[0].speed;
    flight->rotation.y += flight->spins[1].speed;
    flight->rotation.z += flight->spins[2].speed;
}

static void TradeSequenceDepart_FlightTask_Appear(SysTask *task, void *data) {
    TradeSequenceDepartFlight *flight = data;

    if (flight->frames != 0) {
        flight->height += flight->speed;
        flight->position.y = flight->height;
        flight->frames--;
        if (flight->frames <= 0) {
            flight->acceleration = 0x200;
            flight->speed = 0;
            TradeSequenceDepart_SetRadius(flight, 0x4000, 0x4000, 60);
            TradeSequenceDepart_SetSpin(flight, 0, -0x800, 30);
            flight->flyingAway = TRUE;
            SysTask_SetFunc(task, TradeSequenceDepart_FlightTask_FlyAway);
        }
    }
    TradeSequenceDepart_UpdateFlight(flight);
    TradeSequence_SetModelPosition(flight->model, &flight->position);
    TradeSequence_SetModelRotation(flight->model, &flight->rotation);
}

static void TradeSequenceDepart_FlightTask_FlyAway(SysTask *task, void *data) {
#pragma unused(task)
    TradeSequenceDepartFlight *flight = data;

    if (!flight->outOfView) {
        flight->speed += flight->acceleration;
        flight->height += flight->speed;
        flight->position.y = flight->height;
        TradeSequenceDepart_UpdateFlight(flight);
        TradeSequence_SetModelPosition(flight->model, &flight->position);
        TradeSequence_SetModelRotation(flight->model, &flight->rotation);
        if (!TradeSequence_IsModelInView(flight->model)) {
            TradeSequence_SetModelVisible(flight->model, FALSE);
            flight->outOfView = TRUE;
        }
    }
}

static BOOL TradeSequenceDepart_IsFlightOutOfView(SysTask *task) {
    return ((TradeSequenceDepartFlight *)SysTask_GetData(task))->outOfView;
}

static BOOL TradeSequenceDepart_IsFlightFlyingAway(SysTask *task) {
    return ((TradeSequenceDepartFlight *)SysTask_GetData(task))->flyingAway;
}

static void TradeSequenceDepart_RestartFlightFromBelow(SysTask *task) {
    CameraAngle angle;
    TradeSequenceDepartFlight *flight = SysTask_GetData(task);

    flight->outOfView = FALSE;
    angle.z = 0;
    angle.y = angle.z;
    angle.x = angle.y;
    TradeSequence_SetCameraPerspectiveType(flight->models, 1);
    TradeSequence_SetCameraAngle(flight->models, &angle);
    flight->speed = 0x58CC;
    flight->height = flight->speed - FX32_CONST(128);
    flight->frames = 20;
    flight->position.y = flight->height;
    TradeSequenceDepart_SetRadius(flight, 0, 0, 20);
    TradeSequenceDepart_SetSpin(flight, 1, -flight->angleSpeed, 30);
    TradeSequenceDepart_SetSpin(flight, 2, 0x100, 50);
    TradeSequence_SetModelScale(flight->model, FX32_CONST(2));
    TradeSequence_SetModelVisible(flight->model, TRUE);
    SysTask_SetFunc(task, TradeSequenceDepart_FlightTask_Rise);
}

static void TradeSequenceDepart_FlightTask_Rise(SysTask *task, void *data) {
    TradeSequenceDepartFlight *flight = data;

    TradeSequenceDepart_UpdateFlight(flight);
    if (flight->frames != 0) {
        flight->height += flight->speed;
        flight->position.y = flight->height;
        flight->frames--;
        if (flight->frames == 0) {
            SysTask_SetFunc(task, TradeSequenceDepart_FlightTask_Hover);
        }
    }
    TradeSequence_SetModelPosition(flight->model, &flight->position);
    TradeSequence_SetModelRotation(flight->model, &flight->rotation);
}

static void TradeSequenceDepart_FlightTask_Hover(SysTask *task, void *data) {
    TradeSequenceDepartFlight *flight = data;

    TradeSequenceDepart_UpdateFlight(flight);
    flight->frames++;
    if (flight->frames > 110) {
        flight->speed = (-0x3A800 - flight->height) / 53;
        flight->frames = 53;
        SysTask_SetFunc(task, TradeSequenceDepart_FlightTask_Sink);
    }
    TradeSequence_SetModelPosition(flight->model, &flight->position);
    TradeSequence_SetModelRotation(flight->model, &flight->rotation);
}

static void TradeSequenceDepart_FlightTask_Sink(SysTask *task, void *data) {
    TradeSequenceDepartFlight *flight = data;

    TradeSequenceDepart_UpdateFlight(flight);
    if (flight->frames != 0) {
        flight->height += flight->speed;
        flight->position.y = flight->height;
        flight->frames--;
        if (flight->frames == 0) {
            SysTask_SetFunc(task, TradeSequenceDepart_FlightTask_Pause);
        }
    }
    TradeSequence_SetModelPosition(flight->model, &flight->position);
    TradeSequence_SetModelRotation(flight->model, &flight->rotation);
}

static void TradeSequenceDepart_FlightTask_Pause(SysTask *task, void *data) {
    TradeSequenceDepartFlight *flight = data;

    TradeSequenceDepart_UpdateFlight(flight);
    flight->frames++;
    if (flight->frames > 1) {
        flight->speed = FX32_CONST(32);
        SysTask_SetFunc(task, TradeSequenceDepart_FlightTask_Leave);
    }
    TradeSequence_SetModelPosition(flight->model, &flight->position);
    TradeSequence_SetModelRotation(flight->model, &flight->rotation);
}

static void TradeSequenceDepart_FlightTask_Leave(SysTask *task, void *data) {
    TradeSequenceDepartFlight *flight = data;

    if (!flight->outOfView) {
        TradeSequenceDepart_UpdateFlight(flight);
        flight->height += flight->speed;
        flight->position.y = flight->height;
        TradeSequence_SetModelPosition(flight->model, &flight->position);
        TradeSequence_SetModelRotation(flight->model, &flight->rotation);
        if (!TradeSequence_IsModelInView(flight->model)) {
            flight->outOfView = TRUE;
        }
    }
}
