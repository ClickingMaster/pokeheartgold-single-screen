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
int WifiConnection_UpdateLogin(void);
void WifiConnection_SetReceiveCallbacks(WifiReceiveCallback recvLocal, WifiReceiveCallback recv);
void WifiConnection_SetDisconnectCallback(void (*callback)(int, void *), void *arg);
void WifiConnection_SetConnectFilterCallback(BOOL (*callback)(int));
void WifiConnection_SetClientCallback(void (*callback)(int, void *), void *arg);
int WifiConnection_StartAnybodyMatchmaking(const char *name, int maxPlayers, int filterLocal, int timeoutSeconds);
int WifiConnection_Update(int success);
int WifiConnection_SendToHost(void *data, int size);
int WifiConnection_SendFromHost(void *data, int size);
int WifiConnection_SendToAll(void *data, int size);
int WifiConnection_CheckError(void);
int WifiConnection_GetAid(void);
void WifiConnection_StartVoiceChat(int heapId);
void WifiConnection_StopVoiceChat(void);
int WifiConnection_ClassifyErrorCode(int errorCode, u32 context);
int WifiConnection_Disconnect(int checkOnly);
int WifiConnection_FinishDisconnect(void);
void WifiConnection_SetErrorCallback(void (*callback)(int));
void WifiConnection_Shutdown(void);
void WifiConnection_SetFriendStatusBuffer(u8 *data, int stride);
void WifiConnection_SetOwnStatusData(void *data, int size);
u8 WifiConnection_GetFriendStatus(int index);
int WifiConnection_StartMatchmaking(int friendIndex, int maxPlayers, BOOL alternate);
int WifiConnection_GetTargetFriendIndex(void);
BOOL WifiConnection_GetVoiceActivity(void);
int WifiConnection_IsVoiceChatEnabled(void);
int WifiConnection_GetNewClientIndex(void);
void WifiConnection_SetVoiceChatEnabled(int value);
void WifiConnection_SetVoiceUnmuted(u16 value);
BOOL WifiConnection_StopServerMatching(void);
void WifiConnection_ResetStopServerMatching(void);
int WifiConnection_GetNumConnectionHost(void);
void WifiConnection_SetExtraHeapEnabled(BOOL enable, int heapId);
void WifiConnection_SetDisconnectWhenAlone(u8 value);
u8 WifiConnection_IsFriendDataChanged(void);
void WifiConnection_ClearFriendDataChanged(void);

#endif // POKEHEARTGOLD_WIFI_CONNECTION_H
