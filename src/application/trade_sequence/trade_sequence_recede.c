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
#include "sys_task_api.h"
#include "system.h"
#include "text.h"

// Scene 2 of the trade animation: the ball model spirals across the top screen and shrinks into the distance on
// the bottom one, then the screen fades to white.

#define TRADE_SEQUENCE_RECEDE_NUM_STATES 2

// Moves the ball model along an ellipse (radii radiusX/radiusY, turning by angleSpeed every frame) around a center
// that itself moves every frame; radii and scale can change over a number of frames.
typedef struct TradeSequenceRecedeFlight {
    struct TradeSequenceRecede *recede;
    SysTask *task;
    TradeSequenceModels *models;
    TradeSequenceModel *ball;
    VecFx32 center;
    VecFx32 offset;
    VecFx32 position;
    TradeSequenceRotation rotation;
    u8 padding_3A[2];
    fx32 centerStepY;
    fx32 centerStepZ;
    fx32 radiusX;
    fx32 radiusY;
    fx32 radiusXStep;
    fx32 radiusYStep;
    fx32 scale;
    fx32 scaleStep;
    int stepFrames;
    int state;
    int timer;
    int elapsed;
    s16 angle;
    s16 angleSpeed;
} TradeSequenceRecedeFlight;

typedef struct TradeSequenceRecede {
    TradeSequence *ts;
    u32 subState;
    int timer;
    BOOL swapScreens;
    TradeSequenceCellAnim cellAnim;
    Sprite *sprite;
    BgConfig *bgConfig;
    TradeSequenceModels *models;
    TradeSequenceModel *ball;
    u8 unk_30[4];
    TradeSequenceRecedeFlight flight;
} TradeSequenceRecede;

enum TradeSequenceRecedeStep {
    TRADE_RECEDE_INIT,
    TRADE_RECEDE_DELAY,
    TRADE_RECEDE_WAIT_FLIGHT,
    TRADE_RECEDE_WAIT_SPRITE_ANIM,
    TRADE_RECEDE_WAIT_FADE_OUT,
};

enum TradeSequenceRecedeFlightState {
    TRADE_RECEDE_FLIGHT_ENTER_VIEW,
    TRADE_RECEDE_FLIGHT_LEAVE_VIEW,
    TRADE_RECEDE_FLIGHT_FLY,
    TRADE_RECEDE_FLIGHT_SHRINK,
    TRADE_RECEDE_FLIGHT_END,
};

typedef BOOL (*TradeSequenceRecedeStateFunc)(TradeSequenceRecede *recede, u32 *state);

static void TradeSequenceRecede_Draw(TradeSequenceRecede *recede);
static BOOL TradeSequenceRecede_SetupScreens(TradeSequenceRecede *recede, u32 *state);
static BOOL TradeSequenceRecede_Run(TradeSequenceRecede *recede, u32 *state);
static void TradeSequenceRecede_InitBgs(TradeSequenceRecede *recede);
static void TradeSequenceRecede_FreeBgs(TradeSequenceRecede *recede);
static void TradeSequenceRecede_Init3D(TradeSequenceRecede *recede);
static void TradeSequenceRecede_Free3D(TradeSequenceRecede *recede);
static void TradeSequenceRecede_CreateSprite(TradeSequenceRecede *recede);
static void TradeSequenceRecede_DeleteSprite(TradeSequenceRecede *recede);
static void TradeSequenceRecede_InitFlight(TradeSequenceRecedeFlight *flight);
static void TradeSequenceRecede_StopFlight(TradeSequenceRecedeFlight *flight);
static void TradeSequenceRecede_StartFlight(TradeSequenceRecede *recede, TradeSequenceRecedeFlight *flight);
static BOOL TradeSequenceRecede_IsFlightDone(TradeSequenceRecedeFlight *flight);
static void TradeSequenceRecede_SetFlightTargets(TradeSequenceRecedeFlight *flight, fx32 radiusX, fx32 radiusY, fx32 scale, int frames);
static void TradeSequenceRecede_UpdateFlight(TradeSequenceRecedeFlight *flight);
static void TradeSequenceRecede_FlightTask(SysTask *task, void *data);
static void TradeSequenceRecede_RequestScreenSwap(TradeSequenceRecede *recede);
static void TradeSequenceRecede_ScreenSwapTask(SysTask *task, void *data);

