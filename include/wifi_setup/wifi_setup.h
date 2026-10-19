#ifndef POKEHEARTGOLD_WIFI_SETUP_H
#define POKEHEARTGOLD_WIFI_SETUP_H

#include "global.h"

#include "nitrowifi/socl_internal.h"

// Access-point setup client: scans for the access point, exchanges UDP packets with it on the local network and
// reads the connection settings it hands out.

typedef void *(*WifiSetupAllocFunc)(u32 size);
typedef void (*WifiSetupFreeFunc)(void *ptr);

typedef struct WifiSetupRuntime {
    WifiSetupAllocFunc alloc;
    s32 (*callback)(s32 event); // see WifiSetup_NotifyStatus
    void *allocation;           // wireless work buffer
    WifiSetupFreeFunc free;
} WifiSetupRuntime;

typedef struct WifiSetupState {
    int encryptBody;
    void *connection;
    BOOL randomSeeded;
    int *status;
    int error;
    void *data;
    u32 randomState;
    u32 randomMultiplier;
    u32 randomIncrement;
    void *key;
    int keyLength;
    u32 requestedMask; // settings the caller asked for
    u32 receivedMask;  // settings received so far
    u32 address;
    u32 netmask;
} WifiSetupState;

// Request/result block passed in by the caller of WifiSetup_RunAoss.
typedef struct WifiSetupRequest {
    u16 requestMask; // which settings to fetch
    u8 unk_002;
    u8 unk_003;
    u16 keyLength; // including the terminator
    u8 key[256];
    s16 connectRetries; // -1 = default
    s16 connectDelay;
    s16 receiveRetries;
    s16 receiveDelay;
    s16 receiveTimeout;
    u8 macAddress[6];
    u8 error;
    u8 result[0x154];
} WifiSetupRequest;

typedef struct WifiSetupCipher {
    u32 x;
    u32 y;
    u8 *table;
    u32 size;
} WifiSetupCipher;

typedef struct WifiSetupPollTime {
    int seconds;
    int microseconds;
} WifiSetupPollTime;

extern WifiSetupState gWifiSetupState;
extern WifiSetupRuntime gWifiSetupRuntime;
extern WifiSetupFreeFunc gWifiSetupFreeHook;
extern OSMessage gWifiSetupMessages[4];
extern OSMessageQueue gWifiSetupMessageQueue;

void *WifiSetup_Alloc(int size);
void WifiSetup_Free(void *ptr);
int WifiSetup_RunAoss(WifiSetupRequest *request);
int WifiSetup_RunAossSession(WifiSetupRequest *request);
int WifiSetup_StoreAossResult(WifiSetupRequest *);
u32 WifiSetup_NextHostAddress(u32 value, u32 mask);
void WifiSetup_FreeWorkBuffers(void);
void WifiSetup_InitState(WifiSetupRequest *request);
void WifiSetup_SetError(int error);
int WifiSetup_GetError(void);
int WifiSetup_SetStatus(int value);
void *WifiSetup_SocketAlloc(s32 unused, s32 size);
void *WifiSetup_SocketFree(s32 unused, s32 ptr);
int WifiSetup_StartupSocket(u32 address, u32 netmask, u32 gateway);
int WifiSetup_CleanupSocket(void);
int WifiSetup_MemCompare(const u8 *a, const u8 *b, int n);
void WifiSetup_MemCopy(void *dst, const void *src, u32 size);
void WifiSetup_MemFill(void *dst, int value, u32 size);
int WifiSetup_RecvFrom(int socket, void *buffer, int size, int flags, SocAddress *address, int *addressLength);
int WifiSetup_Poll(int count, SocPollFd *readSet, void *writeSet, void *exceptSet, WifiSetupPollTime *timeout);
int WifiSetup_SendTo(int socket, const void *buffer, int size, int flags, SocAddress *address, int addressLength);
int WifiSetup_SetSockOpt(int socket, int level, int name, const void *value, int length);
int WifiSetup_Socket(int domain, int type, int protocol);
int WifiSetup_Bind(int socket, SocAddress *address, int addressLength);
int WifiSetup_Close(int socket);
u32 WifiSetup_HostToNet32(u32 x);
u16 WifiSetup_HostToNet16(int x);
u32 WifiSetup_NetToHost32(u32 x);
u16 WifiSetup_NetToHost16(int x);
s32 WifiSetup_StrLen(const char *str);
u16 WifiSetup_Random(void);
void WifiSetup_ClearPollSet(SocPollFd *set);
void WifiSetup_AddToPollSet(int socket, SocPollFd *set);
void WifiSetup_CopyConnectSsid(const void *src, void *dst);
void WifiSetup_StoreConnectResult(void *dst, const void *src, u32 value);
BOOL WifiSetup_OnTimeoutAlarm(void *message);
int WifiSetup_Startup(WifiSetupAllocFunc alloc, WifiSetupFreeFunc free);
int WifiSetup_Cleanup(void);
int WifiSetup_SearchAccessPoints(void **output);

#endif // POKEHEARTGOLD_WIFI_SETUP_H
