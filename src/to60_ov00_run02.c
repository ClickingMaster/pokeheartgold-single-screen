#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

typedef void (*RecvCb)(int, void *, int);

typedef struct RecvEntry {
    u32 unk0;
    u8 *buf;
    int size;
    u32 unkC;
    int pos;
    u32 unk14;
    int total;
    u8 unk1C;
    u8 state;
    u8 saved;
    u8 unk1F;
    u16 unk20;
    u16 type;
    u32 tickLo;
    u32 tickHi;
    int tickOn;
} RecvEntry;

typedef struct RecvWork {
    RecvEntry entries[32];
    u32 unk600;
    RecvCb cb;
    int tickOn;
} RecvWork;

extern RecvWork *ov00_0221B0A0;

typedef void (*HttpDoneCb)(void *, int, int, int);
typedef void (*HttpProgCb)(int, void *, int, int, int, int);

typedef struct HttpInit {
    int param;
    HttpDoneCb cb;
    HttpProgCb prog;
    int flag;
} HttpInit;

typedef struct HttpReq {
    HttpInit init;
    void *buf;
    int handle;
    struct HttpReq *next;
} HttpReq;

typedef struct HttpList {
    HttpReq *head;
    int count;
} HttpList;

extern HttpList ov00_0221B0A4;

typedef struct Ctx {
    void *(*alloc)(int, int);
    void (*free)(int, void *, int);
    u8 flags;
    u8 state;
    u8 prevState;
    u8 unkB;
    int unkC;
    int unk10;
    u8 unk14;
    u8 unk15;
    u8 maxState;
    u8 unk17;
    char ssid[10];
    u8 unk22;
    u8 pad23;
} Ctx;

typedef struct Profile {
    u8 pad0[0x40];
    u8 ssid[0x20];
    u8 pad60[0x20];
    u8 key[0x51];
    u8 wep[0x15];
    u8 keyType : 2;
    u8 keyRest : 6;
    u8 padE7[0x19];
} Profile;

typedef struct Svc {
    u8 lo : 4;
    u8 hi : 4;
    u8 idx;
    u8 unk2;
    u8 len;
    char name[0x20];
} Svc;

typedef struct ApStat {
    u8 status;
    u8 idx;
    u8 rssi;
    u8 pri : 7;
    u8 flag : 1;
} ApStat;

typedef struct Ap {
    u16 unk0;
    u16 rssi;
    u8 bssid[6];
    u16 ssidLen;
    u8 ssid[0x20];
    u16 cap;
    u8 unk2E[8];
    u16 unk36;
    u8 rest[0x88];
} Ap;

typedef struct Work {
    Profile prof[3];
    Svc svc[9];
    ApStat stat[11];
    Ap ap[11];
    u32 tickLo;
    u32 tickHi;
    u8 conn[0x52];
    u8 unkD0A;
    u8 b0 : 2;
    u8 b2 : 2;
    u8 b4 : 4;
    u8 c0 : 4;
    u8 c4 : 2;
    u8 c6 : 2;
    u8 unkD0D;
    u8 unkD0E;
    u8 unkD0F;
    u8 unkD10;
    u8 unkD11;
    u8 unkD12;
    u8 unkD13;
    u8 unkD14;
    u8 unkD15;
    u8 padD16[2];
} Work;

typedef struct Globals {
    void *p0;
    void *p4;
    void *p8;
    Ctx *ctx;
    Work *work;
} Globals;

extern Globals ov00_0221B0AC;

#define RSSI_CALC(v) ((u8)(((v) & 2) ? (u8)((v) >> 2) : (u8)(((v) >> 2) + 25)))

typedef struct InitParam {
    void *(*alloc)(int, int);
    void (*free)(int, void *, int);
    u8 u8_;
    u8 u9;
    u8 uA;
    u8 uB;
} InitParam;

typedef struct WcmEvent {
    s16 notify;
    s16 result;
    void *ptr;
    int reason;
} WcmEvent;

extern char ov00_02216060[];
extern char ov00_02217B8C[];
extern char ov00_02216058[];
extern char ov00_02216068[];

extern int ov00_021EE6B0(int);
extern u8 ov00_021F9CF0(int, void *, int);
extern int ov00_021F992C(void *);
extern RecvEntry *ov00_021F9CBC(int);
extern void ov00_021F2FDC(int, int, int);
extern void ov00_021EC24C(int, int);
extern int ov00_021EE64C(void);
extern int ov00_0220D284(int);
extern void ov38_0221D7B4(void);
extern void ov38_0221D808(void);
extern void ov38_0221DAEC(void);
extern int ov00_021EC230(void);
extern void ov00_021EC2EC(int, void *, int);
extern void *ov00_021EC2A8(int, int);
extern int ov38_0221D864(const char *, int, void *, HttpReq *);
extern int ghttpGetExA(const char *, int, void *, int, int, int, int, void *, void *, void *);
extern void DWCi_BM_GetApInfo(void *);
extern int WCM_Init(void *, int);
extern u8 ov00_021FCBD8(void);
extern u8 ov00_021FC408(void);
extern u8 ov00_021FC32C(void);
extern u8 ov00_021FCC60(void);
extern void *WCM_GetApEssid(int);
extern void DC_InvalidateRange(void *, u32);
extern void ov00_021FD17C(void *, char *);
extern int strncmp(const char *, const char *, u32);
extern void ov00_021FC4CC(u16);
extern int ov00_021FD1C8(u8 *);
extern int ov00_021FD11C(u8 *);
extern int WCM_GetPhase(void);
extern void WCM_ConnectAsync(void *, void *, u32);
extern void ov00_021FD1EC(u8 *, u8 *);
extern void ov00_021FD154(u8 *, u8 *);
extern void ov00_021EA518(void);
extern void ov00_021EA420(void);
extern void WCM_Finish(void);
extern void WCM_CleanupAsync(void);
extern void WCM_EndSearchAsync(void);
extern void WCM_DisconnectAsync(void);
extern void WCM_TerminateAsync(void);
extern int SOCL_CalmDown(void);
extern int SOC_Cleanup(void);
extern u8 ov00_021FC150(Work *);
extern u8 ov00_021FC258(Work *);
extern u8 ov00_021FC2E8(Work *);