static const TradeSequenceRecedeStateFunc sTradeSequenceRecedeStates[TRADE_SEQUENCE_RECEDE_NUM_STATES] = {
    TradeSequenceRecede_SetupScreens,
    TradeSequenceRecede_Run,
};

static const GXRgb sTradeSequenceRecedeEdgeColors[8] = {
    GX_RGB(4, 4, 4),
    GX_RGB(4, 4, 4),
    GX_RGB(4, 4, 4),
    GX_RGB(4, 4, 4),
    GX_RGB(4, 4, 4),
    GX_RGB(4, 4, 4),
    GX_RGB(4, 4, 4),
    GX_RGB(4, 4, 4),
};

static const GraphicsModes sTradeSequenceRecedeGraphicsModes = {
    .dispMode = GX_DISPMODE_GRAPHICS,
    .bgMode = GX_BGMODE_0,
    .subMode = GX_BGMODE_0,
    ._2d3dMode = GX_BG0_AS_3D,
};

static const BgTemplate sTradeSequenceRecedeBgTemplate = {
    .x = 0,
    .y = 0,
    .bufferSize = 0,
    .baseTile = 0,
    .size = GF_BG_SCR_SIZE_256x512,
    .colorMode = GX_BG_COLORMODE_16,
    .screenBase = GX_BG_SCRBASE_0xf000,
    .charBase = GX_BG_CHARBASE_0x00000,
    .bgExtPltt = GX_BG_EXTPLTT_01,
    .priority = 3,
    .areaOver = GX_BG_AREAOVER_REPEAT,
    .mosaic = FALSE,
};

