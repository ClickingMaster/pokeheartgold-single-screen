#ifndef POKEHEARTGOLD_WIFI_PLAZA_SURVEY_H
#define POKEHEARTGOLD_WIFI_PLAZA_SURVEY_H

#include "bg_window.h"
#include "filesystem.h"
#include "list_menu_2d.h"
#include "list_menu_items.h"
#include "message_format.h"
#include "msgdata.h"
#include "options.h"
#include "overlay_manager.h"
#include "player_data.h"
#include "pm_string.h"
#include "render_window.h"
#include "save.h"
#include "sprite.h"
#include "unk_02009D48.h"
#include "unk_0200A090.h"

// Wi-Fi Plaza "Plaza Survey", started by the Wi-Fi Plaza lobby overlay (OVY_45): mode 0 asks the
// player to take part and answer this week's question, mode 1 shows the results of this Wi-Fi Plaza
// and of all Wi-Fi Plazas last week as bar graphs.

#define WIFI_PLAZA_SURVEY_NUM_MSG_BANKS      9
#define WIFI_PLAZA_SURVEY_NUM_ANSWERS        3
#define WIFI_PLAZA_SURVEY_NUM_RESULT_WINDOWS 6

enum WiFiPlazaSurveyMode {
    WIFI_PLAZA_SURVEY_MODE_ANSWER,
    WIFI_PLAZA_SURVEY_MODE_RESULTS,
};

// Arguments built by OVY_45.
typedef struct WiFiPlazaSurveyArgs {
    SaveData *saveData;
    void *lobby;
    int mode;
} WiFiPlazaSurveyArgs;

// The player's answer as stored by the lobby / in the save data.
typedef struct WiFiPlazaSurveyAnswerRecord {
    u32 surveyId;
    u32 answer;
} WiFiPlazaSurveyAnswerRecord;

typedef struct WiFiPlazaSurveyQuestion {
    u32 questionNo;    // msg_0761 (< 50) or msg_0765
    u32 surveyId;      // compared with the player's answer record
    u32 isSpecial;     // "Special Plaza Survey"
    u32 hasCustomText; // texts sent by the server instead of the message banks
    String *question;
    String *answers[WIFI_PLAZA_SURVEY_NUM_ANSWERS];
} WiFiPlazaSurveyQuestion; // size: 0x20

typedef struct WiFiPlazaSurveyTally {
    u32 counts[WIFI_PLAZA_SURVEY_NUM_ANSWERS];
    u64 total;
} WiFiPlazaSurveyTally; // size: 0x14

typedef struct WiFiPlazaSurveyMessages {
    MsgData *msgData[WIFI_PLAZA_SURVEY_NUM_MSG_BANKS]; // msg_0777, questions 0761/0765, answers 0758-0760/0762-0764
    MessageFormat *format;
    String *expanded;
    String *raw;
    void *lobby;
} WiFiPlazaSurveyMessages; // size: 0x34

typedef struct WiFiPlazaSurveyMessageBox {
    u32 printerId;
    u32 textSpeed;
    Window window;
    String *string;
    WaitingIcon *waitingIcon;
    struct ListMenu2D *yesNoMenu;
} WiFiPlazaSurveyMessageBox; // size: 0x24

// Mode 0: answer this week's survey.
typedef struct WiFiPlazaSurveyAnswer {
    Window window;
    u32 cursor;
    Sprite *cursorSprite;
    SpriteResource *resources[4];
    u16 state;
    u16 nextState;
    WiFiPlazaSurveyQuestion question;
} WiFiPlazaSurveyAnswer; // size: 0x4C

// Mode 1: results of this plaza and of all plazas last week.
typedef struct WiFiPlazaSurveyResults {
    Window windows[WIFI_PLAZA_SURVEY_NUM_RESULT_WINDOWS]; // title, answers, 3 bars, player's answer
    u16 state;
    u16 nextState;
    s32 barFrame;
    int barLength[WIFI_PLAZA_SURVEY_NUM_ANSWERS];
    int barTarget[WIFI_PLAZA_SURVEY_NUM_ANSWERS];
    void *barCharFile;
    NNSG2dCharacterData *barChar;
    WiFiPlazaSurveyQuestion current;
    WiFiPlazaSurveyQuestion lastWeek;
    WiFiPlazaSurveyTally plazaTally;
    WiFiPlazaSurveyTally lastWeekTally;
} WiFiPlazaSurveyResults; // size: 0xF0

typedef struct WiFiPlazaSurveyWork {
    BgConfig *bgConfig;
    SpriteList *spriteList;
    G2dRenderer renderer;
    GF_2DGfxResMan *resMans[4];
    NARC *narc; // a/2/0/7
    WiFiPlazaSurveyMessages messages;

    union {
        WiFiPlazaSurveyAnswer answer;
        WiFiPlazaSurveyResults results;
    } mode;

    WiFiPlazaSurveyMessageBox msgBox;
} WiFiPlazaSurveyWork; // size: 0x28C

