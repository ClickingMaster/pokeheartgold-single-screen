#include "global.h"

#include "constants/pokemon.h"
#include "constants/sndseq.h"

#include "application/trade_sequence/trade_sequence_internal.h"
#include "application/trade_sequence/trade_sequence_models.h"
#include "msgdata/msg/msg_0179.h"

#include "bg_window.h"
#include "filesystem.h"
#include "gf_3d_vramman.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "message_format.h"
#include "msgdata.h"
#include "pm_string.h"
#include "pokemon.h"
#include "pokepic.h"
#include "render_window.h"
#include "screen_fade_legacy.h"
#include "sound_playback.h"
#include "sprite.h"
#include "sprite_resource_header.h"
#include "swap_3d_buffers.h"
#include "sys_task.h"
#include "sys_task_api.h"
#include "text.h"
#include "wireless_status_icon.h"

// Scene 0 of the trade animation: the sent Pokemon's picture and the "will be sent to" / "Bye-bye" messages, then
// a flash replaces the picture by a 3D model that bounces until the screen fades out.

#define TRADE_SEQUENCE_SEND_NUM_STATES 4

typedef struct TradeSequenceSend {
    TradeSequence *ts;
    u32 subState;
    int timer;
    PokepicManager *pokepicManager;
    Pokepic *pokepic;
    PokepicAnimScript animScript[10];
    Sprite *sprites[2];
    TradeSequenceCellAnim cellAnim;
    BgConfig *bgConfig;
    Window window;
    String *msgTemplate;
    String *msgString;
    TradeSequenceModels *models;
    TradeSequenceModel *model;
    SysTask *brightnessTask;
    SysTask *bounceTask;
    NARC *pokeAnimNarc;
} TradeSequenceSend;

typedef struct TradeSequenceSendBrightness {
    TradeSequenceSend *send;
    fx32 value;
    fx32 target;
    fx32 step;
    int frames;
} TradeSequenceSendBrightness;

typedef struct TradeSequenceSendBounce {
    TradeSequenceSend *send;
    TradeSequenceModel *model;
    VecFx32 position;
    TradeSequenceRotation rotation;
    u16 padding_1A;
    fx32 speed;
    int bounces;
    BOOL landed;
    int frames;
    s16 spin;
} TradeSequenceSendBounce;

enum TradeSequenceSendMessageStep {
    TRADE_SEND_MSG_INIT,
    TRADE_SEND_MSG_PRINT_SENDING,
    TRADE_SEND_MSG_PRINT_BYE_BYE,
    TRADE_SEND_MSG_CLEAR,
    TRADE_SEND_MSG_WAIT_END,
};

enum TradeSequenceSendReplaceStep {
    TRADE_SEND_REPLACE_START_FLASH,
    TRADE_SEND_REPLACE_WAIT_FLASH,
    TRADE_SEND_REPLACE_WAIT_BALL_ANIM,
    TRADE_SEND_REPLACE_WAIT_BOUNCES,
    TRADE_SEND_REPLACE_WAIT_FADE_OUT,
};

typedef BOOL (*TradeSequenceSendStateFunc)(TradeSequenceSend *send, u32 *state);

void DrawFrame2AroundWindow(Window *window, u32 baseTile, u32 paletteNum);

