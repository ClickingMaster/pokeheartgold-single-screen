#ifndef POKEHEARTGOLD_NINTENDO_WFC_CONNECT_UNUSED_H
#define POKEHEARTGOLD_NINTENDO_WFC_CONNECT_UNUSED_H

#include "global.h"

#include "frontier_wifi_upload_record.h"
#include "msgdata.h"
#include "nintendo_wfc_connect.h"
#include "nintendo_wfc_connect_internal.h"
#include "render_window.h"
#include "save_wii_message.h"
#include "sprite.h"
#include "yes_no_prompt.h"

enum NintendoWfcUnusedMainState {
    NINTENDO_WFC_UNUSED_MAIN_WAIT_STARTUP,
    NINTENDO_WFC_UNUSED_MAIN_SESSION_INIT,
    NINTENDO_WFC_UNUSED_MAIN_WAIT_FADE_IN,
    NINTENDO_WFC_UNUSED_MAIN_SESSION_MAIN,
    NINTENDO_WFC_UNUSED_MAIN_SESSION_EXIT,
    NINTENDO_WFC_UNUSED_MAIN_DONE,
};

enum NintendoWfcUnusedState {
    NINTENDO_WFC_UNUSED_STATE_ASK_CONNECT,
    NINTENDO_WFC_UNUSED_STATE_HANDLE_CONNECT_CHOICE,
    NINTENDO_WFC_UNUSED_STATE_START_CONNECT,
    NINTENDO_WFC_UNUSED_STATE_WAIT_CONNECT,
    NINTENDO_WFC_UNUSED_STATE_START_AUTH,
    NINTENDO_WFC_UNUSED_STATE_WAIT_AUTH,
    NINTENDO_WFC_UNUSED_STATE_INIT_HTTP,
    NINTENDO_WFC_UNUSED_STATE_REQUEST_INFO,
    NINTENDO_WFC_UNUSED_STATE_WAIT_INFO,
    NINTENDO_WFC_UNUSED_STATE_REQUEST_SET_PROFILE,
    NINTENDO_WFC_UNUSED_STATE_WAIT_SET_PROFILE,
    NINTENDO_WFC_UNUSED_STATE_REQUEST_ROOM_COUNT,
    NINTENDO_WFC_UNUSED_STATE_WAIT_ROOM_COUNT,
    NINTENDO_WFC_UNUSED_STATE_ASK_CHALLENGE_ROOM,
    NINTENDO_WFC_UNUSED_STATE_START_CHALLENGE_ROOM_INPUT,
    NINTENDO_WFC_UNUSED_STATE_HANDLE_CHALLENGE_ROOM_INPUT,
    NINTENDO_WFC_UNUSED_STATE_PRINT_RECEIVING_TRAINERS,
    NINTENDO_WFC_UNUSED_STATE_REQUEST_TRAINERS_DOWNLOAD,
    NINTENDO_WFC_UNUSED_STATE_WAIT_TRAINERS_DOWNLOAD,
    NINTENDO_WFC_UNUSED_STATE_PRINT_TRAINERS_RECEIVED,
    NINTENDO_WFC_UNUSED_STATE_PRINT_ROOM_UNAVAILABLE,
    NINTENDO_WFC_UNUSED_STATE_ASK_CANCEL_CHALLENGE,
    NINTENDO_WFC_UNUSED_STATE_HANDLE_CANCEL_CHALLENGE_CHOICE,
    NINTENDO_WFC_UNUSED_STATE_ASK_CANCEL_DOWNLOAD,
    NINTENDO_WFC_UNUSED_STATE_HANDLE_CANCEL_DOWNLOAD_CHOICE,
    NINTENDO_WFC_UNUSED_STATE_PRINT_SENDING_RECORD,
    NINTENDO_WFC_UNUSED_STATE_REQUEST_UPLOAD,
    NINTENDO_WFC_UNUSED_STATE_WAIT_UPLOAD,
    NINTENDO_WFC_UNUSED_STATE_PRINT_RECORD_SENT,
    NINTENDO_WFC_UNUSED_STATE_ASK_LEADERS_RANK,
    NINTENDO_WFC_UNUSED_STATE_START_LEADERS_RANK_INPUT,
    NINTENDO_WFC_UNUSED_STATE_HANDLE_LEADERS_RANK_INPUT,
    NINTENDO_WFC_UNUSED_STATE_ASK_LEADERS_ROOM,
    NINTENDO_WFC_UNUSED_STATE_START_LEADERS_ROOM_INPUT,
    NINTENDO_WFC_UNUSED_STATE_HANDLE_LEADERS_ROOM_INPUT,
    NINTENDO_WFC_UNUSED_STATE_PRINT_RECEIVING_LEADERS,
    NINTENDO_WFC_UNUSED_STATE_REQUEST_LEADERS_DOWNLOAD,
    NINTENDO_WFC_UNUSED_STATE_WAIT_LEADERS_DOWNLOAD,
    NINTENDO_WFC_UNUSED_STATE_PRINT_LEADERS_RECEIVED,
    NINTENDO_WFC_UNUSED_STATE_START_SAVE,
    NINTENDO_WFC_UNUSED_STATE_SAVE,
    NINTENDO_WFC_UNUSED_STATE_PRINT_DISCONNECTING,
    NINTENDO_WFC_UNUSED_STATE_CLOSE_CONNECTION,
    NINTENDO_WFC_UNUSED_STATE_PRINT_DISCONNECTED,
    NINTENDO_WFC_UNUSED_STATE_UNUSED_IDLE,
    NINTENDO_WFC_UNUSED_STATE_START_DISCONNECT,
    NINTENDO_WFC_UNUSED_STATE_START_EXIT,
    NINTENDO_WFC_UNUSED_STATE_WAIT_MESSAGE,
    NINTENDO_WFC_UNUSED_STATE_OPEN_YES_NO_PROMPT,
    NINTENDO_WFC_UNUSED_STATE_WAIT_MESSAGE_AND_DELAY,
    NINTENDO_WFC_UNUSED_STATE_UNUSED_HANDLE_YES_NO,
    NINTENDO_WFC_UNUSED_STATE_ASK_LOG_OUT,
    NINTENDO_WFC_UNUSED_STATE_HANDLE_LOG_OUT_CHOICE,
    NINTENDO_WFC_UNUSED_STATE_SHOW_HTTP_ERROR,
    NINTENDO_WFC_UNUSED_STATE_DISCONNECT,
    NINTENDO_WFC_UNUSED_STATE_SHOW_DWC_ERROR,
    NINTENDO_WFC_UNUSED_STATE_WAIT_ERROR_DISMISS,
};

