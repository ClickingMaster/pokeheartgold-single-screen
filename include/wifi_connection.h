#ifndef POKEHEARTGOLD_WIFI_CONNECTION_H
#define POKEHEARTGOLD_WIFI_CONNECTION_H

#include "global.h"

// Connection states (WifiConnection.state)
#define WIFI_CONNECTION_STATE_INIT              0
#define WIFI_CONNECTION_STATE_WAIT_LINK         1
#define WIFI_CONNECTION_STATE_LOGIN             2
#define WIFI_CONNECTION_STATE_LOGGING_IN        3
#define WIFI_CONNECTION_STATE_LOGGED_IN         4
#define WIFI_CONNECTION_STATE_MATCHING          5
#define WIFI_CONNECTION_STATE_MATCHED           6
#define WIFI_CONNECTION_STATE_CONNECTED         7
#define WIFI_CONNECTION_STATE_CONNECTED_VOICE   8
#define WIFI_CONNECTION_STATE_CANCELLED         9
#define WIFI_CONNECTION_STATE_CANCEL_REQUESTED  10
#define WIFI_CONNECTION_STATE_CANCELLED_BY_PEER 11
#define WIFI_CONNECTION_STATE_ERROR             12
#define WIFI_CONNECTION_STATE_FATAL_ERROR       14
#define WIFI_CONNECTION_STATE_ERROR_CALLBACK    15
#define WIFI_CONNECTION_STATE_DISCONNECTING     16
#define WIFI_CONNECTION_STATE_DISCONNECTED      17
#define WIFI_CONNECTION_STATE_TIMED_OUT         18

typedef struct WifiFriendData {
    u32 data[3];
} WifiFriendData;

typedef void (*WifiReceiveCallback)(u32 aid, void *data, u16 size);

// 0x1108 bytes, 32-byte aligned inside a Heap_Alloc'd block (rawAlloc).
typedef struct WifiConnection {
    u8 sendBuffer[0x100];                    // 0x0000, 4-byte header + payload
    WifiFriendData *friendList;              // 0x0100, 32 entries
    u8 loginControl[0xF10 - 0x104];          // 0x0104, library login control block
    void *userData;                          // 0x0F10
    u8 libConfig[0x64];                      // 0x0F14
    void *saveData;                          // 0x0F78
    void *rawAlloc;                          // 0x0F7C
    void *recvBuffers[4];                    // 0x0F80
    void *heapMemory;                        // 0x0F90
    NNSFndHeapHandle heap;                   // 0x0F94
    void *extraHeapMemory;                   // 0x0F98
    NNSFndHeapHandle extraHeap;              // 0x0F9C
    u32 extraHeapFreeSize;                   // 0x0FA0
    WifiReceiveCallback recvLocalCallback;   // 0x0FA4
    WifiReceiveCallback recvCallback;        // 0x0FA8
    void (*disconnectCallback)(int, void *); // 0x0FAC
    void *disconnectCallbackArg;             // 0x0FB0
    void (*clientCallback)(int, void *);     // 0x0FB4
    void *clientCallbackArg;                 // 0x0FB8
    BOOL (*connectFilterCallback)(int);      // 0x0FBC
    void (*errorCallback)(int);              // 0x0FC0
    char matchFilter[0x80];                  // 0x0FC4
    u8 friendStatus[32];                     // 0x1044
    u8 *friendStatusData;                    // 0x1064
    int friendStatusStride;                  // 0x1068
    int friendStatusIndex;                   // 0x106C
    int state;                               // 0x1070
    int matchType;                           // 0x1074
    int sendBusy;                            // 0x1078
    int maxPlayers;                          // 0x107C
    int conferenceAidMask;                   // 0x1080
    int heapId;                              // 0x1084
    int heapId2;                             // 0x1088
    u32 heapSize;                            // 0x108C
    int voiceChatActive;                     // 0x1090
    int targetFriendIndex;                   // 0x1094
    int newClientIndex;                      // 0x1098
    int voiceChatEnabled;                    // 0x109C
    int matchInProgress;                     // 0x10A0
    int voiceChatMode;                       // 0x10A4
    int recvTimeoutArmed;                    // 0x10A8
    int keepAliveTimers[8];                  // 0x10AC
    int retryCount;                          // 0x10CC
    int peerVoiceFlag;                       // 0x10D0
    u16 ownVoiceFlag;                        // 0x10D4
    u16 voiceUnmuted;                        // 0x10D6
    int matchedAidBitmap;                    // 0x10D8
    u8 sendSequence;                         // 0x10DC
    u8 recvSequence;                         // 0x10DD
    u8 forceError;                           // 0x10DE
    u8 unk_10DF;                             // 0x10DF
    u8 stopMatchingState;                    // 0x10E0
    u8 recvBufferFromGameHeap[4];            // 0x10E1
    u8 disconnectWhenAlone;                  // 0x10E5
    u8 friendDataChanged;                    // 0x10E6
    u8 filler_10E7[0x1108 - 0x10E7];         // 0x10E7
} WifiConnection;

