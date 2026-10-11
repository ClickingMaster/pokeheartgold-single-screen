#include "global.h"

#include "constants/sndseq.h"

#include "bg_window.h"
#include "camera.h"
#include "filesystem.h"
#include "gf_3d_vramman.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "math_util.h"
#include "sound_playback.h"
#include "sprite.h"
#include "sys_task.h"
#include "sys_task_api.h"
#include "system.h"
#include "trade_sequence_internal.h"
#include "trade_sequence_models.h"
#include "unk_0200FA24.h"
#include "unk_02026E30.h"

// Scene 2 of the trade animation: the ball model spirals across the screen, the screens swap as it leaves the top
// one, and it shrinks into the distance on the bottom one where a sprite animation plays; then the screen fades to
// white.

#define TRADE_SEQUENCE_RECEDE_NUM_STATES 2

// Moves the ball model along an ellipse (radii radiusX/radiusY, turning by angleSpeed every frame) around a center
// that itself moves every frame; radii and scale can change over a number of frames.
typedef struct TradeSequenceRecedeFlight {
    struct TradeSequenceRecede *recede; // 0x00
    SysTask *task;                      // 0x04
    TradeSequenceModels *models;        // 0x08
    TradeSequenceModel *ball;           // 0x0C
    VecFx32 center;                     // 0x10
    VecFx32 offset;                     // 0x1C
    VecFx32 position;                   // 0x28
    TradeSequenceRotation rotation;     // 0x34
    u8 padding_3A[2];                   // 0x3A
    fx32 centerStepY;                   // 0x3C
    fx32 centerStepZ;                   // 0x40
    fx32 radiusX;                       // 0x44
    fx32 radiusY;                       // 0x48
    fx32 radiusXStep;                   // 0x4C
    fx32 radiusYStep;                   // 0x50
    fx32 scale;                         // 0x54
    fx32 scaleStep;                     // 0x58
    int stepFrames;                     // 0x5C
    int state;                          // 0x60
    int timer;                          // 0x64
    int elapsed;                        // 0x68
    s16 angle;                          // 0x6C
    s16 angleSpeed;                     // 0x6E
} TradeSequenceRecedeFlight;            // size: 0x70

typedef struct TradeSequenceRecede {
    TradeSequence *ts;                // 0x00
    u32 subState;                     // 0x04
    int timer;                        // 0x08
    BOOL swapScreens;                 // 0x0C
    TradeSequenceCellAnim cellAnim;   // 0x10
    Sprite *sprite;                   // 0x20
    BgConfig *bgConfig;               // 0x24
    TradeSequenceModels *models;      // 0x28
    TradeSequenceModel *ball;         // 0x2C
    u8 unk_30[4];                     // 0x30
    TradeSequenceRecedeFlight flight; // 0x34
} TradeSequenceRecede;                // size: 0xA4

typedef BOOL (*TradeSequenceRecedeStateFunc)(TradeSequenceRecede *recede, u32 *state);

extern const TradeSequenceRecedeStateFunc sTradeSequenceRecedeStates[TRADE_SEQUENCE_RECEDE_NUM_STATES];
extern const GraphicsModes sTradeSequenceRecedeGraphicsModes;
extern const GXRgb sTradeSequenceRecedeEdgeColors[8];
extern const BgTemplate sTradeSequenceRecedeBgTemplate;
extern const GraphicsBanks sTradeSequenceRecedeGraphicsBanks;