typedef struct BattleRoomDownload BattleRoomDownload;

typedef struct NintendoWfcUnused {
    NintendoWifiConnectArgs *args;
    BgConfig *bgConfig;
    u8 padding_0008[8];
    int mode; // index into sNintendoWfcUnusedModes
    int nextMode;
    int unk_0018;
    int state; // index into sNintendoWfcUnusedStates
    int nextState;
    void *heapMem;
    NNSFndHeapHandle expHeap;
    u8 connection[0x64]; // passed to DWC_InitInetEx
    u32 ownRank;
    u32 rank;
    int roomCount;
    u32 room;
    u8 downloadBuffer[0xA38];
    FrontierWifiUploadRecord uploadRecord;
    u8 padding_0BBC[0x14];
    MessageFormat *msgFormat;
    MsgData *msgDataTower; // Wi-Fi Battle Tower
    MsgData *msgDataWfc;   // Nintendo WFC
    MsgData *msgDataError; // Nintendo WFC errors
    String *msgString;
    String *titleString;
    String *roomString;
    String *rankString;
    String *errorString;
    u32 textPrinterId;
    SpriteList *spriteList;
    G2dRenderer renderer;
    GF_2DGfxResMan *resMan[4];
    SpriteResource *resObjs[4];
    u8 padding_0D44[0x20];
    SpriteResourcesHeader spriteHeader;
    u8 padding_0D88[0x48];
    Sprite *arrows[2]; // up/down arrows next to the number windows
    Window msgWindow;
    Window rankWindow;
    Window roomWindow;
    Window titleWindow;
    Window errorWindow;
    u8 padding_0E28[0xE4];
    WaitingIcon *waitingIcon;
    int dwcError;
    int dwcErrorCode;
    int dwcErrorType;
    int httpResult;
    int messageTimer;
    WifiServerProfile profile;
    u32 profileResponse[2];
    s16 subState;
    s16 subTimer;
    int timeoutCounter;
    NintendoWfcPaletteCycle paletteCycle;
    YesNoPrompt *yesNoPrompt;
} NintendoWfcUnused;

typedef struct NintendoWfcUnusedGlobal {
    NintendoWfcUnused *wfc;
    NNSFndHeapHandle heap; // heap of the DWC allocator callbacks
} NintendoWfcUnusedGlobal;

typedef int (*NintendoWfcUnusedModeFunc)(NintendoWfcUnused *wfc, int state);

