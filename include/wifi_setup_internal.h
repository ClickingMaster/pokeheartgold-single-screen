#ifndef POKEHEARTGOLD_WIFI_SETUP_INTERNAL_H
#define POKEHEARTGOLD_WIFI_SETUP_INTERNAL_H

#include "global.h"

#include "wifi_setup_wcm.h"

// Access-point setup client, second module (OVY_13 Thumb 0x022230F8-0x02226F58): worker thread,
// access-point scan, packet records, AES key wrap, MD5. Shared by src/wifi_setup_packet.c,
// src/wifi_setup_crypto.c and src/wifi_setup_api.c.

typedef void *(*WifiSetupWorkAllocFunc)(u32 size);
typedef void (*WifiSetupWorkFreeFunc)(void *ptr);

// Access point found by the scan (0xC0 bytes each, in WifiSetupWork.apList)
typedef struct WifiSetupApInfo {
    u8 unk_00[4];    // 0x00
    u8 bssid[6];     // 0x04
    u16 ssidLength;  // 0x0A
    char ssid[32];   // 0x0C
    u16 capability;  // 0x2C, bit 4: privacy
    u8 unk_2E[0x92]; // 0x2E
} WifiSetupApInfo;

// Scan result as compared by WifiSetup_PickSetupAccessPoint (0x30 bytes)
typedef struct WifiSetupApEntry {
    u32 ssidLength;
    char ssid[32];
    u8 unk_24[4];
    u8 bssid[6];
    u16 privacy;
} WifiSetupApEntry;

typedef struct WifiSetupApEntryList {
    u32 count;
    WifiSetupApEntry entries[];
} WifiSetupApEntryList;

// ov13_0224DF30 (0x80 bytes, up to ov13_0224DFB0)
typedef struct WifiSetupWork {
    u8 unk_00;                           // 0x00, sent as record 0x301
    u8 unk_01[3];                        // 0x01
    WifiSetupWorkAllocFunc alloc;        // 0x04
    WifiSetupWcmConfig *wcmConfig;       // 0x08
    WifiSetupWorkFreeFunc free;          // 0x0C
    u32 stop;                            // 0x10, set to make the worker thread finish
    int packetLength;                    // 0x14
    u32 selection;                       // 0x18, index of the chosen access point
    int step;                            // 0x1C, WifiSetup_RunSession state machine
    u32 connected;                       // 0x20
    u32 socketStarted;                   // 0x24
    void *connectKey;                    // 0x28, key description (0x60 bytes)
    int wcmState;                        // 0x2C, see wifi_setup_wcm.h
    void *threadStack;                   // 0x30
    u32 recordLength;                    // 0x34
    s32 state;                           // 0x38, 1-5 while the worker thread runs
    void *wcmWork;                       // 0x3C, WCM_Init work area
    void *buffer;                        // 0x40, aligned start of allocation
    void *searchBssid;                   // 0x44
    void *searchSsid;                    // 0x48
    void *connectAp;                     // 0x4C, access point description (0xC0 bytes)
    int extendedRequest;                 // 0x50, also send records 4 and 5
    void *allocation;                    // 0x54
    int connectOption;                   // 0x58
    WifiSetupApInfo *apList;             // 0x5C, room for apListCount entries
    int searchOption;                    // 0x60
    s32 active;                          // 0x64
    int eventWrite;                      // 0x68, ov13_0224DFB4 queue
    int eventRead;                       // 0x6C
    int apListCount;                     // 0x70
    s32 resultReady;                     // 0x74
    s32 error;                           // 0x78
    void (*statusCallback)(s32 *status); // 0x7C, receives {state, ms left, error}
} WifiSetupWork;

// ov13_02245A58
typedef struct WifiSetupConfig {
    u8 *packetBuffer;    // 0x00
    BOOL debug;          // 0x04
    u32 threadStackSize; // 0x08
    s32 deadline;        // 0x0C, ms tick, -1 = none
    volatile u32 count;  // 0x10
} WifiSetupConfig;

typedef struct WifiSetupMd5Context {
    u32 state[4];
    u32 bitsLo;
    u32 bitsHi;
    u8 buffer[64];
} WifiSetupMd5Context;

extern WifiSetupWork ov13_0224DF30;
extern WifiSetupConfig ov13_02245A58;
extern OSThread ov13_0224DFDC;

