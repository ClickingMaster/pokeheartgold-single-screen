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
#include "palette.h"
#include "sound_playback.h"
#include "sprite.h"
#include "sys_task.h"
#include "sys_task_api.h"
#include "trade_sequence_internal.h"
#include "trade_sequence_models.h"
#include "unk_0200FA24.h"
#include "unk_02026E30.h"

// Scene 1 of the trade animation: the Poke Ball model fades in over a zooming background and circles up off the top
// screen; the screens swap and it rises through the bottom screen while the background scrolls and light streaks
// fall, then the screen fades to white.

#define TRADE_SEQUENCE_DEPART_NUM_STATES  4
#define TRADE_SEQUENCE_DEPART_NUM_STREAKS 20

// Rotation speed around one axis, changing by step until it reaches target.
typedef struct TradeSequenceDepartSpin {
    s16 speed;             // 0x0
    s16 step;              // 0x2
    s16 target;            // 0x4
    u16 frames;            // 0x6
} TradeSequenceDepartSpin; // size: 0x8

// Moves the ball model: it circles a center (an ellipse of radii radiusX/radiusZ, turning by angleSpeed every frame)
// while its height and its rotation speeds change over time.
typedef struct TradeSequenceDepartFlight {
    TradeSequenceModel *model;        // 0x00
    TradeSequenceModels *models;      // 0x04
    VecFx32 center;                   // 0x08
    VecFx32 position;                 // 0x14
    TradeSequenceRotation rotation;   // 0x20
    u8 padding_26[2];                 // 0x26
    fx32 height;                      // 0x28
    fx32 speed;                       // 0x2C
    fx32 acceleration;                // 0x30
    u8 unk_34[4];                     // 0x34
    fx32 radiusX;                     // 0x38
    fx32 radiusZ;                     // 0x3C
    fx32 radiusXStep;                 // 0x40
    fx32 radiusZStep;                 // 0x44
    fx32 radiusXTarget;               // 0x48
    fx32 radiusZTarget;               // 0x4C
    int radiusFrames;                 // 0x50
    s16 angle;                        // 0x54
    s16 angleSpeed;                   // 0x56
    s16 angleSpeedTarget;             // 0x58
    s16 angleSpeedStep;               // 0x5A
    TradeSequenceDepartSpin spins[3]; // 0x5C, x, y, z
    int angleSpeedFrames;             // 0x74
    BOOL outOfView;                   // 0x78
    BOOL flyingAway;                  // 0x7C
    int frames;                       // 0x80
} TradeSequenceDepartFlight;          // size: 0x84

typedef struct TradeSequenceDepartStreak {
    Sprite *head;            // 0x0
    Sprite *tail;            // 0x4
} TradeSequenceDepartStreak; // size: 0x8

typedef struct TradeSequenceDepart {
    TradeSequence *ts;                                                    // 0x000
    u32 subState;                                                         // 0x004
    int timer;                                                            // 0x008
    TradeSequenceCellAnim cellAnim;                                       // 0x00C
    TradeSequenceDepartStreak streaks[TRADE_SEQUENCE_DEPART_NUM_STREAKS]; // 0x01C
    u32 savedRngSeed;                                                     // 0x0BC
    BgConfig *bgConfig;                                                   // 0x0C0
    TradeSequencePulses *pulses;                                          // 0x0C4
    int pulsesRunning;                                                    // 0x0C8
    TradeSequenceBgZoom *bgZoom;                                          // 0x0CC
    int bgZoomFinished;                                                   // 0x0D0
    TradeSequenceModels *models;                                          // 0x0D4
    TradeSequenceModel *ball;                                             // 0x0D8
    TradeSequenceDepartFlight flight;                                     // 0x0DC
    SysTask *flightTask;                                                  // 0x160
    SysTask *scrollTask;                                                  // 0x164
    BOOL swapScreens;                                                     // 0x168
} TradeSequenceDepart;                                                    // size: 0x16C