extern WifiConnection *gWifiConnection;

int WifiConnection_Init(void *saveData, int heapId, u32 heapSize, int maxPlayers);
void WifiConnection_Destroy(void);
int WifiConnection_UpdateLogin(void);
void WifiConnection_SetReceiveCallbacks(WifiReceiveCallback recvLocal, WifiReceiveCallback recv);
void WifiConnection_SetDisconnectCallback(void (*callback)(int, void *), void *arg);
void WifiConnection_SetConnectFilterCallback(BOOL (*callback)(int));
void WifiConnection_SetClientCallback(void (*callback)(int, void *), void *arg);
int WifiConnection_StartAnybodyMatchmaking(const char *name, int maxPlayers, int filterLocal, int timeoutSeconds);
void WifiConnection_FinishCancel(void);
int WifiConnection_Update(int success);
int WifiConnection_SendToHost(void *data, int size);
int WifiConnection_SendFromHost(void *data, int size);
int WifiConnection_SendToAll(void *data, int size);
void WifiConnection_LoginCallback(int result, int unused, int arg);
void WifiConnection_TimeoutCallback(void);
void WifiConnection_UpdateServersCallback(u32 result);
void WifiConnection_FriendStatusCallback(void);
void WifiConnection_DeleteFriendCallback(int index, int srcIndex);
void WifiConnection_BuddyFriendCallback(void);
void WifiConnection_ResetKeepAliveTimers(void);
void WifiConnection_SetRecvTimeouts(void);
void WifiConnection_OnConnected(int aid);
void WifiConnection_AnybodyMatchedCallback(int result, int cancelled);
BOOL WifiConnection_EvalPlayerCallback(void);
void WifiConnection_SendDoneCallback(u32 result, s32 aid);
void WifiConnection_ReadPeerVoiceFlag(u32 header);
void WifiConnection_ReceiveCallback(u16 aid, const u8 *data, int size);
void WifiConnection_ConnectionClosedCallback(int result, int unused1, int unused2, int arg);
void *WifiConnection_Alloc(int unused, u32 size, int alignment);
void WifiConnection_Free(int unused, void *ptr, u32 size);
int WifiConnection_CheckError(void);
int WifiConnection_CanSendToAll(void);
int WifiConnection_SendKeepAlive(int aid);
int WifiConnection_UpdateConnected(void);
int WifiConnection_GetAid(void);
void WifiConnection_ClearVoiceChatActive(void);
void WifiConnection_StartVoiceChat(int heapId);
void WifiConnection_StopVoiceChat(void);
int WifiConnection_ClassifyErrorCode(int errorCode, u32 context);
int WifiConnection_Disconnect(int checkOnly);
int WifiConnection_FinishDisconnect(void);
void WifiConnection_SetErrorCallback(void (*callback)(int));
void WifiConnection_Shutdown(void);
void WifiConnection_SetFriendStatusBuffer(u8 *data, int stride);
void WifiConnection_UpdateFriendStatus(void);
void WifiConnection_SetOwnStatusData(void *data, int size);
u8 WifiConnection_GetFriendStatus(int index);
int WifiConnection_StartMatchmaking(int friendIndex, int maxPlayers, BOOL alternate);
int WifiConnection_GetTargetFriendIndex(void);
void WifiConnection_GameServerMatchedCallback(int result, int cancelled, int cancelledByMe, int unused, int aid);
void WifiConnection_ConnectedToServerCallback(u32 result, u32 cancelled, u32 a2, u32 a3, u32 aid);
void WifiConnection_NewClientCallback(int index);
void WifiConnection_CreateTokenAndScanFriends(void);
BOOL WifiConnection_GetVoiceActivity(void);
int WifiConnection_IsVoiceChatEnabled(void);
int WifiConnection_GetNewClientIndex(void);
void WifiConnection_SetVoiceChatEnabled(int value);
void WifiConnection_SetVoiceUnmuted(u16 value);
void WifiConnection_StopServerMatchingCallback(void);
BOOL WifiConnection_StopServerMatching(void);
void WifiConnection_ResetStopServerMatching(void);
void WifiConnection_GetNumConnectionHost(void);
void WifiConnection_SetExtraHeapEnabled(BOOL enable, int heapId);
void WifiConnection_SetDisconnectWhenAlone(u8 value);
void WifiConnection_FreeRecvBuffer(int aid);
void WifiConnection_AllocRecvBuffer(int aid);
void WifiConnection_FreeRecvBuffers(void);
u8 WifiConnection_IsFriendDataChanged(void);
void WifiConnection_ClearFriendDataChanged(void);

#endif // POKEHEARTGOLD_WIFI_CONNECTION_H