static const GraphicsBanks sTradeSequenceRecedeGraphicsBanks = {
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

void *TradeSequenceRecede_Init(TradeSequence *ts) {
    TradeSequenceRecede *recede = Heap_Alloc(HEAP_ID_57, sizeof(TradeSequenceRecede));
    if (recede != NULL) {
        recede->ts = ts;
        recede->subState = 0;
        recede->bgConfig = TradeSequence_GetBgConfig(ts);
        recede->models = NULL;
        TradeSequenceRecede_InitFlight(&recede->flight);
    }
    return recede;
}

void TradeSequenceRecede_Exit(void *data) {
    TradeSequenceRecede *recede = data;

    if (recede != NULL) {
        Main_SetVBlankIntrCB(NULL, NULL);
        TradeSequenceRecede_FreeBgs(recede);
        TradeSequenceRecede_Free3D(recede);
        TradeSequenceRecede_DeleteSprite(recede);
        TradeSequenceRecede_StopFlight(&recede->flight);
        Heap_Free(recede);
    }
}

BOOL TradeSequenceRecede_Main(void *data, int *state) {
    TradeSequenceRecede *recede = data;

    if (*state < NELEMS(sTradeSequenceRecedeStates)) {
        if (sTradeSequenceRecedeStates[*state](recede, &recede->subState)) {
            *state = *state + 1;
            recede->subState = 0;
        }
        TradeSequenceRecede_Draw(recede);
        return FALSE;
    }
    return TRUE;
}

static void TradeSequenceRecede_Draw(TradeSequenceRecede *recede) {
    if (recede->models != NULL) {
        G3X_Reset();
        TradeSequence_DrawModels(recede->models);
        RequestSwap3DBuffers(GX_SORTMODE_AUTO, GX_BUFFERMODE_Z);
    }
}

static BOOL TradeSequenceRecede_SetupScreens(TradeSequenceRecede *recede, u32 *state) {
    TradeSequenceRecede_InitBgs(recede);
    TradeSequenceRecede_Init3D(recede);
    TradeSequenceRecede_CreateSprite(recede);
    recede->swapScreens = FALSE;
    SysTask_CreateOnVWaitQueue(TradeSequenceRecede_ScreenSwapTask, recede, 0);
    BeginNormalPaletteFade(0, 1, 1, RGB_WHITE, 3, 1, HEAP_ID_57);
    return TRUE;
}

static BOOL TradeSequenceRecede_Run(TradeSequenceRecede *recede, u32 *state) {
    switch (*state) {
    case TRADE_RECEDE_INIT:
        recede->timer = 0;
        (*state)++;
        break;
    case TRADE_RECEDE_DELAY:
        recede->timer++;
        if (recede->timer > 10) {
            PlaySE(SEQ_SE_DP_KOUKAN06);
            TradeSequenceRecede_StartFlight(recede, &recede->flight);
            TradeSequence_SetModelVisible(recede->ball, TRUE);
            (*state)++;
        }
        break;
    case TRADE_RECEDE_WAIT_FLIGHT:
        if (TradeSequenceRecede_IsFlightDone(&recede->flight)) {
            PlaySE(SEQ_SE_DP_KOUKAN07);
            Sprite_SetAnimCtrlSeq(recede->sprite, 3);
            Sprite_SetDrawFlag(recede->sprite, TRUE);
            (*state)++;
        }
        break;
    case TRADE_RECEDE_WAIT_SPRITE_ANIM:
        if (!Sprite_IsAnimated(recede->sprite)) {
            BeginNormalPaletteFade(0, 0, 0, RGB_WHITE, 16, 1, HEAP_ID_57);
            (*state)++;
        }
        break;
    case TRADE_RECEDE_WAIT_FADE_OUT:
        if (IsPaletteFadeFinished()) {
            return TRUE;
        }
        break;
    }
    return FALSE;
}

static void TradeSequenceRecede_InitBgs(TradeSequenceRecede *recede) {
    int scrnFile;
    int charFile;
    int palFile;
    int palOffset;

    GfGfx_SetBanks(&sTradeSequenceRecedeGraphicsBanks);
    reg_GX_POWCNT &= ~REG_GX_POWCNT_DSEL_MASK;
    SetBothScreensModesAndDisable(&sTradeSequenceRecedeGraphicsModes);
    InitBgFromTemplate(recede->bgConfig, GF_BG_LYR_MAIN_3, &sTradeSequenceRecedeBgTemplate, GF_BG_TYPE_TEXT);
    InitBgFromTemplate(recede->bgConfig, GF_BG_LYR_SUB_3, &sTradeSequenceRecedeBgTemplate, GF_BG_TYPE_TEXT);

    switch (TradeSequence_GetTimeOfDay(recede->ts)) {
    default:
    case TRADE_SEQUENCE_BACKDROP_DAY:
        scrnFile = 15;
        charFile = 16;
        palFile = 17;
        palOffset = 0;
        break;
    case TRADE_SEQUENCE_BACKDROP_EVENING:
        scrnFile = 15;
        charFile = 16;
        palFile = 17;
        palOffset = 0x20;
        break;
    case TRADE_SEQUENCE_BACKDROP_NIGHT:
        scrnFile = 15;
        charFile = 16;
        palFile = 17;
        palOffset = 0x40;
        break;
    case TRADE_SEQUENCE_BACKDROP_GTS:
        scrnFile = 24;
        charFile = 25;
        palFile = 26;
        palOffset = 0;
        break;
    }
    GfGfxLoader_LoadCharData(NARC_a_0_8_9, charFile, recede->bgConfig, GF_BG_LYR_MAIN_3, 0, 0, TRUE, HEAP_ID_57);
    GfGfxLoader_LoadCharData(NARC_a_0_8_9, charFile, recede->bgConfig, GF_BG_LYR_SUB_3, 0, 0, TRUE, HEAP_ID_57);
    GfGfxLoader_LoadScrnData(NARC_a_0_8_9, scrnFile, recede->bgConfig, GF_BG_LYR_MAIN_3, 0, 0, TRUE, HEAP_ID_57);
    GfGfxLoader_LoadScrnData(NARC_a_0_8_9, scrnFile, recede->bgConfig, GF_BG_LYR_SUB_3, 0, 0, TRUE, HEAP_ID_57);
    GfGfxLoader_GXLoadPalWithSrcOffset(NARC_a_0_8_9, palFile, GF_PAL_LOCATION_MAIN_BG, palOffset, GF_PAL_SLOT_0_OFFSET, 0x20, HEAP_ID_57);
    GfGfxLoader_GXLoadPalWithSrcOffset(NARC_a_0_8_9, palFile, GF_PAL_LOCATION_SUB_BG, palOffset, GF_PAL_SLOT_0_OFFSET, 0x20, HEAP_ID_57);
    BgSetPosTextAndCommit(recede->bgConfig, GF_BG_LYR_MAIN_3, BG_POS_OP_SET_Y, 256);
    BgSetPosTextAndCommit(recede->bgConfig, GF_BG_LYR_SUB_3, BG_POS_OP_SET_Y, 0);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
}

static void TradeSequenceRecede_FreeBgs(TradeSequenceRecede *recede) {
    FreeBgTilemapBuffer(recede->bgConfig, GF_BG_LYR_MAIN_3);
    FreeBgTilemapBuffer(recede->bgConfig, GF_BG_LYR_SUB_3);
}

static void TradeSequenceRecede_Init3D(TradeSequenceRecede *recede) {
    CameraAngle angle;
    VecFx32 cameraPos;

    NNS_G3dInit();
    G3X_InitMtxStack();
    G3X_SetShading(GX_SHADING_TOON);
    G3X_AntiAlias(TRUE);
    G3X_AlphaTest(FALSE, 0);
    G3X_AlphaBlend(TRUE);
    G3X_EdgeMarking(TRUE);
    G3X_SetEdgeColorTable(sTradeSequenceRecedeEdgeColors);
    G3X_SetClearColor(RGB(31, 31, 16), 0, 0x7FFF, 63, FALSE);
    G3_SwapBuffers(GX_SORTMODE_AUTO, GX_BUFFERMODE_W);
    G3_ViewPort(0, 0, 255, 191);
    GF_3DVramMan_InitFrameTexVramManager(1, TRUE);
    GF_3DVramMan_InitFramePlttVramManager(0x4000, TRUE);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG0, GF_PLANE_TOGGLE_ON);
    G2_SetBG0Priority(0);
    recede->models = TradeSequence_CreateModels(1, 0, 0, 0);
    recede->ball = TradeSequence_LoadModel(recede->models, 0, NARC_a_0_8_9, 27, 0, 0, 0, FALSE);
    angle.x = 0x18E3;
    angle.y = 0;
    angle.z = 0;
    TradeSequence_SetCameraAngle(recede->models, &angle);
    TradeSequence_SetCameraPerspectiveAngle(recede->models, 0x7D2);
    TradeSequence_GetCameraPos(recede->models, &cameraPos);
}

