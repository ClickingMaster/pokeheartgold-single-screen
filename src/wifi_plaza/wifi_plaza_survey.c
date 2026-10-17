#include "wifi_plaza/wifi_plaza_survey.h"

#include "global.h"

#include "constants/sndseq.h"

#include "msgdata/msg.naix"
#include "msgdata/msg/msg_0777.h"

#include "bg_window.h"
#include "font.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "oam_manager.h"
#include "obj_char_transfer.h"
#include "obj_pltt_transfer.h"
#include "save_plaza_survey.h"
#include "screen_fade.h"
#include "sound_playback.h"
#include "sprite_transfer.h"
#include "system.h"
#include "text.h"
#include "vram_transfer_manager.h"
#include "wireless_status_icon.h"

typedef void (*WiFiPlazaSurveyInitFunc)(WiFiPlazaSurveyModeWork *mode, WiFiPlazaSurveyMessages *messages, WiFiPlazaSurveyWork *work, WiFiPlazaSurveyArgs *args, enum HeapID heapID);
typedef BOOL (*WiFiPlazaSurveyMainFunc)(WiFiPlazaSurveyModeWork *mode, WiFiPlazaSurveyMessages *messages, WiFiPlazaSurveyWork *work, WiFiPlazaSurveyMessageBox *msgBox, WiFiPlazaSurveyArgs *args, enum HeapID heapID);
typedef void (*WiFiPlazaSurveyExitFunc)(WiFiPlazaSurveyModeWork *mode, WiFiPlazaSurveyMessages *messages, WiFiPlazaSurveyWork *work);

enum WiFiPlazaSurveyAnswerState {
    WIFI_PLAZA_SURVEY_ANSWER_PRINT_INTRO,
    WIFI_PLAZA_SURVEY_ANSWER_OPEN_YES_NO,
    WIFI_PLAZA_SURVEY_ANSWER_HANDLE_YES_NO,
    WIFI_PLAZA_SURVEY_ANSWER_PRINT_QUESTION_INTRO,
    WIFI_PLAZA_SURVEY_ANSWER_SHOW_ANSWERS,
    WIFI_PLAZA_SURVEY_ANSWER_SELECT,
    WIFI_PLAZA_SURVEY_ANSWER_SUBMIT,
    WIFI_PLAZA_SURVEY_ANSWER_WAIT_SUBMIT,
    WIFI_PLAZA_SURVEY_ANSWER_PRINT_THANKS,
    WIFI_PLAZA_SURVEY_ANSWER_FINISH,
    WIFI_PLAZA_SURVEY_ANSWER_WAIT_MESSAGE,
};

enum WiFiPlazaSurveyResultsState {
    WIFI_PLAZA_SURVEY_RESULTS_PRINT_INTRO,
    WIFI_PLAZA_SURVEY_RESULTS_PRINT_QUESTION,
    WIFI_PLAZA_SURVEY_RESULTS_WAIT_QUESTION,
    WIFI_PLAZA_SURVEY_RESULTS_PRINT_RESULTS_INTRO,
    WIFI_PLAZA_SURVEY_RESULTS_SHOW_RESULTS,
    WIFI_PLAZA_SURVEY_RESULTS_ANIMATE_BARS,
    WIFI_PLAZA_SURVEY_RESULTS_WAIT_RESULTS,
    WIFI_PLAZA_SURVEY_RESULTS_ASK_LAST_WEEK,
    WIFI_PLAZA_SURVEY_RESULTS_HANDLE_YES_NO,
    WIFI_PLAZA_SURVEY_RESULTS_FADE_OUT,
    WIFI_PLAZA_SURVEY_RESULTS_SHOW_LAST_WEEK,
    WIFI_PLAZA_SURVEY_RESULTS_FADE_IN,
    WIFI_PLAZA_SURVEY_RESULTS_WAIT_FADE_IN,
    WIFI_PLAZA_SURVEY_RESULTS_PRINT_LAST_WEEK_INTRO,
    WIFI_PLAZA_SURVEY_RESULTS_PRINT_LAST_WEEK_QUESTION,
    WIFI_PLAZA_SURVEY_RESULTS_WAIT_LAST_WEEK_QUESTION,
    WIFI_PLAZA_SURVEY_RESULTS_PRINT_LAST_WEEK_RESULTS_INTRO,
    WIFI_PLAZA_SURVEY_RESULTS_SHOW_LAST_WEEK_RESULTS,
    WIFI_PLAZA_SURVEY_RESULTS_ANIMATE_LAST_WEEK_BARS,
    WIFI_PLAZA_SURVEY_RESULTS_WAIT_LAST_WEEK_RESULTS,
    WIFI_PLAZA_SURVEY_RESULTS_PRINT_CLOSING,
    WIFI_PLAZA_SURVEY_RESULTS_FINISH,
    WIFI_PLAZA_SURVEY_RESULTS_WAIT_MESSAGE,
};

#define WIFI_PLAZA_SURVEY_BAR_LENGTH 80 // pixels
#define WIFI_PLAZA_SURVEY_BAR_FRAMES 20

static WiFiPlazaSurveyInitFunc sWiFiPlazaSurveyInitFuncs[] = {
    [WIFI_PLAZA_SURVEY_MODE_ANSWER] = WiFiPlazaSurveyAnswer_Init,
    [WIFI_PLAZA_SURVEY_MODE_RESULTS] = WiFiPlazaSurveyResults_Init,
};

static WiFiPlazaSurveyMainFunc sWiFiPlazaSurveyMainFuncs[] = {
    [WIFI_PLAZA_SURVEY_MODE_ANSWER] = WiFiPlazaSurveyAnswer_Main,
    [WIFI_PLAZA_SURVEY_MODE_RESULTS] = WiFiPlazaSurveyResults_Main,
};

static WiFiPlazaSurveyExitFunc sWiFiPlazaSurveyExitFuncs[] = {
    [WIFI_PLAZA_SURVEY_MODE_ANSWER] = WiFiPlazaSurveyAnswer_Exit,
    [WIFI_PLAZA_SURVEY_MODE_RESULTS] = WiFiPlazaSurveyResults_Exit,
};

BOOL WiFiPlazaSurvey_Init(OverlayManager *man, int *state) {
    WiFiPlazaSurveyWork *work;
    WiFiPlazaSurveyArgs *args;

    args = OverlayManager_GetArgs(man);

    Heap_Create(HEAP_ID_3, HEAP_ID_129, 0x50000);
    work = OverlayManager_CreateAndGetData(man, sizeof(WiFiPlazaSurveyWork), HEAP_ID_129);
    memset(work, 0, sizeof(WiFiPlazaSurveyWork));
    WiFiPlazaSurvey_InitGraphics(work, args->saveData, HEAP_ID_129);
    WiFiPlazaSurvey_InitMessages(&work->messages, args->lobby, HEAP_ID_129);
    WiFiPlazaSurvey_InitMessageBox(&work->msgBox, work, args->saveData, HEAP_ID_129);
    GF_ASSERT(args->mode < 2);
    sWiFiPlazaSurveyInitFuncs[args->mode](&work->mode, &work->messages, work, args, HEAP_ID_129);
    Main_SetVBlankIntrCB(WiFiPlazaSurvey_VBlankCB, work);
    HBlankInterruptDisable();
    return TRUE;
}

BOOL WiFiPlazaSurvey_Main(OverlayManager *man, int *state) {
    WiFiPlazaSurveyWork *work = OverlayManager_GetData(man);
    WiFiPlazaSurveyArgs *args = OverlayManager_GetArgs(man);

    switch (*state) {
    case 0:
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, RGB_BLACK, 6, 1, HEAP_ID_129);
        WiFiPlazaWork_SetLowVolume(args->lobby, 1);
        (*state)++;
        break;
    case 1:
        if (IsPaletteFadeFinished() == TRUE) {
            (*state)++;
        }
        break;
    case 2:
        if (IsPaletteFadeFinished() == TRUE) {
            if (WiFiPlazaWork_IsClosing(args->lobby) == TRUE) {
                WiFiPlazaWork_SetAppQuitForClosing(args->lobby);
                (*state)++;
                break;
            }
            if (WiFiPlaza_HasConnectionFailed() == TRUE) {
                (*state)++;
                break;
            }
        }
        if (sWiFiPlazaSurveyMainFuncs[args->mode](&work->mode, &work->messages, work, &work->msgBox, args, HEAP_ID_129) == TRUE) {
            (*state)++;
        }
        break;
    case 3:
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, RGB_BLACK, 6, 1, HEAP_ID_129);
        (*state)++;
        break;
    case 4:
        if (IsPaletteFadeFinished() == TRUE) {
            WiFiPlazaSurvey_StopWaitingIcon(&work->msgBox);
            return TRUE;
        }
        break;
    }
    WiFiPlazaSurvey_RenderSprites(work);
    return FALSE;
}

