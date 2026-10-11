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
void *WifiSetup_LocalizePath(void *a);
int WifiSetup_GetTextFlags(void);
void *WifiSetupFile_Load(void *a, int b, int c);
void WifiSetupFile_Free(void *a);
void *WifiSetupOamBlock_Alloc(int a, int b, int c);
void WifiSetupOamBlock_Free(void *a);
int WifiSetupOamBlock_GetOam(void *a, int b);
int WifiSetupOamBlock_GetCount(void *a);
void WifiSetupOamBlock_SetAffineMode(void *a, int b, int c, int d);
void WifiSetupOamBlock_SetModeAndPalette(void *a, int b, int c, int d);
void WifiSetupOamBlock_SetPosition(void *a, int b, int c, int d);
void WifiSetupOamBlock_SetPriority(void *a, int b, int c);
void WifiSetupOamBlock_GetPosition(void *a, int b, int *x, int *y);
void WifiSetupOamTemplate_Copy(int a, int b, int c);
void *WifiSetupOamTemplate_AllocBlock(int a, int b, int c);
void *WifiSetupText_AllocObjCanvas(int a, int b, int c, int d, int *e, int f);
void WifiSetupText_FreeObjCanvas(void *a);
void WifiSetupText_DrawInRect(void *a, int b, int c, int d, int e, int f, int g, int h);
void WifiSetupText_ShowObjCanvas(void *a, int b, int c, void *d, int e);
void WifiSetup_SetRect(int a, int b, int c, int d, void *e);
void WifiSetup_RectFromPosSize(void *a, void *b, void *c);
void WifiSetupDisplay_SetWindowRect(int a, int b, void *c);
void WifiSetupDisplay_SetWindowPlanes(int a, int b, int c, int d);
void *WifiSetupHeap_AllocZeroed(u32 size, int align);
void WifiSetupHeap_FreeAndClear(void *pp);
int WifiSetupInput_IsKeyPressed(int a);
int WifiSetupInput_IsTouchPressed(void *a);
void *WifiSetupTask_Create(int a, void *fn, int c, int d);
void WifiSetupTask_SetFunc(void *task, void *fn);
void WifiSetupTask_DestroyLater(int a, void *task);
void WifiSetupTask_Destroy(int a, void *task);
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

void ov13_0223C9D4(void *buf);
void ov13_0223CA14(u32 ggid, u32 tgid);
void ov13_0223CB34(Ov13MBRegistry *reg, u32 channel);
void ov13_0223CEA0(void);
BOOL ov13_0223CF80(void);
void ov13_0223CFE8(void);
void ov13_0223D0E0(void);

void ov13_0223C49C(void *task) {
    ov13_0224F5B8->unk1B = -1;
    ov13_0224F5B8->unk18++;
    if (ov13_0224F5B8->unk18 < 0x78) {
        return;
    }
    WifiSetupTask_DestroyLater(0, task);
    ov13_0224F5B8->task = WifiSetupTask_Create(1, ov13_0223C7FC, 0, 0x78);
}

void ov13_0223C508(int y) {
    int i;
    int k;
    int top;
    int bottom;
    int left;
    int right;
    Ov13Bytes8 win;
    WifiSetupOamBlock_SetPosition(ov13_0224F5B8->obj0, -1, ov13_022458D4[ov13_0224F5B8->kind].x, y);
    WifiSetupOamBlock_SetPosition(ov13_0224F5B8->obj1, -1, ov13_022458D4[ov13_0224F5B8->kind].x + 8, y + 8);
    ov13_0223C6B0(ov13_0224F5B8->obj0, y);
    ov13_0223C6B0(ov13_0224F5B8->obj1, y);
    for (i = 0; i < ov13_02245934[ov13_0224F5B8->kind]; i++) {
        k = ov13_0224588A[ov13_0224F5B8->kind][i];
        WifiSetupOamBlock_SetPosition(ov13_0224F5B8->objs[i], -1, ov13_02245904[ov13_0224F5B8->kind][k].x, y + ov13_02245904[ov13_0224F5B8->kind][k].y - ov13_022458D4[ov13_0224F5B8->kind].y);
        ov13_0223C6B0(ov13_0224F5B8->objs[i], y);
    }
    top = (u8)y;
    if (top >= 0xC0) {
        bottom = 0;
        top = 0;
    } else {
        bottom = top + ov13_022458EC[ov13_0224F5B8->kind].y;
    }
    left = ov13_022458D4[ov13_0224F5B8->kind].x;
    right = left + ov13_022458EC[ov13_0224F5B8->kind].x;
    if (bottom > 0xC0) {
        bottom = 0xC0;
    }
    WifiSetup_SetRect(left, top, right, bottom, &win);
    WifiSetupDisplay_SetWindowRect(0, 0, &win);
}

void ov13_0223C6B0(void *obj, int y) {
    int n = WifiSetupOamBlock_GetCount(obj);
    int i;
    int x;
    int yy;
    int f;
    for (i = 0; i < n; i++) {
        WifiSetupOamBlock_GetPosition(obj, i, &x, &yy);
        if (yy >= y && yy < 0xC0) {
            f = 0;
        } else {
            f = 0x200;
        }
        WifiSetupOamBlock_SetAffineMode(obj, i, f, 0);
    }
}

