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
#include "wireless_status_icon.h"

// Scene 4 of the trade animation (trades and receiving): after a sprite animation the ball model comes out of the
// distance on the bottom screen, spirals toward the viewer, and the screens swap as it leaves the bottom one; then the
// screen fades to white.

#define TRADE_SEQUENCE_APPROACH_NUM_STATES 2

// Moves the ball model along an ellipse (radii radiusX/radiusY, turning by angleSpeed every frame) around a center
// that itself moves every frame; radii and scale can change over a number of frames.
typedef struct TradeSequenceApproachFlight {
    struct TradeSequenceApproach *approach; // 0x00
    SysTask *task;                          // 0x04
    TradeSequenceModels *models;            // 0x08
    TradeSequenceModel *ball;               // 0x0C
    VecFx32 center;                         // 0x10
    VecFx32 offset;                         // 0x1C
    VecFx32 position;                       // 0x28
    TradeSequenceRotation rotation;         // 0x34
    u8 padding_3A[2];                       // 0x3A
    fx32 centerStepY;                       // 0x3C
    fx32 centerStepZ;                       // 0x40
    fx32 radiusX;                           // 0x44
    fx32 radiusY;                           // 0x48
    fx32 radiusXStep;                       // 0x4C
    fx32 radiusYStep;                       // 0x50
    fx32 radiusXTarget;                     // 0x54
    fx32 radiusYTarget;                     // 0x58
    fx32 scale;                             // 0x5C
    fx32 scaleStep;                         // 0x60
    fx32 scaleTarget;                       // 0x64
    int stepFrames;                         // 0x68
    int state;                              // 0x6C
    int timer;                              // 0x70
    int frames;                             // 0x74
    s16 angle;                              // 0x78
    s16 angleSpeed;                         // 0x7A
} TradeSequenceApproachFlight;              // size: 0x7C

typedef struct TradeSequenceApproach {
    TradeSequence *ts;                  // 0x00
    u32 subState;                       // 0x04
    u8 unk_08[4];                       // 0x08
    BOOL swapScreens;                   // 0x0C
    TradeSequenceCellAnim cellAnim;     // 0x10
    Sprite *sprite;                     // 0x20
    BgConfig *bgConfig;                 // 0x24
    TradeSequenceModels *models;        // 0x28
    TradeSequenceModel *ball;           // 0x2C
    u8 unk_30[4];                       // 0x30
    TradeSequenceApproachFlight flight; // 0x34
} TradeSequenceApproach;                // size: 0xB0

typedef BOOL (*TradeSequenceApproachStateFunc)(TradeSequenceApproach *approach, u32 *state);

extern const TradeSequenceApproachStateFunc sTradeSequenceApproachStates[TRADE_SEQUENCE_APPROACH_NUM_STATES];
extern const GraphicsModes sTradeSequenceApproachGraphicsModes;
extern const GXRgb sTradeSequenceApproachEdgeColors[8];
extern const BgTemplate sTradeSequenceApproachBgTemplate;
extern const GraphicsBanks sTradeSequenceApproachGraphicsBanks;
extern u32 _0224C020[]; // 1 in the data: the ball spirals (when 0 it would only follow the center)

// No header declares it.
void WirelessStatusIcon_ShowOnBothScreens(int a0, enum HeapID heapID);

TradeSequenceApproach *TradeSequenceApproach_Init(TradeSequence *ts);
void TradeSequenceApproach_Exit(TradeSequenceApproach *approach);
BOOL TradeSequenceApproach_Main(TradeSequenceApproach *approach, u32 *state);
BOOL TradeSequenceApproach_SetupScreens(TradeSequenceApproach *approach, u32 *state);
BOOL TradeSequenceApproach_Run(TradeSequenceApproach *approach, u32 *state);
void TradeSequenceApproach_InitBgs(TradeSequenceApproach *approach);
void TradeSequenceApproach_FreeBgs(TradeSequenceApproach *approach);
void TradeSequenceApproach_Init3D(TradeSequenceApproach *approach);
void TradeSequenceApproach_Free3D(TradeSequenceApproach *approach);
void TradeSequenceApproach_CreateSprite(TradeSequenceApproach *approach);
void TradeSequenceApproach_DeleteSprite(TradeSequenceApproach *approach);
void TradeSequenceApproach_InitFlight(TradeSequenceApproachFlight *flight);
void TradeSequenceApproach_StopFlight(TradeSequenceApproachFlight *flight);
void TradeSequenceApproach_StartFlight(TradeSequenceApproach *approach, TradeSequenceApproachFlight *flight);
BOOL TradeSequenceApproach_IsFlightDone(TradeSequenceApproachFlight *flight);
void TradeSequenceApproach_SetFlightTargets(TradeSequenceApproachFlight *flight, fx32 radiusX, fx32 radiusY, fx32 scale, int frames);
void TradeSequenceApproach_UpdateFlight(TradeSequenceApproachFlight *flight);
void TradeSequenceApproach_FlightTask(SysTask *task, void *data);
void TradeSequenceApproach_RequestScreenSwap(TradeSequenceApproach *approach);
void TradeSequenceApproach_VBlankCallback(void *data);

