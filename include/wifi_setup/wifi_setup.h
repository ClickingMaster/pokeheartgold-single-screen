#ifndef POKEHEARTGOLD_WIFI_SETUP_H
#define POKEHEARTGOLD_WIFI_SETUP_H

#include "global.h"

#include "nitrowifi/socl_internal.h"

// Access-point setup client (OVY_13 Thumb library): scans for the access point, exchanges
// UDP packets with it on the local network and reads the connection settings it hands out.

typedef void *(*WifiSetupAllocFunc)(u32 size);
typedef void (*WifiSetupFreeFunc)(void *ptr);

typedef struct WifiSetupRuntime {
    WifiSetupAllocFunc alloc;   // 0x00
    s32 (*callback)(s32 event); // 0x04, see WifiSetup_NotifyStatus
    void *allocation;           // 0x08, wireless work buffer
    WifiSetupFreeFunc free;     // 0x0C, also addressed through its own label gWifiSetupFreeHook
} WifiSetupRuntime;

typedef struct WifiSetupState {
    int encryptBody;      // 0x00
    void *connection;     // 0x04
    BOOL randomSeeded;    // 0x08
    int *status;          // 0x0C
    int error;            // 0x10
    void *data;           // 0x14
    u32 randomState;      // 0x18
    u32 randomMultiplier; // 0x1C
    u32 randomIncrement;  // 0x20
    void *key;            // 0x24
    int keyLength;        // 0x28
    u32 requestedMask;    // 0x2C, settings the caller asked for
    u32 receivedMask;     // 0x30, settings received so far
    u32 address;          // 0x34
    u32 netmask;          // 0x38
} WifiSetupState;

// Request/result block passed in by the caller of WifiSetup_RunAoss.
typedef struct WifiSetupRequest {
    u16 requestMask;    // 0x000, which settings to fetch
    u8 unk_002;         // 0x002
    u8 unk_003;         // 0x003
    u16 keyLength;      // 0x004, including the terminator
    u8 key[256];        // 0x006
    s16 connectRetries; // 0x106, -1 = default
    s16 connectDelay;   // 0x108
    s16 receiveRetries; // 0x10A
    s16 receiveDelay;   // 0x10C
    s16 receiveTimeout; // 0x10E
    u8 macAddress[6];   // 0x110
    u8 error;           // 0x116
    u8 result[0x154];   // 0x117
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
