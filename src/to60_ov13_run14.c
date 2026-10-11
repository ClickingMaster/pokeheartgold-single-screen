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

void ov13_0223B1A0(void);
void ov13_0223B1E0(int n);
BOOL ov13_0223B2A8(void);

void ov13_0223B178(void *task) {
    WifiSetupTask_DestroyLater(0, task);
    ov13_0224F5AC.p->task = NULL;
}

void ov13_0223B1A0(void) {
    ov13_0224F5AC.p->unk18 = 1;
    ov13_0224F5AC.p->task = WifiSetupTask_Create(0, ov13_0223B2C8, 0, 0x78);
}

void ov13_0223B1E0(int n) {
    int i;
    for (i = 0; i < 5; i += 2) {
        WifiSetupOamTemplate_Copy(1, ov13_022457EC[n][i], WifiSetupOamBlock_GetOam(ov13_0224F5AC.p->objs[i], 0));
        WifiSetupOamBlock_SetPriority(ov13_0224F5AC.p->objs[i], -1, 0);
        WifiSetupOamBlock_SetPosition(ov13_0224F5AC.p->objs[i], -1, ov13_0224580A[i].x, ov13_0224580A[i].y);
        WifiSetupOamBlock_SetModeAndPalette(ov13_0224F5AC.p->objs[i], -1, 0, ov13_022457FB[n][i]);
    }
    ov13_0223B6F4(n);
}

BOOL ov13_0223B2A8(void) {
    return ov13_0224F5AC.p->task != NULL;
}