BOOL WiFiPlazaSurvey_Exit(OverlayManager *man, int *state) {
    WiFiPlazaSurveyWork *work = OverlayManager_GetData(man);
    WiFiPlazaSurveyArgs *args = OverlayManager_GetArgs(man);

    Main_SetVBlankIntrCB(NULL, NULL);
    HBlankInterruptDisable();
    sWiFiPlazaSurveyExitFuncs[args->mode](&work->mode, &work->messages, work);
    WiFiPlazaSurvey_FreeMessageBox(&work->msgBox, HEAP_ID_129);
    WiFiPlazaSurvey_FreeMessages(&work->messages);
    WiFiPlazaSurvey_FreeGraphics(work);
    OverlayManager_FreeData(man);
    Heap_Destroy(HEAP_ID_129);
    StopAllSE(0);
    return TRUE;
}

void WiFiPlazaSurvey_VBlankCB(void *data) {
    WiFiPlazaSurvey_DoVBlankUpdates(data);
}

static const GraphicsBanks sWiFiPlazaSurveyGraphicsBanks = {
    .bg = GX_VRAM_BG_256_AB,
    .bgextpltt = GX_VRAM_BGEXTPLTT_NONE,
    .subbg = GX_VRAM_SUB_BG_128_C,
    .subbgextpltt = GX_VRAM_SUB_BGEXTPLTT_NONE,
    .obj = GX_VRAM_OBJ_16_G,
    .objextpltt = GX_VRAM_OBJEXTPLTT_NONE,
    .subobj = GX_VRAM_SUB_OBJ_NONE,
    .subobjextpltt = GX_VRAM_SUB_OBJEXTPLTT_NONE,
    .tex = GX_VRAM_TEX_NONE,
    .texpltt = GX_VRAM_TEXPLTT_NONE,
};

void WiFiPlazaSurvey_InitGraphics(WiFiPlazaSurveyWork *work, SaveData *saveData, enum HeapID heapID) {
    Options *options = Save_PlayerData_GetOptionsAddr(saveData);
    work->narc = NARC_New(NARC_a_2_0_7, heapID);
    GF_CreateVramTransferManager(32, heapID);
    GfGfx_SetBanks(&sWiFiPlazaSurveyGraphicsBanks);
    WiFiPlazaSurvey_InitBgs(work, options, heapID);
    WiFiPlazaSurvey_InitSpriteSystem(work, heapID);
}

void WiFiPlazaSurvey_FreeGraphics(WiFiPlazaSurveyWork *work) {
    NARC_Delete(work->narc);
    GF_DestroyVramTransferManager();
    WiFiPlazaSurvey_FreeBgs(work);
    WiFiPlazaSurvey_FreeSpriteSystem(work);
}

void WiFiPlazaSurvey_RenderSprites(WiFiPlazaSurveyWork *work) {
    SpriteList_RenderAndAnimateSprites(work->spriteList);
}

void WiFiPlazaSurvey_DoVBlankUpdates(WiFiPlazaSurveyWork *work) {
    DoScheduledBgGpuUpdates(work->bgConfig);
    OamManager_ApplyAndResetBuffers();
    GF_RunVramTransferTasks();
}

static const u32 sWiFiPlazaSurveyBgLayers[4] = { GF_BG_LYR_MAIN_0, GF_BG_LYR_MAIN_1, GF_BG_LYR_MAIN_2, GF_BG_LYR_SUB_0 };

static const BgTemplate sWiFiPlazaSurveyBgTemplates[4] = {
    {
     .x = 0,
     .y = 0,
     .bufferSize = 0x800,
     .baseTile = 0,
     .size = GF_BG_SCR_SIZE_256x256,
     .colorMode = GX_BG_COLORMODE_16,
     .screenBase = GX_BG_SCRBASE_0xf800,
     .charBase = GX_BG_CHARBASE_0x08000,
     .bgExtPltt = GX_BG_EXTPLTT_01,
     .priority = 3,
     .areaOver = GX_BG_AREAOVER_XLU,
     .mosaic = FALSE,
     },
    {
     .x = 0,
     .y = 0,
     .bufferSize = 0x800,
     .baseTile = 0,
     .size = GF_BG_SCR_SIZE_256x256,
     .colorMode = GX_BG_COLORMODE_16,
     .screenBase = GX_BG_SCRBASE_0xf000,
     .charBase = GX_BG_CHARBASE_0x00000,
     .bgExtPltt = GX_BG_EXTPLTT_01,
     .priority = 1,
     .areaOver = GX_BG_AREAOVER_XLU,
     .mosaic = FALSE,
     },
    {
     .x = 0,
     .y = 0,
     .bufferSize = 0x800,
     .baseTile = 0,
     .size = GF_BG_SCR_SIZE_256x256,
     .colorMode = GX_BG_COLORMODE_16,
     .screenBase = GX_BG_SCRBASE_0xe800,
     .charBase = GX_BG_CHARBASE_0x10000,
     .bgExtPltt = GX_BG_EXTPLTT_01,
     .priority = 2,
     .areaOver = GX_BG_AREAOVER_XLU,
     .mosaic = FALSE,
     },
    {
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
     },
};

static const GraphicsModes sWiFiPlazaSurveyGraphicsModes = {
    .dispMode = GX_DISPMODE_GRAPHICS,
    .bgMode = GX_BGMODE_0,
    .subMode = GX_BGMODE_0,
    ._2d3dMode = GX_BG0_AS_2D,
};

void WiFiPlazaSurvey_InitBgs(WiFiPlazaSurveyWork *work, Options *options, enum HeapID heapID) {
    int i;
    SetBothScreensModesAndDisable(&sWiFiPlazaSurveyGraphicsModes);
    work->bgConfig = BgConfig_Alloc(heapID);
    gSystem.screensFlipped = FALSE;
    GfGfx_SwapDisplay();
    for (i = 0; i < 4; i++) {
        InitBgFromTemplate(work->bgConfig, sWiFiPlazaSurveyBgLayers[i], &sWiFiPlazaSurveyBgTemplates[i], GF_BG_TYPE_TEXT);
        BG_ClearCharDataRange(sWiFiPlazaSurveyBgLayers[i], 32, 0, heapID);
        BgClearTilemapBufferAndCommit(work->bgConfig, sWiFiPlazaSurveyBgLayers[i]);
    }
    LoadFontPal1(GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_4_OFFSET, heapID);
    LoadUserFrameGfx2(work->bgConfig, GF_BG_LYR_MAIN_1, 1, 5, Options_GetFrame(options), heapID);
    LoadUserFrameGfx1(work->bgConfig, GF_BG_LYR_MAIN_1, 31, 6, 0, heapID);
    GfGfxLoader_LoadCharData(NARC_a_1_9_6, 2, work->bgConfig, GF_BG_LYR_SUB_0, 0, 0, FALSE, heapID);
    GfGfxLoader_LoadScrnData(NARC_a_1_9_6, 6, work->bgConfig, GF_BG_LYR_SUB_0, 0, 0, FALSE, heapID);
    GfGfxLoader_GXLoadPal(NARC_a_1_9_6, 0, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_0_OFFSET, 0, heapID);
}

void WiFiPlazaSurvey_FreeBgs(WiFiPlazaSurveyWork *work) {
    int i;
    const u32 *layer = sWiFiPlazaSurveyBgLayers;
    for (i = 0; i < 4; i++, layer++) {
        FreeBgTilemapBuffer(work->bgConfig, (u8)*layer);
    }
    Heap_Free(work->bgConfig);
}

static const ObjCharTransferTemplate sWiFiPlazaSurveyObjCharTransferTemplate = {
    .maxTasks = 32,
    .sizeMain = 0x4000,
    .sizeSub = 0,
    .heapID = HEAP_ID_129,
};

