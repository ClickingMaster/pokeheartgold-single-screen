#pragma force_active on
#pragma warn_unusedvar off
// clang-format off
#include "global.h"
#include <nitro/code32.h>
#include "dgt.h"
// clang-format on

typedef struct KeyTable {
    s8 *keys[4];
} KeyTable;

// clang-format on

typedef struct WProfile {
    u8 unk00[0x40];
    u8 ssid[32];     // 0x40
    u8 ssid2[32];    // 0x60
    u8 unk80[0x40];
    u8 ip[4];        // 0xc0
    u8 gateway[4];   // 0xc4
    u8 dns1[4];      // 0xc8
    u8 dns2[4];      // 0xcc
    u8 prefix;       // 0xd0
    u8 unkD1[0x16];
    u8 wepMode;      // 0xe7
    u8 unkE8[0x18];
} WProfile;

typedef struct WSsidEntry {
    u8 flags : 4;    // 0x0
    u8 flagsHi : 4;
    u8 index;        // 0x1
    u8 unk2;
    u8 len;          // 0x3
    u8 ssid[32];     // 0x4
} WSsidEntry;

typedef struct WApInfo {
    u8 status;       // 0x0
    u8 unk1;
    u8 rssi;         // 0x2
    u8 ch : 7;       // 0x3
    u8 done : 1;
} WApInfo;

typedef struct WBss {
    u16 length;
    u16 rssi;
    u8 bssid[6];     // 0x4
    u16 ssidLength;  // 0xa
    u8 ssid[32];     // 0xc
    u8 unk2C[0xa];
    u16 channel;     // 0x36
    u8 unk38[0x88];
} WBss;

typedef struct WCtx {
    WProfile profiles[3];   // 0x000
    WSsidEntry entries[9];  // 0x300
    WApInfo ap[11];         // 0x444
    WBss bss[11];           // 0x470
    OSTick tick;            // 0xcb0
    u8 unkCB8[0x52];
    u8 unkD0A;              // 0xd0a
    u8 d0bLo : 4;           // 0xd0b
    u8 retry : 4;
    u8 profileSel : 4;      // 0xd0c
    u8 d0cB4 : 2;
    u8 mode : 2;
    u8 profileIdx;          // 0xd0d
    u8 nextState;           // 0xd0e
    u8 cur;                 // 0xd0f
    u8 numEntries;          // 0xd10
    s8 channel;             // 0xd11
    u8 numAps;              // 0xd12
    u8 selAp;               // 0xd13
    u8 unkD14;
    u8 chIter;              // 0xd15
    u16 chMask;             // 0xd16
} WCtx;

#define TICKS_TO_MS(t) (((t) * 64) / 33514)
#define TICKS_TO_SEC(t) (((t) * 64) / 33514000)

extern void *ov00_021FAE14(int id);
extern int ov00_021FAECC(void);
extern void ov00_021FAEE8(int err);
extern int ov00_021FAFC8(int idx);
extern void ov00_021FAF20(int idx);
extern int ov00_021FBF24(int a);
extern u8 _02108FC0[];
extern u8 _02108FC8[];
extern u32 ov00_02216074[];
extern void ov00_021FB62C(void *arg);

extern int WCM_GetPhase(void);
extern int WCM_EndSearchAsync(void);
extern int WCM_DisconnectAsync(void);
extern int WCM_TerminateAsync(void);
extern int WCM_SearchAsync(void *bssid, void *ssid, u32 option);
extern int WCM_StartupAsync(void *config, void *cb);


extern int ov00_021EA340(void *p);
extern void ov00_021EA420(void);
extern void ov00_021EA518(void);
extern int ov00_021EA580(void);
extern u32 ov00_021EA5C0(void);
extern void *ov00_021EC2A8(int kind, u32 size);
extern void ov00_021EC2EC(int kind, void *p, int x);
extern u32 ov00_021FF368(void);
extern void ov00_021FF3CC(void);
extern int ov00_021FF694(int sock);
extern int ov00_021FF804(int a, int b, int c);
extern void ov00_021FF818(int sock);
extern int ov00_021FF940(int sock, void *buf, int len, int flags, void *addr, int *addrlen);
extern int ov00_021FF984(int sock, void *buf, int len, int flags, void *addr, int addrlen);
extern u32 ov00_021FFA44(const char *s);
extern int _021D4420;
extern u8 ov00_022160A8[];
extern u8 ov00_02216103[];
extern u8 ov00_02217C0C[];
extern u8 ov00_02217C18[];
extern KeyTable ov00_02217BA4;
extern u8 ov00_0221611C[];
extern u8 ov00_0221612C[];
extern u8 ov00_0221613C[];
extern char ov00_02217C24[];
extern char ov00_02217C48[];

