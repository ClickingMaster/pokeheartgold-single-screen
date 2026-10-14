#include "global.h"

#include "constants/pokemon.h"
#include "constants/sndseq.h"

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
#include "sound_playback.h"
#include "sprite.h"
#include "sys_task.h"
#include "sys_task_api.h"
#include "text.h"
#include "trade_sequence_internal.h"
#include "trade_sequence_models.h"
#include "trade_sequence_receive_internal.h"
#include "screen_fade_legacy.h"
#include "swap_3d_buffers.h"

// Scene 6 of the trade animation (trades and receiving): the ball lands with a flash and the received Pokemon
// appears with its cry, then "<Pokemon> sent over <Pokemon>." / "Take good care of <Pokemon>!" (trade) or "<Pokemon>
// was taken out." with a fanfare, and the screen fades out. The ball's bounce task is in
// trade_sequence_receive_bounce.c.

#define TRADE_SEQUENCE_RECEIVE_NUM_STATES 4

typedef struct TradeSequenceReceiveBrightness {
    TradeSequenceReceive *receive; // 0x00
    fx32 value;                    // 0x04
    fx32 target;                   // 0x08
    fx32 step;                     // 0x0C
    int frames;                    // 0x10
} TradeSequenceReceiveBrightness;  // size: 0x14

typedef BOOL (*TradeSequenceReceiveStateFunc)(TradeSequenceReceive *receive, u32 *state);

extern const TradeSequenceRotation sTradeSequenceReceiveModelRotation;
extern const TradeSequenceReceiveStateFunc sTradeSequenceReceiveStates[TRADE_SEQUENCE_RECEIVE_NUM_STATES];
extern const GraphicsModes sTradeSequenceReceiveGraphicsModes;
extern const BgTemplate sTradeSequenceReceiveBgTemplate_Text;
extern const BgTemplate sTradeSequenceReceiveBgTemplate_Background;
extern const GraphicsBanks sTradeSequenceReceiveGraphicsBanks;

// Defined in render_window.c; not declared in its header.
void DrawFrame2AroundWindow(Window *window, u32 baseTile, u32 paletteNum);

TradeSequenceReceive *TradeSequenceReceive_Init(TradeSequence *ts);
void TradeSequenceReceive_Exit(TradeSequenceReceive *receive);
BOOL TradeSequenceReceive_Main(TradeSequenceReceive *receive, u32 *state);
void TradeSequenceReceive_Draw(TradeSequenceReceive *receive);
BOOL TradeSequenceReceive_SetupScreens(TradeSequenceReceive *receive, u32 *state);
BOOL TradeSequenceReceive_WaitFadeIn(TradeSequenceReceive *receive, u32 *state);
BOOL TradeSequenceReceive_ShowMon(TradeSequenceReceive *receive, u32 *state);
BOOL TradeSequenceReceive_ShowMessages(TradeSequenceReceive *receive, u32 *state);
BOOL TradeSequenceReceive_WaitFramesAndFanfare(TradeSequenceReceive *receive, int frames);
void TradeSequenceReceive_Init3D(TradeSequenceReceive *receive);
Pokepic *TradeSequenceReceive_CreateMonPic(TradeSequenceReceive *receive);
void TradeSequenceReceive_CreateSprites(TradeSequenceReceive *receive);
void TradeSequenceReceive_DeleteSprites(TradeSequenceReceive *receive);
void TradeSequenceReceive_StopBrightness(TradeSequenceReceive *receive);
void TradeSequenceReceive_BrightnessTask(SysTask *task, void *data);
void TradeSequenceReceive_StartBounce(TradeSequenceReceive *receive, SysTask **task);

TradeSequenceReceive *TradeSequenceReceive_Init(TradeSequence *ts) {
    TradeSequenceReceive *receive = Heap_Alloc(HEAP_ID_57, sizeof(TradeSequenceReceive));

    if (receive != NULL) {
        receive->ts = ts;
        receive->subState = 0;
        receive->bgConfig = TradeSequence_GetBgConfig(ts);
        receive->pokepicManager = PokepicManager_Create(HEAP_ID_57);
        receive->pokepic = NULL;
        receive->msgTemplate = String_New(300, HEAP_ID_57);
        receive->msgString = String_New(300, HEAP_ID_57);
        receive->brightnessTask = NULL;
        receive->bounceTask = NULL;
        receive->models = NULL;
        receive->pokeAnimNarc = NARC_New(NARC_a_1_8_0, HEAP_ID_57);
    }
    return receive;
}