static void TradeSequenceRecede_Free3D(TradeSequenceRecede *recede) {
    if (recede->models != NULL) {
        TradeSequence_DestroyModels(recede->models);
    }
}

static void TradeSequenceRecede_CreateSprite(TradeSequenceRecede *recede) {
    NNSG2dImagePaletteProxy palette;
    NNSG2dImageProxy image;
    SpriteResourcesHeader header;

    TradeSequence_LoadCellAnim(&recede->cellAnim, NARC_a_0_8_9, 7, 8);
    NNS_G2dInitImagePaletteProxy(&palette);
    NNS_G2dInitImageProxy(&image);
    GfGfxLoader_LoadImageMapping(NARC_a_0_8_9, 9, TRUE, GF_BG_LYR_MAIN_0, 0, NNS_G2D_VRAM_TYPE_2DMAIN, 0, HEAP_ID_57, &image);
    GfGfxLoader_PartiallyLoadPalette(NARC_a_0_8_9, 10, NNS_G2D_VRAM_TYPE_2DMAIN, 0, HEAP_ID_57, &palette);
    TradeSequence_InitSpriteHeader(&header, &recede->cellAnim, &image, &palette, 0);
    recede->sprite = TradeSequence_CreateSprite(recede->ts, &header, 128, 96, 0, NNS_G2D_VRAM_TYPE_2DMAIN);
    Sprite_SetDrawFlag(recede->sprite, FALSE);
}

