#ifndef POKEHEARTGOLD_WIFI_SETUP_INTERNAL_H
#define POKEHEARTGOLD_WIFI_SETUP_INTERNAL_H

#include "global.h"

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

// ov13_0224DF30 (0x80 bytes, up to ov13_0224DFB0)
typedef struct WifiSetupWork {
    u32 unk_00;                          // 0x00
    WifiSetupWorkAllocFunc alloc;        // 0x04
    u32 unk_08;                          // 0x08
    WifiSetupWorkFreeFunc free;          // 0x0C
    u32 stop;                            // 0x10, set to make the worker thread finish
    u32 unk_14;                          // 0x14
    u32 selection;                       // 0x18, index of the chosen access point
    u32 unk_1C;                          // 0x1C
    u32 unk_20;                          // 0x20
    u32 unk_24;                          // 0x24
    u32 unk_28;                          // 0x28
    u32 unk_2C;                          // 0x2C
    void *threadStack;                   // 0x30
    u32 unk_34;                          // 0x34
    s32 state;                           // 0x38, 1-5 while the worker thread runs
    u32 unk_3C;                          // 0x3C
    u32 unk_40;                          // 0x40
    u32 unk_44;                          // 0x44
    u32 unk_48;                          // 0x48
    u32 unk_4C;                          // 0x4C
    int extendedRequest;                 // 0x50, also send records 4 and 5
    u32 unk_54;                          // 0x54
    u32 unk_58;                          // 0x58
    WifiSetupApInfo *apList;             // 0x5C, room for apListCount entries
    u32 unk_60;                          // 0x60
    s32 active;                          // 0x64
    u32 unk_68;                          // 0x68
    u32 unk_6C;                          // 0x6C
    int apListCount;                     // 0x70
    s32 resultReady;                     // 0x74
    s32 error;                           // 0x78
    void (*statusCallback)(s32 *status); // 0x7C, receives {state, ms left, error}
} WifiSetupWork;

// ov13_02245A58
typedef struct WifiSetupConfig {
    u8 *packetBuffer;     // 0x00
    BOOL debug;           // 0x04
    u32 threadStackSize;  // 0x08
    s32 deadline;         // 0x0C, ms tick, -1 = none
    volatile u32 count;   // 0x10
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
// result block handed to the caller (ov13_02226F28)
typedef struct WifiSetupResult {
    u32 words[58];
} WifiSetupResult;

extern WifiSetupResult ov13_0224E09C;

extern const u8 ov13_02245AD8[]; // MD5 padding

int ov13_02223EE0(u32 count);
int ov13_02223F84(void);
void ov13_02224100(void);
int ov13_02224F3C(void);

// wifi_setup_crypto.c
void ov13_02225320(void *arg);
int ov13_02225358(u8 *out, const u8 *in, u32 length, const void *key, u32 keyLength);
int ov13_02225510(u8 *out, const u8 *in, u32 length, const void *key, u32 keyLength);
void ov13_022256C8(const u8 *a, const u8 *b, u8 *out);
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
int ov13_02226D0C(char *dst, int value);
void ov13_02226C94(u8 *digest, const u8 *src, u32 len);
u32 ov13_02226CBC(void);
void *ov13_02226CD8(u32 count, u32 size);
void ov13_02226CFC(void *ptr);
int ov13_02226D40(char *dst, const s8 *mac);
int ov13_02226D74(u32 priority, u32 count, void (*statusCallback)(s32 *), WifiSetupWorkAllocFunc alloc, WifiSetupWorkFreeFunc free, u32 stackSize);
int ov13_02226E5C(void);
int ov13_02226EF0(s32 *status);
int ov13_02226F28(void *dst);
void ov13_02226F3C(void);

#endif // POKEHEARTGOLD_WIFI_SETUP_INTERNAL_H
