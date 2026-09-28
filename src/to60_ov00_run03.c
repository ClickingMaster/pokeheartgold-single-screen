#pragma force_active on
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


int ov00_021FCBD8(void);
int ov00_021FCC60(void);

// @fn ov00_021FC150
int ov00_021FC150(WCtx *ctx) {
    int i;
    u8 count = 0;
    WProfile *p = ctx->profiles;
    WSsidEntry *e = ctx->entries;

    for (i = 0; i < 3; i++, p++) {
        if (ctx->profileSel != 0 && ctx->profileSel != i + 1) {
            continue;
        }
        if (p->wepMode == 0xFF) {
            continue;
        }
        if (CopySsid(e, p->ssid, i)) {
            count++;
            e++;
        }
        if (p->wepMode == 1) {
            if (CopySsid(e, p->ssid2, i + 3)) {
                count++;
                e++;
            }
        }
    }
    return count;
}

// @fn ov00_021FC258
int ov00_021FC258(WCtx *ctx) {
    int i;
    u8 count = 0;
    for (i = 0; i < ctx->numAps; i++) {
        if (ctx->ap[i].status == 0 && ctx->bss[i].channel - 1 != ctx->ap[i].ch) {
            ctx->ap[i].done = 0;
            count++;
        } else {
            ctx->ap[i].done = 1;
        }
    }
    return count;
}

// @fn ov00_021FC2E8
int ov00_021FC2E8(WCtx *ctx) {
    u8 i;
    u8 result = 0;
    u8 n = ctx->numAps;
    for (i = 0; i < n; i++) {
        if (ctx->ap[i].done == 0) {
            result = i;
            break;
        }
    }
    return result;
}

// @fn ov00_021FC32C
int ov00_021FC32C(void) {
    WCtx *ctx = ov00_021FAE14(0x10);
    u32 state = 9;
    switch (WCM_GetPhase()) {
    case 3:
        state = ctx->nextState;
        if (ctx->mode == 1) {
            state = 7;
            ctx->ap[ctx->selAp].status = 0;
        } else if (state >= 3 && state <= 5) {
            ov00_021FC568(state);
        }
        break;
    case 6:
        WCM_EndSearchAsync();
        break;
    case 9:
        WCM_DisconnectAsync();
        break;
    case 12:
        WCM_TerminateAsync();
        ov00_021FAEE8(4);
        state = 0x11;
        break;
    case 11:
        ov00_021FAEE8(0);
        state = 0x11;
        break;
    }
    return state;
}

// @fn ov00_021FC408
int ov00_021FC408(void) {
    WCtx *ctx = ov00_021FAE14(0x10);
    int state = ov00_021FAECC();
    int phase = WCM_GetPhase();
    if (state == 2 && phase == 3) {
        state = ov00_021FC648(ctx);
    } else if (state == 6) {
        state = ov00_021FCA90(ctx, state);
    } else if (phase == 3 || phase == 6) {
        state = ov00_021FCA90(ctx, state);
        if (state != 7) {
            if (state == 3) {
                state = ov00_021FC698(ctx);
            } else if (state == 4) {
                state = ov00_021FC784(ctx);
            } else if (state == 5) {
                state = ov00_021FC8AC(ctx);
            }
        }
    }
    return state;
}

// @fn ov00_021FC4CC
void ov00_021FC4CC(u32 ch) {
    WCtx *ctx = ov00_021FAE14(0x10);
    if (ch > 13) {
        ch = 13;
    }
    ctx->chMask |= 1 << (ch - 1);
}

// @fn ov00_021FC500
s8 ov00_021FC500(int n) {
    WCtx *ctx = ov00_021FAE14(0x10);
    u16 mask = ctx->chMask;
    u8 i;
    u8 k;
    if (mask == 0) {
        return -1;
    }
    for (i = 0, k = 0; i < 13; i++) {
        if (mask & (1 << i)) {
            if (k == n) {
                return (s8)i;
            }
            k++;
        }
    }
    return -1;
}

// @fn ov00_021FC568
void ov00_021FC568(int state) {
    WCtx *ctx = ov00_021FAE14(0x10);
    switch (state) {
    case 3:
        ctx->tick = OS_GetTick();
        ov00_021FCBB4(_02108FC0, _02108FC8, ctx->channel, 0x200000);
        break;
    case 4:
        ctx->tick = OS_GetTick();
        ov00_021FCBB4(ctx->bss[ctx->cur].bssid, ctx->bss[ctx->cur].ssid, ctx->bss[ctx->cur].channel - 1, 0x300000);
        break;
    case 5:
        ctx->tick = OS_GetTick();
        ov00_021FCBB4(_02108FC0, ctx->entries[ctx->cur].ssid, ctx->channel, 0x300000);
        break;
    }
}

// @fn ov00_021FC648
int ov00_021FC648(WCtx *ctx) {
    ctx->tick = OS_GetTick();
    ctx->channel = 0;
    ctx->tick = OS_GetTick();
    ov00_021FCBB4(_02108FC0, _02108FC8, ctx->channel, 0x200000);
    return 3;
}