void TradeSequenceReceive_Exit(TradeSequenceReceive *receive) {
    if (receive != NULL) {
        TradeSequenceReceive_StopBrightness(receive);
        TradeSequenceReceive_DeleteSprites(receive);
        TradeSequenceReceive_StopBounce(receive->bounceTask);
        String_Delete(receive->msgTemplate);
        String_Delete(receive->msgString);
        if (receive->models != NULL) {
            TradeSequence_DestroyModels(receive->models);
        }
        FreeBgTilemapBuffer(receive->bgConfig, GF_BG_LYR_MAIN_1);
        RemoveWindow(&receive->window);
        if (receive->pokepic != NULL) {
            Pokepic_Delete(receive->pokepic);
        }
        NARC_Delete(receive->pokeAnimNarc);
        PokepicManager_Delete(receive->pokepicManager);
        Heap_Free(receive);
    }
}

BOOL TradeSequenceReceive_Main(TradeSequenceReceive *receive, u32 *state) {
    if (*state < TRADE_SEQUENCE_RECEIVE_NUM_STATES) {
        if (sTradeSequenceReceiveStates[*state](receive, &receive->subState)) {
            (*state)++;
            receive->subState = 0;
        }
        TradeSequenceReceive_Draw(receive);
        return FALSE;
    }
    return TRUE;
}

void TradeSequenceReceive_Draw(TradeSequenceReceive *receive) {
    u32 one;

    G3X_Reset();
    NNS_G3dGeBufferOP_N(G3OP_MTX_PUSH, NULL, 0);
    NNS_G3dGeFlushBuffer();
    NNS_G2dSetupSoftwareSpriteCamera();
    PokepicManager_HandleLoadImgAndOrPltt(receive->pokepicManager);
    PokepicManager_DrawAll(receive->pokepicManager);
    one = 1;
    NNS_G3dGeBufferOP_N(G3OP_MTX_POP, &one, 1);
    if (receive->models != NULL) {
        TradeSequence_DrawModels(receive->models);
    }
    RequestSwap3DBuffers(GX_SORTMODE_AUTO, GX_BUFFERMODE_Z);
}

BOOL TradeSequenceReceive_SetupScreens(TradeSequenceReceive *receive, u32 *state) {
    TradeSequenceRotation rotation;

    GfGfx_SetBanks(&sTradeSequenceReceiveGraphicsBanks);
    reg_GX_POWCNT |= REG_GX_POWCNT_DSEL_MASK;
    SetBothScreensModesAndDisable(&sTradeSequenceReceiveGraphicsModes);
    InitBgFromTemplate(receive->bgConfig, GF_BG_LYR_MAIN_1, &sTradeSequenceReceiveBgTemplate_Text, GF_BG_TYPE_TEXT);
    InitBgFromTemplate(receive->bgConfig, GF_BG_LYR_MAIN_2, &sTradeSequenceReceiveBgTemplate_Background, GF_BG_TYPE_TEXT);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG0, GF_PLANE_TOGGLE_ON);
    G2_SetBG0Priority(1);
    BG_FillCharDataRange(receive->bgConfig, GF_BG_LYR_MAIN_1, 0, 1, 0);
    FillBgTilemapRect(receive->bgConfig, GF_BG_LYR_MAIN_1, 0, 0, 0, 32, 32, 0);
    LoadUserFrameGfx2(receive->bgConfig, GF_BG_LYR_MAIN_1, 109, 2, (u8)TradeSequence_GetFrameType(receive->ts), HEAP_ID_57);
    AddWindowParameterized(receive->bgConfig, &receive->window, GF_BG_LYR_MAIN_1, 2, 19, 27, 4, 1, 1);
    FillWindowPixelBuffer(&receive->window, 15);
    GfGfxLoader_GXLoadPal(NARC_graphic_font, 8, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_1_OFFSET, 32, HEAP_ID_57);
    BgCommitTilemapBufferToVram(receive->bgConfig, GF_BG_LYR_MAIN_1);
    GfGfxLoader_LoadCharData(NARC_a_0_8_9, 22, receive->bgConfig, GF_BG_LYR_MAIN_2, 0, 0, TRUE, HEAP_ID_57);
    GfGfxLoader_LoadScrnData(NARC_a_0_8_9, 21, receive->bgConfig, GF_BG_LYR_MAIN_2, 0, 0, TRUE, HEAP_ID_57);
    GfGfxLoader_GXLoadPal(NARC_a_0_8_9, 23, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_0_OFFSET, 32, HEAP_ID_57);
    TradeSequenceReceive_Init3D(receive);
    receive->models = TradeSequence_CreateModels(1, 0, 0, 0);
    receive->pokepic = TradeSequenceReceive_CreateMonPic(receive);
    Pokepic_SetAttr(receive->pokepic, POKEPIC_VANISHED, TRUE);
    receive->ball = TradeSequence_LoadModel(receive->models, 0, NARC_a_0_8_9, 27, 0, FX32_CONST(19), FX32_CONST(120), FALSE);
    rotation = sTradeSequenceReceiveModelRotation;
    TradeSequence_SetModelRotation(receive->ball, &rotation);
    TradeSequenceReceive_CreateSprites(receive);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
    G2_SetBlendAlpha(GX_BLEND_PLANEMASK_BG0, GX_BLEND_PLANEMASK_BG2, 16, 0);
    BeginNormalPaletteFade(3, 1, 1, RGB_BLACK, 16, 1, HEAP_ID_57);
    return TRUE;
}

