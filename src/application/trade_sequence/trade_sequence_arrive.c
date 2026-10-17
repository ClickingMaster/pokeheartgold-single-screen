#include "global.h"

#include "application/trade_sequence/trade_sequence_internal.h"
#include "application/trade_sequence/trade_sequence_models.h"

#include "bg_window.h"
#include "camera.h"
#include "gf_3d_vramman.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "screen_fade_legacy.h"
#include "swap_3d_buffers.h"
#include "sys_task.h"
#include "sys_task_api.h"
#include "text.h"

// Scene 5 of the trade animation (trades and receiving): the ball model, spinning, comes down onto the top screen
// while the backgrounds scroll into place, fades out over reversed pulses, then the background zooms out and the
// screen fades to black.

#define TRADE_SEQUENCE_ARRIVE_NUM_STATES 3

typedef struct TradeSequenceArrive {
    TradeSequence *ts;              // 0x00
    u32 subState;                   // 0x04
    int timer;                      // 0x08
    BgConfig *bgConfig;             // 0x0C
    TradeSequencePulses *pulses;    // 0x10
    int pulsesRunning;              // 0x14
    TradeSequenceBgZoom *bgZoom;    // 0x18
    int bgZoomFinished;             // 0x1C
    TradeSequenceModels *models;    // 0x20
    TradeSequenceModel *ball;       // 0x24
    TradeSequenceRotation rotation; // 0x28
    u8 padding_2E[2];               // 0x2E
    SysTask *lowerTask;             // 0x30
    SysTask *scrollTask;            // 0x34
    SysTask *alphaFadeTask;         // 0x38
} TradeSequenceArrive;              // size: 0x3C

// Scrolls layer 2 of both screens down into place.
typedef struct TradeSequenceArriveScroll {
    TradeSequenceArrive *arrive; // 0x00
    BgConfig *bgConfig;          // 0x04
    int frames;                  // 0x08
    fx32 mainY;                  // 0x0C
    fx32 subY;                   // 0x10
    fx32 speed;                  // 0x14
    u8 unk_18[4];                // 0x18
    SysTask **task;              // 0x1C
    u8 unk_20[4];                // 0x20
} TradeSequenceArriveScroll;     // size: 0x24

// Moves the ball down to its final height.
typedef struct TradeSequenceArriveLower {
    SysTask **task;           // 0x00
    int frames;               // 0x04
    TradeSequenceModel *ball; // 0x08
    VecFx32 position;         // 0x0C
    fx32 step;                // 0x18
} TradeSequenceArriveLower;   // size: 0x1C

typedef struct TradeSequenceArriveAlphaFade {
    SysTask **task;             // 0x00
    fx32 alpha;                 // 0x04
    fx32 step;                  // 0x08
    fx32 target;                // 0x0C
    int frames;                 // 0x10
    TradeSequenceModel *model;  // 0x14
} TradeSequenceArriveAlphaFade; // size: 0x18

typedef BOOL (*TradeSequenceArriveStateFunc)(TradeSequenceArrive *arrive, u32 *state);

static void TradeSequenceArrive_Draw(TradeSequenceArrive *arrive);
static BOOL TradeSequenceArrive_SetupScreens(TradeSequenceArrive *arrive, u32 *state);
static BOOL TradeSequenceArrive_LowerAndFade(TradeSequenceArrive *arrive, u32 *state);
static BOOL TradeSequenceArrive_ZoomOut(TradeSequenceArrive *arrive, u32 *state);
static void TradeSequenceArrive_InitBgs(TradeSequenceArrive *arrive);
static void TradeSequenceArrive_FreeBgs(TradeSequenceArrive *arrive);
static void TradeSequenceArrive_Init3D(TradeSequenceArrive *arrive);
static void TradeSequenceArrive_Free3D(TradeSequenceArrive *arrive);
static void TradeSequenceArrive_StartScroll(TradeSequenceArrive *arrive, SysTask **task);
static void TradeSequenceArrive_StopScroll(SysTask *task);
static void TradeSequenceArrive_StartLower(TradeSequenceArrive *arrive, SysTask **task);
static void TradeSequenceArrive_StopLower(SysTask *task);
static void TradeSequenceArrive_StartAlphaFade(TradeSequenceModel *model, int start, int target, int frames, SysTask **task);
static void TradeSequenceArrive_StopAlphaFade(SysTask *task);
static void TradeSequenceArrive_ScrollTask(SysTask *task, void *data);
static void TradeSequenceArrive_LowerTask(SysTask *task, void *data);
static void TradeSequenceArrive_AlphaFadeTask(SysTask *task, void *data);

