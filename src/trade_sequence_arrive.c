#include "global.h"

#include "bg_window.h"
#include "filesystem.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "trade_sequence_arrive_internal.h"
#include "trade_sequence_internal.h"
#include "trade_sequence_models.h"
#include "screen_fade_legacy.h"
#include "swap_3d_buffers.h"

// Scene 5 of the trade animation (trades and receiving): the ball model, spinning, comes down onto the top screen
// while the backgrounds scroll into place, fades out over reversed pulses, then the background zooms out and the
// screen fades to black. The 3D setup and the tasks are in trade_sequence_arrive_tasks.c.

#define TRADE_SEQUENCE_ARRIVE_NUM_STATES 3

typedef BOOL (*TradeSequenceArriveStateFunc)(TradeSequenceArrive *arrive, u32 *state);

extern const u16 sTradeSequenceArrivePaletteOffsets[];
extern const TradeSequenceArriveStateFunc sTradeSequenceArriveStates[TRADE_SEQUENCE_ARRIVE_NUM_STATES];
extern const GraphicsModes sTradeSequenceArriveGraphicsModes;
extern const BgTemplate sTradeSequenceArriveBgTemplate_Layer2;
extern const BgTemplate sTradeSequenceArriveBgTemplate_Layer3;
extern const GraphicsBanks sTradeSequenceArriveGraphicsBanks;

TradeSequenceArrive *TradeSequenceArrive_Init(TradeSequence *ts);
void TradeSequenceArrive_Exit(TradeSequenceArrive *arrive);
BOOL TradeSequenceArrive_Main(TradeSequenceArrive *arrive, u32 *state);
void TradeSequenceArrive_Draw(TradeSequenceArrive *arrive);
BOOL TradeSequenceArrive_SetupScreens(TradeSequenceArrive *arrive, u32 *state);
BOOL TradeSequenceArrive_LowerAndFade(TradeSequenceArrive *arrive, u32 *state);
BOOL TradeSequenceArrive_ZoomOut(TradeSequenceArrive *arrive, u32 *state);
void TradeSequenceArrive_InitBgs(TradeSequenceArrive *arrive);
void TradeSequenceArrive_FreeBgs(TradeSequenceArrive *arrive);

TradeSequenceArrive *TradeSequenceArrive_Init(TradeSequence *ts) {
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

void TradeSequenceArrive_Exit(TradeSequenceArrive *arrive) {
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

BOOL TradeSequenceArrive_Main(TradeSequenceArrive *arrive, u32 *state) {
    if (*state < TRADE_SEQUENCE_ARRIVE_NUM_STATES) {
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
void TradeSequenceArrive_Draw(TradeSequenceArrive *arrive) {
    if (arrive->models != NULL) {
        arrive->rotation.y += 0x1000;
        TradeSequence_SetModelRotation(arrive->ball, &arrive->rotation);
        G3X_Reset();
        TradeSequence_DrawModels(arrive->models);
        RequestSwap3DBuffers(GX_SORTMODE_AUTO, GX_BUFFERMODE_Z);
    }
}

BOOL TradeSequenceArrive_SetupScreens(TradeSequenceArrive *arrive, u32 *state) {
    TradeSequenceArrive_InitBgs(arrive);
    TradeSequenceArrive_Init3D(arrive);
    BeginNormalPaletteFade(0, 1, 1, RGB_WHITE, 8, 1, HEAP_ID_57);
    return TRUE;
}

BOOL TradeSequenceArrive_LowerAndFade(TradeSequenceArrive *arrive, u32 *state) {
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

BOOL TradeSequenceArrive_ZoomOut(TradeSequenceArrive *arrive, u32 *state) {
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

void TradeSequenceArrive_InitBgs(TradeSequenceArrive *arrive) {
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

void TradeSequenceArrive_FreeBgs(TradeSequenceArrive *arrive) {
    if (arrive->pulsesRunning) {
        TradeSequence_DestroyPulses(arrive->pulses);
    }
    FreeBgTilemapBuffer(arrive->bgConfig, GF_BG_LYR_MAIN_2);
    FreeBgTilemapBuffer(arrive->bgConfig, GF_BG_LYR_SUB_2);
    FreeBgTilemapBuffer(arrive->bgConfig, GF_BG_LYR_MAIN_3);
    FreeBgTilemapBuffer(arrive->bgConfig, GF_BG_LYR_SUB_3);
}