BOOL TradeSequenceReceive_WaitFadeIn(TradeSequenceReceive *receive, u32 *state) {
    if (*state == 0 && IsPaletteFadeFinished()) {
        Sprite_SetAnimCtrlSeq(receive->sprites[0], 1);
        Sprite_SetDrawFlag(receive->sprites[0], TRUE);
        return TRUE;
    }
    return FALSE;
}

#pragma force_active on

BOOL TradeSequenceReceive_ShowMon(TradeSequenceReceive *receive, u32 *state) {
    u8 cryDelay[4];

    switch (*state) {
    case 0:
        TradeSequenceReceive_StartBounce(receive, &receive->bounceTask);
        (*state)++;
        // fallthrough
    case 1:
        if (receive->bounceTask == NULL) {
            Sprite_SetAnimCtrlSeq(receive->sprites[1], 2);
            Sprite_SetDrawFlag(receive->sprites[1], TRUE);
            TradeSequence_SetModelVisible(receive->ball, FALSE);
            receive->timer = 0;
            (*state)++;
        }
        break;
    case 2:
        receive->timer++;
        if (receive->timer > 10) {
            Pokepic_SetAttr(receive->pokepic, POKEPIC_VANISHED, FALSE);
            TradeSequenceReceive_StartBrightness(receive, 16, 0, 12);
            receive->subState++; // = (*state)++
        }
        break;
    case 3:
        if (TradeSequenceReceive_IsBrightnessDone(receive)) {
            if (GetBoxMonData(TradeSequence_GetReceivedBoxMon(receive->ts), MON_DATA_IS_EGG, NULL) == FALSE) {
                NARC_ReadPokepicCryDelay(receive->pokeAnimNarc, cryDelay, TradeSequence_GetReceivedSpecies(receive->ts), 1);
                u16 species = TradeSequence_GetReceivedSpecies(receive->ts);
                int form = TradeSequence_GetReceivedForm(receive->ts);
                PlayCryDelayed(species, cryDelay[0], (u8)form);
                ((void (*)(Pokepic *, int))Pokepic_StartAnim)(receive->pokepic, 1); // the original call passes a 2nd argument
            }
            receive->timer = 0;
            (*state)++;
        }
        break;
    case 4:
        receive->timer++;
        if (receive->timer > 30) {
            return TRUE;
        }
        break;
    }
    return FALSE;
}

BOOL TradeSequenceReceive_ShowMessages(TradeSequenceReceive *receive, u32 *state) {
    switch (*state) {
    case 0:
        receive->timer = 0;
        ++*state;
        break;
    case 1:
        if (++receive->timer > 10) {
            MsgData *msgData = TradeSequence_GetMsgData(receive->ts);
            MessageFormat *msgFormat = TradeSequence_GetMsgFormat(receive->ts);
            int msgId, next;
            if (TradeSequence_GetTradeMode(receive->ts) == 1) {
                msgId = msg_0179_00002;
                next = 2;
            } else {
                msgId = msg_0179_00005;
                next = 3;
            }
            ReadMsgDataIntoString(msgData, msgId, receive->msgTemplate);
            StringExpandPlaceholders(msgFormat, receive->msgString, receive->msgTemplate);
            AddTextPrinterParameterized(&receive->window, 1, receive->msgString, 0, 0, TEXT_SPEED_NOTRANSFER, NULL);
            DrawFrame2AroundWindow(&receive->window, 109, 2);
            CopyWindowToVram(&receive->window);
            PlayFanfare(SEQ_ME_SHINKAOME);
            receive->timer = 0;
            receive->fanfarePlaying = TRUE;
            *state = next;
        }
        break;
    case 2:
        if (TradeSequenceReceive_WaitFramesAndFanfare(receive, 60)) {
            MsgData *msgData = TradeSequence_GetMsgData(receive->ts);
            MessageFormat *msgFormat = TradeSequence_GetMsgFormat(receive->ts);
            ReadMsgDataIntoString(msgData, msg_0179_00003, receive->msgTemplate);
            StringExpandPlaceholders(msgFormat, receive->msgString, receive->msgTemplate);
            FillWindowPixelBuffer(&receive->window, 15);
            AddTextPrinterParameterized(&receive->window, 1, receive->msgString, 0, 0, TEXT_SPEED_NOTRANSFER, NULL);
            CopyWindowPixelsToVram_TextMode(&receive->window);
            receive->timer = 0;
            ++*state;
        }
        break;
    case 3:
        if (TradeSequenceReceive_WaitFramesAndFanfare(receive, 60)) {
            ClearFrameAndWindow2(&receive->window, FALSE);
            receive->timer = 0;
            ++*state;
        }
        break;
    case 4:
        if (++receive->timer > 10) {
            BeginNormalPaletteFade(3, 0, 0, RGB_BLACK, 16, 1, HEAP_ID_57);
            ++*state;
        }
        break;
    case 5:
        if (IsPaletteFadeFinished()) {
            return TRUE;
        }
        break;
    }
    return FALSE;
}