// @fn ov00_021FC698
int ov00_021FC698(WCtx *ctx) {
    if (TICKS_TO_MS(OS_GetTick() - ctx->tick) >= 300) {
        ctx->channel += 2;
        if (ctx->channel >= 13) {
            return ov00_021FC9CC(ctx, 3);
        }
        ctx->tick = OS_GetTick();
        ov00_021FCBB4(_02108FC0, _02108FC8, ctx->channel, 0x200000);
    }
    return 3;
}

// @fn ov00_021FC73C
int ov00_021FC73C(WCtx *ctx) {
    ctx->chIter = 0;
    ctx->retry++;
    ov00_021FBF24(0);
    ctx->channel = 1;
    return 3;
}

// @fn ov00_021FC784
int ov00_021FC784(WCtx *ctx) {
    if (TICKS_TO_MS(OS_GetTick() - ctx->tick) >= 150 || ctx->ap[ctx->cur].done == 1) {
        ctx->ap[ctx->cur].done = 1;
        for (; ctx->cur < ctx->numAps; ctx->cur++) {
            if (!ctx->ap[ctx->cur].done) {
                break;
            }
        }
        if (ctx->numAps <= ctx->cur) {
            ctx->cur = 0;
            return ov00_021FC9CC(ctx, 4);
        }
        ctx->tick = OS_GetTick();
        ov00_021FCBB4(ctx->bss[ctx->cur].bssid, ctx->bss[ctx->cur].ssid, ctx->bss[ctx->cur].channel - 1, 0x300000);
    }
    return 4;
}

// @fn ov00_021FC8AC
int ov00_021FC8AC(WCtx *ctx) {
    if (TICKS_TO_MS(OS_GetTick() - ctx->tick) >= 150 || ctx->entries[ctx->cur].flags == 1) {
        ctx->entries[ctx->cur].flags = 0;
        ctx->cur++;
        if (ctx->numEntries <= ctx->cur) {
            ctx->chIter++;
            ctx->cur = 0;
            ctx->channel = ov00_021FC500(ctx->chIter);
        }
        if (ctx->channel < 0) {
            ctx->chIter = 0;
            return ov00_021FC9CC(ctx, 5);
        }
        ctx->tick = OS_GetTick();
        ov00_021FCBB4(_02108FC0, ctx->entries[ctx->cur].ssid, ctx->channel, 0x300000);
    }
    return 5;
}

// @fn ov00_021FC9CC
int ov00_021FC9CC(WCtx *ctx, int state) {
    switch (state) {
    case 3:
        if (ctx->numAps != 0 || ctx->chMask != 0) {
            if (ov00_021FBF24(1)) {
                state = 4;
            } else {
                state = ov00_021FCB54(ctx);
            }
        } else if (ctx->retry >= 1) {
            state = 6;
        } else {
            state = ov00_021FC73C(ctx);
        }
        break;
    case 4:
        state = ov00_021FCB54(ctx);
        break;
    case 5:
        if (ctx->retry >= 1) {
            state = 6;
        } else {
            state = ov00_021FC73C(ctx);
        }
        break;
    }
    ov00_021FC568(state);
    return state;
}

// @fn ov00_021FCA90
int ov00_021FCA90(WCtx *ctx, int state) {
    u8 i;
    u8 n;
    if (state == 0x11) {
        return state;
    }
    n = ctx->numAps;
    for (i = 0; i < n; i++) {
        if (ctx->ap[i].status == 0) {
            break;
        }
    }
    if (state == 6) {
        if (n == i) {
            if (i == 0) {
                ov00_021FAEE8(5);
            } else {
                ov00_021FAEE8(6);
            }
            return 0x11;
        }
    } else {
        if (n == 0) {
            return state;
        }
        if (n == i) {
            return state;
        }
        if (ctx->ap[i].rssi < 20) {
            return state;
        }
    }
    ctx->selAp = i;
    if (WCM_EndSearchAsync() != 1) {
        ctx->nextState = state;
        state = 7;
    }
    return state;
}

// @fn ov00_021FCB54
int ov00_021FCB54(WCtx *ctx) {
    if (ctx->chMask != 0 && ov00_021FBF24(2)) {
        ctx->channel = ov00_021FC500(0);
        return 5;
    }
    if (ctx->retry >= 1) {
        return 6;
    }
    return ov00_021FC73C(ctx);
}

// @fn ov00_021FCBB4
int ov00_021FCBB4(void *bssid, void *ssid, int ch, u32 option) {
    if (ch > 12) {
        ch = 12;
    }
    return WCM_SearchAsync(bssid, ssid, option | ov00_02216074[ch]);
}

// @fn ov00_021FCBD8
int ov00_021FCBD8(void) {
    int phase = WCM_GetPhase();
    WCtx *ctx = ov00_021FAE14(0x10);
    u32 cfg[4];
    int rc;
    if (phase == 1) {
        cfg[0] = ctx->unkD0A;
        cfg[1] = 0;
        cfg[2] = 0;
        cfg[3] = 0;
        ov00_021FBF24(0);
        rc = WCM_StartupAsync(cfg, ov00_021FB62C);
        if (rc == 1 || rc >= 4) {
            ov00_021FAEE8(1);
            return 0x11;
        }
    } else {
        return 1;
    }
    return 2;
}