void WiFiPlazaSurvey_InitSpriteSystem(WiFiPlazaSurveyWork *work, enum HeapID heapID) {
    int i;
    NNS_G2dInitOamManagerModule();
    OamManager_Create(0, 126, 0, 31, 0, 126, 0, 31, heapID);
    ObjCharTransfer_InitEx((ObjCharTransferTemplate *)&sWiFiPlazaSurveyObjCharTransferTemplate, GX_OBJVRAMMODE_CHAR_1D_32K, GX_OBJVRAMMODE_CHAR_1D_32K);
    ObjPlttTransfer_Init(32, heapID);
    ObjCharTransfer_ClearBuffers();
    ObjPlttTransfer_Reset();
    G2dRenderer_SetObjCharTransferReservedRegion(NNS_G2D_VRAM_TYPE_2DMAIN, GX_OBJVRAMMODE_CHAR_1D_32K);
    G2dRenderer_SetPlttTransferReservedRegion(NNS_G2D_VRAM_TYPE_2DMAIN);
    work->spriteList = G2dRenderer_Init(32, &work->renderer, heapID);
    for (i = 0; i < 4; i++) {
        work->resMans[i] = Create2DGfxResObjMan(32, (GfGfxResType)i, heapID);
    }
    WirelessStatusIcon_Create();
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
}

void WiFiPlazaSurvey_FreeSpriteSystem(WiFiPlazaSurveyWork *work) {
    int i;
    SpriteList_Delete(work->spriteList);
    for (i = 0; i < 4; i++) {
        Destroy2DGfxResObjMan(work->resMans[i]);
    }
    ObjCharTransfer_Destroy();
    ObjPlttTransfer_Destroy();
    OamManager_Free();
}

static const int sWiFiPlazaSurveyMsgBanks[WIFI_PLAZA_SURVEY_NUM_MSG_BANKS] = {
    NARC_msg_msg_0777_bin,
    NARC_msg_msg_0761_bin,
    NARC_msg_msg_0765_bin,
    NARC_msg_msg_0758_bin,
    NARC_msg_msg_0759_bin,
    NARC_msg_msg_0760_bin,
    NARC_msg_msg_0762_bin,
    NARC_msg_msg_0763_bin,
    NARC_msg_msg_0764_bin,
};

void WiFiPlazaSurvey_InitMessages(WiFiPlazaSurveyMessages *messages, struct WiFiPlazaWork *lobby, enum HeapID heapID) {
    int i;
    messages->lobby = lobby;
    for (i = 0; i < WIFI_PLAZA_SURVEY_NUM_MSG_BANKS; i++) {
        messages->msgData[i] = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, sWiFiPlazaSurveyMsgBanks[i], heapID);
    }
    messages->format = MessageFormat_New(heapID);
    messages->expanded = String_New(256, heapID);
    messages->raw = String_New(256, heapID);
}

void WiFiPlazaSurvey_FreeMessages(WiFiPlazaSurveyMessages *messages) {
    int i;
    for (i = 0; i < WIFI_PLAZA_SURVEY_NUM_MSG_BANKS; i++) {
        DestroyMsgData(messages->msgData[i]);
    }
    MessageFormat_Delete(messages->format);
    String_Delete(messages->expanded);
    String_Delete(messages->raw);
}

String *WiFiPlazaSurvey_GetMessage(WiFiPlazaSurveyMessages *messages, int bank, int msgId) {
    GF_ASSERT((u32)bank < WIFI_PLAZA_SURVEY_NUM_MSG_BANKS);
    ReadMsgDataIntoString(messages->msgData[bank], msgId, messages->raw);
    StringExpandPlaceholders(messages->format, messages->expanded, messages->raw);
    return messages->expanded;
}

// Questions 0-49 are in msg_0761 (bank 1), the special questions 50-59 in msg_0765 (bank 2).
String *WiFiPlazaSurvey_GetQuestionTextFromMsg(WiFiPlazaSurveyMessages *messages, u32 questionNo) {
    if (questionNo >= 60) {
        GF_ASSERT(questionNo >= 60);
        questionNo = 0;
    }
    if (questionNo < 50) {
        return WiFiPlazaSurvey_GetMessage(messages, 1, questionNo);
    }
    return WiFiPlazaSurvey_GetMessage(messages, 2, questionNo - 50);
}

// Answers A-C of questions 0-49 are in msg_0758-0760 (banks 3-5), those of 50-59 in msg_0762-0764 (banks 6-8).
String *WiFiPlazaSurvey_GetAnswerTextFromMsg(WiFiPlazaSurveyMessages *messages, u32 questionNo, int answer) {
    if (questionNo >= 60) {
        GF_ASSERT(questionNo >= 60);
        questionNo = 0;
    }
    if (questionNo < 50) {
        return WiFiPlazaSurvey_GetMessage(messages, answer + 3, questionNo);
    }
    return WiFiPlazaSurvey_GetMessage(messages, answer + 6, questionNo - 50);
}

void WiFiPlazaSurvey_BufferPlayerName(WiFiPlazaSurveyMessages *messages, int index, enum HeapID heapID) {
    PlayerProfile *profile = PlayerProfile_New(heapID);
    struct WiFiPlazaPlayer *player = WiFiPlazaWork_GetMyPlayer(messages->lobby);
    WiFiPlazaPlayer_CopyToProfile(player, profile, heapID);
    BufferPlayersName(messages->format, index, profile);
    Heap_Free(profile);
}

void WiFiPlazaSurveyAnswer_Init(WiFiPlazaSurveyModeWork *mode, WiFiPlazaSurveyMessages *messages, WiFiPlazaSurveyWork *work, WiFiPlazaSurveyArgs *args, enum HeapID heapID) {
    WiFiPlazaSurveyAnswer_Setup(&mode->answer, messages, work, heapID);
}

BOOL WiFiPlazaSurveyAnswer_Main(WiFiPlazaSurveyModeWork *mode, WiFiPlazaSurveyMessages *messages, WiFiPlazaSurveyWork *work, WiFiPlazaSurveyMessageBox *msgBox, WiFiPlazaSurveyArgs *args, enum HeapID heapID) {
    return WiFiPlazaSurveyAnswer_Run(&mode->answer, messages, work, msgBox, args, heapID);
}

void WiFiPlazaSurveyAnswer_Exit(WiFiPlazaSurveyModeWork *mode, WiFiPlazaSurveyMessages *messages, WiFiPlazaSurveyWork *work) {
    WiFiPlazaSurveyAnswer_Teardown(&mode->answer, messages, work);
}

static const WindowTemplate sWiFiPlazaSurveyAnswerWindowTemplate = {
    .bgId = GF_BG_LYR_MAIN_2,
    .left = 11,
    .top = 7,
    .width = 15,
    .height = 8,
    .palette = 3,
    .baseTile = 1,
};

void WiFiPlazaSurveyAnswer_Setup(WiFiPlazaSurveyAnswer *answer, WiFiPlazaSurveyMessages *messages, WiFiPlazaSurveyWork *work, enum HeapID heapID) {
    SpriteResourcesHeader header;
    GfGfxLoader_GXLoadPalFromOpenNarc(work->narc, 0xC7, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_0_OFFSET, 0x80, heapID);
    GfGfxLoader_LoadCharDataFromOpenNarc(work->narc, 0xC8, work->bgConfig, GF_BG_LYR_MAIN_0, 0, 0, FALSE, heapID);
    GfGfxLoader_LoadScrnDataFromOpenNarc(work->narc, 0xC9, work->bgConfig, GF_BG_LYR_MAIN_0, 0, 0, FALSE, heapID);
    answer->resources[GF_GFX_RES_TYPE_CHAR] = AddCharResObjFromOpenNarc(work->resMans[GF_GFX_RES_TYPE_CHAR], work->narc, 0xCD, FALSE, 20, 1, heapID);
    answer->resources[GF_GFX_RES_TYPE_PLTT] = AddPlttResObjFromOpenNarc(work->resMans[GF_GFX_RES_TYPE_PLTT], work->narc, 0xCC, FALSE, 20, 1, 3, heapID);
    GF_ASSERT(SpriteTransfer_CreateCharTransferTask_AllocAtEnd(answer->resources[GF_GFX_RES_TYPE_CHAR]));
    GF_ASSERT(SpriteTransfer_CreatePlttTransferTask(answer->resources[GF_GFX_RES_TYPE_PLTT]));
    GF2DGfxResObj_FreeData(answer->resources[GF_GFX_RES_TYPE_CHAR]);
    GF2DGfxResObj_FreeData(answer->resources[GF_GFX_RES_TYPE_PLTT]);
    answer->resources[GF_GFX_RES_TYPE_CELL] = AddCellOrAnimResObjFromOpenNarc(work->resMans[GF_GFX_RES_TYPE_CELL], work->narc, 0xCE, FALSE, 20, GF_GFX_RES_TYPE_CELL, heapID);
    answer->resources[GF_GFX_RES_TYPE_ANIM] = AddCellOrAnimResObjFromOpenNarc(work->resMans[GF_GFX_RES_TYPE_ANIM], work->narc, 0xCF, FALSE, 20, GF_GFX_RES_TYPE_ANIM, heapID);
    {
        SimpleSpriteTemplate template = { 0 };
        CreateSpriteResourcesHeader(&header, 20, 20, 20, 20, -1, -1, 0, 0, work->resMans[GF_GFX_RES_TYPE_CHAR], work->resMans[GF_GFX_RES_TYPE_PLTT], work->resMans[GF_GFX_RES_TYPE_CELL], work->resMans[GF_GFX_RES_TYPE_ANIM], NULL, NULL);
        template.spriteList = work->spriteList;
        template.header = &header;
        template.priority = 0;
        template.whichScreen = NNS_G2D_VRAM_TYPE_2DMAIN;
        template.heapID = heapID;
        answer->cursorSprite = Sprite_Create(&template);
        Sprite_SetDrawFlag(answer->cursorSprite, FALSE);
        AddWindow(work->bgConfig, &answer->window, &sWiFiPlazaSurveyAnswerWindowTemplate);
        WiFiPlazaSurvey_InitQuestion(&answer->question, TRUE, heapID);
    }
}