extern int SOC_Startup(void *cfg);
extern int SOC_Cleanup(void);
extern int SOCL_CalmDown(void);
extern u32 SOC_GetHostID(void);
extern void SOC_SetResolver(u32 *dns1, u32 *dns2);
extern void *SOC_GetHostByName(const char *name);
extern int sprintf(char *, const char *, ...);

typedef struct SocCfg {
    u32 u0;
    u32 u4;
    u32 u8;
    u32 useDhcp; // 0xc
    u32 ip;      // 0x10
    u32 mask;    // 0x14
    u32 gw;      // 0x18
    u32 dns1;    // 0x1c
    u32 dns2;    // 0x20
} SocCfg;

typedef struct WUnk1 {
    u8 unk0[0x10];
    u32 u10;
    u8 u14;
    u8 u15;
} WUnk1;

typedef struct Bytes24 {
    u8 b[24];
} Bytes24;

typedef struct WMEntry {
    u8 data[0x6e];
    u16 crc;
} WMEntry;

typedef struct WMScanParam {
    void *buf;          // 0x1300
    u16 bufSize;        // 0x1304
    u16 channelList;    // 0x1306
    u16 maxChannelTime; // 0x1308
    u8 bssid[6];        // 0x130a
    u16 scanType;       // 0x1310
    u8 unk12[0x32];
} WMScanParam;

typedef struct WGlobal {
    u8 wm[0xf00];
    u8 scanBuf[0x400];     // 0xf00
    WMScanParam param;     // 0x1300
    void (*cb)(void *);    // 0x1344
    WMEntry entries[16];   // 0x1348
    u16 u1a48[16];         // 0x1a48
    u16 u1a68[16];         // 0x1a68
    int count;             // 0x1a88
    int writeIdx;          // 0x1a8c
    u32 state;             // 0x1a90
    u16 crc;               // 0x1a94
    u16 crcTable[256];     // 0x1a96
} WGlobal;

extern WGlobal *ov00_0221B0C0;

typedef struct WScanBss {
    u16 length;
    u16 rssi;      // 0x2
    u8 unk4[2];
    u8 b6[4];      // 0x6
    u8 unkA[0x36];
    u16 u40;       // 0x40
    u16 unk42;
    u32 u44;       // 0x44
    u16 u48;       // 0x48
    u8 unk4A[6];
    u8 data[0x70]; // 0x50
} WScanBss;

typedef struct WScanCb {
    u16 apiid;
    u16 errcode;   // 0x2
    u16 unk4;
    u16 unk6;
    u16 state;     // 0x8
    u16 unkA;
    u16 unkC;
    u16 bssCount;  // 0xe
    WScanBss *bss[16]; // 0x10
} WScanCb;

extern u16 MATH_CalcCRC16(const MATHCRC16Table *table, const void *data, u32 len);
extern void DC_InvalidateRange(void *p, u32 size);
extern void CRYPTO_RC4FastInit(void *ctx, const void *key, u32 len);
extern void CRYPTO_RC4FastEncrypt(void *ctx, const void *src, u32 len, void *dst);

typedef struct WOut {
    u32 valid;
    u16 u4;
    u8 data[0x70];
    u8 pad[2];
} WOut;

typedef struct HEntry {
    u32 valid;       // 0x00
    u16 rssi;        // 0x04
    u8 name[0x20];   // 0x06
    u8 key[10];      // 0x26
    u8 unk30[0x1a];
    u8 pass[0x20];   // 0x4a
    u8 unk6a;
    u8 mode;         // 0x6b
    u8 flags;        // 0x6c
    u8 unk6d[0xb];
} HEntry;

typedef struct HDest {
    u8 name[0x20];
    u8 pass[0x20];
    u32 mode;
    u8 key[10];
} HDest;

typedef struct HA {
    HDest *dest;
    u32 tick[2];
    HEntry entries[16];
    int best;
} HA;