CameraAngle sTradeSequenceArriveCameraAngle[2]; // only [0] is used

static const u16 sTradeSequenceArrivePaletteOffsets[] = { 0x20, 0x60, 0xA0, 0xE0 };

static const TradeSequenceArriveStateFunc sTradeSequenceArriveStates[TRADE_SEQUENCE_ARRIVE_NUM_STATES] = {
    TradeSequenceArrive_SetupScreens,
    TradeSequenceArrive_LowerAndFade,
    TradeSequenceArrive_ZoomOut,
};

static const GraphicsModes sTradeSequenceArriveGraphicsModes = {
    .dispMode = GX_DISPMODE_GRAPHICS,
    .bgMode = GX_BGMODE_4,
    .subMode = GX_BGMODE_4,
    ._2d3dMode = GX_BG0_AS_3D,
};

static const GXRgb sTradeSequenceArriveEdgeColors[8] = {
    GX_RGB(4, 4, 4),
    GX_RGB(4, 4, 4),
    GX_RGB(4, 4, 4),
    GX_RGB(4, 4, 4),
    GX_RGB(4, 4, 4),
    GX_RGB(4, 4, 4),
    GX_RGB(4, 4, 4),
    GX_RGB(4, 4, 4),
};

static const BgTemplate sTradeSequenceArriveBgTemplate_Layer3 = {
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

static const BgTemplate sTradeSequenceArriveBgTemplate_Layer2 = {
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

static const GraphicsBanks sTradeSequenceArriveGraphicsBanks = {
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

void *TradeSequenceArrive_Init(TradeSequence *ts) {
    TradeSequenceArrive *arrive = Heap_Alloc(HEAP_ID_57, sizeof(TradeSequenceArrive));
    if (arrive != NULL) {
        arrive->ts = ts;
        arrive->subState = 0;
        arrive->bgConfig = TradeSequence_GetBgConfig(ts);
        arrive->models = NULL;
        arrive->pulses = NULL;
        arrive->pulsesRunning = FALSE;
        arrive->bgZoom = NULL;
        arrive->lowerTask = NULL;
        arrive->scrollTask = NULL;
        arrive->alphaFadeTask = NULL;
    }
    return arrive;
}

void TradeSequenceArrive_Exit(void *data) {
    TradeSequenceArrive *arrive = data;

    if (arrive != NULL) {
        TradeSequenceArrive_FreeBgs(arrive);
        TradeSequenceArrive_Free3D(arrive);
        if (arrive->lowerTask != NULL) {
            TradeSequenceArrive_StopLower(arrive->lowerTask);
        }
        if (arrive->scrollTask != NULL) {
            TradeSequenceArrive_StopScroll(arrive->scrollTask);
        }
        if (arrive->alphaFadeTask != NULL) {
            TradeSequenceArrive_StopAlphaFade(arrive->alphaFadeTask);
        }
        Heap_Free(arrive);
    }
}

BOOL TradeSequenceArrive_Main(void *data, int *state) {
    TradeSequenceArrive *arrive = data;

    if (*state < NELEMS(sTradeSequenceArriveStates)) {
        if (sTradeSequenceArriveStates[*state](arrive, &arrive->subState)) {
            *state = *state + 1;
            arrive->subState = 0;
        }
        TradeSequenceArrive_Draw(arrive);
        return FALSE;
    }
    return TRUE;
}

// Spins the ball and draws the models.
static void TradeSequenceArrive_Draw(TradeSequenceArrive *arrive) {
    if (arrive->models != NULL) {
        arrive->rotation.y += 0x1000;
        TradeSequence_SetModelRotation(arrive->ball, &arrive->rotation);
        G3X_Reset();
        TradeSequence_DrawModels(arrive->models);
        RequestSwap3DBuffers(GX_SORTMODE_AUTO, GX_BUFFERMODE_Z);
    }
}

static BOOL TradeSequenceArrive_SetupScreens(TradeSequenceArrive *arrive, u32 *state) {
    TradeSequenceArrive_InitBgs(arrive);
    TradeSequenceArrive_Init3D(arrive);
    BeginNormalPaletteFade(0, 1, 1, RGB_WHITE, 8, 1, HEAP_ID_57);
    return TRUE;
}

static BOOL TradeSequenceArrive_LowerAndFade(TradeSequenceArrive *arrive, u32 *state) {
    switch (*state) {
    case 0:
        if (IsPaletteFadeFinished()) {
            arrive->timer = 0;
            TradeSequenceArrive_StartLower(arrive, &arrive->lowerTask);
            (*state)++;
        }
        break;
    case 1:
        arrive->timer++;
        if (arrive->timer > 8) {
            TradeSequenceArrive_StartScroll(arrive, &arrive->scrollTask);
            (*state)++;
        }
        break;
    case 2:
        if (arrive->scrollTask == NULL && arrive->lowerTask == NULL) {
            (*state)++;
        }
        break;
    case 3:
        arrive->pulses = TradeSequence_CreatePulses(&arrive->pulsesRunning, -1);
        arrive->timer = 0;
        (*state)++;
        break;
    case 4:
        arrive->timer++;
        if (arrive->timer > 30) {
            TradeSequenceArrive_StartAlphaFade(arrive->ball, 31, 0, 40, &arrive->alphaFadeTask);
            (*state)++;
        }
        break;
    case 5:
        if (arrive->alphaFadeTask == NULL) {
            TradeSequence_StopPulses(arrive->pulses);
            (*state)++;
        }
        break;
    case 6:
        if (!arrive->pulsesRunning) {
            return TRUE;
        }
        break;
    }
    return FALSE;
}

static BOOL TradeSequenceArrive_ZoomOut(TradeSequenceArrive *arrive, u32 *state) {
    switch (*state) {
    case 0:
        arrive->bgZoom = TradeSequence_StartBgZoom(arrive->bgConfig, FX32_ONE, 0x400, 1, 0x128, 1, &arrive->bgZoomFinished);
        arrive->timer = 0;
        (*state)++;
        break;
    case 1:
        arrive->timer++;
        if (arrive->timer > 13) {
            BeginNormalPaletteFade(0, 0, 0, RGB_BLACK, 8, 1, HEAP_ID_57);
            (*state)++;
        }
        break;
    case 2:
        if (IsPaletteFadeFinished() && arrive->bgZoomFinished) {
            return TRUE;
        }
        break;
    default:
        break;
    }
    return FALSE;
}

static void TradeSequenceArrive_InitBgs(TradeSequenceArrive *arrive) {
    OSIntrMode intrMode;
    void *buf;
    u32 timeOfDay;
    u16 srcOffset;

    GfGfx_SetBanks(&sTradeSequenceArriveGraphicsBanks);
    reg_GX_POWCNT |= REG_GX_POWCNT_DSEL_MASK;
    SetBothScreensModesAndDisable(&sTradeSequenceArriveGraphicsModes);
    InitBgFromTemplate(arrive->bgConfig, GF_BG_LYR_MAIN_2, &sTradeSequenceArriveBgTemplate_Layer2, GF_BG_TYPE_AFFINE);
    InitBgFromTemplate(arrive->bgConfig, GF_BG_LYR_SUB_2, &sTradeSequenceArriveBgTemplate_Layer2, GF_BG_TYPE_AFFINE);
    intrMode = OS_DisableInterrupts();
    InitBgFromTemplate(arrive->bgConfig, GF_BG_LYR_MAIN_3, &sTradeSequenceArriveBgTemplate_Layer3, GF_BG_TYPE_256x16PLTT);
    InitBgFromTemplate(arrive->bgConfig, GF_BG_LYR_SUB_3, &sTradeSequenceArriveBgTemplate_Layer3, GF_BG_TYPE_256x16PLTT);
    OS_RestoreInterrupts(intrMode);
    GfGfxLoader_LoadCharData(NARC_a_0_8_9, 2, arrive->bgConfig, GF_BG_LYR_MAIN_2, 0, 0, TRUE, HEAP_ID_57);
    GfGfxLoader_LoadCharData(NARC_a_0_8_9, 2, arrive->bgConfig, GF_BG_LYR_SUB_2, 0, 0, TRUE, HEAP_ID_57);
    GfGfxLoader_LoadScrnData(NARC_a_0_8_9, 1, arrive->bgConfig, GF_BG_LYR_MAIN_2, 0, 0, TRUE, HEAP_ID_57);
    GfGfxLoader_LoadScrnData(NARC_a_0_8_9, 1, arrive->bgConfig, GF_BG_LYR_SUB_2, 0, 0, TRUE, HEAP_ID_57);
    timeOfDay = TradeSequence_GetTimeOfDay(arrive->ts);
    GfGfxLoader_GXLoadPal(NARC_a_0_8_9, 3, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_0_OFFSET, 0x20, HEAP_ID_57);
    GfGfxLoader_GXLoadPal(NARC_a_0_8_9, 3, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_0_OFFSET, 0x20, HEAP_ID_57);
    srcOffset = sTradeSequenceArrivePaletteOffsets[timeOfDay];
    GfGfxLoader_GXLoadPalWithSrcOffset(NARC_a_0_8_9, 3, GF_PAL_LOCATION_MAIN_BG, srcOffset, GF_PAL_SLOT_1_OFFSET, 0x40, HEAP_ID_57);
    GfGfxLoader_GXLoadPalWithSrcOffset(NARC_a_0_8_9, 3, GF_PAL_LOCATION_SUB_BG, srcOffset, GF_PAL_SLOT_1_OFFSET, 0x40, HEAP_ID_57);
    GfGfxLoader_LoadCharData(NARC_a_0_8_9, 5, arrive->bgConfig, GF_BG_LYR_MAIN_3, 0, 0, TRUE, HEAP_ID_57);
    GfGfxLoader_LoadCharData(NARC_a_0_8_9, 5, arrive->bgConfig, GF_BG_LYR_SUB_3, 0, 0, TRUE, HEAP_ID_57);
    GfGfxLoader_LoadScrnData(NARC_a_0_8_9, 4, arrive->bgConfig, GF_BG_LYR_MAIN_3, 0, 0, TRUE, HEAP_ID_57);
    GfGfxLoader_LoadScrnData(NARC_a_0_8_9, 4, arrive->bgConfig, GF_BG_LYR_SUB_3, 0, 0, TRUE, HEAP_ID_57);
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
    BgSetPosTextAndCommit(arrive->bgConfig, GF_BG_LYR_MAIN_2, BG_POS_OP_SET_Y, -384);
    BgSetPosTextAndCommit(arrive->bgConfig, GF_BG_LYR_SUB_2, BG_POS_OP_SET_Y, -192);
    BgSetPosTextAndCommit(arrive->bgConfig, GF_BG_LYR_MAIN_3, BG_POS_OP_SET_Y, 67);
    ToggleBgLayer(GF_BG_LYR_SUB_3, GF_PLANE_TOGGLE_OFF);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
}

static void TradeSequenceArrive_FreeBgs(TradeSequenceArrive *arrive) {
    if (arrive->pulsesRunning) {
        TradeSequence_DestroyPulses(arrive->pulses);
    }
    FreeBgTilemapBuffer(arrive->bgConfig, GF_BG_LYR_MAIN_2);
    FreeBgTilemapBuffer(arrive->bgConfig, GF_BG_LYR_SUB_2);
    FreeBgTilemapBuffer(arrive->bgConfig, GF_BG_LYR_MAIN_3);
    FreeBgTilemapBuffer(arrive->bgConfig, GF_BG_LYR_SUB_3);
}

static void TradeSequenceArrive_Init3D(TradeSequenceArrive *arrive) {
    NNS_G3dInit();
    G3X_InitMtxStack();
    G3X_SetShading(GX_SHADING_TOON);
    G3X_AntiAlias(TRUE);
    G3X_AlphaTest(FALSE, 0);
    G3X_AlphaBlend(TRUE);
    G3X_EdgeMarking(TRUE);
    G3X_SetEdgeColorTable(sTradeSequenceArriveEdgeColors);
    G3X_SetClearColor(RGB(31, 31, 16), 0, 0x7FFF, 63, FALSE);
    G3_SwapBuffers(GX_SORTMODE_AUTO, GX_BUFFERMODE_W);
    G3_ViewPort(0, 0, 255, 191);
    GF_3DVramMan_InitFrameTexVramManager(1, TRUE);
    GF_3DVramMan_InitFramePlttVramManager(0x4000, TRUE);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG0, GF_PLANE_TOGGLE_ON);
    G2_SetBG0Priority(0);
    arrive->models = TradeSequence_CreateModels(1, 0, 0, 0);
    arrive->ball = TradeSequence_LoadModel(arrive->models, 0, NARC_a_0_8_9, 27, 0, FX32_CONST(140), 0, TRUE);
    sTradeSequenceArriveCameraAngle[0].x = 0;
    sTradeSequenceArriveCameraAngle[0].y = 0;
    sTradeSequenceArriveCameraAngle[0].z = 0;
    TradeSequence_SetCameraAngle(arrive->models, &sTradeSequenceArriveCameraAngle[0]);
    TradeSequence_SetCameraPerspectiveType(arrive->models, 1);
    TradeSequence_SetModelScale(arrive->ball, FX32_CONST(2));
    TradeSequence_GetModelRotation(arrive->ball, &arrive->rotation);
}

static void TradeSequenceArrive_Free3D(TradeSequenceArrive *arrive) {
    if (arrive->models != NULL) {
        TradeSequence_DestroyModels(arrive->models);
        arrive->models = NULL;
    }
}

static void TradeSequenceArrive_StartScroll(TradeSequenceArrive *arrive, SysTask **task) {
    TradeSequenceArriveScroll *scroll = Heap_Alloc(HEAP_ID_57, sizeof(TradeSequenceArriveScroll));
    if (scroll) {
        scroll->arrive = arrive;
        scroll->bgConfig = arrive->bgConfig;
        scroll->mainY = FX32_CONST(-384);
        scroll->subY = FX32_CONST(-192);
        scroll->speed = 0x9999;
        scroll->frames = 40;
        scroll->task = task;
        *task = SysTask_CreateOnVBlankQueue(TradeSequenceArrive_ScrollTask, scroll, 0);
        if (*task == NULL) {
            Heap_Free(scroll);
        }
    }
}

static void TradeSequenceArrive_ScrollTask(SysTask *task, void *data) {
    TradeSequenceArriveScroll *scroll = data;

    if (scroll->frames) {
        scroll->mainY += scroll->speed;
        scroll->subY += scroll->speed;
        BgSetPosTextAndCommit(scroll->bgConfig, GF_BG_LYR_MAIN_2, BG_POS_OP_SET_Y, scroll->mainY >> FX32_SHIFT);
        BgSetPosTextAndCommit(scroll->bgConfig, GF_BG_LYR_SUB_2, BG_POS_OP_SET_Y, scroll->subY >> FX32_SHIFT);
        scroll->frames--;
    } else {
        BgSetPosTextAndCommit(scroll->bgConfig, GF_BG_LYR_MAIN_2, BG_POS_OP_SET_Y, 0);
        BgSetPosTextAndCommit(scroll->bgConfig, GF_BG_LYR_SUB_2, BG_POS_OP_SET_Y, 192);
        TradeSequenceArrive_StopScroll(task);
    }
}

static void TradeSequenceArrive_StopScroll(SysTask *task) {
    TradeSequenceArriveScroll *scroll;
    if (task != NULL) {
        scroll = SysTask_GetData(task);
        *scroll->task = NULL;
        TradeSequence_DeferFree(scroll); // may run in the V-blank task itself
        SysTask_Destroy(task);
    }
}

static void TradeSequenceArrive_StartLower(TradeSequenceArrive *arrive, SysTask **task) {
    TradeSequenceArriveLower *lower = Heap_Alloc(HEAP_ID_57, sizeof(TradeSequenceArriveLower));
    if (lower) {
        lower->task = task;
        lower->ball = arrive->ball;
        TradeSequence_GetModelPosition(lower->ball, &lower->position);
        lower->step = (FX32_CONST(50) - lower->position.y) / 16;
        lower->frames = 16;
        *task = SysTask_CreateOnMainQueue(TradeSequenceArrive_LowerTask, lower, 0);
        if (!*task) {
            Heap_Free(lower);
        }
    }
}

static void TradeSequenceArrive_LowerTask(SysTask *task, void *data) {
    TradeSequenceArriveLower *lower = data;

    if (lower->frames) {
        lower->position.y += lower->step;
        TradeSequence_SetModelPosition(lower->ball, &lower->position);
        lower->frames--;
        return;
    }
    lower->position.y = FX32_CONST(50);
    TradeSequence_SetModelPosition(lower->ball, &lower->position);
    TradeSequenceArrive_StopLower(task);
}

static void TradeSequenceArrive_StopLower(SysTask *task) {
    TradeSequenceArriveLower *lower;
    if (task != NULL) {
        lower = SysTask_GetData(task);
        *lower->task = NULL;
        Heap_Free(lower);
        SysTask_Destroy(task);
    }
}

static void TradeSequenceArrive_StartAlphaFade(TradeSequenceModel *model, int start, int target, int frames, SysTask **task) {
    TradeSequenceArriveAlphaFade *fade = Heap_Alloc(HEAP_ID_57, sizeof(TradeSequenceArriveAlphaFade));
    if (fade) {
        fade->task = task;
        fade->alpha = start << FX32_SHIFT;
        fade->target = target << FX32_SHIFT;
        fade->step = (fade->target - fade->alpha) / frames;
        fade->frames = frames;
        fade->model = model;
        TradeSequence_SetModelAlpha(model, start);
        *task = SysTask_CreateOnMainQueue(TradeSequenceArrive_AlphaFadeTask, fade, 0);
        if (!*task) {
            Heap_Free(fade);
        }
    } else {
        *task = NULL;
    }
}

static void TradeSequenceArrive_AlphaFadeTask(SysTask *task, void *data) {
    TradeSequenceArriveAlphaFade *fade = data;

    fade->frames--;
    if (fade->frames > 0) {
        fade->alpha += fade->step;
        TradeSequence_SetModelAlpha(fade->model, fade->alpha >> FX32_SHIFT);
        return;
    }
    TradeSequence_SetModelAlpha(fade->model, fade->target >> FX32_SHIFT);
    TradeSequenceArrive_StopAlphaFade(task);
}

static void TradeSequenceArrive_StopAlphaFade(SysTask *task) {
    TradeSequenceArriveAlphaFade *fade;
    if (task != NULL) {
        fade = SysTask_GetData(task);
        *fade->task = NULL;
        Heap_Free(fade);
        SysTask_Destroy(task);
    }
}
