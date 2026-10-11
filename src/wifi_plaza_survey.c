#include "wifi_plaza_survey.h"

#include "global.h"

#include "msgdata/msg.naix"

#include "bg_window.h"
#include "font.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "obj_char_transfer.h"
#include "obj_pltt_transfer.h"
#include "screen_fade.h"
#include "sound_playback.h"
#include "sprite_transfer.h"
#include "system.h"
#include "oam_manager.h"
#include "unk_0203A3B0.h"
#include "vram_transfer_manager.h"

typedef void (*WiFiPlazaSurveyInitFunc)(void *modeWork, WiFiPlazaSurveyMessages *messages, WiFiPlazaSurveyWork *work, WiFiPlazaSurveyArgs *args, enum HeapID heapID);
typedef BOOL (*WiFiPlazaSurveyMainFunc)(void *modeWork, WiFiPlazaSurveyMessages *messages, WiFiPlazaSurveyWork *work, WiFiPlazaSurveyMessageBox *msgBox, WiFiPlazaSurveyArgs *args, enum HeapID heapID);
typedef void (*WiFiPlazaSurveyExitFunc)(void *modeWork, WiFiPlazaSurveyMessages *messages, WiFiPlazaSurveyWork *work);

extern const WindowTemplate sWiFiPlazaSurveyAnswerWindowTemplate;
extern const GraphicsModes sWiFiPlazaSurveyGraphicsModes;
extern ObjCharTransferTemplate sWiFiPlazaSurveyObjCharTransferTemplate;
extern const u32 sWiFiPlazaSurveyBgLayers[4];
extern const int sWiFiPlazaSurveyMsgBanks[WIFI_PLAZA_SURVEY_NUM_MSG_BANKS];
extern const GraphicsBanks sWiFiPlazaSurveyGraphicsBanks;
extern const BgTemplate sWiFiPlazaSurveyBgTemplates[4];
extern const WiFiPlazaSurveyExitFunc sWiFiPlazaSurveyExitFuncs[];
extern const WiFiPlazaSurveyMainFunc sWiFiPlazaSurveyMainFuncs[];
extern const WiFiPlazaSurveyInitFunc sWiFiPlazaSurveyInitFuncs[];

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
        ov45_0222A520(args->lobby, 1);
        (*state)++;
        break;
    case 1:
        if (IsPaletteFadeFinished() == TRUE) {
            (*state)++;
        }
        break;
    case 2:
        if (IsPaletteFadeFinished() == TRUE) {
            if (ov45_0222A330(args->lobby) == TRUE) {
                ov45_0222A4A8(args->lobby);
                (*state)++;
                break;
            }
            if (ov45_0222D844() == TRUE) {
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

#pragma force_active on

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

#pragma force_active reset

void WiFiPlazaSurvey_FreeBgs(WiFiPlazaSurveyWork *work) {
    int i;
    const u32 *layer = sWiFiPlazaSurveyBgLayers;
    for (i = 0; i < 4; i++, layer++) {
        FreeBgTilemapBuffer(work->bgConfig, (u8)*layer);
    }
    Heap_Free(work->bgConfig);
}

void WiFiPlazaSurvey_InitSpriteSystem(WiFiPlazaSurveyWork *work, enum HeapID heapID) {
    int i;
    NNS_G2dInitOamManagerModule();
    OamManager_Create(0, 126, 0, 31, 0, 126, 0, 31, heapID);
    ObjCharTransfer_InitEx(&sWiFiPlazaSurveyObjCharTransferTemplate, GX_OBJVRAMMODE_CHAR_1D_32K, GX_OBJVRAMMODE_CHAR_1D_32K);
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

#pragma force_active on

void WiFiPlazaSurvey_InitMessages(WiFiPlazaSurveyMessages *messages, void *lobby, enum HeapID heapID) {
    int i;
    messages->lobby = lobby;
    for (i = 0; i < WIFI_PLAZA_SURVEY_NUM_MSG_BANKS; i++) {
        messages->msgData[i] = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, sWiFiPlazaSurveyMsgBanks[i], heapID);
    }
    messages->format = MessageFormat_New(heapID);
    messages->expanded = String_New(256, heapID);
    messages->raw = String_New(256, heapID);
}

#pragma force_active reset

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
        GF_ASSERT(questionNo >= 60); // repeats the test (codegen)
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
        GF_ASSERT(questionNo >= 60); // repeats the test (codegen)
        questionNo = 0;
    }
    if (questionNo < 50) {
        return WiFiPlazaSurvey_GetMessage(messages, answer + 3, questionNo);
    }
    return WiFiPlazaSurvey_GetMessage(messages, answer + 6, questionNo - 50);
}