// Makes the light streaks fall down both screens, revealing one more every 12 frames.
typedef struct TradeSequenceDepartStreakFall {
    SysTask **task;                                       // 0x000
    TradeSequenceDepart *depart;                          // 0x004
    BOOL visible[TRADE_SEQUENCE_DEPART_NUM_STREAKS];      // 0x008
    VecFx32 positions[TRADE_SEQUENCE_DEPART_NUM_STREAKS]; // 0x058
    int delay;                                            // 0x148
    int revealed;                                         // 0x14C
} TradeSequenceDepartStreakFall;                          // size: 0x150

// Scrolls the backgrounds of both screens up faster and faster; starts the falling streaks on the way.
typedef struct TradeSequenceDepartScroll {
    TradeSequenceDepart *depart;        // 0x000
    TradeSequenceDepartStreakFall fall; // 0x004
    BgConfig *bgConfig;                 // 0x154
    fx32 mainY;                         // 0x158
    fx32 subY;                          // 0x15C
    fx32 speed;                         // 0x160
    fx32 distance;                      // 0x164
    SysTask **task;                     // 0x168
    SysTask *fallTask;                  // 0x16C
} TradeSequenceDepartScroll;            // size: 0x170

typedef struct TradeSequenceDepartAlphaFade {
    fx32 alpha;                 // 0x00
    fx32 step;                  // 0x04
    fx32 target;                // 0x08
    int frames;                 // 0x0C
    TradeSequenceModel *model;  // 0x10
} TradeSequenceDepartAlphaFade; // size: 0x14

typedef BOOL (*TradeSequenceDepartStateFunc)(TradeSequenceDepart *depart, u32 *state);

extern const u16 sTradeSequenceDepartPaletteOffsets[];
extern const GraphicsModes sTradeSequenceDepartGraphicsModes;
extern const TradeSequenceDepartStateFunc sTradeSequenceDepartStates[TRADE_SEQUENCE_DEPART_NUM_STATES];
extern const GXRgb sTradeSequenceDepartEdgeColors[8];
extern const BgTemplate sTradeSequenceDepartBgTemplate_Layer2;
extern const BgTemplate sTradeSequenceDepartBgTemplate_Layer3;
extern const GraphicsBanks sTradeSequenceDepartGraphicsBanks;
extern s32 ov71_0224C0C4;
extern CameraAngle sTradeSequenceDepartCameraAngle;