static void TradeSequenceRecede_DeleteSprite(TradeSequenceRecede *recede) {
    if (recede->sprite != NULL) {
        Sprite_Delete(recede->sprite);
    }
    TradeSequence_FreeCellAnim(&recede->cellAnim);
}

static void TradeSequenceRecede_InitFlight(TradeSequenceRecedeFlight *flight) {
    flight->task = NULL;
}

static void TradeSequenceRecede_StopFlight(TradeSequenceRecedeFlight *flight) {
    if (flight->task != NULL) {
        SysTask_Destroy(flight->task);
        flight->task = NULL;
    }
}

static void TradeSequenceRecede_StartFlight(TradeSequenceRecede *recede, TradeSequenceRecedeFlight *flight) {
    flight->recede = recede;
    flight->ball = recede->ball;
    flight->models = recede->models;
    flight->center.x = 0;
    flight->center.y = -0xAF200;
    flight->center.z = 0xE2000;
    flight->angle = -10128;
    flight->angleSpeed = -2736;
    flight->centerStepY = 0x134C4;
    flight->centerStepZ = -0xB2AA;
    flight->state = TRADE_RECEDE_FLIGHT_ENTER_VIEW;
    flight->timer = 0;
    flight->elapsed = 0;
    flight->rotation.x = 0x2000;
    flight->rotation.y = -0x2000;
    flight->rotation.z = 0;
    TradeSequence_SetModelRotation(flight->ball, &flight->rotation);
    flight->scale = FX32_ONE;
    flight->radiusX = 0x12800;
    flight->radiusY = 0x18C00;
    flight->stepFrames = 0;
    TradeSequenceRecede_UpdateFlight(flight);
    TradeSequence_SetModelPosition(flight->ball, &flight->position);
    TradeSequence_GetModelRotation(flight->ball, &flight->rotation);
    flight->task = SysTask_CreateOnMainQueue(TradeSequenceRecede_FlightTask, flight, 0);
}

static BOOL TradeSequenceRecede_IsFlightDone(TradeSequenceRecedeFlight *flight) {
    if (flight->task == NULL) {
        return TRUE;
    }
    return FALSE;
}

static void TradeSequenceRecede_SetFlightTargets(TradeSequenceRecedeFlight *flight, fx32 radiusX, fx32 radiusY, fx32 scale, int frames) {
    if (frames) {
        flight->radiusXStep = (radiusX - flight->radiusX) / frames;
        flight->radiusYStep = (radiusY - flight->radiusY) / frames;
        flight->scaleStep = (scale - flight->scale) / frames;
        flight->stepFrames = frames;
    }
}

static void TradeSequenceRecede_UpdateFlight(TradeSequenceRecedeFlight *flight) {
    int angle = (u16)flight->angle / 182; // to degrees
    if (angle >= 360) {
        angle -= 360;
    }
    flight->angle += flight->angleSpeed;
    if (flight->stepFrames) {
        flight->radiusX += flight->radiusXStep;
        flight->radiusY += flight->radiusYStep;
        flight->scale += flight->scaleStep;
        TradeSequence_SetModelScale(flight->ball, flight->scale);
        flight->stepFrames--;
    }
    flight->offset.x = FX_Mul(GF_CosDegNoWrap(angle), flight->radiusX);
    flight->offset.y = FX_Mul(GF_SinDegNoWrap(angle), flight->radiusY);
    flight->offset.z = 0;
    flight->rotation.x += 0x300;
    TradeSequence_SetModelRotation(flight->ball, &flight->rotation);
    VEC_Add(&flight->offset, &flight->center, &flight->position);
}