static void TradeSequenceSend_Draw(TradeSequenceSend *send);
static BOOL TradeSequenceSend_SetupScreens(TradeSequenceSend *send, u32 *state);
static BOOL TradeSequenceSend_WaitFadeIn(TradeSequenceSend *send, u32 *state);
static BOOL TradeSequenceSend_ShowMessages(TradeSequenceSend *send, u32 *state);
static BOOL TradeSequenceSend_ReplaceMonWithModel(TradeSequenceSend *send, u32 *state);
static void TradeSequenceSend_Init3D(TradeSequenceSend *send);
static Pokepic *TradeSequenceSend_CreateMonPic(TradeSequenceSend *send);
static void TradeSequenceSend_CreateSprites(TradeSequenceSend *send);
static void TradeSequenceSend_DeleteSprites(TradeSequenceSend *send);
static void TradeSequenceSend_StartBrightness(TradeSequenceSend *send, int start, int target, int frames);
static BOOL TradeSequenceSend_IsBrightnessDone(TradeSequenceSend *send);
static void TradeSequenceSend_StopBrightness(TradeSequenceSend *send);
static void TradeSequenceSend_BrightnessTask(SysTask *task, void *data);
static void TradeSequenceSend_StartBounce(TradeSequenceSend *send);
static BOOL TradeSequenceSend_IsBounceDone(TradeSequenceSend *send);
static int TradeSequenceSend_GetBounceCount(TradeSequenceSend *send);
static void TradeSequenceSend_StopBounce(TradeSequenceSend *send);
static void TradeSequenceSend_BounceTask(SysTask *task, void *data);

static const TradeSequenceRotation sTradeSequenceSendModelRotation = {
    .x = 0,
    .y = -0x1000,
    .z = 0,
};

static const TradeSequenceSendStateFunc sTradeSequenceSendStates[TRADE_SEQUENCE_SEND_NUM_STATES] = {
    TradeSequenceSend_SetupScreens,
    TradeSequenceSend_WaitFadeIn,
    TradeSequenceSend_ShowMessages,
    TradeSequenceSend_ReplaceMonWithModel,
};

static const GraphicsModes sTradeSequenceSendGraphicsModes = {
    .dispMode = GX_DISPMODE_GRAPHICS,
    .bgMode = GX_BGMODE_0,
    .subMode = GX_BGMODE_0,
    ._2d3dMode = GX_BG0_AS_3D,
};

static const BgTemplate sTradeSequenceSendBgTemplate_Text = {
    .x = 0,
    .y = 0,
    .bufferSize = 0x800,
    .baseTile = 0,
    .size = GF_BG_SCR_SIZE_256x256,
    .colorMode = GX_BG_COLORMODE_16,
    .screenBase = GX_BG_SCRBASE_0xf800,
    .charBase = GX_BG_CHARBASE_0x00000,
    .bgExtPltt = GX_BG_EXTPLTT_01,
    .priority = 0,
    .areaOver = GX_BG_AREAOVER_XLU,
    .mosaic = FALSE,
};

static const BgTemplate sTradeSequenceSendBgTemplate_Background = {
    .x = 0,
    .y = 0,
    .bufferSize = 0,
    .baseTile = 0,
    .size = GF_BG_SCR_SIZE_256x256,
    .colorMode = GX_BG_COLORMODE_16,
    .screenBase = GX_BG_SCRBASE_0xf000,
    .charBase = GX_BG_CHARBASE_0x04000,
    .bgExtPltt = GX_BG_EXTPLTT_01,
    .priority = 2,
    .areaOver = GX_BG_AREAOVER_XLU,
    .mosaic = FALSE,
};