typedef struct HState {
    HA *a;
    void *b;
    int state;
} HState;
extern HState ov00_0221B0C4;

typedef struct NSockAddr {
    u8 len;
    u8 family;
    u16 port;
    u32 addr;
} NSockAddr;

typedef struct IState {
    int state;       // 0x00
    int sock;        // 0x04
    NSockAddr addr;  // 0x08
    u8 buf[0x40];    // 0x10
    int len;         // 0x50
    u32 time;        // 0x54
    int retries;     // 0x58
} IState;
extern IState ov00_0221B0D0;
extern NSockAddr ov00_0221B0D8;
extern u8 ov00_0221B0DC[];
extern u8 ov00_0221B0E0[];
extern u8 ov00_0221B0E5[];
extern s8 ov00_0221B12C[];
extern char ov00_0221B16C[];

extern int WM_Initialize(void *wm, void *cb, int dma);
extern int WM_End(void *cb);
extern u16 WM_GetAllowedChannel(void);
extern u16 WM_GetDispersionScanPeriod(void);
extern int WM_StartScanEx(void *cb, void *param);
extern int WM_EndScan(void *cb);

int ov00_021FC150(WCtx *ctx);
int ov00_021FC258(WCtx *ctx);
int ov00_021FC2E8(WCtx *ctx);
int ov00_021FC32C(void);
int ov00_021FC408(void);
void ov00_021FC4CC(u32 ch);
s8 ov00_021FC500(int n);
void ov00_021FC568(int state);
int ov00_021FC648(WCtx *ctx);
int ov00_021FC698(WCtx *ctx);
int ov00_021FC73C(WCtx *ctx);
int ov00_021FC784(WCtx *ctx);
int ov00_021FC8AC(WCtx *ctx);
int ov00_021FC9CC(WCtx *ctx, int state);
int ov00_021FCA90(WCtx *ctx, int state);
int ov00_021FCB54(WCtx *ctx);
int ov00_021FCBB4(void *bssid, void *ssid, int ch, u32 option);
int ov00_021FCD5C(WCtx *ctx);
int ov00_021FCDC0(WCtx *ctx);
int ov00_021FCE50(void);
int ov00_021FCE7C(WCtx *ctx);
int ov00_021FCEE8(WCtx *ctx);
int ov00_021FCEFC(void);
void ov00_021FCF2C(u32 *src, WCtx *ctx, SocCfg *cfg);
u32 ov00_021FCFEC(u8 *b);
u32 ov00_021FD040(int prefix);
void ov00_021FD094(WCtx *ctx);
void ov00_021FD1FC(u8 *src, u8 *dst);
void ov00_021FD300(u8 *src, u8 *out);
void ov00_021FD34C(u8 *src, u8 *dst);
int ov00_021FD4F8(u8 c);
int ov00_021FD568(u8 *src, u8 *dst, u32 len, u32 max);
u8 ov00_021FD6D0(int x);
void ov00_021FD6E4(void *arg);
u32 ov00_021FD9C0(void);
int ov00_021FDB9C(int idx, WOut *out);
int ov00_021FD9CC(void *buf, void *cb, int dma);
int ov00_021FDA5C(void *cb);
int ov00_021FDA9C(void (*cb)(void *));
int ov00_021FDB64(void *cb);
int ov00_021FDC38(void *out);
int ov00_021FE0CC(u8 *s);
void ov00_021FE100(void *arg);
void ov00_021FE114(void *arg);
void ov00_021FE128(void *arg);
void ov00_021FE13C(void *arg);
void ov00_021FE150(void *arg);
int ov00_021FE164(const char *host, int port, NSockAddr *addr);
void ov00_021FE1D8(void);
int ov00_021FE320(s8 *buf, int len, NSockAddr *from, u32 *out);

static inline BOOL CopySsid(WSsidEntry *e, u8 *ssid, s32 idx) {
    u8 j;
    for (j = 0; j < 32; j++) {
        if (ssid[j] == 0) {
            break;
        }
        e->ssid[j] = ssid[j];
    }
    if (j != 0) {
        e->len = j;
        e->index = idx;
        return TRUE;
    }
    return FALSE;
}


int ov00_021FD11C(u8 *src);
void ov00_021FD154(u8 *src, u8 *out);
void ov00_021FD17C(u8 *src, u8 *out);
int ov00_021FD1C8(u8 *src);
void ov00_021FD1EC(u8 *src, u8 *dst);