// @fn ov00_021FCC60
int ov00_021FCC60(void) {
    int state = ov00_021FAECC();
    WCtx *ctx = ov00_021FAE14(0x10);
    if (WCM_GetPhase() == 9) {
        switch (state) {
        case 10:
            state = ov00_021FCD5C(ctx);
            break;
        case 12:
            state = ov00_021FCDC0(ctx);
            break;
        case 13:
            state = ov00_021FCE50();
            break;
        case 14:
            state = ov00_021FCE7C(ctx);
            break;
        case 15:
            state = ov00_021FCEE8(ctx);
            break;
        case 11:
            state = ov00_021FCEFC();
            break;
        }
    } else {
        switch (state) {
        case 15:
            state = ov00_021FCEE8(ctx);
            break;
        case 11:
            state = ov00_021FCEFC();
            break;
        case 14:
            ov00_021EA518();
            ov00_021EA420();
        default:
            ctx->ap[ctx->selAp].status = 2;
            state = 11;
            break;
        }
    }
    return state;
}

// @fn ov00_021FCD5C
int ov00_021FCD5C(WCtx *ctx) {
    u32 *a = ov00_021FAE14(1);
    SocCfg *b = ov00_021FAE14(4);
    ov00_021FCF2C(a, ctx, b);
    _021D4420 = 4;
    if (SOC_Startup(b) == 0) {
        return 12;
    }
    ov00_021FAEE8(2);
    return 0x11;
}

// @fn ov00_021FCDC0
int ov00_021FCDC0(WCtx *ctx) {
    if (SOC_GetHostID()) {
        ov00_021FD094(ctx);
        return ctx->d0cB4 == 1 ? 15 : 13;
    }
    if (TICKS_TO_SEC(OS_GetTick() - ctx->tick) < 10) {
        return 12;
    }
    ctx->ap[ctx->selAp].status = 1;
    return 11;
}

// @fn ov00_021FCE50
int ov00_021FCE50(void) {
    if (!ov00_021EA340(ov00_021FAE14(8))) {
        return 14;
    }
    ov00_021FAEE8(3);
    return 0x11;
}

// @fn ov00_021FCE7C
int ov00_021FCE7C(WCtx *ctx) {
    WUnk1 *p = ov00_021FAE14(1);
    int rc = ov00_021EA580();
    if (rc) {
        if (p->u15 == ov00_021FAFC8(ctx->profileIdx)) {
            p->u10 = ov00_021EA5C0();
        }
        ov00_021EA420();
        if (rc == 11) {
            return 15;
        }
        ctx->ap[ctx->selAp].status = 1;
        return 11;
    }
    return 14;
}

// @fn ov00_021FCEE8
int ov00_021FCEE8(WCtx *ctx) {
    ov00_021FAF20(ctx->profileIdx);
    return 16;
}

// @fn ov00_021FCEFC
int ov00_021FCEFC(void) {
    int rc;
    if (SOCL_CalmDown()) {
        return 11;
    }
    rc = SOC_Cleanup();
    return (rc == 0 || rc == -39) ? 9 : 11;
}

// @fn ov00_021FCF2C
void ov00_021FCF2C(u32 *src, WCtx *ctx, SocCfg *cfg) {
    WProfile *p;
    MI_CpuCopy8(ov00_022160A8, cfg, 0x58);
    cfg->u4 = src[0];
    cfg->u8 = src[1];
    if (ctx->profileIdx >= 6) {
        return;
    }
    p = &ctx->profiles[ov00_021FAFC8(ctx->profileIdx)];
    if (p->ip[0]) {
        cfg->useDhcp = 0;
        cfg->ip = ov00_021FCFEC(p->ip);
        cfg->mask = ov00_021FD040(p->prefix);
        cfg->gw = ov00_021FCFEC(p->gateway);
        cfg->dns1 = ov00_021FCFEC(p->dns1);
        cfg->dns2 = ov00_021FCFEC(p->dns2);
    } else {
        cfg->useDhcp = 1;
        cfg->ip = 0;
        cfg->mask = 0;
        cfg->gw = 0;
        cfg->dns1 = 0;
        cfg->dns2 = 0;
    }
}

#define SWAP32(v) ((((v) >> 24) & 0xFF) | (((v) >> 8) & 0xFF00) | (((v) << 8) & 0xFF0000) | (((v) << 24) & 0xFF000000))

// @fn ov00_021FCFEC
u32 ov00_021FCFEC(u8 *b) {
    u32 v = 0;
    v |= b[0] << 24;
    v |= b[1] << 16;
    v |= b[2] << 8;
    v |= b[3];
    return SWAP32(v);
}

// @fn ov00_021FD040
u32 ov00_021FD040(int prefix) {
    int i;
    int n = 32 - prefix;
    u32 mask = 0xFFFFFFFF;
    for (i = 0; i < n; i++) {
        mask <<= 1;
    }
    return SWAP32(mask);
}
