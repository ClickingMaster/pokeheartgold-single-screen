#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

typedef struct Ov13Pos2 {
    u16 x;
    u16 y;
} Ov13Pos2;

typedef struct Ov13Bytes6 {
    u8 v[6];
} Ov13Bytes6;

typedef struct Ov13Bytes8 {
    u8 v[8];
} Ov13Bytes8;

typedef struct Ov13Banner {
    void *objs[5];
    void *task;
    u8 unk18;
} Ov13Banner;

typedef struct Ov13BannerState {
    void *bg;
    Ov13Banner *p;
} Ov13BannerState;

typedef struct Ov13ScanParam {
    void *buf;
    u8 pad4[4];
    u16 period;
    u8 padA[10];
    u8 ssid[8];
    u8 pad1C;
    u8 unk1D;
    u8 pad1E[2];
    u8 name[0x24];
} Ov13ScanParam;

typedef struct Ov13ScanEntry {
    u8 mac[6];
    u8 flag;
} Ov13ScanEntry;

typedef struct Ov13ScanStatus {
    u16 state;
    u8 pad[0x7CE];
} Ov13ScanStatus;

typedef struct Ov13ScanWork {
    u8 pad0[0xF00];
    u8 scanBuf[0x400];
    Ov13ScanEntry entries[16];
    void (*cb)(int);
    Ov13ScanParam param;
    Ov13ScanStatus status;
    OSTick tick;
    void *task;
    vu8 unk1B94;
    u8 unk1B95;
    u8 unk1B96;
    u8 pad1B97[9];
} Ov13ScanWork;

typedef struct Ov13Bss {
    u8 pad0[4];
    u8 bssid[6];
    u8 padA[2];
    u8 ssid[8];
    u8 pad14;
    u8 unk15;
} Ov13Bss;

typedef struct Ov13WMArg {
    u16 apiid;
    u16 errcode;
    u8 pad4[4];
    u16 state;
    u8 padA[4];
    u16 count;
    Ov13Bss *bss[1];
} Ov13WMArg;

typedef struct Ov13Select {
    void *obj0;
    void *obj1;
    void *objs[2];
    void *unk10;
    void *task;
    u16 unk18;
    s8 unk1A;
    s8 unk1B;
    u8 kind;
    u8 unk1D;
    u8 unk1E;
} Ov13Select;

typedef struct Ov13MBHead {
    u16 state;
    u16 m2;
    u16 m4;
    u16 m6;
    u16 m8;
    u16 mA;
    u16 mC;
} Ov13MBHead;

typedef struct Ov13MBChild {
    u8 user[0x16];
    Ov13Bytes6 mac;
    u16 aid;
} Ov13MBChild;

typedef struct Ov13MBUser {
    u8 color : 4;
    u8 player : 4;
    u8 nameLength;
    u16 name[10];
} Ov13MBUser;

typedef struct Ov13MBRegistry {
    const char *path;
} Ov13MBRegistry;

typedef struct Ov13MBConnArg {
    u8 pad0[0xA];
    Ov13Bytes6 mac;
} Ov13MBConnArg;

extern Ov13BannerState ov13_0224F5AC;
extern void *ov13_0224F5B0[];
extern Ov13ScanWork *ov13_0224F5B4;
extern Ov13Select *ov13_0224F5B8;

typedef struct Ov13MBWork {
    Ov13MBHead head;
    Ov13MBChild children[1];
    u8 seg[0x10040 - 0x2C];
    u8 mbWork[0x1B140 - 0x10040];
    void *p140;
    void *p144;
} Ov13MBWork;

extern Ov13MBWork *ov13_0224F5BC;
extern void *ov13_0224F4F8;
extern u8 ov13_022457EC[][5];
extern u8 ov13_022457FB[][5];
extern Ov13Pos2 ov13_0224580A[];
extern u8 ov13_02245820[];
extern u8 ov13_02245828[];
extern Ov13ScanParam ov13_02245834;
extern u8 ov13_02245878[];
extern u8 ov13_02245882[];
extern u8 ov13_0224588A[][2];
extern s8 ov13_02245894[][2];
extern u8 ov13_0224589E[][2];
extern Ov13Pos2 ov13_022458A8[];
extern Ov13Pos2 ov13_022458BC[];
extern Ov13Pos2 ov13_022458D4[];
extern Ov13Pos2 ov13_022458EC[];
extern Ov13Pos2 ov13_02245904[][2];
extern u8 ov13_0224592C[];
extern u8 ov13_02245934[];
extern void *ov13_02246854[];