// The OVY_45 lobby has no public header.
extern BOOL ov45_0222A330(void *lobby);
extern void ov45_0222A4A8(void *lobby);
extern void ov45_0222A520(void *lobby, int value);
extern int ov45_0222A53C(void *lobby);
extern void *ov45_0222A578(void *lobby, u32 index);
extern void *ov45_0222A5C0(void *lobby);
extern void ov45_0222A7DC(void *lobby, u32 surveyId, u32 answer);
extern void ov45_0222A844(void *player, PlayerProfile *profile, enum HeapID heapID);
extern void *ov45_0222AA54(void *player);
extern void ov45_0222AB0C(void *player, WiFiPlazaSurveyAnswerRecord *record);
extern int ov45_0222AB28(void *lobby, int value);
extern void ov45_0222B1FC(void *lobby, const WiFiPlazaSurveyTally *tally);
extern BOOL ov45_0222B21C(void *lobby, WiFiPlazaSurveyTally *tally);
extern BOOL ov45_0222D844(void);
extern void ov45_0222F4D4(int answer);
extern int ov45_0222F524(void);
extern u32 ov45_0222F544(int field);
extern const u16 *ov45_0222F628(int field);
extern int ov45_0222F6B0(int which, void *value);

BOOL WiFiPlazaSurvey_Init(OverlayManager *man, int *state);
BOOL WiFiPlazaSurvey_Main(OverlayManager *man, int *state);
BOOL WiFiPlazaSurvey_Exit(OverlayManager *man, int *state);
void WiFiPlazaSurvey_VBlankCB(void *data);
void WiFiPlazaSurvey_InitGraphics(WiFiPlazaSurveyWork *work, SaveData *saveData, enum HeapID heapID);
void WiFiPlazaSurvey_FreeGraphics(WiFiPlazaSurveyWork *work);
void WiFiPlazaSurvey_RenderSprites(WiFiPlazaSurveyWork *work);
void WiFiPlazaSurvey_DoVBlankUpdates(WiFiPlazaSurveyWork *work);
void WiFiPlazaSurvey_InitBgs(WiFiPlazaSurveyWork *work, Options *options, enum HeapID heapID);
void WiFiPlazaSurvey_FreeBgs(WiFiPlazaSurveyWork *work);
void WiFiPlazaSurvey_InitSpriteSystem(WiFiPlazaSurveyWork *work, enum HeapID heapID);
void WiFiPlazaSurvey_FreeSpriteSystem(WiFiPlazaSurveyWork *work);
void WiFiPlazaSurvey_InitMessages(WiFiPlazaSurveyMessages *messages, void *lobby, enum HeapID heapID);
void WiFiPlazaSurvey_FreeMessages(WiFiPlazaSurveyMessages *messages);
String *WiFiPlazaSurvey_GetMessage(WiFiPlazaSurveyMessages *messages, int bank, int msgId);
String *WiFiPlazaSurvey_GetQuestionTextFromMsg(WiFiPlazaSurveyMessages *messages, u32 questionNo);
String *WiFiPlazaSurvey_GetAnswerTextFromMsg(WiFiPlazaSurveyMessages *messages, u32 questionNo, int answer);
void WiFiPlazaSurvey_BufferPlayerName(WiFiPlazaSurveyMessages *messages, int index, enum HeapID heapID);
void WiFiPlazaSurveyAnswer_Init(WiFiPlazaSurveyAnswer *answer, WiFiPlazaSurveyMessages *messages, WiFiPlazaSurveyWork *work, WiFiPlazaSurveyArgs *args, enum HeapID heapID);
BOOL WiFiPlazaSurveyAnswer_Main(WiFiPlazaSurveyAnswer *answer, WiFiPlazaSurveyMessages *messages, WiFiPlazaSurveyWork *work, WiFiPlazaSurveyMessageBox *msgBox, WiFiPlazaSurveyArgs *args, enum HeapID heapID);
void WiFiPlazaSurveyAnswer_Exit(WiFiPlazaSurveyAnswer *answer, WiFiPlazaSurveyMessages *messages, WiFiPlazaSurveyWork *work);
void WiFiPlazaSurveyAnswer_Setup(WiFiPlazaSurveyAnswer *answer, WiFiPlazaSurveyMessages *messages, WiFiPlazaSurveyWork *work, enum HeapID heapID);
BOOL WiFiPlazaSurveyAnswer_Run(WiFiPlazaSurveyAnswer *answer, WiFiPlazaSurveyMessages *messages, WiFiPlazaSurveyWork *work, WiFiPlazaSurveyMessageBox *msgBox, WiFiPlazaSurveyArgs *args, enum HeapID heapID);
void WiFiPlazaSurveyAnswer_Teardown(WiFiPlazaSurveyAnswer *answer, WiFiPlazaSurveyMessages *messages, WiFiPlazaSurveyWork *work);
void WiFiPlazaSurveyAnswer_PrintAnswers(WiFiPlazaSurveyAnswer *answer, WiFiPlazaSurveyMessages *messages, WiFiPlazaSurveyWork *work);
void WiFiPlazaSurveyAnswer_UpdateCursor(WiFiPlazaSurveyAnswer *answer);
void WiFiPlazaSurveyAnswer_StartCursorAnim(WiFiPlazaSurveyAnswer *answer);
void WiFiPlazaSurvey_InitMessageBox(WiFiPlazaSurveyMessageBox *msgBox, WiFiPlazaSurveyWork *work, SaveData *saveData, enum HeapID heapID);
void WiFiPlazaSurvey_FreeMessageBox(WiFiPlazaSurveyMessageBox *msgBox, enum HeapID heapID);
void WiFiPlazaSurvey_PrintMessage(WiFiPlazaSurveyMessageBox *msgBox, String *string);
void WiFiPlazaSurvey_PrintMessageInstant(WiFiPlazaSurveyMessageBox *msgBox, String *string);
void WiFiPlazaSurvey_StartWaitingIcon(WiFiPlazaSurveyMessageBox *msgBox);
void WiFiPlazaSurvey_StopWaitingIcon(WiFiPlazaSurveyMessageBox *msgBox);
BOOL WiFiPlazaSurvey_IsMessageFinished(WiFiPlazaSurveyMessageBox *msgBox);
void WiFiPlazaSurvey_ClearMessageBox(WiFiPlazaSurveyMessageBox *msgBox);
void WiFiPlazaSurvey_CreateYesNoMenu(WiFiPlazaSurveyMessageBox *msgBox, WiFiPlazaSurveyWork *work, enum HeapID heapID);
u32 WiFiPlazaSurvey_HandleYesNoMenu(WiFiPlazaSurveyMessageBox *msgBox, enum HeapID heapID);
void WiFiPlazaSurvey_DestroyYesNoMenu(WiFiPlazaSurveyMessageBox *msgBox, enum HeapID heapID);
void WiFiPlazaSurveyResults_Init(WiFiPlazaSurveyResults *results, WiFiPlazaSurveyMessages *messages, WiFiPlazaSurveyWork *work, WiFiPlazaSurveyArgs *args, enum HeapID heapID);
BOOL WiFiPlazaSurveyResults_Main(WiFiPlazaSurveyResults *results, WiFiPlazaSurveyMessages *messages, WiFiPlazaSurveyWork *work, WiFiPlazaSurveyMessageBox *msgBox, WiFiPlazaSurveyArgs *args, enum HeapID heapID);
void WiFiPlazaSurveyResults_Exit(WiFiPlazaSurveyResults *results);
void WiFiPlazaSurveyResults_Setup(WiFiPlazaSurveyResults *results, WiFiPlazaSurveyMessages *messages, WiFiPlazaSurveyWork *work, WiFiPlazaSurveyArgs *args, enum HeapID heapID);
BOOL WiFiPlazaSurveyResults_Run(WiFiPlazaSurveyResults *results, WiFiPlazaSurveyMessages *messages, WiFiPlazaSurveyWork *work, WiFiPlazaSurveyMessageBox *msgBox, WiFiPlazaSurveyArgs *args, enum HeapID heapID);
void WiFiPlazaSurveyResults_Teardown(WiFiPlazaSurveyResults *results);
void WiFiPlazaSurveyResults_PrintTitle(Window *window, WiFiPlazaSurveyMessages *messages, int msgId, u32 color);
void WiFiPlazaSurveyResults_PrintQuestion(WiFiPlazaSurveyResults *results, WiFiPlazaSurveyQuestion *question, WiFiPlazaSurveyMessages *messages, WiFiPlazaSurveyWork *work, WiFiPlazaSurveyAnswerRecord *record, enum HeapID heapID, u32 color, u32 highlightColor, BOOL highlight);
void WiFiPlazaSurveyResults_StartBars(WiFiPlazaSurveyResults *results, WiFiPlazaSurveyTally *tally, WiFiPlazaSurveyWork *work, int palette);
BOOL WiFiPlazaSurveyResults_UpdateBars(WiFiPlazaSurveyResults *results, WiFiPlazaSurveyWork *work);
void WiFiPlazaSurveyResults_ShowLastWeek(WiFiPlazaSurveyResults *results, WiFiPlazaSurveyMessages *messages, WiFiPlazaSurveyWork *work, enum HeapID heapID);
void WiFiPlazaSurvey_InitQuestion(WiFiPlazaSurveyQuestion *question, BOOL current, enum HeapID heapID);
void WiFiPlazaSurvey_FreeQuestion(WiFiPlazaSurveyQuestion *question);
String *WiFiPlazaSurvey_GetQuestionText(WiFiPlazaSurveyQuestion *question, WiFiPlazaSurveyMessages *messages);
String *WiFiPlazaSurvey_GetAnswerText(WiFiPlazaSurveyQuestion *question, WiFiPlazaSurveyMessages *messages, int answer);
void WiFiPlazaSurvey_LoadLastWeekTotals(WiFiPlazaSurveyTally *tally);
u64 WiFiPlazaSurvey_GetBarLength(WiFiPlazaSurveyTally *tally, int answer, u32 maxLength);
void WiFiPlazaSurvey_CountPlazaAnswers(WiFiPlazaSurveyTally *tally, void *lobby);

#endif // POKEHEARTGOLD_WIFI_PLAZA_SURVEY_H