TradeSequenceRecede *TradeSequenceRecede_Init(TradeSequence *ts);
void TradeSequenceRecede_Exit(TradeSequenceRecede *recede);
BOOL TradeSequenceRecede_Main(TradeSequenceRecede *recede, u32 *state);
void TradeSequenceRecede_Draw(TradeSequenceRecede *recede);
BOOL TradeSequenceRecede_SetupScreens(TradeSequenceRecede *recede, u32 *state);
BOOL TradeSequenceRecede_Run(TradeSequenceRecede *recede, u32 *state);
void TradeSequenceRecede_InitBgs(TradeSequenceRecede *recede);
void TradeSequenceRecede_FreeBgs(TradeSequenceRecede *recede);
void TradeSequenceRecede_Init3D(TradeSequenceRecede *recede);
void TradeSequenceRecede_Free3D(TradeSequenceRecede *recede);
void TradeSequenceRecede_CreateSprite(TradeSequenceRecede *recede);
void TradeSequenceRecede_DeleteSprite(TradeSequenceRecede *recede);
void TradeSequenceRecede_InitFlight(TradeSequenceRecedeFlight *flight);
void TradeSequenceRecede_StopFlight(TradeSequenceRecedeFlight *flight);
void TradeSequenceRecede_StartFlight(TradeSequenceRecede *recede, TradeSequenceRecedeFlight *flight);
BOOL TradeSequenceRecede_IsFlightDone(TradeSequenceRecedeFlight *flight);
void TradeSequenceRecede_SetFlightTargets(TradeSequenceRecedeFlight *flight, fx32 radiusX, fx32 radiusY, fx32 scale, int frames);
void TradeSequenceRecede_UpdateFlight(TradeSequenceRecedeFlight *flight);
void TradeSequenceRecede_FlightTask(SysTask *task, void *data);
void TradeSequenceRecede_RequestScreenSwap(TradeSequenceRecede *recede);
void TradeSequenceRecede_ScreenSwapTask(SysTask *task, void *data);

