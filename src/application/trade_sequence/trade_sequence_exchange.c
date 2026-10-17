#include "global.h"

#include "constants/sndseq.h"

#include "application/trade_sequence/trade_sequence_internal.h"

#include "bg_window.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "screen_fade_legacy.h"
#include "sound_playback.h"
#include "sys_task_api.h"
#include "text.h"

// Scene 3 of the trade animation (trades only): the pictures of the sent and the received Pokemon slide past each
// other, then the screen fades to white.

#define TRADE_SEQUENCE_EXCHANGE_NUM_STATES 2

typedef struct TradeSequenceExchange {
    TradeSequence *ts;   // 0x00
    u32 subState;        // 0x04
    int frames;          // 0x08
    BgConfig *bgConfig;  // 0x0C
    fx32 sentY;          // 0x10
    fx32 receivedY;      // 0x14
    fx32 sentStep;       // 0x18
    fx32 receivedStep;   // 0x1C
    BOOL slideDone;      // 0x20
} TradeSequenceExchange; // size: 0x24

typedef BOOL (*TradeSequenceExchangeStateFunc)(TradeSequenceExchange *exchange, u32 *state);

static BOOL TradeSequenceExchange_SetupScreens(TradeSequenceExchange *exchange, u32 *state);
static BOOL TradeSequenceExchange_Slide(TradeSequenceExchange *exchange, u32 *state);
static void TradeSequenceExchange_SlideTask(SysTask *task, void *data);
static void TradeSequenceExchange_InitBgs(TradeSequenceExchange *exchange);
static void TradeSequenceExchange_FreeBgs(TradeSequenceExchange *exchange);
static void TradeSequenceExchange_SetMonPicPositions(BgConfig *bgConfig, u32 sentY, u32 receivedY);

static const TradeSequenceExchangeStateFunc sTradeSequenceExchangeStates[TRADE_SEQUENCE_EXCHANGE_NUM_STATES] = {
    TradeSequenceExchange_SetupScreens,
    TradeSequenceExchange_Slide,
};

static const GraphicsModes sTradeSequenceExchangeGraphicsModes = {
    .dispMode = GX_DISPMODE_GRAPHICS,
    .bgMode = GX_BGMODE_0,
    .subMode = GX_BGMODE_0,
    ._2d3dMode = GX_BG0_AS_3D,
};

static const BgTemplate sTradeSequenceExchangeBgTemplate_ReceivedMon = {
    .x = 0,
    .y = 0,
    .bufferSize = 0x1000,
    .baseTile = 0,
    .size = GF_BG_SCR_SIZE_256x512,
    .colorMode = GX_BG_COLORMODE_16,
    .screenBase = GX_BG_SCRBASE_0xe000,
    .charBase = GX_BG_CHARBASE_0x10000,
    .bgExtPltt = GX_BG_EXTPLTT_01,
    .priority = 2,
    .areaOver = GX_BG_AREAOVER_REPEAT,
    .mosaic = FALSE,
};

static const BgTemplate sTradeSequenceExchangeBgTemplate_SentMon = {
    .x = 0,
    .y = 0,
    .bufferSize = 0x1000,
    .baseTile = 0,
    .size = GF_BG_SCR_SIZE_256x512,
    .colorMode = GX_BG_COLORMODE_16,
    .screenBase = GX_BG_SCRBASE_0xd000,
    .charBase = GX_BG_CHARBASE_0x00000,
    .bgExtPltt = GX_BG_EXTPLTT_01,
    .priority = 1,
    .areaOver = GX_BG_AREAOVER_REPEAT,
    .mosaic = FALSE,
};

static const BgTemplate sTradeSequenceExchangeBgTemplate_Background = {
    .x = 0,
    .y = 0,
    .bufferSize = 0,
    .baseTile = 0,
    .size = GF_BG_SCR_SIZE_256x256,
    .colorMode = GX_BG_COLORMODE_16,
    .screenBase = GX_BG_SCRBASE_0xf000,
    .charBase = GX_BG_CHARBASE_0x04000,
    .bgExtPltt = GX_BG_EXTPLTT_01,
    .priority = 3,
    .areaOver = GX_BG_AREAOVER_REPEAT,
    .mosaic = FALSE,
};