BOOL WiFiPlazaSurveyAnswer_Run(WiFiPlazaSurveyAnswer *answer, WiFiPlazaSurveyMessages *messages, WiFiPlazaSurveyWork *work, WiFiPlazaSurveyMessageBox *msgBox, WiFiPlazaSurveyArgs *args, enum HeapID heapID) {
    switch (answer->state) {
    case WIFI_PLAZA_SURVEY_ANSWER_PRINT_INTRO:
        WiFiPlazaSurvey_PrintMessage(msgBox, WiFiPlazaSurvey_GetMessage(messages, 0, msg_0777_00094));
        answer->nextState = WIFI_PLAZA_SURVEY_ANSWER_OPEN_YES_NO;
        answer->state = WIFI_PLAZA_SURVEY_ANSWER_WAIT_MESSAGE;
        break;
    case WIFI_PLAZA_SURVEY_ANSWER_OPEN_YES_NO:
        WiFiPlazaSurvey_CreateYesNoMenu(msgBox, work, heapID);
        answer->state = WIFI_PLAZA_SURVEY_ANSWER_HANDLE_YES_NO;
        break;
    case WIFI_PLAZA_SURVEY_ANSWER_HANDLE_YES_NO:
        switch (WiFiPlazaSurvey_HandleYesNoMenu(msgBox, heapID)) {
        case 0:
            answer->state = WIFI_PLAZA_SURVEY_ANSWER_PRINT_QUESTION_INTRO;
            break;
        case LIST_CANCEL:
            answer->state = WIFI_PLAZA_SURVEY_ANSWER_FINISH;
            break;
        }
        break;
    case WIFI_PLAZA_SURVEY_ANSWER_PRINT_QUESTION_INTRO:
        WiFiPlazaSurvey_PrintMessage(msgBox, WiFiPlazaSurvey_GetMessage(messages, 0, answer->question.isSpecial == FALSE ? msg_0777_00092 : msg_0777_00093));
        answer->nextState = WIFI_PLAZA_SURVEY_ANSWER_SHOW_ANSWERS;
        answer->state = WIFI_PLAZA_SURVEY_ANSWER_WAIT_MESSAGE;
        break;
    case WIFI_PLAZA_SURVEY_ANSWER_SHOW_ANSWERS:
        WiFiPlazaSurveyAnswer_PrintAnswers(answer, messages, work);
        WiFiPlazaSurvey_PrintMessageInstant(msgBox, WiFiPlazaSurvey_GetQuestionText(&answer->question, messages));
        WiFiPlazaSurveyAnswer_UpdateCursor(answer);
        WiFiPlazaSurveyAnswer_StartCursorAnim(answer);
        answer->state = WIFI_PLAZA_SURVEY_ANSWER_SELECT;
        break;
    case WIFI_PLAZA_SURVEY_ANSWER_SELECT:
        if (gSystem.newKeys & PAD_BUTTON_A) {
            PlaySE(SEQ_SE_DP_SELECT);
            answer->state++;
            break;
        } else if (gSystem.newKeys & PAD_KEY_UP) {
            if ((int)answer->cursor - 1 >= 0) {
                PlaySE(SEQ_SE_DP_SELECT78);
                answer->cursor--;
            }
        } else if (gSystem.newKeys & PAD_KEY_DOWN) {
            if ((int)answer->cursor + 1 < WIFI_PLAZA_SURVEY_NUM_ANSWERS) {
                PlaySE(SEQ_SE_DP_SELECT78);
                answer->cursor++;
            }
        }
        WiFiPlazaSurveyAnswer_UpdateCursor(answer);
        break;
    case WIFI_PLAZA_SURVEY_ANSWER_SUBMIT:
        WiFiPlazaWork_SetSurveyAnswer(args->lobby, answer->question.surveyId, answer->cursor);
        WiFiPlazaLobbyClient_SubmitSurveyAnswer(answer->cursor);
        WiFiPlazaSurvey_PrintMessageInstant(msgBox, WiFiPlazaSurvey_GetMessage(messages, 0, msg_0777_00096));
        WiFiPlazaSurvey_StartWaitingIcon(msgBox);
        PlaySE(SEQ_SE_PL_BREC80);
        answer->state = WIFI_PLAZA_SURVEY_ANSWER_WAIT_SUBMIT;
        break;
    case WIFI_PLAZA_SURVEY_ANSWER_WAIT_SUBMIT:
        if (WiFiPlazaLobbyClient_GetSurveySubmitState() == 1) {
            break;
        }
        PlaySE(SEQ_SE_DP_BUTTON9);
        WiFiPlazaSurvey_StopWaitingIcon(msgBox);
        WiFiPlazaSurvey_PrintMessage(msgBox, WiFiPlazaSurvey_GetMessage(messages, 0, msg_0777_00097));
        answer->nextState = WIFI_PLAZA_SURVEY_ANSWER_PRINT_THANKS;
        answer->state = WIFI_PLAZA_SURVEY_ANSWER_WAIT_MESSAGE;
        break;
    case WIFI_PLAZA_SURVEY_ANSWER_PRINT_THANKS:
        WiFiPlazaSurvey_PrintMessage(msgBox, WiFiPlazaSurvey_GetMessage(messages, 0, answer->question.isSpecial == FALSE ? msg_0777_00098 : msg_0777_00099));
        answer->nextState = WIFI_PLAZA_SURVEY_ANSWER_FINISH;
        answer->state = WIFI_PLAZA_SURVEY_ANSWER_WAIT_MESSAGE;
        break;
    case WIFI_PLAZA_SURVEY_ANSWER_FINISH:
        WiFiPlazaSurvey_ClearMessageBox(msgBox);
        return TRUE;
    case WIFI_PLAZA_SURVEY_ANSWER_WAIT_MESSAGE:
        if (WiFiPlazaSurvey_IsMessageFinished(msgBox) == TRUE) {
            answer->state = answer->nextState;
        }
        break;
    default:
        GF_ASSERT(FALSE);
        break;
    }
    return FALSE;
}

void WiFiPlazaSurveyAnswer_Teardown(WiFiPlazaSurveyAnswer *answer, WiFiPlazaSurveyMessages *messages, WiFiPlazaSurveyWork *work) {
    int i;
    WiFiPlazaSurvey_FreeQuestion(&answer->question);
    RemoveWindow(&answer->window);
    Sprite_Delete(answer->cursorSprite);
    SpriteTransfer_DeleteCharTransferTask(answer->resources[GF_GFX_RES_TYPE_CHAR]);
    SpriteTransfer_DeletePlttTransferTask(answer->resources[GF_GFX_RES_TYPE_PLTT]);
    for (i = 0; i < 4; i++) {
        DestroySingle2DGfxResObj(work->resMans[i], answer->resources[i]);
    }
}