int ov00_021FA4B0(int err);
HttpReq *ov00_021FA60C(HttpInit *init);
void ov00_021FA668(HttpReq *req);
void ov00_021FA6EC(void);
int ov00_021FA138(void *r, int res, void *buf, int len, void *param);
void ov00_021FA1D4(void *r, int a, void *b, int c, int d, int e, HttpReq *req);
int ov00_021FA2E4(const char *url, int len, int flag, int *post, HttpProgCb prog, HttpDoneCb cb, int param);
void *ov00_021FAC4C(int kind, int size);
void ov00_021FAC8C(int kind, void *p, int size);
void ov00_021FACF0(void);
void *ov00_021FAE14(int kind);
void ov00_021FAE6C(u8 st);
u8 ov00_021FAECC(void);
void ov00_021FAEE8(int v);
int ov00_021FAF0C(void);
u8 ov00_021FAFC8(u8 x);
void ov00_021FAFD8(void);
int ov00_021FB010(void);
void ov00_021FAB68(const u8 *ssid, const void *key, int type);
void ov00_021FB0B0(Ap *bss);
int ov00_021FB21C(Ap *ap);
int ov00_021FB2B0(Ap *ap, int n, Svc *s);
int ov00_021FB32C(Ap *ap, Work *w);
int ov00_021FB3BC(int x, Ap *bss, u8 y, Work *w);
void ov00_021FB484(u8 x, Ap *bss, u8 y, Work *w);
void ov00_021FB4D0(int idx, Ap *bss, u8 y, Work *w);
void ov00_021FB53C(int i, Work *w);
u8 ov00_021FB6B8(void);
u8 ov00_021FB6FC(Work *w);
u8 ov00_021FB7F4(Work *w);
u8 ov00_021FB930(Work *w);
u32 ov00_021FBACC(Work *w);
u32 ov00_021FBAE8(Work *w);
BOOL ov00_021FBB04(Work *w, int kind, u8 *out);
int ov00_021FBBF0(u8 *st);
int ov00_021FBC78(void);
int ov00_021FBD08(void);
u8 ov00_021FBD38(void);
int ov00_021FBD5C(void);
int ov00_021FBDAC(int v);
int ov00_021FBDF0(void);
int ov00_021FBDF8(Ctx *c);
int ov00_021FBE14(Ctx *c);
int ov00_021FBFB8(Ap *ap);
u8 ov00_021FC004(Work *w);
u8 ov00_021FC064(Work *w);

u8 ov00_021FBF24(int kind);

u8 ov00_021FBF24(int kind) {
    Work *w = ov00_021FAE14(0x10);
    switch (kind) {
    case 0:
        MIi_CpuClear32(0, (u32 *)w->svc, sizeof(w->svc));
        w->unkD10 = ov00_021FC004(w);
        break;
    case 1:
        w->unkD10 = ov00_021FC258(w);
        w->unkD0F = ov00_021FC2E8(w);
        break;
    case 2:
        MIi_CpuClear32(0, (u32 *)w->svc, sizeof(w->svc));
        w->unkD0F = 0;
        w->unkD10 = ov00_021FC064(w);
        break;
    }
    return w->unkD10;
}

int ov00_021FBFB8(Ap *ap) {
    Work *w = ov00_021FAE14(0x10);
    if ((w->c0 == 0 || w->c0 == 6) && strncmp((char *)ap->ssid, ov00_02216060, 8) == 0) {
        return 8;
    }
    return 0;
}

u8 ov00_021FC004(Work *w) {
    Svc *s = w->svc;
    u8 n = ov00_021FC150(w);
    s += n;
    if (w->c0 == 0 || w->c0 == 6) {
        MI_CpuCopy8(ov00_02216060, s->name, 8);
        s->len = 8;
        s->idx = 8;
        n++;
    }
    return n;
}

u8 ov00_021FC064(Work *w) {
    u8 n;
    Svc *s = w->svc;
    n = ov00_021FC150(w);
    s += n;
    if (w->c0 == 0 || w->c0 == 4) {
        MI_CpuCopy8(ov00_02217B8C, s->name, 8);
        s->len = 8;
        s->idx = 6;
        n++;
        s++;
    }
    if (w->c0 == 0 || w->c0 == 7) {
        MI_CpuCopy8(ov00_02216058, s->name, 8);
        s->len = 8;
        s->idx = 9;
        n++;
        s++;
    }
    if (w->c0 == 0 || w->c0 == 8) {
        MI_CpuCopy8(ov00_02216068, s->name, 11);
        s->len = 11;
        s->idx = 10;
        n++;
    }
    return n;
}