TradeSequenceApproach *TradeSequenceApproach_Init(TradeSequence *ts) {
    TradeSequenceApproach *approach = Heap_Alloc(HEAP_ID_57, sizeof(TradeSequenceApproach));
    if (approach != NULL) {
        approach->ts = ts;
        approach->subState = 0;
        approach->bgConfig = TradeSequence_GetBgConfig(ts);
        approach->models = NULL;
        approach->swapScreens = FALSE;
        TradeSequenceApproach_InitFlight(&approach->flight);
        Main_SetVBlankIntrCB(TradeSequenceApproach_VBlankCallback, approach);
    }
    return approach;
}

void TradeSequenceApproach_Exit(TradeSequenceApproach *approach) {
    if (approach != NULL) {
        Main_SetVBlankIntrCB(NULL, NULL);
        TradeSequenceApproach_FreeBgs(approach);
        TradeSequenceApproach_Free3D(approach);
        TradeSequenceApproach_DeleteSprite(approach);
        TradeSequenceApproach_StopFlight(&approach->flight);
        Heap_Free(approach);
    }
}

BOOL TradeSequenceApproach_Main(TradeSequenceApproach *approach, u32 *state) {
    if (*state < TRADE_SEQUENCE_APPROACH_NUM_STATES) {
        if (sTradeSequenceApproachStates[*state](approach, &approach->subState)) {
            *state = *state + 1;
            approach->subState = 0;
        }
        return FALSE;
    }
    return TRUE;
}

BOOL TradeSequenceApproach_SetupScreens(TradeSequenceApproach *approach, u32 *state) {
    TradeSequenceApproach_InitBgs(approach);
    TradeSequenceApproach_Init3D(approach);
    TradeSequenceApproach_CreateSprite(approach);
    BeginNormalPaletteFade(0, 1, 1, RGB_WHITE, 8, 1, HEAP_ID_57);
    return TRUE;
}

BOOL TradeSequenceApproach_Run(TradeSequenceApproach *approach, u32 *state) {
    switch (*state) {
    case 0:
        if (IsPaletteFadeFinished()) {
            Sprite_SetAnimCtrlSeq(approach->sprite, 3);
            Sprite_SetDrawFlag(approach->sprite, TRUE);
            (*state)++;
        }
        break;
    case 1:
        if (!Sprite_IsAnimated(approach->sprite)) {
            PlaySE(SEQ_SE_DP_KOUKAN05);
            TradeSequenceApproach_StartFlight(approach, &approach->flight);
            TradeSequence_SetModelVisible(approach->ball, TRUE);
            (*state)++;
        }
        break;
    case 2:
        if (TradeSequenceApproach_IsFlightDone(&approach->flight)) {
            BeginNormalPaletteFade(0, 0, 0, RGB_WHITE, 16, 1, HEAP_ID_57);
            (*state)++;
        }
        break;
    case 3:
        if (IsPaletteFadeFinished()) {
            return TRUE;
        }
        break;
    }
    return FALSE;
}

#pragma force_active on