TradeSequenceDepart *TradeSequenceDepart_Init(TradeSequence *ts);
void TradeSequenceDepart_Exit(TradeSequenceDepart *depart);
BOOL TradeSequenceDepart_Main(TradeSequenceDepart *depart, u32 *state);
void TradeSequenceDepart_Draw(TradeSequenceDepart *depart);
BOOL TradeSequenceDepart_SetupScreens(TradeSequenceDepart *depart, u32 *state);
BOOL TradeSequenceDepart_WaitBgZoom(TradeSequenceDepart *depart, u32 *state);
BOOL TradeSequenceDepart_FlyOffTopScreen(TradeSequenceDepart *depart, u32 *state);
BOOL TradeSequenceDepart_FlyThroughBottomScreen(TradeSequenceDepart *depart, u32 *state);
void TradeSequenceDepart_InitBgs(TradeSequenceDepart *depart);
void TradeSequenceDepart_FreeBgs(TradeSequenceDepart *depart);
void TradeSequenceDepart_Init3D(TradeSequenceDepart *depart);
void TradeSequenceDepart_Free3D(TradeSequenceDepart *depart);
void TradeSequenceDepart_CreateStreaks(TradeSequenceDepart *depart);
void TradeSequenceDepart_DeleteStreaks(TradeSequenceDepart *depart);
void TradeSequenceDepart_RequestScreenSwap(TradeSequenceDepart *depart);
void TradeSequenceDepart_ScreenSwapTask(SysTask *task, void *data);
void TradeSequenceDepart_StartStreakFall(TradeSequenceDepart *depart, SysTask **task, TradeSequenceDepartStreakFall *fall);
void TradeSequenceDepart_StreakFallTask(SysTask *task, void *data);
void TradeSequenceDepart_StopStreakFall(SysTask *task);
void TradeSequenceDepart_StartScroll(TradeSequenceDepart *depart, SysTask **task);
void TradeSequenceDepart_ScrollTask(SysTask *task, void *data);
void TradeSequenceDepart_StopScroll(SysTask *task);
void TradeSequenceDepart_StartAlphaFade(TradeSequenceModel *model, int start, int target, int frames);
void TradeSequenceDepart_AlphaFadeTask(SysTask *task, void *data);
SysTask *TradeSequenceDepart_StartFlight(TradeSequenceDepart *depart);
void TradeSequenceDepart_SetAngleSpeed(TradeSequenceDepartFlight *flight, int target, int frames);
void TradeSequenceDepart_SetRadius(TradeSequenceDepartFlight *flight, fx32 radiusX, fx32 radiusZ, int frames);
void TradeSequenceDepart_SetSpin(TradeSequenceDepartFlight *flight, int axis, int target, int frames);
void TradeSequenceDepart_UpdateFlight(TradeSequenceDepartFlight *flight);
void TradeSequenceDepart_FlightTask_Appear(SysTask *task, void *data);
void TradeSequenceDepart_FlightTask_FlyAway(SysTask *task, void *data);
BOOL TradeSequenceDepart_IsFlightOutOfView(SysTask *task);
BOOL TradeSequenceDepart_IsFlightFlyingAway(SysTask *task);
void TradeSequenceDepart_RestartFlightFromBelow(SysTask *task);
void TradeSequenceDepart_FlightTask_Rise(SysTask *task, void *data);
void TradeSequenceDepart_FlightTask_Hover(SysTask *task, void *data);
void TradeSequenceDepart_FlightTask_Sink(SysTask *task, void *data);
void TradeSequenceDepart_FlightTask_Pause(SysTask *task, void *data);
void TradeSequenceDepart_FlightTask_Leave(SysTask *task, void *data);