#define MBW_HEAD     (ov13_0224F5BC->head)
#define MBW_CHILD(n) (ov13_0224F5BC->children[n])
#define MBW_P140     (ov13_0224F5BC->p140)
#define MBW_P144     (ov13_0224F5BC->p144)

int ov13_0222BAF4(void *a, int b, int c, int d);
void *ov13_022278C4(void *a);
int ov13_02227B1C(void);
void *ov13_0223F95C(void *a, int b, int c);
void ov13_0223FA50(void *a);
void *ov13_0223FC04(int a, int b, int c);
void ov13_0223FD60(void *a);
int ov13_0223FDE0(void *a, int b);
int ov13_0223FDEC(void *a);
void ov13_0223FDF4(void *a, int b, int c, int d);
void ov13_0223FE98(void *a, int b, int c, int d);
void ov13_0223FF18(void *a, int b, int c, int d);
void ov13_02240008(void *a, int b, int c);
void ov13_02240068(void *a, int b, int *x, int *y);
void ov13_022400F0(int a, int b, int c);
void *ov13_022401B0(int a, int b, int c);
void *ov13_02240798(int a, int b, int c, int d, int *e, int f);
void ov13_02240874(void *a);
void ov13_02240CC0(void *a, int b, int c, int d, int e, int f, int g, int h);
void ov13_02240D94(void *a, int b, int c, void *d, int e);
void ov13_02240E5C(int a, int b, int c, int d, void *e);
void ov13_02240E74(void *a, void *b, void *c);
void ov13_02240F58(int a, int b, void *c);
void ov13_02241070(int a, int b, int c, int d);
void *ov13_0224128C(u32 size, int align);
void ov13_022412B0(void *pp);
int ov13_02241694(int a);
int ov13_022417DC(void *a);
void *ov13_02242528(int a, void *fn, int c, int d);
void ov13_022425C8(void *task, void *fn);
void ov13_022425D0(int a, void *task);
void ov13_022425F0(int a, void *task);
void ov13_0223D448(int a);
int ov13_0223D45C(void);
int ov13_0223D4C8(void);
void ov13_0223D66C(void *a);

int WM_Initialize(void *buf, void *cb, u16 dma);
int WM_ReadStatus(void *status);
u16 WM_GetDispersionScanPeriod(void);
int WM_StartScanEx(void *cb, void *param);
int WM_Reset(void *cb);
int WM_End(void *cb);

int MB_Init(void *work, void *user, u32 ggid, u32 tgid, u32 dma);
void MB_SetParentCommParam(u16 a, u16 b);
void MB_CommSetParentStateCallback(void *cb);
int MB_StartParentFromIdle(u32 channel);
u32 MB_GetSegmentLength(FSFile *file);
BOOL MB_ReadSegment(FSFile *file, void *buf, u32 len);
BOOL MB_RegisterFile(void *reg, void *buf);
BOOL MB_CommResponseRequest(u16 aid, u32 ack);
void MB_DisconnectChild(u16 aid);
BOOL MB_CommIsBootable(u16 aid);
void *MB_CommGetChildUser(u16 aid);
void MB_End(void);

void ov13_0223B178(void *task);
void ov13_0223B2C8(void *task);
void ov13_0223B35C(void *task);
void ov13_0223B438(void *task);
void ov13_0223B514(void *task);
void ov13_0223B5F0(void *task);
void ov13_0223B6F4(int n);
void ov13_0223B73C(void *task);
BOOL ov13_0223B90C(void);
void ov13_0223BA3C(Ov13WMArg *arg);
void ov13_0223BACC(Ov13WMArg *arg);
void ov13_0223BC24(Ov13WMArg *arg);
void ov13_0223BD30(void *task);
void ov13_0223C20C(void *task);
void ov13_0223C26C(void *task);
void ov13_0223C340(void *task);
void ov13_0223C49C(void *task);
void ov13_0223C508(int y);
void ov13_0223C6B0(void *obj, int y);
void ov13_0223C73C(int i);
void ov13_0223C7FC(void *task);
void ov13_0223C838(void *task);
void ov13_0223C8C8(void *task);
void ov13_0223C924(void *task);
BOOL ov13_0223CB7C(Ov13MBRegistry *reg);
void ov13_0223CC50(u16 aid);
void ov13_0223CCF0(u16 aid);
void ov13_0223CDC8(u16 aid);
void ov13_0223D0F4(void);
void ov13_0223D124(u16 aid, u32 status, void *arg);