void TradeSequenceApproach_InitBgs(TradeSequenceApproach *approach) {
    int scrnFile, charFile, palFile, palOffset;

    GfGfx_SetBanks(&sTradeSequenceApproachGraphicsBanks);
    reg_GX_POWCNT |= REG_GX_POWCNT_DSEL_MASK;
    SetBothScreensModesAndDisable(&sTradeSequenceApproachGraphicsModes);
    InitBgFromTemplate(approach->bgConfig, GF_BG_LYR_MAIN_3, &sTradeSequenceApproachBgTemplate, GF_BG_TYPE_TEXT);
    InitBgFromTemplate(approach->bgConfig, GF_BG_LYR_SUB_3, &sTradeSequenceApproachBgTemplate, GF_BG_TYPE_TEXT);
    switch (TradeSequence_GetTimeOfDay(approach->ts)) {
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
        palOffset = 32;
        break;
    case 2:
        scrnFile = 15;
        charFile = 16;
        palFile = 17;
        palOffset = 64;
        break;
    case 3:
        scrnFile = 24;
        charFile = 25;
        palFile = 26;
        palOffset = 0;
        break;
    }
    GfGfxLoader_LoadCharData(NARC_a_0_8_9, charFile, approach->bgConfig, GF_BG_LYR_MAIN_3, 0, 0, TRUE, HEAP_ID_57);
    GfGfxLoader_LoadCharData(NARC_a_0_8_9, charFile, approach->bgConfig, GF_BG_LYR_SUB_3, 0, 0, TRUE, HEAP_ID_57);
    GfGfxLoader_LoadScrnData(NARC_a_0_8_9, scrnFile, approach->bgConfig, GF_BG_LYR_MAIN_3, 0, 0, TRUE, HEAP_ID_57);
    GfGfxLoader_LoadScrnData(NARC_a_0_8_9, scrnFile, approach->bgConfig, GF_BG_LYR_SUB_3, 0, 0, TRUE, HEAP_ID_57);
    GfGfxLoader_GXLoadPalWithSrcOffset(NARC_a_0_8_9, palFile, GF_PAL_LOCATION_MAIN_BG, palOffset, GF_PAL_SLOT_0_OFFSET, 32, HEAP_ID_57);
    GfGfxLoader_GXLoadPalWithSrcOffset(NARC_a_0_8_9, palFile, GF_PAL_LOCATION_SUB_BG, palOffset, GF_PAL_SLOT_0_OFFSET, 32, HEAP_ID_57);
    BgSetPosTextAndCommit(approach->bgConfig, GF_BG_LYR_MAIN_3, BG_POS_OP_SET_Y, 0);
    BgSetPosTextAndCommit(approach->bgConfig, GF_BG_LYR_SUB_3, BG_POS_OP_SET_Y, 256);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
    WirelessStatusIcon_Create();
    WirelessStatusIcon_ShowOnBothScreens(1, HEAP_ID_56);
}

#pragma force_active reset

void TradeSequenceApproach_FreeBgs(TradeSequenceApproach *approach) {
    FreeBgTilemapBuffer(approach->bgConfig, GF_BG_LYR_MAIN_3);
    FreeBgTilemapBuffer(approach->bgConfig, GF_BG_LYR_SUB_3);
}

#pragma force_active on

void TradeSequenceApproach_Init3D(TradeSequenceApproach *approach) {
    CameraAngle angle;
    VecFx32 cameraPos;

    NNS_G3dInit();
    G3X_InitMtxStack();
    G3X_SetShading(GX_SHADING_TOON);
    G3X_AntiAlias(TRUE);
    G3X_AlphaTest(FALSE, 0);
    G3X_AlphaBlend(TRUE);
    G3X_EdgeMarking(TRUE);
    G3X_SetEdgeColorTable(sTradeSequenceApproachEdgeColors);
    G3X_SetClearColor(RGB(31, 31, 16), 0, 0x7FFF, 63, FALSE);
    G3_SwapBuffers(GX_SORTMODE_AUTO, GX_BUFFERMODE_W);
    G3_ViewPort(0, 0, 255, 191);
    GF_3DVramMan_InitFrameTexVramManager(1, TRUE);
    GF_3DVramMan_InitFramePlttVramManager(0x4000, TRUE);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG0, GF_PLANE_TOGGLE_ON);
    G2_SetBG0Priority(0);
    approach->models = TradeSequence_CreateModels(1, 0, 0, 0);
    approach->ball = TradeSequence_LoadModel(approach->models, 0, NARC_a_0_8_9, 27, 0, 0, 0, FALSE);
    angle.x = 0x18E3;
    angle.y = 0;
    angle.z = 0;
    TradeSequence_SetCameraAngle(approach->models, &angle);
    angle.x = 0x1150;
    TradeSequence_AdjustCameraAngle(approach->models, &angle);
    TradeSequence_SetCameraPerspectiveAngle(approach->models, 0x7D2);
    TradeSequence_GetCameraPos(approach->models, &cameraPos);
}

#pragma force_active reset