TradeSequenceDepart *TradeSequenceDepart_Init(TradeSequence *ts) {
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

void TradeSequenceDepart_Exit(TradeSequenceDepart *depart) {
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

BOOL TradeSequenceDepart_Main(TradeSequenceDepart *depart, u32 *state) {
    if (*state < TRADE_SEQUENCE_DEPART_NUM_STATES) {
        if (sTradeSequenceDepartStates[*state](depart, &depart->subState)) {
            *state = *state + 1;
            depart->subState = 0;
        }
        TradeSequenceDepart_Draw(depart);
        return FALSE;
    }
    return TRUE;
}

void TradeSequenceDepart_Draw(TradeSequenceDepart *depart) {
    if (depart->models != NULL) {
        G3X_Reset();
        TradeSequence_DrawModels(depart->models);
        RequestSwap3DBuffers(GX_SORTMODE_AUTO, GX_BUFFERMODE_Z);
    }
}

BOOL TradeSequenceDepart_SetupScreens(TradeSequenceDepart *depart, u32 *state) {
    TradeSequenceDepart_InitBgs(depart);
    TradeSequenceDepart_Init3D(depart);
    TradeSequenceDepart_CreateStreaks(depart);
    BeginNormalPaletteFade(0, 1, 1, RGB_BLACK, 8, 1, HEAP_ID_57);
    return TRUE;
}

BOOL TradeSequenceDepart_WaitBgZoom(TradeSequenceDepart *depart, u32 *state) {
    switch (*state) {
    case 0:
        if (IsPaletteFadeFinished()) {
            ov71_0224C0C4 = 0;
            (*state)++;
        }
        break;
    case 1:
        if (ov71_0224C0C4 < 30) {
            ov71_0224C0C4++;
        }
        if (depart->bgZoomFinished) {
            return TRUE;
        }
        break;
    }
    return FALSE;
}

BOOL TradeSequenceDepart_FlyOffTopScreen(TradeSequenceDepart *depart, u32 *state) {
    switch (*state) {
    case 0:
        depart->timer = 0;
        (*state)++;
        break;
    case 1:
        if (++depart->timer > 8) {
            TradeSequenceDepart_StartAlphaFade(depart->ball, 0, 31, 24);
            depart->flightTask = TradeSequenceDepart_StartFlight(depart);
            TradeSequence_SetModelAlpha(depart->ball, 0);
            TradeSequence_SetModelVisible(depart->ball, TRUE);
            (*state)++;
        }
        break;
    case 2:
        if (TradeSequenceDepart_IsFlightFlyingAway(depart->flightTask)) {
            (*state)++;
        }
        break;
    case 3:
        if (TradeSequenceDepart_IsFlightOutOfView(depart->flightTask)) {
            (*state)++;
        }
        break;
    case 4:
        TradeSequenceDepart_RequestScreenSwap(depart);
        return TRUE;
    }
    return FALSE;
}

BOOL TradeSequenceDepart_FlyThroughBottomScreen(TradeSequenceDepart *depart, u32 *state) {
    switch (*state) {
    case 0:
        TradeSequenceDepart_RestartFlightFromBelow(depart->flightTask);
        depart->timer = 0;
        (*state)++;
        break;
    case 1:
        if (++depart->timer > 2) {
            depart->timer = 0;
            TradeSequence_StopPulses(depart->pulses);
            (*state)++;
        }
        break;
    case 2:
        if (!depart->pulsesRunning) {
            TradeSequenceDepart_StartScroll(depart, &depart->scrollTask);
            (*state)++;
        }
        break;
    case 3:
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
    case 4:
        if (IsPaletteFadeFinished()) {
            return TRUE;
        }
        break;
    }
    return FALSE;
}

#pragma force_active on

void TradeSequenceDepart_InitBgs(TradeSequenceDepart *depart) {
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

#pragma force_active reset

void TradeSequenceDepart_FreeBgs(TradeSequenceDepart *depart) {
    if (depart->pulsesRunning) {
        TradeSequence_DestroyPulses(depart->pulses);
    }
    FreeBgTilemapBuffer(depart->bgConfig, GF_BG_LYR_MAIN_2);
    FreeBgTilemapBuffer(depart->bgConfig, GF_BG_LYR_SUB_2);
    FreeBgTilemapBuffer(depart->bgConfig, GF_BG_LYR_MAIN_3);
    FreeBgTilemapBuffer(depart->bgConfig, GF_BG_LYR_SUB_3);
}

#pragma force_active on

void TradeSequenceDepart_Init3D(TradeSequenceDepart *depart) {
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
    // sTradeSequenceDepartCameraAngle is set through the address of the variable before it (as in the original).
    ((CameraAngle *)(&ov71_0224C0C4 + 1))->x = 0xEBE0;
    ((CameraAngle *)(&ov71_0224C0C4 + 1))->y = 0;
    ((CameraAngle *)(&ov71_0224C0C4 + 1))->z = 0;
    TradeSequence_SetCameraAngle(depart->models, &sTradeSequenceDepartCameraAngle);
    TradeSequence_SetModelScale(depart->ball, 0x1C00);
}

#pragma force_active reset

void TradeSequenceDepart_Free3D(TradeSequenceDepart *depart) {
    if (depart->models != NULL) {
        TradeSequence_DestroyModels(depart->models);
    }
}

#pragma force_active on

void TradeSequenceDepart_CreateStreaks(TradeSequenceDepart *depart) {
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
        p = (TradeSequenceDepart *)((u8 *)p + sizeof(TradeSequenceDepartStreak)); // walks streaks[] through a shifted base pointer
    } while (++i < TRADE_SEQUENCE_DEPART_NUM_STREAKS);
}

#pragma force_active reset

void TradeSequenceDepart_DeleteStreaks(TradeSequenceDepart *depart) {
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
        p = (TradeSequenceDepart *)((u8 *)p + sizeof(TradeSequenceDepartStreak)); // walks streaks[] through a shifted base pointer
    } while (i < TRADE_SEQUENCE_DEPART_NUM_STREAKS);
    TradeSequence_FreeCellAnim(&depart->cellAnim);
}

void TradeSequenceDepart_RequestScreenSwap(TradeSequenceDepart *depart) {
    depart->swapScreens = TRUE;
}

// Once requested (on the next V-wait): moves the layer 2 backgrounds, shows layer 3 on the sub engine instead of the main
// one and swaps the screens, so that the ball, which left the top screen, comes into view on the bottom one.
void TradeSequenceDepart_ScreenSwapTask(SysTask *task, void *data) {
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

void TradeSequenceDepart_StartStreakFall(TradeSequenceDepart *depart, SysTask **task, TradeSequenceDepartStreakFall *fall) {
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

void TradeSequenceDepart_StreakFallTask(SysTask *task, void *data) {
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

void TradeSequenceDepart_StopStreakFall(SysTask *task) {
    if (task != NULL) {
        *((TradeSequenceDepartStreakFall *)SysTask_GetData(task))->task = NULL;
        SysTask_Destroy(task);
    }
}

#pragma force_active on

void TradeSequenceDepart_StartScroll(TradeSequenceDepart *depart, SysTask **task) {
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

void TradeSequenceDepart_ScrollTask(SysTask *task, void *data) {
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

#pragma force_active reset

void TradeSequenceDepart_StopScroll(SysTask *task) {
    TradeSequenceDepartScroll *scroll;
    if (task != NULL) {
        scroll = SysTask_GetData(task);
        *scroll->task = NULL;
        TradeSequenceDepart_StopStreakFall(scroll->fallTask);
        Heap_Free(scroll);
        SysTask_Destroy(task);
    }
}

void TradeSequenceDepart_StartAlphaFade(TradeSequenceModel *model, int start, int target, int frames) {
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

void TradeSequenceDepart_AlphaFadeTask(SysTask *task, void *data) {
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

SysTask *TradeSequenceDepart_StartFlight(TradeSequenceDepart *depart) {
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

void TradeSequenceDepart_SetAngleSpeed(TradeSequenceDepartFlight *flight, int target, int frames) {
    if (frames != 0) {
        flight->angleSpeedStep = (target - flight->angleSpeed) / frames;
        flight->angleSpeed += flight->angleSpeedStep;
    } else {
        flight->angleSpeed = target;
    }
    flight->angleSpeedTarget = target;
    flight->angleSpeedFrames = frames;
}

void TradeSequenceDepart_SetRadius(TradeSequenceDepartFlight *flight, fx32 radiusX, fx32 radiusZ, int frames) {
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

void TradeSequenceDepart_SetSpin(TradeSequenceDepartFlight *flight, int axis, int target, int frames) {
    if (frames != 0) {
        flight->spins[axis].step = (target - flight->spins[axis].speed) / frames;
    }
    flight->spins[axis].target = target;
    flight->spins[axis].frames = frames;
}

void TradeSequenceDepart_UpdateFlight(TradeSequenceDepartFlight *flight) {
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

void TradeSequenceDepart_FlightTask_Appear(SysTask *task, void *data) {
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

void TradeSequenceDepart_FlightTask_FlyAway(SysTask *task, void *data) {
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

BOOL TradeSequenceDepart_IsFlightOutOfView(SysTask *task) {
    return ((TradeSequenceDepartFlight *)SysTask_GetData(task))->outOfView;
}

BOOL TradeSequenceDepart_IsFlightFlyingAway(SysTask *task) {
    return ((TradeSequenceDepartFlight *)SysTask_GetData(task))->flyingAway;
}

void TradeSequenceDepart_RestartFlightFromBelow(SysTask *task) {
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

void TradeSequenceDepart_FlightTask_Rise(SysTask *task, void *data) {
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

void TradeSequenceDepart_FlightTask_Hover(SysTask *task, void *data) {
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

void TradeSequenceDepart_FlightTask_Sink(SysTask *task, void *data) {
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

void TradeSequenceDepart_FlightTask_Pause(SysTask *task, void *data) {
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

void TradeSequenceDepart_FlightTask_Leave(SysTask *task, void *data) {
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