static const GraphicsBanks sTradeSequenceSendGraphicsBanks = {
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

void *TradeSequenceSend_Init(TradeSequence *ts) {
    TradeSequenceSend *send = Heap_Alloc(HEAP_ID_57, sizeof(TradeSequenceSend));
    if (send) {
        send->ts = ts;
        send->subState = 0;
        send->bgConfig = TradeSequence_GetBgConfig(ts);
        send->pokepicManager = PokepicManager_Create(HEAP_ID_57);
        send->pokepic = NULL;
        send->msgTemplate = String_New(300, HEAP_ID_57);
        send->msgString = String_New(300, HEAP_ID_57);
        send->brightnessTask = NULL;
        send->bounceTask = NULL;
        send->pokeAnimNarc = NARC_New(NARC_a_1_8_0, HEAP_ID_57);
    }
    return send;
}

void TradeSequenceSend_Exit(void *data) {
    TradeSequenceSend *send = data;

    if (send) {
        TradeSequenceSend_StopBrightness(send);
        TradeSequenceSend_StopBounce(send);
        TradeSequenceSend_DeleteSprites(send);
        String_Delete(send->msgTemplate);
        String_Delete(send->msgString);
        TradeSequence_DestroyModels(send->models);
        FreeBgTilemapBuffer(send->bgConfig, GF_BG_LYR_MAIN_1);
        FreeBgTilemapBuffer(send->bgConfig, GF_BG_LYR_MAIN_2);
        FreeBgTilemapBuffer(send->bgConfig, GF_BG_LYR_SUB_2);
        RemoveWindow(&send->window);
        NARC_Delete(send->pokeAnimNarc);
        if (send->pokepic) {
            Pokepic_Delete(send->pokepic);
        }
        PokepicManager_Delete(send->pokepicManager);
        Heap_Free(send);
    }
}

BOOL TradeSequenceSend_Main(void *data, int *state) {
    TradeSequenceSend *send = data;

    if (*state < NELEMS(sTradeSequenceSendStates)) {
        if (sTradeSequenceSendStates[*state](send, &send->subState)) {
            *state = *state + 1;
            send->subState = 0;
        }
        TradeSequenceSend_Draw(send);
        return FALSE;
    }
    return TRUE;
}

static void TradeSequenceSend_Draw(TradeSequenceSend *send) {
    u32 one;

    G3X_Reset();
    NNS_G3dGeBufferOP_N(G3OP_MTX_PUSH, NULL, 0);
    NNS_G3dGeFlushBuffer();
    NNS_G2dSetupSoftwareSpriteCamera();
    PokepicManager_HandleLoadImgAndOrPltt(send->pokepicManager);
    PokepicManager_DrawAll(send->pokepicManager);
    one = 1;
    NNS_G3dGeBufferOP_N(G3OP_MTX_POP, &one, 1);
    TradeSequence_DrawModels(send->models);
    RequestSwap3DBuffers(GX_SORTMODE_AUTO, GX_BUFFERMODE_Z);
}

static BOOL TradeSequenceSend_SetupScreens(TradeSequenceSend *send, u32 *state) {
    TradeSequenceRotation rotation;

    GfGfx_SetBanks(&sTradeSequenceSendGraphicsBanks);
    reg_GX_POWCNT &= ~REG_GX_POWCNT_DSEL_MASK;
    SetBothScreensModesAndDisable(&sTradeSequenceSendGraphicsModes);
    InitBgFromTemplate(send->bgConfig, GF_BG_LYR_MAIN_1, &sTradeSequenceSendBgTemplate_Text, GF_BG_TYPE_TEXT);
    InitBgFromTemplate(send->bgConfig, GF_BG_LYR_MAIN_2, &sTradeSequenceSendBgTemplate_Background, GF_BG_TYPE_TEXT);
    InitBgFromTemplate(send->bgConfig, GF_BG_LYR_SUB_2, &sTradeSequenceSendBgTemplate_Background, GF_BG_TYPE_TEXT);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG0, GF_PLANE_TOGGLE_ON);
    G2_SetBG0Priority(1);
    BG_FillCharDataRange(send->bgConfig, GF_BG_LYR_MAIN_1, 0, 1, 0);
    FillBgTilemapRect(send->bgConfig, GF_BG_LYR_MAIN_1, 0, 0, 0, 32, 32, 0);
    LoadUserFrameGfx2(send->bgConfig, GF_BG_LYR_MAIN_1, 109, 2, (u8)TradeSequence_GetFrameType(send->ts), HEAP_ID_57);
    AddWindowParameterized(send->bgConfig, &send->window, GF_BG_LYR_MAIN_1, 2, 19, 27, 4, 1, 1);
    FillWindowPixelBuffer(&send->window, 15);
    GfGfxLoader_GXLoadPal(NARC_graphic_font, 8, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_1_OFFSET, 32, HEAP_ID_57);
    GfGfxLoader_LoadCharData(NARC_a_0_8_9, 22, send->bgConfig, GF_BG_LYR_MAIN_2, 0, 0, TRUE, HEAP_ID_57);
    GfGfxLoader_LoadScrnData(NARC_a_0_8_9, 21, send->bgConfig, GF_BG_LYR_MAIN_2, 0, 0, TRUE, HEAP_ID_57);
    GfGfxLoader_GXLoadPal(NARC_a_0_8_9, 23, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_0_OFFSET, 32, HEAP_ID_57);
    GfGfxLoader_LoadCharData(NARC_a_0_8_9, 22, send->bgConfig, GF_BG_LYR_SUB_2, 0, 0, TRUE, HEAP_ID_57);
    GfGfxLoader_LoadScrnData(NARC_a_0_8_9, 21, send->bgConfig, GF_BG_LYR_SUB_2, 0, 0, TRUE, HEAP_ID_57);
    GfGfxLoader_GXLoadPal(NARC_a_0_8_9, 23, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_0_OFFSET, 32, HEAP_ID_57);
    BgCommitTilemapBufferToVram(send->bgConfig, GF_BG_LYR_MAIN_1);
    TradeSequenceSend_Init3D(send);
    send->models = TradeSequence_CreateModels(1, 0, 0, 0);
    send->pokepic = TradeSequenceSend_CreateMonPic(send);
    send->model = TradeSequence_LoadModel(send->models, 0, NARC_a_0_8_9, 27, 0, FX32_CONST(26.5), FX32_CONST(115.5), FALSE);
    rotation = sTradeSequenceSendModelRotation;
    TradeSequence_SetModelRotation(send->model, &rotation);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
    TradeSequenceSend_CreateSprites(send);
    G2_BlendNone();
    WirelessStatusIcon_Create();
    WirelessStatusIcon_ShowOnBothScreens(1, HEAP_ID_56);
    BeginNormalPaletteFade(0, 1, 1, RGB_BLACK, 16, 1, HEAP_ID_57);
    return TRUE;
}