void TradeSequenceApproach_Free3D(TradeSequenceApproach *approach) {
    if (approach->models != NULL) {
        TradeSequence_DestroyModels(approach->models);
    }
}

#pragma force_active on

void TradeSequenceApproach_CreateSprite(TradeSequenceApproach *approach) {
    NNSG2dImagePaletteProxy palette;
    NNSG2dImageProxy image;
    SpriteResourcesHeader header;

    TradeSequence_LoadCellAnim(&approach->cellAnim, NARC_a_0_8_9, 7, 8);
    NNS_G2dInitImagePaletteProxy(&palette);
    NNS_G2dInitImageProxy(&image);
    GfGfxLoader_LoadImageMapping(NARC_a_0_8_9, 9, TRUE, GF_BG_LYR_MAIN_0, 0, NNS_G2D_VRAM_TYPE_2DMAIN, 0, HEAP_ID_57, &image);
    GfGfxLoader_PartiallyLoadPalette(NARC_a_0_8_9, 10, NNS_G2D_VRAM_TYPE_2DMAIN, 0, HEAP_ID_57, &palette);
    TradeSequence_InitSpriteHeader(&header, &approach->cellAnim, &image, &palette, 1);
    approach->sprite = TradeSequence_CreateSprite(approach->ts, &header, 128, 96, 0, NNS_G2D_VRAM_TYPE_2DMAIN);
    Sprite_SetDrawFlag(approach->sprite, FALSE);
}

#pragma force_active reset

void TradeSequenceApproach_DeleteSprite(TradeSequenceApproach *approach) {
    if (approach->sprite != NULL) {
        Sprite_Delete(approach->sprite);
    }
    TradeSequence_FreeCellAnim(&approach->cellAnim);
}

void TradeSequenceApproach_InitFlight(TradeSequenceApproachFlight *flight) {
    flight->task = NULL;
}

void TradeSequenceApproach_StopFlight(TradeSequenceApproachFlight *flight) {
    if (flight->task != NULL) {
        SysTask_Destroy(flight->task);
        flight->task = NULL;
    }
}

#pragma force_active on

void TradeSequenceApproach_StartFlight(TradeSequenceApproach *approach, TradeSequenceApproachFlight *flight) {
    flight->approach = approach;
    flight->ball = approach->ball;
    flight->models = approach->models;
    flight->center.x = 0;
    flight->center.y = 0x3D6C00;
    flight->center.z = -0x1BC000;
    flight->angle = -10128;
    flight->angleSpeed = -2736;
    flight->centerStepY = -0x10D3A;
    flight->centerStepZ = 0x936D;
    flight->state = 0;
    flight->timer = 0;
    flight->frames = 0;
    flight->rotation.x = 0x2000;
    flight->rotation.y = -0x2000;
    flight->rotation.z = 0;
    TradeSequence_SetModelRotation(flight->ball, &flight->rotation);
    flight->scale = 0x200;
    flight->radiusX = 0x800;
    flight->radiusY = 0x800;
    flight->stepFrames = 0;
    TradeSequenceApproach_SetFlightTargets(flight, 74 << 10, 99 << 10, FX32_ONE, 12);
    TradeSequenceApproach_UpdateFlight(flight);
    TradeSequence_SetModelPosition(flight->ball, &flight->position);
    TradeSequence_GetModelRotation(flight->ball, &flight->rotation);
    flight->task = SysTask_CreateOnMainQueue(TradeSequenceApproach_FlightTask, flight, 0);
}

#pragma force_active reset

BOOL TradeSequenceApproach_IsFlightDone(TradeSequenceApproachFlight *flight) {
    if (flight->task == NULL) {
        return TRUE;
    }
    return FALSE;
}

void TradeSequenceApproach_SetFlightTargets(TradeSequenceApproachFlight *flight, fx32 radiusX, fx32 radiusY, fx32 scale, int frames) {
    volatile int divisor = frames; // the divisions reload it (as in the original)

    if (frames != 0) {
        flight->radiusXStep = (radiusX - flight->radiusX) / divisor;
        flight->radiusYStep = (radiusY - flight->radiusY) / divisor;
        flight->scaleStep = (scale - flight->scale) / divisor;
        flight->radiusXTarget = radiusX;
        flight->radiusYTarget = radiusY;
        flight->scaleTarget = scale;
        flight->stepFrames = frames;
    }
}