void ov13_0223C73C(int i) {
    int k;
    WifiSetupOamTemplate_Copy(0, ov13_0224589E[ov13_0224F5B8->kind][i] + 1, WifiSetupOamBlock_GetOam(ov13_0224F5B8->objs[i], 0));
    k = ov13_0224588A[ov13_0224F5B8->kind][i];
    WifiSetupOamBlock_SetPosition(ov13_0224F5B8->objs[i], -1, ov13_02245904[ov13_0224F5B8->kind][k].x, ov13_02245904[ov13_0224F5B8->kind][k].y);
    WifiSetupOamBlock_SetPriority(ov13_0224F5B8->objs[i], -1, 0);
}

void ov13_0223C7FC(void *task) {
    ov13_0224F5B8->unk1D++;
    if (ov13_0224F5B8->unk1D < 8) {
        return;
    }
    WifiSetupTask_SetFunc(task, ov13_0223C838);
}

void ov13_0223C838(void *task) {
    int x;
    int y;
    WifiSetupOamBlock_GetPosition(ov13_0224F5B8->obj0, 0, &x, &y);
    y += 12;
    ov13_0223C508(y);
    if (y < 0xC0) {
        return;
    }
    if (ov13_0224F5B8->unk1E) {
        WifiSetupTask_SetFunc(task, ov13_0223C8C8);
    } else {
        WifiSetupTask_SetFunc(task, ov13_0223C924);
    }
}

void ov13_0223C8C8(void *task) {
    ov13_0224F5B8->unk1A++;
    G2x_ChangeBlendBrightness_((u16 *)&reg_G2_BLDCNT, ov13_0224F5B8->unk1A);
    if (ov13_0224F5B8->unk1A < 0) {
        return;
    }
    WifiSetupTask_SetFunc(task, ov13_0223C924);
}

void ov13_0223C924(void *task) {
    int i;
    GX_SetVisibleWnd(0);
    WifiSetupOamBlock_Free(ov13_0224F5B8->obj0);
    WifiSetupOamBlock_Free(ov13_0224F5B8->obj1);
    for (i = 0; i < ov13_02245934[ov13_0224F5B8->kind]; i++) {
        if (ov13_0224F5B8->objs[i] != NULL) {
            WifiSetupOamBlock_Free(ov13_0224F5B8->objs[i]);
        }
    }
    WifiSetupText_FreeObjCanvas(ov13_0224F5B8->unk10);
    WifiSetupTask_DestroyLater(1, task);
    WifiSetupHeap_FreeAndClear(&ov13_0224F5B0[2]);
}

void ov13_0223C9D4(void *buf) {
    ov13_0224F5BC = buf;
    ov13_0223D66C((u8 *)buf + 0x1B160);
    MBW_P140 = NULL;
    MBW_P144 = NULL;
}

void ov13_0223CA14(u32 ggid, u32 tgid) {
    Ov13MBUser user;
    OSOwnerInfo info;
    OS_GetOwnerInfo(&info);
    user.color = info.favoriteColor;
    user.nameLength = info.nickNameLength;
    MI_CpuCopy8(info.nickName, user.name, info.nickNameLength * 2);
    user.player = 0;
    MBW_HEAD = (Ov13MBHead) { 0, 0, 0, 0, 0, 0, 0 };
    MBW_P140 = ov13_0224F5BC->mbWork;
    if (MB_Init(MBW_P140, &user, ggid, tgid, 2) != 0) {
        OS_Terminate();
    }
    MB_SetParentCommParam(0x100, 1);
    MB_CommSetParentStateCallback(ov13_0223D124);
    ov13_0223D448(1);
}

void ov13_0223CB34(Ov13MBRegistry *reg, u32 channel) {
    ov13_0223D448(2);
    if (MB_StartParentFromIdle(channel) != 0) {
        ov13_0223D448(7);
        return;
    }
    if (!ov13_0223CB7C(reg)) {
        OS_Terminate();
    }
}

BOOL ov13_0223CB7C(Ov13MBRegistry *reg) {
    FSFile file;
    FSFile *fp;
    BOOL ok = FALSE;
    if (reg->path == NULL) {
        fp = NULL;
    } else {
        FS_InitFile(&file);
        if (!FS_OpenFile(&file, reg->path)) {
            return FALSE;
        }
        fp = &file;
    }
    if (MB_GetSegmentLength(fp) != 0) {
        MBW_P144 = ov13_0224F5BC->seg;
        if (MBW_P144 != NULL && MB_ReadSegment(fp, MBW_P144, 0x10000)) {
            if (MB_RegisterFile(reg, MBW_P144)) {
                ok = TRUE;
            }
        }
    }
    if (fp == &file) {
        FS_CloseFile(&file);
    }
    return ok;
}

void ov13_0223CC50(u16 aid) {
    if (MB_CommResponseRequest(aid, 1)) {
        return;
    }
    ov13_MBKickChild(aid);
}