// result block handed to the caller (WifiSetup_GetSimpleStartResult)
typedef struct WifiSetupResult {
    u32 words[58];
} WifiSetupResult;

extern WifiSetupResult ov13_0224E09C;

extern const u8 ov13_02245AD8[]; // MD5 padding

// wifi_setup_link.c
void WifiSetupLink_HandleEvent(WifiSetupWcmEvent *event);
int WifiSetupLink_ReturnToIdle(void);
int WifiSetupLink_CopyApList(WifiSetupApInfo *dst, int max);
int WifiSetupLink_StartSearch(const u8 *bssid, const u8 *ssid, int len, int option);
int WifiSetupLink_StartDisconnect(void);
int WifiSetupLink_StartCleanup(void);
int WifiSetupLink_StartConnect(void *ap, void *key, int option);
int WifiSetupLink_Init(void (*eventCallback)(u32 event, int arg), u8 *buffer, int size);
int WifiSetupLink_GetState(void);
void *WifiSetupLink_Alloc(int unused, int size);
void WifiSetupLink_Free(int unused, void *ptr, int size);
void WifiSetupLink_ClearEvents(void);
u32 WifiSetupLink_PopEvent(void);
void WifiSetupLink_PushEvent(u32 event, int unused);
void WifiSetupLink_PostEvent(u32 event);
int WifiSetupLink_Open(u32 count);
int WifiSetupLink_Close(void);
int WifiSetupLink_ConnectSelectedAp(void);
void WifiSetupLink_DisconnectAp(void);

// wifi_setup_worker.c
int WifiSetup_RunSession(void);

// wifi_setup_ap_select.c
int WifiSetup_PickSetupAccessPoint(const WifiSetupApEntryList *current, const WifiSetupApEntryList *previous, u32 *selected);

// wifi_setup_crypto.c
void WifiSetup_SimpleStartThread(void *arg);
int WifiSetup_AesKeyWrap(u8 *out, const u8 *in, u32 length, const void *key, u32 keyLength);
int WifiSetup_AesKeyUnwrap(u8 *out, const u8 *in, u32 length, const void *key, u32 keyLength);
void WifiSetup_XorBlock8(const u8 *a, const u8 *b, u8 *out);
int WifiSetup_AesSetEncryptKey(u32 *roundKeys, const u8 *key, int keyBits);
int WifiSetup_AesSetDecryptKey(u32 *roundKeys, const u8 *key, int keyBits);
void WifiSetup_AesEncryptBlock(const u32 *roundKeys, int rounds, const u8 *in, u8 *out);
void WifiSetup_AesDecryptBlock(const u32 *roundKeys, int rounds, const u8 *in, u8 *out);

// wifi_setup_crypto.c (MD5) and the MD5 block transform
void WifiSetup_Md5Init(WifiSetupMd5Context *context);
void WifiSetup_Md5Update(WifiSetupMd5Context *context, const u8 *input, u32 len);
void WifiSetup_Md5Final(u8 *digest, WifiSetupMd5Context *context);
void WifiSetup_Md5Transform(WifiSetupMd5Context *context, const u8 *input);

// wifi_setup_api.c
void WifiSetup_EncodeLittleEndian(u8 *dst, const volatile u32 *src, u32 len);
void WifiSetup_DecodeLittleEndian(u32 *dst, const u8 *src, u32 len);
void WifiSetup_CopyBytes(u8 *dst, const u8 *src, u32 len);
void WifiSetup_FillBytes(void *dst, int value, u32 count);
int WifiSetup_FormatHexByte(char *dst, int value);
void WifiSetup_CalcMd5(u8 *digest, const u8 *src, u32 len);
u32 WifiSetup_GetMilliseconds(void);
void *WifiSetup_WorkCalloc(u32 count, u32 size);
void WifiSetup_WorkFree(void *ptr);
int WifiSetup_FormatMacAddress(char *dst, const s8 *mac);
int WifiSetup_StartSimpleStart(u32 priority, u32 count, void (*statusCallback)(s32 *), WifiSetupWorkAllocFunc alloc, WifiSetupWorkFreeFunc free, u32 stackSize);
int WifiSetup_StopSimpleStart(void);
int WifiSetup_GetSimpleStartStatus(s32 *status);
int WifiSetup_GetSimpleStartResult(void *dst);
void WifiSetup_ReportSimpleStartStatus(void);

#endif // POKEHEARTGOLD_WIFI_SETUP_INTERNAL_H