// @fn ov00_021FD11C
int ov00_021FD11C(u8 *src) {
    u8 buf[0x18];
    ov00_021FD1FC(src, buf);
    return memcmp(buf, ov00_02217C0C, 8) == 0;
}

// @fn ov00_021FD154
void ov00_021FD154(u8 *src, u8 *out) {
    u8 buf[0x18];
    ov00_021FD1FC(src, buf);
    ov00_021FD300(buf, out);
}

// @fn ov00_021FD17C
void ov00_021FD17C(u8 *src, u8 *out) {
    u8 buf[0x18];
    ov00_021FD1FC(src, buf);
    if (memcmp(buf, ov00_02217C0C, 8)) {
        return;
    }
    MI_CpuCopy8(buf + 8, out, 10);
}

// @fn ov00_021FD1C8
int ov00_021FD1C8(u8 *src) {
    return memcmp(src, ov00_02217C18, 8) == 0;
}

// @fn ov00_021FD1EC
void ov00_021FD1EC(u8 *src, u8 *dst) {
    ov00_021FD34C(src + 0xc, dst);
}

// @fn ov00_021FD1FC
void ov00_021FD1FC(u8 *src, u8 *dst) {
    Bytes24 tbl = *(Bytes24 *)ov00_02216103;
    int i;
    u8 j;
    u8 k;
    u8 t;
    u8 next;
    u8 *old;
    ov00_021FD568(src, dst, 0x20, 0x18);
    for (i = 0; i < 0x18; i++) {
        dst[i] ^= ov00_02217BA4.keys[2][i];
    }
    for (i = 0; i < 0x18; i++) {
        j = i;
        k = j;
        t = dst[i];
        while (tbl.b[j] != 0xFF) {
            old = &tbl.b[k];
            k = *old;
            next = dst[k];
            dst[tbl.b[j]] = t;
            *old = 0xFF;
            j = k;
            t = next;
        }
    }
    for (i = 0; i < 0x18; i++) {
        dst[i] ^= ov00_02217BA4.keys[0][i];
    }
}

// @fn ov00_021FD300
void ov00_021FD300(u8 *src, u8 *out) {
    struct DGTHash1Context ctx;
    u8 digest[20];
    DGT_Hash1Reset(&ctx);
    DGT_Hash1SetSource(&ctx, src, 0x18);
    DGT_Hash1GetDigest_R(digest, &ctx);
    MI_CpuCopy8(digest + 3, out, 13);
}

// @fn ov00_021FD34C
void ov00_021FD34C(u8 *src, u8 *dst) {
    int i;
    u8 tmp[13];
    u8 *p;
    u8 *q;
    int v;
    for (i = 0; i < 13; i++) {
        dst[i] = src[i] ^ src[0xd + i % 7];
    }
    for (i = 0; i < 7; i++) {
        dst[3 + i] ^= src[0xd + i];
    }
    for (i = 0; i < 13; i++) {
        dst[i] ^= ov00_02217BA4.keys[1][i];
    }
    MI_CpuCopy8(dst, tmp, 13);
    for (i = 0; i < 13; i++) {
        dst[ov00_0221611C[i]] = tmp[i];
    }
    for (i = 0; i < 13; i++) {
        dst[i] ^= ov00_02217BA4.keys[3][i];
    }
    for (i = 0; i < 13; i++) {
        v = dst[i];
        dst[i] = (ov00_0221612C[(v >> 4) & 0xF] << 4) | ov00_0221612C[v & 0xF];
    }
    for (i = 0; i < 3; i++) {
        dst[i] ^= dst[i + 6];
        dst[i + 3] ^= dst[i + 9];
        dst[i + 6] ^= dst[i + 3];
        dst[i + 9] ^= dst[i];
        dst[12] ^= dst[i];
    }
}

// @fn ov00_021FD4F8
int ov00_021FD4F8(u8 c) {
    if (c >= 'A' && c <= 'Z') {
        return c - 'A';
    }
    if (c >= 'a' && c <= 'z') {
        return c - 'a' + 26u;
    }
    if (c >= '0' && c <= '9') {
        return c - '0' + 52u;
    }
    if (c == '+') {
        return 62;
    }
    if (c == '/') {
        return 63;
    }
    return -(c != '=');
}
