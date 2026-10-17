#ifndef POKEHEARTGOLD_NINTENDO_WFC_CONNECT_UNUSED_H
#define POKEHEARTGOLD_NINTENDO_WFC_CONNECT_UNUSED_H

#include "global.h"

#include "frontier_wifi_upload_record.h"
#include "msgdata.h"
#include "nintendo_wfc_connect.h"
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

typedef struct NintendoWfcUnusedPaletteCycle {
    SysTask *task;               // 0x000
    int active;                  // 0x004
    u16 basePltt[64];            // 0x008
    u16 cyclePltt[21 * 16];      // 0x088
    s16 index;                   // 0x328
    s8 direction;                // 0x32A
    u8 tick;                     // 0x32B
    u32 screens;                 // 0x32C, 0: both, 1: main only, 2: sub only
} NintendoWfcUnusedPaletteCycle; // size: 0x330

typedef struct NintendoWfcUnused {
    NintendoWifiConnectArgs *args;              // 0x0000
    BgConfig *bgConfig;                         // 0x0004
    u8 filler_0008[8];                          // 0x0008
    int mode;                                   // 0x0010, index into sNintendoWfcUnusedModes
    int nextMode;                               // 0x0014
    int unk_0018;                               // 0x0018
    int state;                                  // 0x001C, index into sNintendoWfcUnusedStates
    int nextState;                              // 0x0020
    void *heapMem;                              // 0x0024
    NNSFndHeapHandle expHeap;                   // 0x0028
    u8 connection[0x64];                        // 0x002C, passed to DWC_InitInetEx
    u32 ownRank;                                // 0x0090
    u32 rank;                                   // 0x0094
    int roomCount;                              // 0x0098
    u32 room;                                   // 0x009C
    u8 downloadBuffer[0xA38];                   // 0x00A0
    FrontierWifiUploadRecord uploadRecord;      // 0x0AD8
    u8 filler_0BBC[0x14];                       // 0x0BBC
    MessageFormat *msgFormat;                   // 0x0BD0
    MsgData *msgDataTower;                      // 0x0BD4, Wi-Fi Battle Tower (bank 799)
    MsgData *msgDataWfc;                        // 0x0BD8, Nintendo WFC (bank 778)
    MsgData *msgDataError;                      // 0x0BDC, Nintendo WFC errors (bank 800)
    String *msgString;                          // 0x0BE0
    String *titleString;                        // 0x0BE4
    String *roomString;                         // 0x0BE8
    String *rankString;                         // 0x0BEC
    String *errorString;                        // 0x0BF0
    u32 textPrinterId;                          // 0x0BF4
    SpriteList *spriteList;                     // 0x0BF8
    G2dRenderer renderer;                       // 0x0BFC
    GF_2DGfxResMan *resMan[4];                  // 0x0D24
    SpriteResource *resObjs[4];                 // 0x0D34
    u8 filler_0D44[0x20];                       // 0x0D44
    SpriteResourcesHeader spriteHeader;         // 0x0D64
    u8 filler_0D88[0x48];                       // 0x0D88
    Sprite *arrows[2];                          // 0x0DD0, up/down arrows next to the number windows
    Window msgWindow;                           // 0x0DD8
    Window rankWindow;                          // 0x0DE8
    Window roomWindow;                          // 0x0DF8
    Window titleWindow;                         // 0x0E08
    Window errorWindow;                         // 0x0E18
    u8 filler_0E28[0xE4];                       // 0x0E28
    WaitingIcon *waitingIcon;                   // 0x0F0C
    int dwcError;                               // 0x0F10
    int dwcErrorCode;                           // 0x0F14
    int dwcErrorType;                           // 0x0F18
    int httpResult;                             // 0x0F1C
    int messageTimer;                           // 0x0F20
    WifiServerProfile profile;                  // 0x0F24
    u32 profileResponse[2];                     // 0x0F88
    s16 subState;                               // 0x0F90
    s16 subTimer;                               // 0x0F92
    int timeoutCounter;                         // 0x0F94
    NintendoWfcUnusedPaletteCycle paletteCycle; // 0x0F98
    YesNoPrompt *yesNoPrompt;                   // 0x12C8
} NintendoWfcUnused;                            // size: 0x12CC

typedef struct NintendoWfcUnusedGlobal {
    NintendoWfcUnused *wfc; // 0x0
    NNSFndHeapHandle heap;  // 0x4, heap of the DWC allocator callbacks
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
// The definition takes (u32 profileId, u64 friendKey); this app passes the key as two words.
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
// A record mixing state; this copy calls it where OVY_72 calls WifiConnection_ClassifyErrorCode.
int RecordMixing_State_AbortLeave(int errorCode, int errorType);

int NintendoWfcUnused_SessionInit(NintendoWfcUnused *wfc, int state);
int NintendoWfcUnused_SessionMain(NintendoWfcUnused *wfc, int state);
int NintendoWfcUnused_SessionExit(NintendoWfcUnused *wfc, int state);
int NintendoWfcUnused_AskConnect(NintendoWfcUnused *wfc);
int NintendoWfcUnused_HandleConnectChoice(NintendoWfcUnused *wfc);
int NintendoWfcUnused_AskLogOut(NintendoWfcUnused *wfc);
int NintendoWfcUnused_HandleLogOutChoice(NintendoWfcUnused *wfc);
void NintendoWfcUnused_StartConnect(NintendoWfcUnused *wfc);
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
int NintendoWfcUnused_UnusedState_Idle(void);
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