static BOOL TradeSequenceSend_WaitFadeIn(TradeSequenceSend *send, u32 *state) {
    if (*state == 0 && IsPaletteFadeFinished()) {
        Sprite_SetAnimCtrlSeq(send->sprites[0], 1);
        Sprite_SetDrawFlag(send->sprites[0], TRUE);
        return TRUE;
    }
    return FALSE;
}

static BOOL TradeSequenceSend_ShowMessages(TradeSequenceSend *send, u32 *state) {
    switch (*state) {
    case TRADE_SEND_MSG_INIT:
        send->timer = 0;
        (*state)++;
        break;
    case TRADE_SEND_MSG_PRINT_SENDING:
        if (++send->timer > 10) {
            MsgData *msgData = TradeSequence_GetMsgData(send->ts);
            MessageFormat *msgFormat = TradeSequence_GetMsgFormat(send->ts);
            int msgId, next;
            if (TradeSequence_GetTradeMode(send->ts) == TRADE_SEQUENCE_MODE_FULL) {
                msgId = msg_0179_00000;
                next = TRADE_SEND_MSG_PRINT_BYE_BYE;
            } else {
                msgId = msg_0179_00004;
                next = TRADE_SEND_MSG_CLEAR;
            }
            ReadMsgDataIntoString(msgData, msgId, send->msgTemplate);
            StringExpandPlaceholders(msgFormat, send->msgString, send->msgTemplate);
            AddTextPrinterParameterized(&send->window, 1, send->msgString, 0, 0, TEXT_SPEED_NOTRANSFER, NULL);
            DrawFrame2AroundWindow(&send->window, 109, 2);
            CopyWindowToVram(&send->window);
            send->timer = 0;
            *state = next;
        }
        break;
    case TRADE_SEND_MSG_PRINT_BYE_BYE:
        if (++send->timer > 60) {
            MsgData *msgData = TradeSequence_GetMsgData(send->ts);
            MessageFormat *msgFormat = TradeSequence_GetMsgFormat(send->ts);
            ReadMsgDataIntoString(msgData, msg_0179_00001, send->msgTemplate);
            StringExpandPlaceholders(msgFormat, send->msgString, send->msgTemplate);
            if (GetBoxMonData(TradeSequence_GetSentBoxMon(send->ts), MON_DATA_IS_EGG, NULL) == FALSE) {
                u8 cryDelay;
                Pokepic_StartAnim(send->pokepic, 1);
                NARC_ReadPokepicCryDelay(send->pokeAnimNarc, &cryDelay, TradeSequence_GetSentSpecies(send->ts), 1);
                u16 species = TradeSequence_GetSentSpecies(send->ts);
                PlayCryDelayed(species, cryDelay, (u8)TradeSequence_GetSentForm(send->ts));
            }
            FillWindowPixelBuffer(&send->window, 15);
            AddTextPrinterParameterized(&send->window, 1, send->msgString, 0, 0, TEXT_SPEED_NOTRANSFER, NULL);
            CopyWindowPixelsToVram_TextMode(&send->window);
            send->timer = 0;
            (*state)++;
        }
        break;
    case TRADE_SEND_MSG_CLEAR:
        if (++send->timer > 60) {
            ClearFrameAndWindow2(&send->window, FALSE);
            send->timer = 0;
            (*state)++;
        }
        break;
    case TRADE_SEND_MSG_WAIT_END:
        if (++send->timer > 20) {
            return TRUE;
        }
        break;
    }
    return FALSE;
}