static const GraphicsBanks sTradeSequenceExchangeGraphicsBanks = {
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

void *TradeSequenceExchange_Init(TradeSequence *ts) {
    TradeSequenceExchange *exchange = Heap_Alloc(HEAP_ID_57, sizeof(TradeSequenceExchange));
    if (exchange != NULL) {
        exchange->ts = ts;
        exchange->subState = 0;
        exchange->bgConfig = TradeSequence_GetBgConfig(ts);
    }
    return exchange;
}

void TradeSequenceExchange_Exit(void *data) {
    TradeSequenceExchange *exchange = data;

    if (exchange != NULL) {
        TradeSequenceExchange_FreeBgs(exchange);
        Heap_Free(exchange);
    }
}

BOOL TradeSequenceExchange_Main(void *data, int *state) {
    TradeSequenceExchange *exchange = data;

    if (*state < NELEMS(sTradeSequenceExchangeStates)) {
        if (sTradeSequenceExchangeStates[*state](exchange, &exchange->subState)) {
            *state = *state + 1;
            exchange->subState = 0;
        }
        return FALSE;
    }
    return TRUE;
}

static BOOL TradeSequenceExchange_SetupScreens(TradeSequenceExchange *exchange, u32 *state) {
    TradeSequenceExchange_InitBgs(exchange);
    BeginNormalPaletteFade(0, 1, 1, RGB_WHITE, 8, 1, HEAP_ID_57);
    return TRUE;
}

static BOOL TradeSequenceExchange_Slide(TradeSequenceExchange *exchange, u32 *state) {
    switch (*state) {
    case 0:
        if (IsPaletteFadeFinished()) {
            exchange->sentY = FX32_CONST(-384);
            exchange->receivedY = FX32_CONST(80);
            exchange->sentStep = 0x5999;
            exchange->receivedStep = -0x2222;
            exchange->frames = 60;
            exchange->slideDone = FALSE;
            SysTask_CreateOnVBlankQueue(TradeSequenceExchange_SlideTask, exchange, 0);
            (*state)++;
        }
        break;
    case 1:
        if (exchange->slideDone) {
            PlaySE(SEQ_SE_DP_KOUKAN03);
            BeginNormalPaletteFade(0, 0, 0, RGB_WHITE, 16, 1, HEAP_ID_57);
            (*state)++;
        }
        break;
    case 2:
        if (IsPaletteFadeFinished()) {
            return TRUE;
        }
        break;
    }
    return FALSE;
}

static void TradeSequenceExchange_SlideTask(SysTask *task, void *data) {
    TradeSequenceExchange *exchange = data;

    if (exchange->frames != 0) {
        exchange->frames--;
        exchange->sentY += exchange->sentStep;
        exchange->receivedY += exchange->receivedStep;
        TradeSequenceExchange_SetMonPicPositions(exchange->bgConfig, exchange->sentY >> FX32_SHIFT, exchange->receivedY >> FX32_SHIFT);
    } else {
        TradeSequenceExchange_SetMonPicPositions(exchange->bgConfig, -48, -48);
        exchange->slideDone = TRUE;
        SysTask_Destroy(task);
    }
}

static void TradeSequenceExchange_InitBgs(TradeSequenceExchange *exchange) {
    GfGfx_SetBanks(&sTradeSequenceExchangeGraphicsBanks);
    reg_GX_POWCNT |= REG_GX_POWCNT_DSEL_MASK;
    SetBothScreensModesAndDisable(&sTradeSequenceExchangeGraphicsModes);
    InitBgFromTemplate(exchange->bgConfig, GF_BG_LYR_MAIN_1, &sTradeSequenceExchangeBgTemplate_SentMon, GF_BG_TYPE_TEXT);
    InitBgFromTemplate(exchange->bgConfig, GF_BG_LYR_SUB_1, &sTradeSequenceExchangeBgTemplate_SentMon, GF_BG_TYPE_TEXT);
    InitBgFromTemplate(exchange->bgConfig, GF_BG_LYR_MAIN_2, &sTradeSequenceExchangeBgTemplate_ReceivedMon, GF_BG_TYPE_TEXT);
    InitBgFromTemplate(exchange->bgConfig, GF_BG_LYR_MAIN_3, &sTradeSequenceExchangeBgTemplate_Background, GF_BG_TYPE_TEXT);
    InitBgFromTemplate(exchange->bgConfig, GF_BG_LYR_SUB_3, &sTradeSequenceExchangeBgTemplate_Background, GF_BG_TYPE_TEXT);
    GfGfxLoader_LoadCharData(NARC_a_0_8_9, 19, exchange->bgConfig, GF_BG_LYR_MAIN_3, 0, 0, TRUE, HEAP_ID_57);
    GfGfxLoader_LoadCharData(NARC_a_0_8_9, 19, exchange->bgConfig, GF_BG_LYR_SUB_3, 0, 0, TRUE, HEAP_ID_57);
    GfGfxLoader_LoadScrnData(NARC_a_0_8_9, 18, exchange->bgConfig, GF_BG_LYR_MAIN_3, 0, 0, TRUE, HEAP_ID_57);
    GfGfxLoader_LoadScrnData(NARC_a_0_8_9, 18, exchange->bgConfig, GF_BG_LYR_SUB_3, 0, 0, TRUE, HEAP_ID_57);
    GfGfxLoader_GXLoadPal(NARC_a_0_8_9, 20, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_0_OFFSET, 0x20, HEAP_ID_57);
    GfGfxLoader_GXLoadPal(NARC_a_0_8_9, 20, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_0_OFFSET, 0x20, HEAP_ID_57);
    BG_FillCharDataRange(exchange->bgConfig, GF_BG_LYR_MAIN_1, 0, 1, 200);
    BG_FillCharDataRange(exchange->bgConfig, GF_BG_LYR_SUB_1, 0, 1, 200);
    BG_FillCharDataRange(exchange->bgConfig, GF_BG_LYR_MAIN_2, 0, 1, 200);
    TradeSequence_LoadMonPicToBg(exchange->ts, 0, GF_BG_LYR_MAIN_1, 14, FALSE);
    TradeSequence_LoadMonPicToBg(exchange->ts, 0, GF_BG_LYR_SUB_1, 14, FALSE);
    TradeSequence_LoadMonPicToBg(exchange->ts, 1, GF_BG_LYR_MAIN_2, 15, FALSE);
    FillBgTilemapRect(exchange->bgConfig, GF_BG_LYR_MAIN_1, 200, 0, 0, 32, 64, 0);
    FillBgTilemapRect(exchange->bgConfig, GF_BG_LYR_SUB_1, 200, 0, 0, 32, 64, 0);
    FillBgTilemapRect(exchange->bgConfig, GF_BG_LYR_MAIN_2, 200, 0, 0, 32, 64, 0);
    TradeSequence_LoadMonPicTilemap(exchange->ts, 0, GF_BG_LYR_MAIN_1, 14, 20, 0);
    TradeSequence_LoadMonPicTilemap(exchange->ts, 0, GF_BG_LYR_SUB_1, 14, 20, 0);
    TradeSequence_LoadMonPicTilemap(exchange->ts, 1, GF_BG_LYR_MAIN_2, 15, 2, 0);
    BgCommitTilemapBufferToVram(exchange->bgConfig, GF_BG_LYR_MAIN_1);
    BgCommitTilemapBufferToVram(exchange->bgConfig, GF_BG_LYR_SUB_1);
    BgCommitTilemapBufferToVram(exchange->bgConfig, GF_BG_LYR_MAIN_2);
    TradeSequenceExchange_SetMonPicPositions(exchange->bgConfig, 80, -384);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
}

static void TradeSequenceExchange_FreeBgs(TradeSequenceExchange *exchange) {
    FreeBgTilemapBuffer(exchange->bgConfig, GF_BG_LYR_MAIN_3);
    FreeBgTilemapBuffer(exchange->bgConfig, GF_BG_LYR_SUB_3);
    FreeBgTilemapBuffer(exchange->bgConfig, GF_BG_LYR_MAIN_2);
    FreeBgTilemapBuffer(exchange->bgConfig, GF_BG_LYR_SUB_2);
    FreeBgTilemapBuffer(exchange->bgConfig, GF_BG_LYR_MAIN_1);
    FreeBgTilemapBuffer(exchange->bgConfig, GF_BG_LYR_SUB_1);
}

// The sent Pokemon is drawn on both screens (its sub-screen copy is 192 pixels further down), the received one on the
// main screen only.
static void TradeSequenceExchange_SetMonPicPositions(BgConfig *bgConfig, u32 sentY, u32 receivedY) {
    u32 sent = sentY & 0x1FF;
    u32 received = receivedY & 0x1FF;
    BgSetPosTextAndCommit(bgConfig, GF_BG_LYR_MAIN_2, BG_POS_OP_SET_Y, received);
    BgSetPosTextAndCommit(bgConfig, GF_BG_LYR_MAIN_1, BG_POS_OP_SET_Y, sent);
    sent += 192;
    BgSetPosTextAndCommit(bgConfig, GF_BG_LYR_SUB_1, BG_POS_OP_SET_Y, sent);
}