static inline void ov13_MBClearChild(u16 aid) {
    u16 mask = ~(1 << aid);
    OSIntrMode e = OS_DisableInterrupts();
    MBW_HEAD.m2 &= mask;
    MBW_HEAD.m4 &= mask;
    MBW_HEAD.m6 &= mask;
    MBW_HEAD.m8 &= mask;
    MBW_HEAD.mA &= mask;
    MBW_HEAD.mC &= mask;
    OS_RestoreInterrupts(e);
}

static inline void ov13_MBKickChild(u16 aid) {
    ov13_MBClearChild(aid);
    MB_DisconnectChild(aid);
}

BOOL ov13_0223B6C8(void);
BOOL ov13_0223B788(void (*cb)(int));
BOOL ov13_0223B940(void);
void ov13_0223BA24(void (*cb)(int));

void ov13_0223B35C(void *task) {
    int x;
    int y;
    int i;
    ov13_02240068(ov13_0224F5AC.p->objs[3], 0, &x, &y);
    x -= 8;
    if (x > 0x68) {
        for (i = 3; i < 5; i++) {
            ov13_0223FF18(ov13_0224F5AC.p->objs[i], -1, x, ov13_0224580A[i].y);
        }
        return;
    }
    x = 0x68;
    for (i = 3; i < 5; i++) {
        ov13_0223FF18(ov13_0224F5AC.p->objs[i], -1, x, ov13_0224580A[i].y);
    }
    ov13_022425C8(task, ov13_0223B438);
}

void ov13_0223B438(void *task) {
    int x;
    int y;
    int i;
    ov13_02240068(ov13_0224F5AC.p->objs[2], 0, &x, &y);
    x -= 8;
    if (x > 0x50) {
        for (i = 2; i < 5; i++) {
            ov13_0223FF18(ov13_0224F5AC.p->objs[i], -1, x, ov13_0224580A[i].y);
        }
        return;
    }
    x = 0x50;
    for (i = 2; i < 5; i++) {
        ov13_0223FF18(ov13_0224F5AC.p->objs[i], -1, x, ov13_0224580A[i].y);
    }
    ov13_022425C8(task, ov13_0223B514);
}

void ov13_0223B514(void *task) {
    int x;
    int y;
    int i;
    ov13_02240068(ov13_0224F5AC.p->objs[1], 0, &x, &y);
    x -= 8;
    if (x > 0x20) {
        for (i = 1; i < 5; i++) {
            ov13_0223FF18(ov13_0224F5AC.p->objs[i], -1, x, ov13_0224580A[i].y);
        }
        return;
    }
    x = 0x20;
    for (i = 1; i < 5; i++) {
        ov13_0223FF18(ov13_0224F5AC.p->objs[i], -1, x, ov13_0224580A[i].y);
    }
    ov13_022425C8(task, ov13_0223B5F0);
}

void ov13_0223B5F0(void *task) {
    int x;
    int y;
    int i;
    ov13_02240068(ov13_0224F5AC.p->objs[0], 0, &x, &y);
    x -= 8;
    for (i = 0; i < 5; i++) {
        ov13_0223FF18(ov13_0224F5AC.p->objs[i], -1, x, ov13_0224580A[i].y);
    }
    if (x > 0x1D6) {
        return;
    }
    if (x < 0x100) {
        return;
    }
    ov13_022425D0(0, task);
    for (i = 0; i < 5; i++) {
        ov13_0223FD60(ov13_0224F5AC.p->objs[i]);
    }
    ov13_022412B0(ov13_0224F5B0);
}

BOOL ov13_0223B6C8(void) {
    if (ov13_0224F5AC.p == NULL) {
        return TRUE;
    }
    return ov13_0224F5AC.p->unk18 == 0;
}

void ov13_0223B6F4(int n) {
    ov13_0224F5AC.bg = ov13_0223F95C(ov13_022278C4(ov13_02246854[n]), 0, 4);
    ov13_02242528(1, ov13_0223B73C, 0, 0x78);
}

void ov13_0223B73C(void *task) {
    DC_FlushRange(ov13_0224F5AC.bg, 0x600);
    GXS_LoadBG1Scr(ov13_0224F5AC.bg, 0, 0x600);
    ov13_0223FA50(ov13_0224F5AC.bg);
    ov13_022425D0(1, task);
}