static BOOL TradeSequenceSend_ReplaceMonWithModel(TradeSequenceSend *send, u32 *state) {
    switch (*state) {
    case TRADE_SEND_REPLACE_START_FLASH:
        PlaySE(SEQ_SE_DP_KOUKAN01);
        TradeSequenceSend_StartBrightness(send, 0, 16, 12);
        (*state)++;
        break;
    case TRADE_SEND_REPLACE_WAIT_FLASH:
        if (TradeSequenceSend_IsBrightnessDone(send)) {
            Pokepic_Delete(send->pokepic);
            Sprite_SetAnimCtrlSeq(send->sprites[1], 0);
            Sprite_SetDrawFlag(send->sprites[1], TRUE);
            (*state)++;
        }
        break;
    case TRADE_SEND_REPLACE_WAIT_BALL_ANIM:
        if (!Sprite_IsAnimated(send->sprites[1])) {
            TradeSequence_SetModelVisible(send->model, TRUE);
            TradeSequenceSend_StartBrightness(send, 16, 0, 16);
            TradeSequenceSend_StartBounce(send);
            (*state)++;
        }
        break;
    case TRADE_SEND_REPLACE_WAIT_BOUNCES:
        if (TradeSequenceSend_GetBounceCount(send) >= 2 || TradeSequenceSend_IsBounceDone(send) == TRUE) {
            BeginNormalPaletteFade(0, 0, 0, RGB_BLACK, 24, 1, HEAP_ID_57);
            (*state)++;
        }
        break;
    case TRADE_SEND_REPLACE_WAIT_FADE_OUT:
        if (IsPaletteFadeFinished() && TradeSequenceSend_IsBounceDone(send)) {
            return TRUE;
        }
        break;
    default:
        break;
    }
    return FALSE;
}

static void TradeSequenceSend_Init3D(TradeSequenceSend *send) {
    u32 texKey, plttKey;

    NNS_G3dInit();
    G3X_InitMtxStack();
    G3X_SetShading(GX_SHADING_TOON);
    G3X_AntiAlias(TRUE);
    G3X_AlphaTest(FALSE, 0);
    G3X_AlphaBlend(TRUE);
    G3X_SetClearColor(RGB_BLACK, 0, 0x7FFF, 63, FALSE);
    G3_SwapBuffers(GX_SORTMODE_AUTO, GX_BUFFERMODE_W);
    G3_ViewPort(0, 0, 255, 191);
    GF_3DVramMan_InitFrameTexVramManager(1, TRUE);
    GF_3DVramMan_InitFramePlttVramManager(0x4000, TRUE);
    texKey = NNS_GfdDefaultFuncAllocTexVram(0x4000, FALSE, 0);
    plttKey = NNS_GfdDefaultFuncAllocPlttVram(0x80, FALSE, 1);
    PokepicManager_SetCharBaseAddrAndSize(send->pokepicManager, (texKey & 0xFFFF) << 3, ((texKey & 0x7FFF0000) >> 16) << 4);
    PokepicManager_SetPlttBaseAddrAndSize(send->pokepicManager, (plttKey & 0xFFFF) << 3, ((plttKey & 0xFFFF0000) >> 16) << 3);
}