void TradeSequenceApproach_UpdateFlight(TradeSequenceApproachFlight *flight) {
    s32 angle;

    if (_0224C020[0] != 0) {
        fx32 radius;
        s64 product;

        angle = (u16)flight->angle / 182; // to degrees
        if (angle >= 360) {
            angle -= 360;
        }
        flight->angle -= flight->angleSpeed;

        if (flight->stepFrames != 0) {
            flight->stepFrames--;
            if (flight->stepFrames != 0) {
                flight->radiusX += flight->radiusXStep;
                flight->radiusY += flight->radiusYStep;
                flight->scale += flight->scaleStep;
            } else {
                flight->radiusX = flight->radiusXTarget;
                flight->radiusY = flight->radiusYTarget;
                flight->scale = flight->scaleTarget;
            }
            TradeSequence_SetModelScale(flight->ball, flight->scale);
        }

        radius = flight->radiusX;
        product = GF_CosDegNoWrap((u16)angle);
        product *= radius;
        flight->offset.x = (product + 0x800) >> FX32_SHIFT;
        radius = flight->radiusY;
        product = GF_SinDegNoWrap((u16)angle);
        product *= radius;
        flight->offset.y = (product + 0x800) >> FX32_SHIFT;
        flight->offset.z = 0;
        flight->rotation.x -= 0x300;
        TradeSequence_SetModelRotation(flight->ball, &flight->rotation);
        VEC_Add(&flight->offset, &flight->center, &flight->position);
    } else {
        flight->position = flight->center;
    }
}

// Lets the ball spiral in place for 12 frames, then moves the center until the ball leaves the bottom screen (screen
// swap) and, 70 frames after the start, hides it.
void TradeSequenceApproach_FlightTask(SysTask *task, void *data) {
    TradeSequenceApproachFlight *flight = data;

    if (flight->frames < 70) {
        flight->frames++;
    }

    switch (flight->state) {
    case 0:
        TradeSequenceApproach_UpdateFlight(flight);
        TradeSequence_SetModelPosition(flight->ball, &flight->position);
        flight->frames++;
        if (flight->frames >= 12) {
            flight->frames = 0;
            flight->state++;
        }
        break;
    case 1:
        if (!TradeSequence_IsModelInView(flight->ball)) {
            TradeSequenceApproach_RequestScreenSwap(flight->approach);
            flight->state++;
        } else {
            flight->center.y += flight->centerStepY;
            flight->center.z += flight->centerStepZ;
            TradeSequenceApproach_UpdateFlight(flight);
            TradeSequence_SetModelPosition(flight->ball, &flight->position);
        }
        break;
    case 2:
        flight->center.y += flight->centerStepY;
        flight->center.z += flight->centerStepZ;
        TradeSequenceApproach_UpdateFlight(flight);
        TradeSequence_SetModelPosition(flight->ball, &flight->position);
        if (flight->frames >= 70) {
            TradeSequence_SetModelVisible(flight->ball, FALSE);
            flight->timer = 0;
            flight->state++;
        }
        break;
    case 3:
        flight->timer++;
        if (flight->timer > 8) {
            flight->task = NULL;
            SysTask_Destroy(task);
        }
        break;
    }
}

void TradeSequenceApproach_RequestScreenSwap(TradeSequenceApproach *approach) {
    approach->swapScreens = TRUE;
}

#pragma force_active on

// Draws the models; once requested, also moves the layer 3 backgrounds, tilts the camera back and swaps the screens.
void TradeSequenceApproach_VBlankCallback(void *data) {
    TradeSequenceApproach *approach = data;
    CameraAngle delta;

    if (approach->models) {
        G3X_Reset();
        TradeSequence_DrawModels(approach->models);
        RequestSwap3DBuffers(GX_SORTMODE_AUTO, GX_BUFFERMODE_Z);
    }
    if (approach->swapScreens) {
        delta.x = 0xEEB0;
        delta.y = 0;
        delta.z = 0;
        BgSetPosTextAndCommit(approach->bgConfig, GF_BG_LYR_MAIN_3, BG_POS_OP_SET_Y, 256);
        BgSetPosTextAndCommit(approach->bgConfig, GF_BG_LYR_SUB_3, BG_POS_OP_SET_Y, 0);
        TradeSequence_AdjustCameraAngle(approach->models, &delta);
        reg_GX_POWCNT &= ~REG_GX_POWCNT_DSEL_MASK;
        approach->swapScreens = FALSE;
    }
}

#pragma force_active reset