// The session functions of each mode.
typedef struct NintendoWfcUnusedModeRow {
    NintendoWfcUnusedModeFunc init;
    NintendoWfcUnusedModeFunc main;
    NintendoWfcUnusedModeFunc exit;
} NintendoWfcUnusedModeRow;

int DWC_GetLastError(void *code);
int DWC_GetLastErrorEx(int *code, int *category);
void DWC_ClearError(void);
void DWC_SetMemFunc(void *(*alloc)(void *, u32, int), void (*freeFunc)(void *, void *));
void DWC_InitInetEx(void *connection, int priority, int unused, int sslPriority);
void DWC_SetAuthServer(int server);
void DWC_ConnectInetAsync(void);
int DWC_CheckInet(void);
void DWC_ProcessInet(void);
int DWC_GetInetStatus(void);
void DWC_CleanupInet(void);
int DWC_GetLinkLevel(void);
void DWC_GetApInfo(void *info);
void DWC_UpdateConnection(void);
void DWC_GetDateTime(void *date, void *time);
void DWC_NASLoginAsync(void);
int DWC_NASLoginProcess(void);
void DWC_ShutdownFriendsMatch(void);
void DWC_ShutdownGHTTP(void);

void NintendoWfcHttp_Update(void);
BOOL NintendoWfcHttp_IsDone(void);
s32 NintendoWfcHttp_GetResult(void);
void NintendoWfcHttp_RequestRoomCount(int rank);
void NintendoWfcHttp_RequestDownload(int rank, int room, void *buffer);
void NintendoWfcHttp_RequestUpload(int rank, int room, int winCount, FrontierWifiUploadRecord *record);
void NintendoWfcHttp_RequestInfo(void);
void NintendoWfcHttp_RequestSetProfile(WifiServerProfile *profile, u32 *response);
void NintendoWfcHttp_Init(u32 profileId, u32 friendKeyLo, u32 friendKeyHi);

BOOL LoadOVY38(void);
void UnloadOVY38(void);
void CommWireless_Terminate(void);
void CommState_InitWfcSession(SaveData *saveData);
void CommState_FreeWfcSession(void);
void CommState_ShowRestartErrorAndHalt(void);
void WirelessStatusIcon_SetLevel(s32 level);
u64 DWC_CreateFriendKey(const void *data);
u8 FrontierData_GetWifiWinCount(FrontierData *data);
void BattleRoomDownload_MarkRoom(BattleRoomDownload *data, u32 group, u32 entry, u32 *date);
BOOL BattleRoomDownload_IsRoomMarked(BattleRoomDownload *data, u32 group, u32 entry, u32 *date);
void BattleRoomDownload_SetTrainers(BattleRoomDownload *data, void *records, u32 a, u32 b);
void BattleRoomDownload_GetTrainersRoom(BattleRoomDownload *data, u8 *out);
void BattleRoomDownload_SetLeaders(BattleRoomDownload *data, void *records, u32 a, u32 b);
int RecordMixing_State_AbortLeave(int errorCode, int errorType);

