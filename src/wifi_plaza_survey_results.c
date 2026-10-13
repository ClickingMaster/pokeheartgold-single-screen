#include "global.h"

#include "constants/sndseq.h"

#include "msgdata/msg/msg_0777.h"

#include "bg_window.h"
#include "font.h"
#include "gf_gfx_loader.h"
#include "heap.h"
#include "screen_fade.h"
#include "sound_playback.h"
#include "sprite_transfer.h"
#include "system.h"
#include "text.h"
#include "wifi_plaza_survey.h"

#define WIFI_PLAZA_SURVEY_BAR_LENGTH 80 // pixels
#define WIFI_PLAZA_SURVEY_BAR_FRAMES 20

// save_block_32.c has no public header: the player's last Plaza Survey answer.
extern void *Save_PlazaSurvey_Get(SaveData *saveData);
extern u32 PlazaSurvey_GetSurveyId(void *data);
extern u32 PlazaSurvey_GetAnswer(void *data);

extern const WindowTemplate sWiFiPlazaSurveyYesNoTemplate;
extern const VecFx32 sWiFiPlazaSurveyCursorPositions[WIFI_PLAZA_SURVEY_NUM_ANSWERS];
extern const WindowTemplate sWiFiPlazaSurveyResultsWindowTemplates[WIFI_PLAZA_SURVEY_NUM_RESULT_WINDOWS];

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

#pragma force_active on

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

#pragma force_active reset

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

void WiFiPlazaSurveyResults_Init(WiFiPlazaSurveyResults *results, WiFiPlazaSurveyMessages *messages, WiFiPlazaSurveyWork *work, WiFiPlazaSurveyArgs *args, enum HeapID heapID) {
    WiFiPlazaSurveyResults_Setup(results, messages, work, args, heapID);
}

BOOL WiFiPlazaSurveyResults_Main(WiFiPlazaSurveyResults *results, WiFiPlazaSurveyMessages *messages, WiFiPlazaSurveyWork *work, WiFiPlazaSurveyMessageBox *msgBox, WiFiPlazaSurveyArgs *args, enum HeapID heapID) {
    return WiFiPlazaSurveyResults_Run(results, messages, work, msgBox, args, heapID);
}

void WiFiPlazaSurveyResults_Exit(WiFiPlazaSurveyResults *results) {
    WiFiPlazaSurveyResults_Teardown(results);
}

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

#pragma force_active on