#pragma force_active reset

BOOL TradeSequenceReceive_WaitFramesAndFanfare(TradeSequenceReceive *receive, int frames) {
    if (receive->fanfarePlaying) {
        receive->fanfarePlaying = IsFanfarePlaying();
    }
    if (receive->timer < frames) {
        receive->timer++;
        return FALSE;
    }
    if (!receive->fanfarePlaying) {
        return TRUE;
    }
    return FALSE;
}

void TradeSequenceReceive_Init3D(TradeSequenceReceive *receive) {
    NNSGfdTexKey texKey;
    NNSGfdPlttKey plttKey;

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
    PokepicManager_SetCharBaseAddrAndSize(receive->pokepicManager, (texKey & 0xFFFF) << 3, ((texKey & 0x7FFF0000) >> 16) << 4);
    PokepicManager_SetPlttBaseAddrAndSize(receive->pokepicManager, (plttKey & 0xFFFF) << 3, ((plttKey & 0xFFFF0000) >> 16) << 3);
}

#pragma force_active on

Pokepic *TradeSequenceReceive_CreateMonPic(TradeSequenceReceive *receive) {
    PokepicTemplate template;
    int y;
    BoxPokemon *boxMon = TradeSequence_GetReceivedBoxMon(receive->ts);

    GetBoxmonSpriteCharAndPlttNarcIds(&template, boxMon, 2, FALSE);
    NARC_ReadPokepicAnimScript(receive->pokeAnimNarc, receive->animScript, TradeSequence_GetReceivedSpecies(receive->ts), 1);
    y = GetBoxMonPicHeightEx(boxMon, 2, FALSE) + 80;
    return PokepicManager_CreatePokepic(receive->pokepicManager, &template, 128, y, 0, 0, receive->animScript, NULL);
}

#pragma force_active reset

void TradeSequenceReceive_CreateSprites(TradeSequenceReceive *receive) {
    NNSG2dImagePaletteProxy palette;
    NNSG2dImageProxy image;
    SpriteResourcesHeader header;

    TradeSequence_LoadCellAnim(&receive->cellAnim, NARC_a_0_8_9, 7, 8);
    NNS_G2dInitImagePaletteProxy(&palette);
    NNS_G2dInitImageProxy(&image);
    GfGfxLoader_LoadImageMapping(NARC_a_0_8_9, 9, TRUE, GF_BG_LYR_MAIN_0, 0, NNS_G2D_VRAM_TYPE_2DMAIN, 0, HEAP_ID_57, &image);
    GfGfxLoader_PartiallyLoadPalette(NARC_a_0_8_9, 10, NNS_G2D_VRAM_TYPE_2DMAIN, 0, HEAP_ID_57, &palette);
    TradeSequence_InitSpriteHeader(&header, &receive->cellAnim, &image, &palette, 2);
    receive->sprites[0] = TradeSequence_CreateSprite(receive->ts, &header, 128, 100, 0, NNS_G2D_VRAM_TYPE_2DMAIN);
    receive->sprites[1] = TradeSequence_CreateSprite(receive->ts, &header, 128, 90, 0, NNS_G2D_VRAM_TYPE_2DMAIN);
    Sprite_SetPriority(receive->sprites[1], 1);
    Sprite_SetDrawFlag(receive->sprites[0], FALSE);
    Sprite_SetDrawFlag(receive->sprites[1], FALSE);
}

