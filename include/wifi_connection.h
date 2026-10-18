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

typedef void (*WifiReceiveCallback)(u32 aid, void *data, u16 size);

typedef struct WifiConnection WifiConnection;

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
void WifiConnection_LoginCallback(int result, int unused, void *param);
void WifiConnection_TimeoutCallback(void);
void WifiConnection_UpdateServersCallback(int result, int unused, void *param);
void WifiConnection_FriendStatusCallback(void);
void WifiConnection_DeleteFriendCallback(int index, int srcIndex);
void WifiConnection_BuddyFriendCallback(void);
void WifiConnection_ResetKeepAliveTimers(void);
void WifiConnection_SetRecvTimeouts(void);
void WifiConnection_OnConnected(int aid);
void WifiConnection_AnybodyMatchedCallback(int result, int cancelled, void *param);
BOOL WifiConnection_EvalPlayerCallback(void);
void WifiConnection_SendDoneCallback(u32 result, s32 aid);
void WifiConnection_ReadPeerVoiceFlag(u32 header);
void WifiConnection_ReceiveCallback(u16 aid, const u8 *data, int size);
void WifiConnection_ConnectionClosedCallback(int result, BOOL isLocal, BOOL isServer, u32 aid, int index, void *param);
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
void WifiConnection_GameServerMatchedCallback(int result, BOOL cancelled, BOOL self, BOOL isServer, int aid, void *param);
void WifiConnection_ConnectedToServerCallback(int result, BOOL cancelled, BOOL self, BOOL isServer, int aid, void *param);
void WifiConnection_NewClientCallback(int index);
void WifiConnection_CreateTokenAndScanFriends(void);
BOOL WifiConnection_GetVoiceActivity(void);
int WifiConnection_IsVoiceChatEnabled(void);
int WifiConnection_GetNewClientIndex(void);
void WifiConnection_SetVoiceChatEnabled(int value);
void WifiConnection_SetVoiceUnmuted(u16 value);
void WifiConnection_StopServerMatchingCallback(void *param);
BOOL WifiConnection_StopServerMatching(void);
void WifiConnection_ResetStopServerMatching(void);
int WifiConnection_GetNumConnectionHost(void);
void WifiConnection_SetExtraHeapEnabled(BOOL enable, int heapId);
void WifiConnection_SetDisconnectWhenAlone(u8 value);
void WifiConnection_FreeRecvBuffer(int aid);
void WifiConnection_AllocRecvBuffer(int aid);
void WifiConnection_FreeRecvBuffers(void);
u8 WifiConnection_IsFriendDataChanged(void);
void WifiConnection_ClearFriendDataChanged(void);

#endif // POKEHEARTGOLD_WIFI_CONNECTION_H