static Pokepic *TradeSequenceSend_CreateMonPic(TradeSequenceSend *send) {
    PokepicTemplate template;
    BoxPokemon *boxMon = TradeSequence_GetSentBoxMon(send->ts);

    GetBoxmonSpriteCharAndPlttNarcIds(&template, boxMon, 2, FALSE);
    NARC_ReadPokepicAnimScript(send->pokeAnimNarc, send->animScript, TradeSequence_GetSentSpecies(send->ts), 1);
    {
        int y = GetBoxMonPicHeightEx(boxMon, 2, FALSE) + 80;
        return PokepicManager_CreatePokepic(send->pokepicManager, &template, 128, y, 0, 0, send->animScript, NULL);
    }
}

static void TradeSequenceSend_CreateSprites(TradeSequenceSend *send) {
    NNSG2dImagePaletteProxy palette;
    NNSG2dImageProxy image;
    SpriteResourcesHeader header;

    TradeSequence_LoadCellAnim(&send->cellAnim, NARC_a_0_8_9, 7, 8);
    NNS_G2dInitImagePaletteProxy(&palette);
    NNS_G2dInitImageProxy(&image);
    GfGfxLoader_LoadImageMapping(NARC_a_0_8_9, 9, TRUE, GF_BG_LYR_MAIN_0, 0, NNS_G2D_VRAM_TYPE_2DMAIN, 0, HEAP_ID_57, &image);
    GfGfxLoader_PartiallyLoadPalette(NARC_a_0_8_9, 10, NNS_G2D_VRAM_TYPE_2DMAIN, 0, HEAP_ID_57, &palette);
    TradeSequence_InitSpriteHeader(&header, &send->cellAnim, &image, &palette, 2);
    send->sprites[0] = TradeSequence_CreateSprite(send->ts, &header, 128, 100, 0, NNS_G2D_VRAM_TYPE_2DMAIN);
    send->sprites[1] = TradeSequence_CreateSprite(send->ts, &header, 128, 90, 0, NNS_G2D_VRAM_TYPE_2DMAIN);
    Sprite_SetPriority(send->sprites[1], 1);
    Sprite_SetDrawFlag(send->sprites[0], FALSE);
    Sprite_SetDrawFlag(send->sprites[1], FALSE);
}

static void TradeSequenceSend_DeleteSprites(TradeSequenceSend *send) {
    s32 i;
    TradeSequenceSend *p;

    i = 0;
    p = send;
    do {
        if (p->sprites[0] != NULL) {
            Sprite_Delete(p->sprites[0]);
        }
        i++;
        p = (TradeSequenceSend *)((u8 *)p + sizeof(Sprite *));
    } while (i < 2);
    TradeSequence_FreeCellAnim(&send->cellAnim);
}

static void TradeSequenceSend_StartBrightness(TradeSequenceSend *send, int start, int target, int frames) {
    TradeSequenceSendBrightness *brightness = Heap_Alloc(HEAP_ID_57, sizeof(*brightness));
    if (brightness) {
        brightness->send = send;
        brightness->value = start << FX32_SHIFT;
        brightness->target = target << FX32_SHIFT;
        brightness->step = (brightness->target - brightness->value) / frames;
        brightness->frames = frames;
        send->brightnessTask = SysTask_CreateOnVBlankQueue(TradeSequenceSend_BrightnessTask, brightness, 0);
        G2_SetBlendBrightness(GX_BLEND_PLANEMASK_BG0, start);
    }
}