TradeSequenceRecede *TradeSequenceRecede_Init(TradeSequence *ts) {
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

void TradeSequenceRecede_Exit(TradeSequenceRecede *recede) {
    if (recede != NULL) {
        Main_SetVBlankIntrCB(NULL, NULL);
        TradeSequenceRecede_FreeBgs(recede);
        TradeSequenceRecede_Free3D(recede);
        TradeSequenceRecede_DeleteSprite(recede);
        TradeSequenceRecede_StopFlight(&recede->flight);
        Heap_Free(recede);
    }
}

BOOL TradeSequenceRecede_Main(TradeSequenceRecede *recede, u32 *state) {
    if (*state < TRADE_SEQUENCE_RECEDE_NUM_STATES) {
        if (sTradeSequenceRecedeStates[*state](recede, &recede->subState)) {
            *state = *state + 1;
            recede->subState = 0;
        }
        TradeSequenceRecede_Draw(recede);
        return FALSE;
    }
    return TRUE;
}

void TradeSequenceRecede_Draw(TradeSequenceRecede *recede) {
    if (recede->models != NULL) {
        G3X_Reset();
        TradeSequence_DrawModels(recede->models);
        RequestSwap3DBuffers(GX_SORTMODE_AUTO, GX_BUFFERMODE_Z);
    }
}

BOOL TradeSequenceRecede_SetupScreens(TradeSequenceRecede *recede, u32 *state) {
    TradeSequenceRecede_InitBgs(recede);
    TradeSequenceRecede_Init3D(recede);
    TradeSequenceRecede_CreateSprite(recede);
    recede->swapScreens = FALSE;
    SysTask_CreateOnVWaitQueue(TradeSequenceRecede_ScreenSwapTask, recede, 0);
    BeginNormalPaletteFade(0, 1, 1, RGB_WHITE, 3, 1, HEAP_ID_57);
    return TRUE;
}

BOOL TradeSequenceRecede_Run(TradeSequenceRecede *recede, u32 *state) {
    switch (*state) {
    case 0:
        recede->timer = 0;
        (*state)++;
        break;
    case 1:
        recede->timer++;
        if (recede->timer > 10) {
            PlaySE(SEQ_SE_DP_KOUKAN06);
            TradeSequenceRecede_StartFlight(recede, &recede->flight);
            TradeSequence_SetModelVisible(recede->ball, TRUE);
            (*state)++;
        }
        break;
    case 2:
        if (TradeSequenceRecede_IsFlightDone(&recede->flight)) {
            PlaySE(SEQ_SE_DP_KOUKAN07);
            Sprite_SetAnimCtrlSeq(recede->sprite, 3);
            Sprite_SetDrawFlag(recede->sprite, TRUE);
            (*state)++;
        }
        break;
    case 3:
        if (!Sprite_IsAnimated(recede->sprite)) {
            BeginNormalPaletteFade(0, 0, 0, RGB_WHITE, 16, 1, HEAP_ID_57);
            (*state)++;
        }
        break;
    case 4:
        if (IsPaletteFadeFinished()) {
            return TRUE;
        }
        break;
    }
    return FALSE;
}

void TradeSequenceRecede_InitBgs(TradeSequenceRecede *recede) {
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
    case 0:
        scrnFile = 15;
        charFile = 16;
        palFile = 17;
        palOffset = 0;
        break;
    case 1:
        scrnFile = 15;
        charFile = 16;
        palFile = 17;
        palOffset = 0x20;
        break;
    case 2:
        scrnFile = 15;
        charFile = 16;
        palFile = 17;
        palOffset = 0x40;
        break;
    case 3:
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

void TradeSequenceRecede_FreeBgs(TradeSequenceRecede *recede) {
    FreeBgTilemapBuffer(recede->bgConfig, GF_BG_LYR_MAIN_3);
    FreeBgTilemapBuffer(recede->bgConfig, GF_BG_LYR_SUB_3);
}

#pragma force_active on

void TradeSequenceRecede_Init3D(TradeSequenceRecede *recede) {
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

#pragma force_active reset

void TradeSequenceRecede_Free3D(TradeSequenceRecede *recede) {
    if (recede->models != NULL) {
        TradeSequence_DestroyModels(recede->models);
    }
}

#pragma force_active on

void TradeSequenceRecede_CreateSprite(TradeSequenceRecede *recede) {
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

#pragma force_active reset

void TradeSequenceRecede_DeleteSprite(TradeSequenceRecede *recede) {
    if (recede->sprite != NULL) {
        Sprite_Delete(recede->sprite);
    }
    TradeSequence_FreeCellAnim(&recede->cellAnim);
}

void TradeSequenceRecede_InitFlight(TradeSequenceRecedeFlight *flight) {
    flight->task = NULL;
}

void TradeSequenceRecede_StopFlight(TradeSequenceRecedeFlight *flight) {
    if (flight->task != NULL) {
        SysTask_Destroy(flight->task);
        flight->task = NULL;
    }
}

#pragma force_active on

void TradeSequenceRecede_StartFlight(TradeSequenceRecede *recede, TradeSequenceRecedeFlight *flight) {
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
    flight->state = 0;
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

#pragma force_active reset

BOOL TradeSequenceRecede_IsFlightDone(TradeSequenceRecedeFlight *flight) {
    if (flight->task == NULL) {
        return TRUE;
    }
    return FALSE;
}

#pragma force_active on

void TradeSequenceRecede_SetFlightTargets(TradeSequenceRecedeFlight *flight, fx32 radiusX, fx32 radiusY, fx32 scale, int frames) {
    if (frames) {
        flight->radiusXStep = (radiusX - flight->radiusX) / frames;
        flight->radiusYStep = (radiusY - flight->radiusY) / frames;
        flight->scaleStep = (scale - flight->scale) / frames;
        flight->stepFrames = frames;
    }
}

void TradeSequenceRecede_UpdateFlight(TradeSequenceRecedeFlight *flight) {
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

#pragma force_active reset

// Flies the ball into view, requests the screen swap once it leaves the top screen, then (after at least 60 frames)
// shrinks it into the distance and hides it.
void TradeSequenceRecede_FlightTask(SysTask *task, void *data) {
    TradeSequenceRecedeFlight *flight = data;

    if (flight->elapsed < 60) {
        flight->elapsed++;
    }
    switch (flight->state) {
    case 0:
        flight->center.y += flight->centerStepY;
        flight->center.z += flight->centerStepZ;
        TradeSequenceRecede_UpdateFlight(flight);
        TradeSequence_SetModelPosition(flight->ball, &flight->position);
        if (TradeSequence_IsModelInView(flight->ball)) {
            flight->state++;
        }
        break;
    case 1:
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
    case 2:
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
    case 3:
        TradeSequenceRecede_UpdateFlight(flight);
        TradeSequence_SetModelPosition(flight->ball, &flight->position);
        flight->timer++;
        if (flight->timer >= 12) {
            TradeSequence_SetModelVisible(flight->ball, FALSE);
            flight->timer = 0;
            flight->state++;
        }
        break;
    case 4:
        flight->timer++;
        if (flight->timer > 8) {
            flight->task = NULL;
            SysTask_Destroy(task);
        }
        break;
    }
}

void TradeSequenceRecede_RequestScreenSwap(TradeSequenceRecede *recede) {
    recede->swapScreens = TRUE;
}

#pragma force_active on

// Once requested (on the next V-wait): moves the layer 3 backgrounds, tilts the camera and swaps the screens.
void TradeSequenceRecede_ScreenSwapTask(SysTask *task, void *data) {
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

#pragma force_active reset