BOOL ov13_0223B788(void (*cb)(int)) {
    OSOwnerInfo info;
    Ov13ScanWork *w = ov13_0224128C(sizeof(Ov13ScanWork), 0x20);
    ov13_0224F5B4 = w;
    w->cb = cb;
    (*(Ov13ScanWork *volatile *)&ov13_0224F5B4)->tick = OS_GetTick();
    if (WM_Initialize(ov13_0224F5B4, ov13_0223BA3C, 3) == 2) {
        do {
            WM_ReadStatus(&ov13_0224F5B4->status);
        } while (ov13_0224F5B4->status.state != 2);
        ov13_0224F5B4->param = ov13_02245834;
        ov13_0224F5B4->param.buf = ov13_0224F5B4->scanBuf;
        ov13_0224F5B4->param.period = WM_GetDispersionScanPeriod();
        OS_GetOwnerInfo(&info);
        MI_CpuCopy8(ov13_02245828, ov13_0224F5B4->param.ssid, 8);
        ov13_0224F5B4->param.unk1D = 1;
        MI_CpuCopy8(info.nickName, ov13_0224F5B4->param.name, info.nickNameLength * 2);
        if (ov13_0223B90C()) {
            ov13_0224F5B4->task = ov13_02242528(0, ov13_0223BD30, 0, 0x78);
            return TRUE;
        }
    }
    ov13_022412B0(&ov13_0224F5B0[1]);
    return FALSE;
}

BOOL ov13_0223B90C(void) {
    return WM_StartScanEx(ov13_0223BA3C, &ov13_0224F5B4->param) == 2;
}

BOOL ov13_0223B940(void) {
    ov13_0224F5B4->unk1B94 = 1;
    WM_ReadStatus(&ov13_0224F5B4->status);
    if (ov13_0224F5B4->status.state != 2) {
        if (WM_Reset(ov13_0223BA3C) != 2) {
            return FALSE;
        }
        do {
            WM_ReadStatus(&ov13_0224F5B4->status);
        } while (ov13_0224F5B4->status.state != 2);
    }
    if (WM_End(ov13_0223BA3C) != 2) {
        return FALSE;
    }
    if (ov13_0224F5B4->task != NULL) {
        ov13_022425F0(0, ov13_0224F5B4->task);
    }
    while (ov13_0224F5B4->unk1B94 != 2) {
    }
    ov13_022412B0(&ov13_0224F5B0[1]);
    return TRUE;
}

void ov13_0223BA24(void (*cb)(int)) {
    ov13_0224F5B4->cb = cb;
}

void ov13_0223BA3C(Ov13WMArg *arg) {
    if (arg->errcode != 0) {
        return;
    }
    if (ov13_0224F5B4->unk1B94 != 0) {
        if (arg->apiid == 2) {
            ov13_0224F5B4->unk1B94 = 2;
        }
        return;
    }
    if (arg->apiid != 0x26) {
        return;
    }
    switch (arg->state) {
    case 5:
        if (ov13_0224F5B4->unk1B96) {
            ov13_0223BC24(arg);
        } else {
            ov13_0223BACC(arg);
        }
        ov13_0223B90C();
        break;
    case 4:
        ov13_0223B90C();
        break;
    default:
        OS_Terminate();
        break;
    }
}

void ov13_0223BACC(Ov13WMArg *arg) {
    Ov13Bss *bss;
    int i;
    int j;
    Ov13ScanWork *w;
    for (i = 0; i < arg->count; i++) {
        bss = arg->bss[i];
        DC_InvalidateRange(bss, 0xC0);
        if (memcmp(bss->ssid, ov13_02245828, 8) != 0) {
            continue;
        }
        w = ov13_0224F5B4;
        for (j = 0; j < 16; j++) {
            if (memcmp(bss->bssid, w->entries[j].mac, 6) == 0) {
                if (w->entries[j].flag != 0) {
                    goto next;
                }
                if (!(bss->unk15 & 1)) {
                    goto next;
                }
                if (w->cb != NULL) {
                    w->cb(1);
                }
                return;
            }
        }
        for (j = 0; j < 16; j++) {
            if (memcmp(w->entries[j].mac, ov13_02245820, 6) == 0) {
                MI_CpuCopy8(bss->bssid, w->entries[j].mac, 6);
                ov13_0224F5B4->entries[j].flag = (bss->unk15 & 1) ? 1 : 0;
                break;
            }
        }
next:;
    }
}