void WiFiPlazaSurveyAnswer_PrintAnswers(WiFiPlazaSurveyAnswer *answer, WiFiPlazaSurveyMessages *messages, WiFiPlazaSurveyWork *work) {
    String *string;
    FillWindowPixelBuffer(&answer->window, 0);
    string = WiFiPlazaSurvey_GetAnswerText(&answer->question, messages, 0);
    AddTextPrinterParameterizedWithColor(&answer->window, 1, string, 0, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 0), NULL);
    string = WiFiPlazaSurvey_GetAnswerText(&answer->question, messages, 1);
    AddTextPrinterParameterizedWithColor(&answer->window, 1, string, 0, 24, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 0), NULL);
    string = WiFiPlazaSurvey_GetAnswerText(&answer->question, messages, 2);
    AddTextPrinterParameterizedWithColor(&answer->window, 1, string, 0, 48, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 0), NULL);
    ScheduleWindowCopyToVram(&answer->window);
}

static const VecFx32 sWiFiPlazaSurveyCursorPositions[WIFI_PLAZA_SURVEY_NUM_ANSWERS] = {
    { FX32_CONST(72), FX32_CONST(64),  0 },
    { FX32_CONST(72), FX32_CONST(88),  0 },
    { FX32_CONST(72), FX32_CONST(112), 0 },
};

void WiFiPlazaSurveyAnswer_UpdateCursor(WiFiPlazaSurveyAnswer *answer) {
    Sprite_SetDrawFlag(answer->cursorSprite, TRUE);
    Sprite_SetMatrix(answer->cursorSprite, (VecFx32 *)&sWiFiPlazaSurveyCursorPositions[answer->cursor]);
}

void WiFiPlazaSurveyAnswer_StartCursorAnim(WiFiPlazaSurveyAnswer *answer) {
    Sprite_SetAnimActiveFlag(answer->cursorSprite, TRUE);
}

void WiFiPlazaSurvey_InitMessageBox(WiFiPlazaSurveyMessageBox *msgBox, WiFiPlazaSurveyWork *work, SaveData *saveData, enum HeapID heapID) {
    AddWindowParameterized(work->bgConfig, &msgBox->window, GF_BG_LYR_MAIN_1, 2, 19, 27, 4, 4, 0x28);
    FillWindowPixelBuffer(&msgBox->window, 15);
    msgBox->string = String_New(256, heapID);
    msgBox->textSpeed = Options_GetTextFrameDelay(Save_PlayerData_GetOptionsAddr(saveData));
}

void WiFiPlazaSurvey_FreeMessageBox(WiFiPlazaSurveyMessageBox *msgBox, enum HeapID heapID) {
    if (TextPrinterCheckActive(msgBox->printerId)) {
        RemoveTextPrinter(msgBox->printerId);
    }
    WiFiPlazaSurvey_StopWaitingIcon(msgBox);
    String_Delete(msgBox->string);
    RemoveWindow(&msgBox->window);
    WiFiPlazaSurvey_DestroyYesNoMenu(msgBox, heapID);
}

void WiFiPlazaSurvey_PrintMessage(WiFiPlazaSurveyMessageBox *msgBox, String *string) {
    if (TextPrinterCheckActive(msgBox->printerId)) {
        RemoveTextPrinter(msgBox->printerId);
    }
    FillWindowPixelBuffer(&msgBox->window, 15);
    String_Copy(msgBox->string, string);
    msgBox->printerId = AddTextPrinterParameterizedWithColor(&msgBox->window, 1, msgBox->string, 0, 0, msgBox->textSpeed, MAKE_TEXT_COLOR(1, 2, 15), NULL);
    DrawFrameAndWindow2(&msgBox->window, TRUE, 1, 5);
}

void WiFiPlazaSurvey_PrintMessageInstant(WiFiPlazaSurveyMessageBox *msgBox, String *string) {
    if (TextPrinterCheckActive(msgBox->printerId)) {
        RemoveTextPrinter(msgBox->printerId);
    }
    FillWindowPixelBuffer(&msgBox->window, 15);
    String_Copy(msgBox->string, string);
    AddTextPrinterParameterizedWithColor(&msgBox->window, 1, msgBox->string, 0, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 15), NULL);
    DrawFrameAndWindow2(&msgBox->window, TRUE, 1, 5);
    ScheduleWindowCopyToVram(&msgBox->window);
}

void WiFiPlazaSurvey_StartWaitingIcon(WiFiPlazaSurveyMessageBox *msgBox) {
    GF_ASSERT(msgBox->waitingIcon == NULL);
    msgBox->waitingIcon = WaitingIcon_New(&msgBox->window, 1);
}

void WiFiPlazaSurvey_StopWaitingIcon(WiFiPlazaSurveyMessageBox *msgBox) {
    if (msgBox->waitingIcon) {
        WaitingIcon_Delete(msgBox->waitingIcon);
        msgBox->waitingIcon = NULL;
    }
}

BOOL WiFiPlazaSurvey_IsMessageFinished(WiFiPlazaSurveyMessageBox *msgBox) {
    return !TextPrinterCheckActive(msgBox->printerId);
}

void WiFiPlazaSurvey_ClearMessageBox(WiFiPlazaSurveyMessageBox *msgBox) {
    if (TextPrinterCheckActive(msgBox->printerId)) {
        RemoveTextPrinter(msgBox->printerId);
    }
    WiFiPlazaSurvey_StopWaitingIcon(msgBox);
    ClearFrameAndWindow2(&msgBox->window, TRUE);
    ClearWindowTilemapAndScheduleTransfer(&msgBox->window);
}

static const WindowTemplate sWiFiPlazaSurveyYesNoTemplate = {
    .bgId = GF_BG_LYR_MAIN_1,
    .left = 25,
    .top = 13,
    .width = 6,
    .height = 4,
    .palette = 4,
    .baseTile = 0x94,
};

void WiFiPlazaSurvey_CreateYesNoMenu(WiFiPlazaSurveyMessageBox *msgBox, WiFiPlazaSurveyWork *work, enum HeapID heapID) {
    GF_ASSERT(msgBox->yesNoMenu == NULL);
    msgBox->yesNoMenu = Std_CreateYesNoMenu(work->bgConfig, &sWiFiPlazaSurveyYesNoTemplate, 31, 6, heapID);
}

u32 WiFiPlazaSurvey_HandleYesNoMenu(WiFiPlazaSurveyMessageBox *msgBox, enum HeapID heapID) {
    int result = Handle2dMenuInput_DeleteOnFinish(msgBox->yesNoMenu, heapID);
    if (result != LIST_NOTHING_CHOSEN) {
        msgBox->yesNoMenu = NULL;
    }
    return result;
}

void WiFiPlazaSurvey_DestroyYesNoMenu(WiFiPlazaSurveyMessageBox *msgBox, enum HeapID heapID) {
    if (msgBox->yesNoMenu) {
        Clear2dMenuWindowAndDelete(msgBox->yesNoMenu, heapID);
        msgBox->yesNoMenu = NULL;
    }
}

void WiFiPlazaSurveyResults_Init(WiFiPlazaSurveyModeWork *mode, WiFiPlazaSurveyMessages *messages, WiFiPlazaSurveyWork *work, WiFiPlazaSurveyArgs *args, enum HeapID heapID) {
    WiFiPlazaSurveyResults_Setup(&mode->results, messages, work, args, heapID);
}

BOOL WiFiPlazaSurveyResults_Main(WiFiPlazaSurveyModeWork *mode, WiFiPlazaSurveyMessages *messages, WiFiPlazaSurveyWork *work, WiFiPlazaSurveyMessageBox *msgBox, WiFiPlazaSurveyArgs *args, enum HeapID heapID) {
    return WiFiPlazaSurveyResults_Run(&mode->results, messages, work, msgBox, args, heapID);
}

void WiFiPlazaSurveyResults_Exit(WiFiPlazaSurveyModeWork *mode, WiFiPlazaSurveyMessages *messages, WiFiPlazaSurveyWork *work) {
    WiFiPlazaSurveyResults_Teardown(&mode->results);
}