static BOOL TradeSequenceSend_IsBrightnessDone(TradeSequenceSend *send) {
    if (send->brightnessTask == NULL) {
        return TRUE;
    }
    return FALSE;
}

static void TradeSequenceSend_StopBrightness(TradeSequenceSend *send) {
    if (send->brightnessTask != NULL) {
        TradeSequence_DeferFree(SysTask_GetData(send->brightnessTask));
        SysTask_Destroy(send->brightnessTask);
        send->brightnessTask = NULL;
    }
}

static void TradeSequenceSend_BrightnessTask(SysTask *task, void *data) {
    TradeSequenceSendBrightness *brightness = data;

    if (--brightness->frames > 0) {
        brightness->value += brightness->step;
        G2_SetBlendBrightness(GX_BLEND_PLANEMASK_BG0, brightness->value >> FX32_SHIFT);
    } else {
        G2_SetBlendBrightness(GX_BLEND_PLANEMASK_BG0, brightness->target >> FX32_SHIFT);
        TradeSequenceSend_StopBrightness(brightness->send);
    }
}

static void TradeSequenceSend_StartBounce(TradeSequenceSend *send) {
    TradeSequenceSendBounce *bounce = Heap_Alloc(HEAP_ID_57, sizeof(*bounce));
    if (bounce) {
        bounce->send = send;
        bounce->model = send->model;
        TradeSequence_GetModelPosition(bounce->model, &bounce->position);
        TradeSequence_GetModelRotation(bounce->model, &bounce->rotation);
        bounce->speed = 0x2F60;
        bounce->bounces = 0;
        bounce->spin = 0;
        bounce->landed = FALSE;
        bounce->frames = 0;
        send->bounceTask = SysTask_CreateOnMainQueue(TradeSequenceSend_BounceTask, bounce, 0);
    }
}

static BOOL TradeSequenceSend_IsBounceDone(TradeSequenceSend *send) {
    if (send->bounceTask == NULL) {
        return TRUE;
    }
    return FALSE;
}

static int TradeSequenceSend_GetBounceCount(TradeSequenceSend *send) {
    if (send->bounceTask != NULL) {
        return ((TradeSequenceSendBounce *)SysTask_GetData(send->bounceTask))->bounces;
    }
    return 0;
}

static void TradeSequenceSend_StopBounce(TradeSequenceSend *send) {
    if (send->bounceTask != NULL) {
        Heap_Free(SysTask_GetData(send->bounceTask));
        SysTask_Destroy(send->bounceTask);
        send->bounceTask = NULL;
    }
}

static void TradeSequenceSend_BounceTask(SysTask *task, void *data) {
    TradeSequenceSendBounce *bounce = data;

    if (!bounce->landed) {
        bounce->position.y += bounce->speed;
        if (bounce->speed >= 0) {
            bounce->speed -= 1920;
        } else {
            bounce->speed -= 1920;
            if (bounce->position.y <= -FX32_CONST(4)) {
                bounce->position.y = -FX32_CONST(4);
                bounce->speed = -(bounce->speed * 44 / 100);
                if (bounce->speed < 4000) {
                    bounce->landed = TRUE;
                }
                PlaySE(SEQ_SE_DP_KON);
                bounce->bounces++;
                switch (bounce->bounces) {
                case 1:
                    bounce->spin = 176;
                    break;
                case 3:
                    bounce->spin += 80;
                    break;
                }
            }
        }
    }
    bounce->rotation.x += bounce->spin;
    bounce->rotation.z -= bounce->spin;
    TradeSequence_SetModelRotation(bounce->model, &bounce->rotation);
    bounce->position.x += bounce->spin * 5;
    bounce->position.z += bounce->spin * 5;
    TradeSequence_SetModelPosition(bounce->model, &bounce->position);
    if (bounce->landed) {
        bounce->spin -= 14;
        bounce->frames++;
        if (bounce->frames > 30) {
            TradeSequenceSend_StopBounce(bounce->send);
        }
    }
}