// Flies the ball into view, requests the screen swap once it leaves the top screen, then (after at least 60 frames)
// shrinks it into the distance and hides it.
static void TradeSequenceRecede_FlightTask(SysTask *task, void *data) {
    TradeSequenceRecedeFlight *flight = data;

    if (flight->elapsed < 60) {
        flight->elapsed++;
    }
    switch (flight->state) {
    case TRADE_RECEDE_FLIGHT_ENTER_VIEW:
        flight->center.y += flight->centerStepY;
        flight->center.z += flight->centerStepZ;
        TradeSequenceRecede_UpdateFlight(flight);
        TradeSequence_SetModelPosition(flight->ball, &flight->position);
        if (TradeSequence_IsModelInView(flight->ball)) {
            flight->state++;
        }
        break;
    case TRADE_RECEDE_FLIGHT_LEAVE_VIEW:
        if (!TradeSequence_IsModelInView(flight->ball)) {
            TradeSequenceRecede_RequestScreenSwap(flight->recede);
            flight->state++;
            return;
        }
        flight->center.y += flight->centerStepY;
        flight->center.z += flight->centerStepZ;
        TradeSequenceRecede_UpdateFlight(flight);
        TradeSequence_SetModelPosition(flight->ball, &flight->position);
        return;
    case TRADE_RECEDE_FLIGHT_FLY:
        flight->center.y += flight->centerStepY;
        flight->center.z += flight->centerStepZ;
        TradeSequenceRecede_UpdateFlight(flight);
        TradeSequence_SetModelPosition(flight->ball, &flight->position);
        if (flight->elapsed >= 60) {
            TradeSequenceRecede_SetFlightTargets(flight, 0x800, 0x800, 0x200, 12);
            flight->timer = 0;
            flight->state++;
        }
        break;
    case TRADE_RECEDE_FLIGHT_SHRINK:
        TradeSequenceRecede_UpdateFlight(flight);
        TradeSequence_SetModelPosition(flight->ball, &flight->position);
        flight->timer++;
        if (flight->timer >= 12) {
            TradeSequence_SetModelVisible(flight->ball, FALSE);
            flight->timer = 0;
            flight->state++;
        }
        break;
    case TRADE_RECEDE_FLIGHT_END:
        flight->timer++;
        if (flight->timer > 8) {
            flight->task = NULL;
            SysTask_Destroy(task);
        }
        break;
    }
}

static void TradeSequenceRecede_RequestScreenSwap(TradeSequenceRecede *recede) {
    recede->swapScreens = TRUE;
}

// Once requested (on the next V-wait): moves the layer 3 backgrounds, tilts the camera and swaps the screens.
static void TradeSequenceRecede_ScreenSwapTask(SysTask *task, void *data) {
    TradeSequenceRecede *recede = data;
    CameraAngle delta;

    if (recede->swapScreens) {
        recede->swapScreens = FALSE;
        delta.x = 0x1150;
        delta.y = 0;
        delta.z = 0;
        BgSetPosTextAndCommit(recede->bgConfig, GF_BG_LYR_MAIN_3, BG_POS_OP_SET_Y, 0);
        BgSetPosTextAndCommit(recede->bgConfig, GF_BG_LYR_SUB_3, BG_POS_OP_SET_Y, 256);
        TradeSequence_AdjustCameraAngle(recede->models, &delta);
        reg_GX_POWCNT |= REG_GX_POWCNT_DSEL_MASK;
        SysTask_Destroy(task);
    }
}
