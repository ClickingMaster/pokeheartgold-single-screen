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
#include "sprite_resource.h"
#include "sprite_resource_header.h"

// Wi-Fi Plaza "Plaza Survey": mode 0 asks the player to take part and answer this week's question, mode 1 shows
// the results of this Wi-Fi Plaza and of all Wi-Fi Plazas last week as bar graphs.

#define WIFI_PLAZA_SURVEY_NUM_MSG_BANKS      9
#define WIFI_PLAZA_SURVEY_NUM_ANSWERS        3
#define WIFI_PLAZA_SURVEY_NUM_RESULT_WINDOWS 6

enum WiFiPlazaSurveyMode {
    WIFI_PLAZA_SURVEY_MODE_ANSWER,
    WIFI_PLAZA_SURVEY_MODE_RESULTS,
};

typedef struct WiFiPlazaSurveyArgs {
    SaveData *saveData;
    struct WiFiPlazaWork *lobby;
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
} WiFiPlazaSurveyQuestion;

typedef struct WiFiPlazaSurveyTally {
    u32 counts[WIFI_PLAZA_SURVEY_NUM_ANSWERS];
    u64 total;
} WiFiPlazaSurveyTally;

typedef struct WiFiPlazaSurveyMessages {
    MsgData *msgData[WIFI_PLAZA_SURVEY_NUM_MSG_BANKS]; // msg_0777, questions 0761/0765, answers 0758-0760/0762-0764
    MessageFormat *format;
    String *expanded;
    String *raw;
    struct WiFiPlazaWork *lobby;
} WiFiPlazaSurveyMessages;

typedef struct WiFiPlazaSurveyMessageBox {
    u32 printerId;
    u32 textSpeed;
    Window window;
    String *string;
    WaitingIcon *waitingIcon;
    struct ListMenu2D *yesNoMenu;
} WiFiPlazaSurveyMessageBox;

// Mode 0: answer this week's survey.
typedef struct WiFiPlazaSurveyAnswer {
    Window window;
    u32 cursor;
    Sprite *cursorSprite;
    SpriteResource *resources[4];
    u16 state;
    u16 nextState;
    WiFiPlazaSurveyQuestion question;
} WiFiPlazaSurveyAnswer;

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
} WiFiPlazaSurveyResults;

typedef union WiFiPlazaSurveyModeWork {
    WiFiPlazaSurveyAnswer answer;
    WiFiPlazaSurveyResults results;
} WiFiPlazaSurveyModeWork;

typedef struct WiFiPlazaSurveyWork {
    BgConfig *bgConfig;
    SpriteList *spriteList;
    G2dRenderer renderer;
    GF_2DGfxResMan *resMans[4];
    NARC *narc; // a/2/0/7
    WiFiPlazaSurveyMessages messages;
    WiFiPlazaSurveyModeWork mode;
    WiFiPlazaSurveyMessageBox msgBox;
} WiFiPlazaSurveyWork;

extern BOOL WiFiPlazaWork_IsClosing(struct WiFiPlazaWork *lobby);
extern void WiFiPlazaWork_SetAppQuitForClosing(struct WiFiPlazaWork *lobby);
extern void WiFiPlazaWork_SetLowVolume(struct WiFiPlazaWork *lobby, int value);
extern int WiFiPlazaWork_GetOwnSlot(struct WiFiPlazaWork *lobby);
extern struct WiFiPlazaPlayer *WiFiPlazaWork_GetPlayer(struct WiFiPlazaWork *lobby, u32 index);
extern struct WiFiPlazaPlayer *WiFiPlazaWork_GetMyPlayer(struct WiFiPlazaWork *lobby);
extern void WiFiPlazaWork_SetSurveyAnswer(struct WiFiPlazaWork *lobby, u32 surveyId, u32 answer);
extern void WiFiPlazaPlayer_CopyToProfile(struct WiFiPlazaPlayer *player, PlayerProfile *profile, enum HeapID heapID);
extern void *WiFiPlazaPlayer_GetRawLanguage(struct WiFiPlazaPlayer *player);
extern void WiFiPlazaPlayer_GetSurvey(struct WiFiPlazaPlayer *player, WiFiPlazaSurveyAnswerRecord *record);
extern int WiFiPlazaWork_IsSpecialVisitor(struct WiFiPlazaWork *lobby, int value);
extern void WiFiPlazaWork_SetSurveyTally(struct WiFiPlazaWork *lobby, const WiFiPlazaSurveyTally *tally);
extern BOOL WiFiPlazaWork_GetSurveyTally(struct WiFiPlazaWork *lobby, WiFiPlazaSurveyTally *tally);
extern BOOL WiFiPlaza_HasConnectionFailed(void);
extern void WiFiPlazaLobbyClient_SubmitSurveyAnswer(int answer);
extern int WiFiPlazaLobbyClient_GetSurveySubmitState(void);
extern u32 WiFiPlazaLobbyClient_GetSurveyValue(int field);
extern const u16 *WiFiPlazaLobbyClient_GetSurveyText(int field);
extern int WiFiPlazaLobbyClient_GetSurveyFlag(int which, void *value);

BOOL WiFiPlazaSurvey_Init(OverlayManager *man, int *state);
BOOL WiFiPlazaSurvey_Main(OverlayManager *man, int *state);
BOOL WiFiPlazaSurvey_Exit(OverlayManager *man, int *state);

#endif // POKEHEARTGOLD_WIFI_PLAZA_SURVEY_H