void TradeSequenceReceive_DeleteSprites(TradeSequenceReceive *receive) {
    int i;
    for (i = 0; i < 2; i++) {
        if (receive->sprites[i] != NULL) {
            Sprite_Delete(receive->sprites[i]);
        }
    }
    TradeSequence_FreeCellAnim(&receive->cellAnim);
}

#pragma force_active on

void TradeSequenceReceive_StartBrightness(TradeSequenceReceive *receive, int start, int target, int frames) {
    TradeSequenceReceiveBrightness *brightness = Heap_Alloc(HEAP_ID_57, sizeof(*brightness));
    if (brightness) {
        brightness->receive = receive;
        brightness->value = start << FX32_SHIFT;
        brightness->target = target << FX32_SHIFT;
        brightness->step = (brightness->target - brightness->value) / frames;
        brightness->frames = frames;
        receive->brightnessTask = SysTask_CreateOnVBlankQueue(TradeSequenceReceive_BrightnessTask, brightness, 0);
        G2_SetBlendBrightness(GX_BLEND_PLANEMASK_BG0, start);
    }
}

#pragma force_active reset

BOOL TradeSequenceReceive_IsBrightnessDone(TradeSequenceReceive *receive) {
    if (receive->brightnessTask == NULL) {
        return TRUE;
    }
    return FALSE;
}

void TradeSequenceReceive_StopBrightness(TradeSequenceReceive *receive) {
    if (receive->brightnessTask != NULL) {
        TradeSequence_DeferFree(SysTask_GetData(receive->brightnessTask));
        SysTask_Destroy(receive->brightnessTask);
        receive->brightnessTask = NULL;
    }
}

void TradeSequenceReceive_BrightnessTask(SysTask *task, void *data) {
    TradeSequenceReceiveBrightness *brightness = data;
    fx32 value;

    brightness->frames--;
    if (brightness->frames > 0) {
        value = brightness->value + brightness->step;
        brightness->value = value;
        G2_SetBlendBrightness(GX_BLEND_PLANEMASK_BG0, value >> FX32_SHIFT);
        return;
    }
    G2_SetBlendBrightness(GX_BLEND_PLANEMASK_BG0, brightness->target >> FX32_SHIFT);
    TradeSequenceReceive_StopBrightness(brightness->receive);
}

#pragma force_active on

void TradeSequenceReceive_StartBounce(TradeSequenceReceive *receive, SysTask **task) {
    TradeSequenceReceiveBounce *bounce = Heap_Alloc(HEAP_ID_57, sizeof(TradeSequenceReceiveBounce));
    if (bounce) {
        u32 zero = 0;
        bounce->state = zero;
        bounce->task = task;
        bounce->models = receive->models;
        bounce->ball = receive->ball;
        bounce->receive = receive;
        bounce->alpha = zero;
        bounce->alphaStep = 0x18CC;
        bounce->alphaFrames = 20;
        bounce->spinSpeed = 0x1000;
        bounce->spinFrames = zero;
        TradeSequence_SetModelAlpha(bounce->ball, 0);
        TradeSequence_SetModelVisible(bounce->ball, TRUE);
        TradeSequence_GetModelPosition(bounce->ball, &bounce->position);
        TradeSequence_GetModelRotation(bounce->ball, &bounce->rotation);
        *task = SysTask_CreateOnMainQueue(TradeSequenceReceive_BounceTask, bounce, 0);
        if (*task == NULL) {
            Heap_Free(bounce);
        }
    } else {
        *task = NULL;
    }
}

#pragma force_active reset

void TradeSequenceReceive_StopBounce(SysTask *task) {
    TradeSequenceReceiveBounce *bounce;
    if (task != NULL) {
        bounce = SysTask_GetData(task);
        *bounce->task = NULL;
        Heap_Free(bounce);
        SysTask_Destroy(task);
    }
}

void TradeSequenceReceive_UpdateSpin(TradeSequenceReceiveBounce *bounce) {
    if (bounce->spinFrames != 0) {
        bounce->spinSpeed += bounce->spinStep;
        bounce->spinFrames--;
        if (bounce->spinFrames == 0) {
            bounce->spinSpeed = bounce->spinTarget;
        }
    }
    bounce->rotation.y += bounce->spinSpeed;
    TradeSequence_SetModelRotation(bounce->ball, &bounce->rotation);
}

void TradeSequenceReceive_SetSpin(TradeSequenceReceiveBounce *bounce, int target, int frames) {
    if (frames != 0) {
        bounce->spinStep = (target - bounce->spinSpeed) / frames;
        bounce->spinFrames = frames;
    }
}