static const WindowTemplate sWiFiPlazaSurveyResultsWindowTemplates[WIFI_PLAZA_SURVEY_NUM_RESULT_WINDOWS] = {
    {
     .bgId = GF_BG_LYR_MAIN_2,
     .left = 7,
     .top = 1,
     .width = 18,
     .height = 3,
     .palette = 3,
     .baseTile = 1,
     },
    {
     .bgId = GF_BG_LYR_MAIN_2,
     .left = 5,
     .top = 5,
     .width = 12,
     .height = 9,
     .palette = 3,
     .baseTile = 0x37,
     },
    {
     .bgId = GF_BG_LYR_MAIN_2,
     .left = 17,
     .top = 5,
     .width = 11,
     .height = 2,
     .palette = 4,
     .baseTile = 0xA3,
     },
    {
     .bgId = GF_BG_LYR_MAIN_2,
     .left = 17,
     .top = 8,
     .width = 11,
     .height = 2,
     .palette = 4,
     .baseTile = 0xB9,
     },
    {
     .bgId = GF_BG_LYR_MAIN_2,
     .left = 17,
     .top = 11,
     .width = 11,
     .height = 2,
     .palette = 4,
     .baseTile = 0xCF,
     },
    {
     .bgId = GF_BG_LYR_MAIN_2,
     .left = 3,
     .top = 15,
     .width = 25,
     .height = 2,
     .palette = 3,
     .baseTile = 0xE5,
     },
};

void WiFiPlazaSurveyResults_Setup(WiFiPlazaSurveyResults *results, WiFiPlazaSurveyMessages *messages, WiFiPlazaSurveyWork *work, WiFiPlazaSurveyArgs *args, enum HeapID heapID) {
    int i;
    GfGfxLoader_GXLoadPalFromOpenNarc(work->narc, 0xC7, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_0_OFFSET, 0x80, heapID);
    GfGfxLoader_LoadCharDataFromOpenNarc(work->narc, 0xC8, work->bgConfig, GF_BG_LYR_MAIN_0, 0, 0, FALSE, heapID);
    GfGfxLoader_LoadScrnDataFromOpenNarc(work->narc, 0xCA, work->bgConfig, GF_BG_LYR_MAIN_0, 0, 0, FALSE, heapID);
    for (i = 0; i < WIFI_PLAZA_SURVEY_NUM_RESULT_WINDOWS; i++) {
        AddWindow(work->bgConfig, &results->windows[i], &sWiFiPlazaSurveyResultsWindowTemplates[i]);
    }
    WiFiPlazaSurveyResults_PrintTitle(&results->windows[0], messages, msg_0777_00109, MAKE_TEXT_COLOR(3, 4, 0));
    results->barCharFile = GfGfxLoader_GetCharDataFromOpenNarc(work->narc, 0xC8, FALSE, &results->barChar, heapID);
    WiFiPlazaSurvey_InitQuestion(&results->current, TRUE, heapID);
    WiFiPlazaSurvey_InitQuestion(&results->lastWeek, FALSE, heapID);
    // the lobby caches the tally of this plaza
    if (!WiFiPlazaWork_GetSurveyTally(args->lobby, &results->plazaTally)) {
        WiFiPlazaSurvey_CountPlazaAnswers(&results->plazaTally, args->lobby);
        WiFiPlazaWork_SetSurveyTally(args->lobby, &results->plazaTally);
    }
    WiFiPlazaSurvey_LoadLastWeekTotals(&results->lastWeekTally);
}