void ov13_0223CCF0(u16 aid) {
    OSIntrMode e;
    if (!MB_CommResponseRequest(aid, 0)) {
        ov13_MBKickChild(aid);
        return;
    }
    e = OS_DisableInterrupts();
    MBW_HEAD.m4 &= ~(1 << aid);
    MBW_HEAD.m2 &= ~(1 << aid);
    OS_RestoreInterrupts(e);
}

void ov13_0223CDC8(u16 aid) {
    OSIntrMode e;
    if (!MB_CommResponseRequest(aid, 2)) {
        ov13_MBKickChild(aid);
        return;
    }
    e = OS_DisableInterrupts();
    MBW_HEAD.m6 &= ~(1 << aid);
    MBW_HEAD.m8 |= 1 << aid;
    OS_RestoreInterrupts(e);
}

void ov13_0223CEA0(void) {
    u16 i;
    ov13_0223D448(3);
    for (i = 1; i < 16; i++) {
        if ((MBW_HEAD.m2 & (1 << i)) && !(MBW_HEAD.m4 & (1 << i))) {
            if (!(MBW_HEAD.m6 & (1 << i))) {
                ov13_MBKickChild(i);
            } else {
                ov13_0223CDC8(i);
            }
        }
    }
}

BOOL ov13_0223CF80(void) {
    u16 i;
    if (MBW_HEAD.m2 == 0) {
        return FALSE;
    }
    for (i = 1; i < 16; i++) {
        if (MBW_HEAD.m2 & (1 << i)) {
            if (!MB_CommIsBootable(i)) {
                return FALSE;
            }
        }
    }
    return TRUE;
}

void ov13_0223CFE8(void) {
    u16 i;
    u16 ok = 0;
    for (i = 1; i < 16; i++) {
        if (MBW_HEAD.mA & (1 << i)) {
            if (!MB_CommResponseRequest(i, 3)) {
                ov13_MBKickChild(i);
            } else {
                ok |= 1 << i;
            }
        }
    }
    if (ok == 0) {
        ov13_0223D448(7);
    } else {
        ov13_0223D448(4);
    }
}

void ov13_0223D0E0(void) {
    ov13_0223D448(6);
    MB_End();
}

void ov13_0223D0F4(void) {
    if (MBW_HEAD.state == 4 && MBW_HEAD.m2 == MBW_HEAD.mC) {
        MB_End();
    }
}

void ov13_0223D124(u16 aid, u32 status, void *arg) {
    Ov13MBHead *w;
    OSIntrMode e;
    void *user;
    u8 *mac;
    switch (status) {
    case 1:
    case 4:
    case 5:
    case 6:
    case 8:
    case 11:
        break;
    case 2:
        if (ov13_0223D45C() != 2) {
            break;
        }
        w = &MBW_HEAD;
        e = OS_DisableInterrupts();
        w->m2 |= 1 << aid;
        OS_RestoreInterrupts(e);
        mac = MBW_CHILD(aid - 1).mac.v;
        mac[0] = ((Ov13MBConnArg *)arg)->mac.v[0];
        mac[1] = ((Ov13MBConnArg *)arg)->mac.v[1];
        mac[2] = ((Ov13MBConnArg *)arg)->mac.v[2];
        mac[3] = ((Ov13MBConnArg *)arg)->mac.v[3];
        mac[4] = ((Ov13MBConnArg *)arg)->mac.v[4];
        mac[5] = ((Ov13MBConnArg *)arg)->mac.v[5];
        MBW_CHILD(aid - 1).aid = aid;
        break;
    case 3:
        if (ov13_0223D4C8() == 6) {
            break;
        }
        ov13_MBClearChild(aid);
        ov13_0223D0F4();
        break;
    case 10:
        if (ov13_0223D45C() != 2) {
            ov13_0223CCF0(aid);
            break;
        }
        MBW_HEAD.m4 |= 1 << aid;
        ov13_0223CC50(aid);
        user = MB_CommGetChildUser(aid);
        if (user != NULL) {
            MI_CpuCopy8(user, MBW_CHILD(aid - 1).user, 0x16);
        }
        break;
    case 14:
        MBW_HEAD.m4 &= ~(1 << aid);
        MBW_HEAD.m6 |= 1 << aid;
        if (ov13_0223D45C() == 3) {
            ov13_0223CDC8(aid);
        }
        break;
    case 7:
        MBW_HEAD.m8 &= ~(1 << aid);
        MBW_HEAD.mA |= 1 << aid;
        break;
    case 9:
        MBW_HEAD.mA &= ~(1 << aid);
        MBW_HEAD.mC |= 1 << aid;
        ov13_0223D0F4();
        break;
    case 12:
        if (ov13_0223D45C() == 4) {
            ov13_0223D448(5);
        } else {
            ov13_0223D448(0);
        }
        if (MBW_P144 != NULL) {
            MBW_P144 = NULL;
        }
        if (MBW_P140 != NULL) {
            MBW_P140 = NULL;
        }
        break;
    case 13:
        switch (*(u16 *)arg) {
        case 1:
        case 2:
        case 9:
            ov13_0223D448(7);
            break;
        case 8:
            break;
        }
        break;
    default:
        OS_Terminate();
        break;
    }
}