void WiFiPlazaSurvey_BufferPlayerName(WiFiPlazaSurveyMessages *messages, int index, enum HeapID heapID) {
    PlayerProfile *profile = PlayerProfile_New(heapID);
    void *player = ov45_0222A5C0(messages->lobby);
    ov45_0222A844(player, profile, heapID);
    BufferPlayersName(messages->format, index, profile);
    Heap_Free(profile);
}

void WiFiPlazaSurveyAnswer_Init(WiFiPlazaSurveyAnswer *answer, WiFiPlazaSurveyMessages *messages, WiFiPlazaSurveyWork *work, WiFiPlazaSurveyArgs *args, enum HeapID heapID) {
    WiFiPlazaSurveyAnswer_Setup(answer, messages, work, heapID);
}

BOOL WiFiPlazaSurveyAnswer_Main(WiFiPlazaSurveyAnswer *answer, WiFiPlazaSurveyMessages *messages, WiFiPlazaSurveyWork *work, WiFiPlazaSurveyMessageBox *msgBox, WiFiPlazaSurveyArgs *args, enum HeapID heapID) {
    return WiFiPlazaSurveyAnswer_Run(answer, messages, work, msgBox, args, heapID);
}

void WiFiPlazaSurveyAnswer_Exit(WiFiPlazaSurveyAnswer *answer, WiFiPlazaSurveyMessages *messages, WiFiPlazaSurveyWork *work) {
    WiFiPlazaSurveyAnswer_Teardown(answer, messages, work);
}

void WiFiPlazaSurveyAnswer_Setup(WiFiPlazaSurveyAnswer *answer, WiFiPlazaSurveyMessages *messages, WiFiPlazaSurveyWork *work, enum HeapID heapID) {
    SpriteResourcesHeader header;
    GfGfxLoader_GXLoadPalFromOpenNarc(work->narc, 0xC7, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_0_OFFSET, 0x80, heapID);
    GfGfxLoader_LoadCharDataFromOpenNarc(work->narc, 0xC8, work->bgConfig, GF_BG_LYR_MAIN_0, 0, 0, FALSE, heapID);
    GfGfxLoader_LoadScrnDataFromOpenNarc(work->narc, 0xC9, work->bgConfig, GF_BG_LYR_MAIN_0, 0, 0, FALSE, heapID);
    answer->resources[GF_GFX_RES_TYPE_CHAR] = AddCharResObjFromOpenNarc(work->resMans[GF_GFX_RES_TYPE_CHAR], work->narc, 0xCD, FALSE, 20, 1, heapID);
    answer->resources[GF_GFX_RES_TYPE_PLTT] = AddPlttResObjFromOpenNarc(work->resMans[GF_GFX_RES_TYPE_PLTT], work->narc, 0xCC, FALSE, 20, 1, 3, heapID);
    GF_ASSERT(SpriteTransfer_CreateCharTransferTask_AllocAtEnd(answer->resources[GF_GFX_RES_TYPE_CHAR]));
    GF_ASSERT(SpriteTransfer_CreatePlttTransferTask(answer->resources[GF_GFX_RES_TYPE_PLTT]));
    sub_0200A740(answer->resources[GF_GFX_RES_TYPE_CHAR]);
    sub_0200A740(answer->resources[GF_GFX_RES_TYPE_PLTT]);
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