int NintendoWfcUnused_SessionInit(NintendoWfcUnused *wfc, int state);
int NintendoWfcUnused_SessionMain(NintendoWfcUnused *wfc, int state);
int NintendoWfcUnused_SessionExit(NintendoWfcUnused *wfc, int state);
int NintendoWfcUnused_AskConnect(NintendoWfcUnused *wfc);
int NintendoWfcUnused_HandleConnectChoice(NintendoWfcUnused *wfc);
int NintendoWfcUnused_AskLogOut(NintendoWfcUnused *wfc);
int NintendoWfcUnused_HandleLogOutChoice(NintendoWfcUnused *wfc);
int NintendoWfcUnused_StartConnect(NintendoWfcUnused *wfc);
int NintendoWfcUnused_WaitConnect(NintendoWfcUnused *wfc);
int NintendoWfcUnused_StartAuth(NintendoWfcUnused *wfc);
int NintendoWfcUnused_WaitAuth(NintendoWfcUnused *wfc);
int NintendoWfcUnused_InitHttp(NintendoWfcUnused *wfc);
int NintendoWfcUnused_RequestInfo(NintendoWfcUnused *wfc);
int NintendoWfcUnused_WaitInfo(NintendoWfcUnused *wfc);
int NintendoWfcUnused_RequestSetProfile(NintendoWfcUnused *wfc);
int NintendoWfcUnused_WaitSetProfile(NintendoWfcUnused *wfc);
int NintendoWfcUnused_RequestRoomCount(NintendoWfcUnused *wfc);
int NintendoWfcUnused_WaitRoomCount(NintendoWfcUnused *wfc);
int NintendoWfcUnused_AskChallengeRoom(NintendoWfcUnused *wfc);
int NintendoWfcUnused_StartChallengeRoomInput(NintendoWfcUnused *wfc);
int NintendoWfcUnused_HandleChallengeRoomInput(NintendoWfcUnused *wfc);
int NintendoWfcUnused_PrintReceivingTrainers(NintendoWfcUnused *wfc);
int NintendoWfcUnused_PrintRoomUnavailable(NintendoWfcUnused *wfc);
int NintendoWfcUnused_RequestTrainersDownload(NintendoWfcUnused *wfc);
int NintendoWfcUnused_WaitTrainersDownload(NintendoWfcUnused *wfc);
int NintendoWfcUnused_PrintTrainersReceived(NintendoWfcUnused *wfc);
int NintendoWfcUnused_PrintSendingRecord(NintendoWfcUnused *wfc);
int NintendoWfcUnused_RequestUpload(NintendoWfcUnused *wfc);
int NintendoWfcUnused_WaitUpload(NintendoWfcUnused *wfc);
int NintendoWfcUnused_PrintRecordSent(NintendoWfcUnused *wfc);
int NintendoWfcUnused_AskLeadersRank(NintendoWfcUnused *wfc);
int NintendoWfcUnused_StartLeadersRankInput(NintendoWfcUnused *wfc);
int NintendoWfcUnused_HandleLeadersRankInput(NintendoWfcUnused *wfc);
int NintendoWfcUnused_AskLeadersRoom(NintendoWfcUnused *wfc);
int NintendoWfcUnused_StartLeadersRoomInput(NintendoWfcUnused *wfc);
int NintendoWfcUnused_HandleLeadersRoomInput(NintendoWfcUnused *wfc);
int NintendoWfcUnused_PrintReceivingLeaders(NintendoWfcUnused *wfc);
int NintendoWfcUnused_RequestLeadersDownload(NintendoWfcUnused *wfc);
int NintendoWfcUnused_WaitLeadersDownload(NintendoWfcUnused *wfc);
int NintendoWfcUnused_PrintLeadersReceived(NintendoWfcUnused *wfc);
int NintendoWfcUnused_StartDisconnect(NintendoWfcUnused *wfc);
int NintendoWfcUnused_AskCancelChallenge(NintendoWfcUnused *wfc);
int NintendoWfcUnused_HandleCancelChallengeChoice(NintendoWfcUnused *wfc);
int NintendoWfcUnused_AskCancelDownload(NintendoWfcUnused *wfc);
int NintendoWfcUnused_HandleCancelDownloadChoice(NintendoWfcUnused *wfc);
int NintendoWfcUnused_PrintDisconnecting(NintendoWfcUnused *wfc);
int NintendoWfcUnused_CloseConnection(NintendoWfcUnused *wfc);
int NintendoWfcUnused_PrintDisconnected(NintendoWfcUnused *wfc);
int NintendoWfcUnused_UnusedState_Idle(NintendoWfcUnused *wfc);
int NintendoWfcUnused_UnusedState_HandleYesNo(NintendoWfcUnused *wfc);
int NintendoWfcUnused_StartSave(NintendoWfcUnused *wfc);
int NintendoWfcUnused_Save(NintendoWfcUnused *wfc);
int NintendoWfcUnused_StartExit(NintendoWfcUnused *wfc);
int NintendoWfcUnused_ShowHttpError(NintendoWfcUnused *wfc);
int NintendoWfcUnused_Disconnect(NintendoWfcUnused *wfc);
int NintendoWfcUnused_WaitMessage(NintendoWfcUnused *wfc);
int NintendoWfcUnused_WaitMessageAndDelay(NintendoWfcUnused *wfc);
int NintendoWfcUnused_OpenYesNoPrompt(NintendoWfcUnused *wfc);
int NintendoWfcUnused_ShowDwcError(NintendoWfcUnused *wfc);
int NintendoWfcUnused_WaitErrorDismiss(NintendoWfcUnused *wfc);

YesNoPrompt *NintendoWfcUnused_CreateYesNoPrompt(BgConfig *bgConfig, u8 y, int tileStart);
int NintendoWfcUnused_HandleYesNoPrompt(YesNoPrompt *prompt);
void NintendoWfcUnused_SetState(NintendoWfcUnused *wfc, int state, int nextState);
int NintendoWfcUnused_GetWifiIconLevel(NintendoWfcUnused *wfc);
void NintendoWfcUnused_SetNextMode(NintendoWfcUnused *wfc, int mode, int a2);

#endif // POKEHEARTGOLD_NINTENDO_WFC_CONNECT_UNUSED_H