BOOL WiFiPlazaSurveyResults_Run(WiFiPlazaSurveyResults *results, WiFiPlazaSurveyMessages *messages, WiFiPlazaSurveyWork *work, WiFiPlazaSurveyMessageBox *msgBox, WiFiPlazaSurveyArgs *args, enum HeapID heapID) {
    WiFiPlazaSurveyAnswerRecord record;
    WiFiPlazaSurveyAnswerRecord savedRecord;
    void *saveBlock;
    switch (results->state) {
    case WIFI_PLAZA_SURVEY_RESULTS_PRINT_INTRO:
        WiFiPlazaSurvey_PrintMessage(msgBox, WiFiPlazaSurvey_GetMessage(messages, 0, results->current.isSpecial == FALSE ? msg_0777_00103 : msg_0777_00105));
        results->nextState = WIFI_PLAZA_SURVEY_RESULTS_PRINT_QUESTION;
        results->state = WIFI_PLAZA_SURVEY_RESULTS_WAIT_MESSAGE;
        break;
    case WIFI_PLAZA_SURVEY_RESULTS_PRINT_QUESTION:
        WiFiPlazaSurvey_PrintMessage(msgBox, WiFiPlazaSurvey_GetQuestionText(&results->current, messages));
        results->nextState = WIFI_PLAZA_SURVEY_RESULTS_WAIT_QUESTION;
        results->state = WIFI_PLAZA_SURVEY_RESULTS_WAIT_MESSAGE;
        break;
    case WIFI_PLAZA_SURVEY_RESULTS_WAIT_QUESTION:
        if (gSystem.newKeys & PAD_BUTTON_A) {
            WiFiPlazaSurvey_PrintMessage(msgBox, WiFiPlazaSurvey_GetMessage(messages, 0, msg_0777_00104));
            results->nextState = WIFI_PLAZA_SURVEY_RESULTS_PRINT_RESULTS_INTRO;
            results->state = WIFI_PLAZA_SURVEY_RESULTS_WAIT_MESSAGE;
        }
        break;
    case WIFI_PLAZA_SURVEY_RESULTS_PRINT_RESULTS_INTRO:
        WiFiPlazaSurvey_PrintMessage(msgBox, WiFiPlazaSurvey_GetMessage(messages, 0, results->current.isSpecial == FALSE ? msg_0777_00107 : msg_0777_00108));
        results->nextState = WIFI_PLAZA_SURVEY_RESULTS_SHOW_RESULTS;
        results->state = WIFI_PLAZA_SURVEY_RESULTS_WAIT_MESSAGE;
        break;
    case WIFI_PLAZA_SURVEY_RESULTS_SHOW_RESULTS:
        WiFiPlazaPlayer_GetSurvey(WiFiPlazaWork_GetMyPlayer(args->lobby), &record);
        WiFiPlazaSurveyResults_PrintQuestion(results, &results->current, messages, work, &record, heapID, MAKE_TEXT_COLOR(9, 10, 0), MAKE_TEXT_COLOR(13, 14, 0), WiFiPlazaWork_IsSpecialVisitor(args->lobby, WiFiPlazaWork_GetOwnSlot(args->lobby)));
        WiFiPlazaSurveyResults_StartBars(results, &results->plazaTally, work, 1);
        PlaySE(SEQ_SE_PL_PINPON2);
        results->state = WIFI_PLAZA_SURVEY_RESULTS_ANIMATE_BARS;
        break;
    case WIFI_PLAZA_SURVEY_RESULTS_ANIMATE_BARS:
        if (WiFiPlazaSurveyResults_UpdateBars(results, work) == TRUE) {
            results->state = WIFI_PLAZA_SURVEY_RESULTS_WAIT_RESULTS;
        }
        break;
    case WIFI_PLAZA_SURVEY_RESULTS_WAIT_RESULTS:
        if (gSystem.newKeys & PAD_BUTTON_A) {
            if (results->lastWeek.questionNo != -1) {
                results->state = WIFI_PLAZA_SURVEY_RESULTS_ASK_LAST_WEEK;
            } else {
                WiFiPlazaSurvey_PrintMessage(msgBox, WiFiPlazaSurvey_GetMessage(messages, 0, msg_0777_00112));
                results->nextState = WIFI_PLAZA_SURVEY_RESULTS_PRINT_CLOSING;
                results->state = WIFI_PLAZA_SURVEY_RESULTS_WAIT_MESSAGE;
            }
        }
        break;
    case WIFI_PLAZA_SURVEY_RESULTS_ASK_LAST_WEEK:
        WiFiPlazaSurvey_PrintMessageInstant(msgBox, WiFiPlazaSurvey_GetMessage(messages, 0, msg_0777_00111));
        WiFiPlazaSurvey_CreateYesNoMenu(msgBox, work, heapID);
        results->state = WIFI_PLAZA_SURVEY_RESULTS_HANDLE_YES_NO;
        break;
    case WIFI_PLAZA_SURVEY_RESULTS_HANDLE_YES_NO:
        switch (WiFiPlazaSurvey_HandleYesNoMenu(msgBox, heapID)) {
        case 0:
            results->state = WIFI_PLAZA_SURVEY_RESULTS_FADE_OUT;
            break;
        case LIST_CANCEL:
            results->state = WIFI_PLAZA_SURVEY_RESULTS_PRINT_CLOSING;
            break;
        }
        break;
    case WIFI_PLAZA_SURVEY_RESULTS_FADE_OUT:
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, RGB_BLACK, 6, 1, heapID);
        results->state = WIFI_PLAZA_SURVEY_RESULTS_SHOW_LAST_WEEK;
        break;
    case WIFI_PLAZA_SURVEY_RESULTS_SHOW_LAST_WEEK:
        if (IsPaletteFadeFinished() == TRUE) {
            WiFiPlazaSurveyResults_ShowLastWeek(results, messages, work, heapID);
            results->state = WIFI_PLAZA_SURVEY_RESULTS_FADE_IN;
        }
        break;
    case WIFI_PLAZA_SURVEY_RESULTS_FADE_IN:
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, RGB_BLACK, 6, 1, heapID);
        results->state = WIFI_PLAZA_SURVEY_RESULTS_WAIT_FADE_IN;
        break;
    case WIFI_PLAZA_SURVEY_RESULTS_WAIT_FADE_IN:
        if (IsPaletteFadeFinished() == TRUE) {
            results->state = WIFI_PLAZA_SURVEY_RESULTS_PRINT_LAST_WEEK_INTRO;
        }
        break;
    case WIFI_PLAZA_SURVEY_RESULTS_PRINT_LAST_WEEK_INTRO:
        WiFiPlazaSurvey_PrintMessage(msgBox, WiFiPlazaSurvey_GetMessage(messages, 0, results->lastWeek.isSpecial == FALSE ? msg_0777_00113 : msg_0777_00115));
        results->nextState = WIFI_PLAZA_SURVEY_RESULTS_PRINT_LAST_WEEK_QUESTION;
        results->state = WIFI_PLAZA_SURVEY_RESULTS_WAIT_MESSAGE;
        break;
    case WIFI_PLAZA_SURVEY_RESULTS_PRINT_LAST_WEEK_QUESTION:
        WiFiPlazaSurvey_PrintMessage(msgBox, WiFiPlazaSurvey_GetQuestionText(&results->lastWeek, messages));
        results->nextState = WIFI_PLAZA_SURVEY_RESULTS_WAIT_LAST_WEEK_QUESTION;
        results->state = WIFI_PLAZA_SURVEY_RESULTS_WAIT_MESSAGE;
        break;
    case WIFI_PLAZA_SURVEY_RESULTS_WAIT_LAST_WEEK_QUESTION:
        if (gSystem.newKeys & PAD_BUTTON_A) {
            WiFiPlazaSurvey_PrintMessage(msgBox, WiFiPlazaSurvey_GetMessage(messages, 0, msg_0777_00114));
            results->nextState = WIFI_PLAZA_SURVEY_RESULTS_PRINT_LAST_WEEK_RESULTS_INTRO;
            results->state = WIFI_PLAZA_SURVEY_RESULTS_WAIT_MESSAGE;
        }
        break;
    case WIFI_PLAZA_SURVEY_RESULTS_PRINT_LAST_WEEK_RESULTS_INTRO:
        WiFiPlazaSurvey_PrintMessage(msgBox, WiFiPlazaSurvey_GetMessage(messages, 0, results->lastWeek.isSpecial == FALSE ? msg_0777_00117 : msg_0777_00118));
        results->nextState = WIFI_PLAZA_SURVEY_RESULTS_SHOW_LAST_WEEK_RESULTS;
        results->state = WIFI_PLAZA_SURVEY_RESULTS_WAIT_MESSAGE;
        break;
    case WIFI_PLAZA_SURVEY_RESULTS_SHOW_LAST_WEEK_RESULTS:
        saveBlock = Save_PlazaSurvey_Get(args->saveData);
        savedRecord.surveyId = PlazaSurvey_GetSurveyId(saveBlock);
        savedRecord.answer = PlazaSurvey_GetAnswer(saveBlock);
        WiFiPlazaSurveyResults_PrintQuestion(results, &results->lastWeek, messages, work, &savedRecord, heapID, MAKE_TEXT_COLOR(11, 12, 0), MAKE_TEXT_COLOR(15, 14, 0), WiFiPlazaWork_IsSpecialVisitor(args->lobby, WiFiPlazaWork_GetOwnSlot(args->lobby)));
        WiFiPlazaSurveyResults_StartBars(results, &results->lastWeekTally, work, 2);
        PlaySE(SEQ_SE_PL_PINPON2);
        results->state = WIFI_PLAZA_SURVEY_RESULTS_ANIMATE_LAST_WEEK_BARS;
        break;
    case WIFI_PLAZA_SURVEY_RESULTS_ANIMATE_LAST_WEEK_BARS:
        if (WiFiPlazaSurveyResults_UpdateBars(results, work) == TRUE) {
            results->state = WIFI_PLAZA_SURVEY_RESULTS_WAIT_LAST_WEEK_RESULTS;
        }
        break;
    case WIFI_PLAZA_SURVEY_RESULTS_WAIT_LAST_WEEK_RESULTS:
        if (gSystem.newKeys & PAD_BUTTON_A) {
            results->state = WIFI_PLAZA_SURVEY_RESULTS_PRINT_CLOSING;
        }
        break;
    case WIFI_PLAZA_SURVEY_RESULTS_PRINT_CLOSING:
        WiFiPlazaSurvey_PrintMessage(msgBox, WiFiPlazaSurvey_GetMessage(messages, 0, msg_0777_00120));
        results->nextState = WIFI_PLAZA_SURVEY_RESULTS_FINISH;
        results->state = WIFI_PLAZA_SURVEY_RESULTS_WAIT_MESSAGE;
        break;
    case WIFI_PLAZA_SURVEY_RESULTS_FINISH:
        WiFiPlazaSurvey_ClearMessageBox(msgBox);
        return TRUE;
    case WIFI_PLAZA_SURVEY_RESULTS_WAIT_MESSAGE:
        if (WiFiPlazaSurvey_IsMessageFinished(msgBox) == TRUE) {
            results->state = results->nextState;
        }
        break;
    default:
        GF_ASSERT(FALSE);
        break;
    }
    return FALSE;
}

void WiFiPlazaSurveyResults_Teardown(WiFiPlazaSurveyResults *results) {
    int i;
    WiFiPlazaSurvey_FreeQuestion(&results->current);
    WiFiPlazaSurvey_FreeQuestion(&results->lastWeek);
    Heap_Free(results->barCharFile);
    results->barCharFile = NULL;
    for (i = 0; i < WIFI_PLAZA_SURVEY_NUM_RESULT_WINDOWS; i++) {
        RemoveWindow(&results->windows[i]);
    }
}

void WiFiPlazaSurveyResults_PrintTitle(Window *window, WiFiPlazaSurveyMessages *messages, int msgId, u32 color) {
    String *string;
    u32 width;
    FillWindowPixelBuffer(window, 0);
    string = WiFiPlazaSurvey_GetMessage(messages, 0, msgId);
    width = FontID_String_GetWidth(1, string, 0);
    AddTextPrinterParameterizedWithColor(window, 1, string, (0x90 - width) / 2, 4, TEXT_SPEED_NOTRANSFER, color, NULL);
    ScheduleWindowCopyToVram(window);
}

// Prints the question's answers and, when the player answered this survey, the player's name and answer.
void WiFiPlazaSurveyResults_PrintQuestion(WiFiPlazaSurveyResults *results, WiFiPlazaSurveyQuestion *question, WiFiPlazaSurveyMessages *messages, WiFiPlazaSurveyWork *work, WiFiPlazaSurveyAnswerRecord *record, enum HeapID heapID, u32 color, u32 highlightColor, BOOL highlight) {
    String *string;
    int width;
    u32 nameColor;
    FillWindowPixelBuffer(&results->windows[1], 0);
    FillWindowPixelBuffer(&results->windows[5], 0);
    string = WiFiPlazaSurvey_GetAnswerText(question, messages, 0);
    AddTextPrinterParameterizedWithColor(&results->windows[1], 1, string, 0, 0, TEXT_SPEED_NOTRANSFER, color, NULL);
    string = WiFiPlazaSurvey_GetAnswerText(question, messages, 1);
    AddTextPrinterParameterizedWithColor(&results->windows[1], 1, string, 0, 24, TEXT_SPEED_NOTRANSFER, color, NULL);
    string = WiFiPlazaSurvey_GetAnswerText(question, messages, 2);
    AddTextPrinterParameterizedWithColor(&results->windows[1], 1, string, 0, 48, TEXT_SPEED_NOTRANSFER, color, NULL);
    if (record->surveyId == question->surveyId && record->answer < WIFI_PLAZA_SURVEY_NUM_ANSWERS) {
        nameColor = highlight == TRUE ? highlightColor : color;
        WiFiPlazaSurvey_BufferPlayerName(messages, 0, heapID);
        string = WiFiPlazaSurvey_GetMessage(messages, 0, msg_0777_00110);
        width = FontID_String_GetWidth(1, string, 0);
        AddTextPrinterParameterizedWithColor(&results->windows[5], 1, string, (96 - width) / 2, 0, TEXT_SPEED_NOTRANSFER, nameColor, NULL);
        string = WiFiPlazaSurvey_GetAnswerText(question, messages, record->answer);
        width = FontID_String_GetWidth(1, string, 0);
        AddTextPrinterParameterizedWithColor(&results->windows[5], 1, string, (104 - width) / 2 + 96, 0, TEXT_SPEED_NOTRANSFER, color, NULL);
    }
    ScheduleWindowCopyToVram(&results->windows[5]);
    ScheduleWindowCopyToVram(&results->windows[1]);
}

