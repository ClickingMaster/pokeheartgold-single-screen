#ifndef POKEHEARTGOLD_WIFI_SETUP_INTERNAL_H
#define POKEHEARTGOLD_WIFI_SETUP_INTERNAL_H

#include "global.h"

#include "wifi_setup/wifi_setup_wcm.h"

// Access-point setup client: worker thread, access-point scan, packet records, AES key wrap, MD5.

typedef void *(*WifiSetupWorkAllocFunc)(u32 size);
typedef void (*WifiSetupWorkFreeFunc)(void *ptr);

// Access point found by the scan (in WifiSetupWork.apList)
typedef struct WifiSetupApInfo {
    u8 unk_00[4];
    u8 bssid[6];
    u16 ssidLength;
    char ssid[32];
    u16 capability; // bit 4: privacy
    u8 unk_2E[0x92];
} WifiSetupApInfo;

// Scan result as compared by WifiSetup_PickSetupAccessPoint
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

typedef struct WifiSetupWork {
    u8 unk_00; // sent as record 0x301
    u8 unk_01[3];
    WifiSetupWorkAllocFunc alloc;
    WifiSetupWcmConfig *wcmConfig;
    WifiSetupWorkFreeFunc free;
    u32 stop; // set to make the worker thread finish
    int packetLength;
    u32 selection; // index of the chosen access point
    int step;      // WifiSetup_RunSession state machine
    u32 connected;
    u32 socketStarted;
    void *connectKey; // key description
    int wcmState;     // see wifi_setup_wcm.h
    void *threadStack;
    u32 recordLength;
    s32 state;     // 1-5 while the worker thread runs
    void *wcmWork; // WCM_Init work area
    void *buffer;  // aligned start of allocation
    void *searchBssid;
    void *searchSsid;
    void *connectAp;     // access point description
    int extendedRequest; // also send records 4 and 5
    void *allocation;
    int connectOption;
    WifiSetupApInfo *apList; // room for apListCount entries
    int searchOption;
    s32 active;
    int eventWrite; // gWifiSetupLinkEventQueue queue
    int eventRead;
    int apListCount;
    s32 resultReady;
    s32 error;
    void (*statusCallback)(s32 *status); // receives {state, ms left, error}
} WifiSetupWork;

// gWifiSetupConfig
typedef struct WifiSetupConfig {
    u8 *packetBuffer;
    BOOL debug;
    u32 threadStackSize;
    s32 deadline; // ms tick, -1 = none
    volatile u32 count;
} WifiSetupConfig;

typedef struct WifiSetupMd5Context {
    u32 state[4];
    u32 bitsLo;
    u32 bitsHi;
    u8 buffer[64];
} WifiSetupMd5Context;

extern WifiSetupWork gWifiSetupWork;
extern WifiSetupConfig gWifiSetupConfig;
extern OSThread gWifiSetupThread;

// result block handed to the caller (WifiSetup_GetSimpleStartResult): the settings received from the
// setup access point
typedef struct WifiSetupResult {
    u8 ssid[32];
    int keyType; // WM_WEPMODE_*, 4-5: WPA
    int unk_24;
    u8 wepKeys[4][32];
    u8 wpaKey[64];
} WifiSetupResult;

extern WifiSetupResult gWifiSetupResult;

extern const u8 sWifiSetupMd5Padding[];

// Connection manager events
int WifiSetupLink_CopyApList(WifiSetupApInfo *dst, int max);
int WifiSetupLink_StartSearch(const u8 *bssid, const u8 *ssid, int len, int option);
int WifiSetupLink_GetState(void);
void *WifiSetupLink_Alloc(int unused, int size);
void WifiSetupLink_Free(int unused, void *ptr, int size);
u32 WifiSetupLink_PopEvent(void);
void WifiSetupLink_PostEvent(u32 event);
int WifiSetupLink_Open(u32 count);
int WifiSetupLink_Close(void);
int WifiSetupLink_ConnectSelectedAp(void);
void WifiSetupLink_DisconnectAp(void);

// Worker thread
int WifiSetup_RunSession(void);

// Access point choice
int WifiSetup_PickSetupAccessPoint(const WifiSetupApEntryList *current, const WifiSetupApEntryList *previous, u32 *selected);

// Thread start-up and AES key wrap
void WifiSetup_SimpleStartThread(void *arg);
int WifiSetup_AesKeyWrap(u8 *out, const u8 *in, u32 length, const void *key, u32 keyLength);
int WifiSetup_AesKeyUnwrap(u8 *out, const u8 *in, u32 length, const void *key, u32 keyLength);
void WifiSetup_XorBlock8(const u8 *a, const u8 *b, u8 *out);
int WifiSetup_AesSetEncryptKey(u32 *roundKeys, const u8 *key, int keyBits);
int WifiSetup_AesSetDecryptKey(u32 *roundKeys, const u8 *key, int keyBits);
void WifiSetup_AesEncryptBlock(const u32 *roundKeys, int rounds, const u8 *in, u8 *out);
void WifiSetup_AesDecryptBlock(const u32 *roundKeys, int rounds, const u8 *in, u8 *out);

// MD5
void WifiSetup_Md5Init(WifiSetupMd5Context *context);
void WifiSetup_Md5Update(WifiSetupMd5Context *context, const u8 *input, u32 len);
void WifiSetup_Md5Final(u8 *digest, WifiSetupMd5Context *context);
void WifiSetup_Md5Transform(WifiSetupMd5Context *context, const u8 *input);

// Byte order helpers
void WifiSetup_EncodeLittleEndian(u8 *dst, const u32 *src, u32 len);
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