BOOL WiFiPlazaSurveyResults_Run(WiFiPlazaSurveyResults *results, WiFiPlazaSurveyMessages *messages, WiFiPlazaSurveyWork *work, WiFiPlazaSurveyMessageBox *msgBox, WiFiPlazaSurveyArgs *args, enum HeapID heapID) {
    WiFiPlazaSurveyAnswerRecord record;
    WiFiPlazaSurveyAnswerRecord savedRecord;
    void *saveBlock;
    switch (results->state) {
    case 0:
        WiFiPlazaSurvey_PrintMessage(msgBox, WiFiPlazaSurvey_GetMessage(messages, 0, results->current.isSpecial == FALSE ? msg_0777_00103 : msg_0777_00105));
        results->nextState = 1;
        results->state = 22;
        break;
    case 1:
        WiFiPlazaSurvey_PrintMessage(msgBox, WiFiPlazaSurvey_GetQuestionText(&results->current, messages));
        results->nextState = 2;
        results->state = 22;
        break;
    case 2:
        if (gSystem.newKeys & PAD_BUTTON_A) {
            WiFiPlazaSurvey_PrintMessage(msgBox, WiFiPlazaSurvey_GetMessage(messages, 0, msg_0777_00104));
            results->nextState = 3;
            results->state = 22;
        }
        break;
    case 3:
        WiFiPlazaSurvey_PrintMessage(msgBox, WiFiPlazaSurvey_GetMessage(messages, 0, results->current.isSpecial == FALSE ? msg_0777_00107 : msg_0777_00108));
        results->nextState = 4;
        results->state = 22;
        break;
    case 4:
        WiFiPlazaPlayer_GetSurvey(WiFiPlazaWork_GetMyPlayer(args->lobby), &record);
        WiFiPlazaSurveyResults_PrintQuestion(results, &results->current, messages, work, &record, heapID, MAKE_TEXT_COLOR(9, 10, 0), MAKE_TEXT_COLOR(13, 14, 0), WiFiPlazaWork_IsSpecialVisitor(args->lobby, WiFiPlazaWork_GetOwnSlot(args->lobby)));
        WiFiPlazaSurveyResults_StartBars(results, &results->plazaTally, work, 1);
        PlaySE(SEQ_SE_PL_PINPON2);
        results->state = 5;
        break;
    case 5:
        if (WiFiPlazaSurveyResults_UpdateBars(results, work) == TRUE) {
            results->state = 6;
        }
        break;
    case 6:
        if (gSystem.newKeys & PAD_BUTTON_A) {
            if (results->lastWeek.questionNo != -1) {
                results->state = 7;
            } else {
                WiFiPlazaSurvey_PrintMessage(msgBox, WiFiPlazaSurvey_GetMessage(messages, 0, msg_0777_00112));
                results->nextState = 20;
                results->state = 22;
            }
        }
        break;
    case 7:
        WiFiPlazaSurvey_PrintMessageInstant(msgBox, WiFiPlazaSurvey_GetMessage(messages, 0, msg_0777_00111));
        WiFiPlazaSurvey_CreateYesNoMenu(msgBox, work, heapID);
        results->state = 8;
        break;
    case 8:
        switch (WiFiPlazaSurvey_HandleYesNoMenu(msgBox, heapID)) {
        case 0:
            results->state = 9;
            break;
        case LIST_CANCEL:
            results->state = 20;
            break;
        }
        break;
    case 9:
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, RGB_BLACK, 6, 1, heapID);
        results->state = 10;
        break;
    case 10:
        if (IsPaletteFadeFinished() == TRUE) {
            WiFiPlazaSurveyResults_ShowLastWeek(results, messages, work, heapID);
            results->state = 11;
        }
        break;
    case 11:
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, RGB_BLACK, 6, 1, heapID);
        results->state = 12;
        break;
    case 12:
        if (IsPaletteFadeFinished() == TRUE) {
            results->state = 13;
        }
        break;
    case 13:
        WiFiPlazaSurvey_PrintMessage(msgBox, WiFiPlazaSurvey_GetMessage(messages, 0, results->lastWeek.isSpecial == FALSE ? msg_0777_00113 : msg_0777_00115));
        results->nextState = 14;
        results->state = 22;
        break;
    case 14:
        WiFiPlazaSurvey_PrintMessage(msgBox, WiFiPlazaSurvey_GetQuestionText(&results->lastWeek, messages));
        results->nextState = 15;
        results->state = 22;
        break;
    case 15:
        if (gSystem.newKeys & PAD_BUTTON_A) {
            WiFiPlazaSurvey_PrintMessage(msgBox, WiFiPlazaSurvey_GetMessage(messages, 0, msg_0777_00114));
            results->nextState = 16;
            results->state = 22;
        }
        break;
    case 16:
        WiFiPlazaSurvey_PrintMessage(msgBox, WiFiPlazaSurvey_GetMessage(messages, 0, results->lastWeek.isSpecial == FALSE ? msg_0777_00117 : msg_0777_00118));
        results->nextState = 17;
        results->state = 22;
        break;
    case 17:
        saveBlock = Save_PlazaSurvey_Get(args->saveData);
        savedRecord.surveyId = PlazaSurvey_GetSurveyId(saveBlock);
        savedRecord.answer = PlazaSurvey_GetAnswer(saveBlock);
        WiFiPlazaSurveyResults_PrintQuestion(results, &results->lastWeek, messages, work, &savedRecord, heapID, MAKE_TEXT_COLOR(11, 12, 0), MAKE_TEXT_COLOR(15, 14, 0), WiFiPlazaWork_IsSpecialVisitor(args->lobby, WiFiPlazaWork_GetOwnSlot(args->lobby)));
        WiFiPlazaSurveyResults_StartBars(results, &results->lastWeekTally, work, 2);
        PlaySE(SEQ_SE_PL_PINPON2);
        results->state = 18;
        break;
    case 18:
        if (WiFiPlazaSurveyResults_UpdateBars(results, work) == TRUE) {
            results->state = 19;
        }
        break;
    case 19:
        if (gSystem.newKeys & PAD_BUTTON_A) {
            results->state = 20;
        }
        break;
    case 20:
        WiFiPlazaSurvey_PrintMessage(msgBox, WiFiPlazaSurvey_GetMessage(messages, 0, msg_0777_00120));
        results->nextState = 21;
        results->state = 22;
        break;
    case 21:
        WiFiPlazaSurvey_ClearMessageBox(msgBox);
        return TRUE;
    case 22:
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

#pragma force_active reset

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
#pragma force_active on

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

#pragma force_active reset

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
#pragma force_active on

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

#pragma force_active reset

// current: this week's survey (fields 0-3 of the lobby's survey data), else last week's (fields 4-7).
void WiFiPlazaSurvey_InitQuestion(WiFiPlazaSurveyQuestion *question, BOOL current, enum HeapID heapID) {
    int base;
    int i;
    const u16 *text;
    question->question = String_New(256, heapID);
    // base doubles as the loop counter here (codegen)
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
void WiFiPlazaSurvey_CountPlazaAnswers(WiFiPlazaSurveyTally *tally, void *lobby) {
    int i;
    memset(tally, 0, sizeof(WiFiPlazaSurveyTally));
    for (i = 0; i < 20; i++) {
        void *player = WiFiPlazaWork_GetPlayer(lobby, i);
        if (player != NULL) {
            void *value = WiFiPlazaPlayer_GetRawLanguage(player);
            WiFiPlazaSurveyAnswerRecord record;
            WiFiPlazaPlayer_GetSurvey(player, &record);
            if (ov45_0222F6B0(0, value) == 1 && record.answer < WIFI_PLAZA_SURVEY_NUM_ANSWERS) {
                tally->counts[record.answer]++;
                tally->total++;
            }
        }
    }
}
