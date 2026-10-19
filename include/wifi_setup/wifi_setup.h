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

// Access point the AOSS exchange runs on (see WifiSetup_InitAossConnectInfo)
typedef struct WifiSetupAossConnectInfo {
    u32 ssidLength;
    u8 ssid[32];
    int mode;
    u32 keyLength;
    u8 key[16];
} WifiSetupAossConnectInfo;

// Address and sequence number carried, scrambled, in each AOSS packet
typedef struct WifiSetupAossTag {
    u8 mac[6];
    u16 sequence;
} WifiSetupAossTag;

// Tags of the join, settings and done requests
typedef struct WifiSetupAossPacketParams {
    WifiSetupAossTag tags[3];
} WifiSetupAossPacketParams;

// One record of a packet body: type, length of the data, data
typedef struct WifiSetupAossRecord {
    u8 type;
    u8 reserved;
    u16 length;
    union {
        u32 status;
        u8 bytes[1];
    } data;
} WifiSetupAossRecord;

typedef struct WifiSetupAossEncryptedBody {
    u16 length;
    u16 seed;
    u8 data[1];
} WifiSetupAossEncryptedBody;

typedef union WifiSetupAossBody {
    WifiSetupAossRecord record;
    WifiSetupAossEncryptedBody encrypted;
    u8 bytes[1476];
} WifiSetupAossBody;

// Packet header (see WifiSetup_WritePacketHeader) followed by the body
typedef struct WifiSetupAossPacket {
    u16 version;
    u16 unk_02;
    u16 unk_04;
    u16 kind;
    u16 unk_08;
    u16 bodyLength;
    u16 flags;
    u8 checksum;
    u8 protocol;
    WifiSetupAossTag tag;
    WifiSetupAossBody body;
} WifiSetupAossPacket;

// Packet received from the access point (gWifiSetupState.data)
typedef struct WifiSetupAossReceived {
    int socket;
    int length;
    u8 padding_08[4];
    WifiSetupAossPacket packet;
} WifiSetupAossReceived;

// WEP keys received from the setup access point: the key length and the four keys
typedef struct WifiSetupKeyRecord {
    u32 unused;
    int length;
    u8 text[40];
    u8 fields[4][64];
} WifiSetupKeyRecord;

// WPA key received from the setup access point
typedef struct WifiSetupWpaKeyRecord {
    u32 unused;
    int length;
    u8 text[40];
    u8 key[64];
} WifiSetupWpaKeyRecord;

// Keys of every security mode received in the settings reply
typedef struct WifiSetupAossKeyRecords {
    u8 padding_00[8];
    WifiSetupKeyRecord wep64;
    WifiSetupKeyRecord wep128;
    WifiSetupWpaKeyRecord wpaTkip;
    WifiSetupWpaKeyRecord wpaAes;
    u8 padding_348[8];
} WifiSetupAossKeyRecords;

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