void WiFiPlazaSurveyResults_StartBars(WiFiPlazaSurveyResults *results, WiFiPlazaSurveyTally *tally, WiFiPlazaSurveyWork *work, int palette) {
    int i;
    results->barFrame = 0;
    for (i = 0; i < WIFI_PLAZA_SURVEY_NUM_ANSWERS; i++) {
        results->barLength[i] = 0;
        results->barTarget[i] = WiFiPlazaSurvey_GetBarLength(tally, i, WIFI_PLAZA_SURVEY_BAR_LENGTH);
        SetWindowPaletteNum(&results->windows[i + 2], palette);
        FillWindowPixelBuffer(&results->windows[i + 2], 0);
    }
}

// Grows the three bars to their lengths over 20 frames, one pixel column at a time.
BOOL WiFiPlazaSurveyResults_UpdateBars(WiFiPlazaSurveyResults *results, WiFiPlazaSurveyWork *work) {
    int limit;
    int i;
    int finished;
    int len;
    int start;
    int j;
    int target;
    if (results->barFrame >= WIFI_PLAZA_SURVEY_BAR_FRAMES) {
        return TRUE;
    }
    results->barFrame++;
    limit = WIFI_PLAZA_SURVEY_BAR_LENGTH * results->barFrame / WIFI_PLAZA_SURVEY_BAR_FRAMES;
    finished = 0;
    for (i = 0; i < WIFI_PLAZA_SURVEY_NUM_ANSWERS; i++) {
        target = results->barTarget[i];
        start = results->barLength[i];
        if (start == target) {
            finished++;
            continue;
        }
        if (target >= limit) {
            len = limit - start;
            results->barLength[i] = limit;
        } else {
            len = target - start;
            results->barLength[i] = target;
        }
        for (j = 0; j < len; j++) {
            BlitBitmapRectToWindow(&results->windows[i + 2], results->barChar->pRawData, 247, 16, 256, 32, start + 4 + j, 0, 1, 16);
        }
        ScheduleWindowCopyToVram(&results->windows[i + 2]);
    }
    if (finished == WIFI_PLAZA_SURVEY_NUM_ANSWERS) {
        return TRUE;
    }
    return FALSE;
}

void WiFiPlazaSurveyResults_ShowLastWeek(WiFiPlazaSurveyResults *results, WiFiPlazaSurveyMessages *messages, WiFiPlazaSurveyWork *work, enum HeapID heapID) {
    int i;
    Window *window;
    GfGfxLoader_LoadScrnDataFromOpenNarc(work->narc, 0xCB, work->bgConfig, GF_BG_LYR_MAIN_0, 0, 0, FALSE, heapID);
    window = results->windows;
    for (i = 0; i < WIFI_PLAZA_SURVEY_NUM_RESULT_WINDOWS; i++, window++) {
        FillWindowPixelBuffer(window, 0);
        CopyWindowToVram(window);
    }
    WiFiPlazaSurveyResults_PrintTitle(&results->windows[0], messages, msg_0777_00119, MAKE_TEXT_COLOR(5, 6, 0));
}

// current: this week's survey (fields 0-3 of the lobby's survey data), else last week's (fields 4-7).
void WiFiPlazaSurvey_InitQuestion(WiFiPlazaSurveyQuestion *question, BOOL current, enum HeapID heapID) {
    int base;
    int i;
    const u16 *text;
    question->question = String_New(256, heapID);
    base = 0;
    do {
        question->answers[base] = String_New(256, heapID);
        base++;
    } while (base < WIFI_PLAZA_SURVEY_NUM_ANSWERS);
    base = current == TRUE ? 0 : 4;
    question->isSpecial = WiFiPlazaLobbyClient_GetSurveyValue(base + 2);
    question->questionNo = WiFiPlazaLobbyClient_GetSurveyValue(base + 1);
    question->surveyId = WiFiPlazaLobbyClient_GetSurveyValue(base);
    question->hasCustomText = WiFiPlazaLobbyClient_GetSurveyValue(base + 3);
    if (question->hasCustomText == TRUE) {
        text = WiFiPlazaLobbyClient_GetSurveyText(base);
        GF_ASSERT(text != NULL);
        CopyU16ArrayToString(question->question, text);
        for (i = 0; i < WIFI_PLAZA_SURVEY_NUM_ANSWERS; i++) {
            text = WiFiPlazaLobbyClient_GetSurveyText(base + 1 + i);
            GF_ASSERT(text != NULL);
            CopyU16ArrayToString(question->answers[i], text);
        }
    }
}

void WiFiPlazaSurvey_FreeQuestion(WiFiPlazaSurveyQuestion *question) {
    int i;
    String_Delete(question->question);
    for (i = 0; i < WIFI_PLAZA_SURVEY_NUM_ANSWERS; i++) {
        String_Delete(question->answers[i]);
    }
}

String *WiFiPlazaSurvey_GetQuestionText(WiFiPlazaSurveyQuestion *question, WiFiPlazaSurveyMessages *messages) {
    if (question->hasCustomText == FALSE) {
        return WiFiPlazaSurvey_GetQuestionTextFromMsg(messages, question->questionNo);
    }
    return question->question;
}

String *WiFiPlazaSurvey_GetAnswerText(WiFiPlazaSurveyQuestion *question, WiFiPlazaSurveyMessages *messages, int answer) {
    if (question->hasCustomText == FALSE) {
        return WiFiPlazaSurvey_GetAnswerTextFromMsg(messages, question->questionNo, answer);
    }
    return question->answers[answer];
}

// Results from all Wi-Fi Plazas last week (fields 8-10).
void WiFiPlazaSurvey_LoadLastWeekTotals(WiFiPlazaSurveyTally *tally) {
    int i;
    tally->total = 0;
    for (i = 0; i < WIFI_PLAZA_SURVEY_NUM_ANSWERS; i++) {
        tally->counts[i] = WiFiPlazaLobbyClient_GetSurveyValue(i + 8);
        tally->total += tally->counts[i];
    }
}

u64 WiFiPlazaSurvey_GetBarLength(WiFiPlazaSurveyTally *tally, int answer, u32 maxLength) {
    GF_ASSERT((u32)answer < WIFI_PLAZA_SURVEY_NUM_ANSWERS);
    return (u64)maxLength * tally->counts[answer] / tally->total;
}

// Results from this Wi-Fi Plaza alone: the answers of the up to 20 players in the lobby.
void WiFiPlazaSurvey_CountPlazaAnswers(WiFiPlazaSurveyTally *tally, struct WiFiPlazaWork *lobby) {
    int i;
    memset(tally, 0, sizeof(WiFiPlazaSurveyTally));
    for (i = 0; i < 20; i++) {
        struct WiFiPlazaPlayer *player = WiFiPlazaWork_GetPlayer(lobby, i);
        if (player != NULL) {
            void *value = WiFiPlazaPlayer_GetRawLanguage(player);
            WiFiPlazaSurveyAnswerRecord record;
            WiFiPlazaPlayer_GetSurvey(player, &record);
            if (WiFiPlazaLobbyClient_GetSurveyFlag(0, value) == 1 && record.answer < WIFI_PLAZA_SURVEY_NUM_ANSWERS) {
                tally->counts[record.answer]++;
                tally->total++;
            }
        }
    }
}
