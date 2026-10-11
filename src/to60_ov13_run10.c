#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

typedef void (*Func)(void);
typedef void (*TaskFunc)(void *, void *);

extern void WifiSetupFade_Start(int, int, int, int);
extern void WifiSetupDisplay_ShowPlanes(int, int);
extern void WifiSetupDisplay_HidePlanes(int, int);
extern void ov13_0222B8E8(Func);
extern void ov13_0222B8F8(int, int);
extern int WifiSetupFade_IsActive(int);
extern void ov13_022279B4(int);
extern int WifiSetupButtonBar_GetSelected(void);
extern void WifiSetupButtonBar_Unlock(void);
extern void WifiSetupButtonBar_Lock(void);
extern void WifiSetupButtonBar_Close(void);
extern int ov13_022272A0(void);
extern void WifiSetupBusyIcon_Destroy(void);
extern void WifiSetupMessage_Hide(void);
extern int WifiSetupInput_IsKeyPressed(int);
extern void WifiSetupButtonBar_SelectIfNone(int);
extern int ov13_02238C2C(void);
extern void ov13_0223AC20(void);
extern void WifiSetupSound_Play(int);
extern void WifiSetupFade_StartDelay(int);
extern void ov13_0223B940(void);
extern void WifiSetupText_FreeBgCanvas(int);
extern void ov13_0223BE6C(int, int, int, int, int);
extern void ov13_0223BA24(void *);
extern int ov13_0223C1DC(void);
extern int ov13_0223C1F0(void);
extern void ov13_0223C198(void);
extern void ov13_022368D8(void);
extern void ov13_022388A0(void);
extern void ov13_02238694(void);
extern void *WifiSetupHeap_AllocZeroed(u32, u32);
extern void WifiSetupHeap_FreeAndClear(void *);
extern void *WifiSetupTask_Create(int, TaskFunc, void *, int);
extern void WifiSetupTask_Destroy(int, void *);
extern void WifiSetupTask_DestroyLater(int, void *);

extern u8 ov13_0224F58C;

typedef struct {
    void *task;
    u16 data[0x300];
    u8 dirty;
} BgBuf;

typedef struct {
    BgBuf *buf;
    void *src;
    void *dst;
} BgState;

extern BgState ov13_0224F590;

void ov13_02238DBC(void);
void ov13_02238DE8(void);
void ov13_02238E10(void);
void ov13_02238E24(void);
void ov13_02238E5C(void);
void ov13_02238E60(void);
void ov13_02238EAC(void);
void ov13_02238ECC(void);
void ov13_02238F38(void);
void ov13_0223902C(int);
void ov13_02239118(void);
void ov13_02239144(void);
void ov13_02239164(void);
void ov13_022391C0(void);
void ov13_02239324(void *, void *);
void ov13_022393B4(void *, void *);
void ov13_0223940C(void *, void *);

typedef struct {
    u16 x;
    u16 y;
} Point;

typedef struct {
    u16 x0;
    u16 y0;
    u16 x1;
    u16 y1;
} Rect;

typedef struct {
    void *obj;
    int unk4;
    int unk8;
    void *task;
    Point pos;
    Point touch;
    u8 start;
    u8 range;
    u8 value;
    u8 kind;
    u8 state;
    u8 result;
    u8 disabled;
} Slider;

extern Slider *ov13_0224F59C;
extern u8 ov13_02245790[];
extern u8 ov13_02245794[];
extern u8 ov13_02245958[];
extern void WifiSetup_SetPoint(int, int, Point *);
extern void *WifiSetupOamTemplate_AllocBlock(int, int, int);
extern void WifiSetupOamBlock_SetPosition(void *, int, int, int);
extern void WifiSetupOamBlock_SetPriority(void *, int, int);
extern void WifiSetupOamBlock_Free(void *);
extern void WifiSetupInput_GetTouchPos(Point *);
extern void ov13_0223ABDC(int);
extern void ov13_0223ABFC(int, int);
extern int WifiSetupInput_IsTouchHeld(void *);
extern int WifiSetupInput_IsTouchRepeated(Rect *);
extern int WifiSetupInput_IsTouchPressed(Rect *);
extern s32 FX_DivS32(s32, s32);

void ov13_022395B4(void *, void *);
void ov13_02239738(void);
void ov13_02239808(int);
void ov13_02239880(int);
void ov13_022398AC(void);
int ov13_02239914(void);
int ov13_022399A0(int);
void ov13_022399EC(int, Rect *);
void ov13_02239AC4(int);

typedef struct {
    u8 ssid[0x20];
    u8 bssid[6];
    u16 link;
    u8 security;
    u8 pad29;
} ScanEntry;

typedef struct {
    u32 w[17];
} ScanParam;

typedef struct {
    u16 state;
} WmStatusHead;

typedef struct {
    u8 unk0[0xf00];
    u8 scanBuf[0x400];
    ScanEntry entries[20];
    ScanParam param;
    WmStatusHead status;
    u8 unk168E[0x1e5c - 0x168e];
    u8 ending;
} WmWork;

typedef struct {
    u16 apiid;
    u16 errcode;
    u16 unk4;
    u16 unk6;
    u16 state;
} WmCallbackHead;

typedef struct {
    u16 length;
    u16 rssi;
    u8 bssid[6];
    u16 ssidLength;
    u8 ssid[0x20];
    u16 capaInfo;
    u8 unk2E[0x3c - 0x2e];
    u16 gameInfoLength;
} BssDesc;

typedef struct {
    u16 apiid;
    u16 errcode;
    u16 unk4;
    u16 unk6;
    u16 state;
    u8 unkA[4];
    u16 bssDescCount;
    BssDesc *bssDesc[16];
    u16 linkLevel[16];
} ScanCallback;

typedef struct {
    u8 id;
    u8 length;
    u8 pad[2];
    u8 *body;
} OtherElement;

typedef struct {
    u8 count;
    u8 pad[3];
    OtherElement element[16];
} OtherElements;

extern WmWork *ov13_0224F5A0;
extern ScanParam ov13_022457A4;
extern u8 ov13_0224579C[];
extern u8 ov13_02245798[];
extern int WM_Initialize(void *, void *, u16);
extern int WM_ReadStatus(void *);
extern u16 WM_GetDispersionScanPeriod(void);
extern int WM_StartScanEx(void *, void *);
extern int WM_Reset(void *);
extern int WM_End(void *);
extern void WM_GetOtherElements(OtherElements *, BssDesc *);
extern int memcmp(const void *, const void *, u32);
extern void DC_InvalidateRange(void *, u32);

int ov13_02239C34(void);
void ov13_02239D60(void *);
void ov13_02239DC4(ScanCallback *);

typedef struct {
    u8 unk0[0x40];
    u8 ssid[0x20];
    u8 unk60[0x20];
    u8 wep[4][0x10];
    u8 ip[4];
    u8 gateway[4];
    u8 dns1[4];
    u8 dns2[4];
    u8 cidr;
    u8 unkD1[0x15];
    u8 mode : 2;
    u8 modeHi : 6;
    u8 state;
    u8 unkE8[7];
    u8 flags;

    union {
        struct {
            u8 mask[4];
            u8 index;
            u8 useDhcp;
            u8 autoDns;
            u8 unkF7;
        } work;

        struct {
            u8 info[0xe];
            u16 crc;
        } page;
    } tail;
} BackupPage;

typedef struct {
    u8 b[0xf0];
} BackupBody;

typedef struct {
    u8 unk0[0x40];
    u8 ssid[0x20];
    u8 unk60[0x20];
    u8 wep[4][0x10];

    union {
        struct {
            u8 ip[4];
            u8 gateway[4];
            u8 dns1[4];
            u8 dns2[4];
        };

        u8 addr[4][4];
    };

    u8 cidr;
    u8 unkD1[0x15];
    u8 mode : 2;
    u8 modeHi : 6;
    u8 state;
    u8 unkE8[7];
    u8 flags;
    u8 mask[4];
    u8 index;
    u8 useDhcp;
    u8 autoDns;
    u8 unkF7;
} BackupWork;

typedef struct {
    BackupPage pages[4];
    BackupWork work;
    MATHCRC16Table crcTable;
} BackupData;

typedef struct {
    u8 ssid[0x20];
    int mode;
    int unk24;
    u8 keys[4][0x20];
} WepSetting;

extern BackupData *ov13_0224F5A4;
extern u8 ov13_022467C8[];
extern u8 ov13_022457E8[];
extern void DWCi_BACKUPlRead(BackupData *);
extern void DWCi_BACKUPlConvMaskAddr(int, u8 *);
extern int DWCi_BACKUPlConvMaskCidr(u8 *);
extern void DWCi_AUTH_GetNewWiFiInfo(void *);
extern void *DWCi_BACKUPlConvWifiInfo(void *);
extern void DWCi_BACKUPlWritePage(BackupData *, int *, void *);
extern u16 MATH_CalcCRC16(const MATHCRC16Table *, const void *, u32);
extern int WifiSetup_StrNLen(const char *, int);
extern long atol(const char *);

void ov13_0223A4F4(void);
void ov13_0223A8F0(int);
void ov13_0223AA58(const char *, u8 *);
int ov13_0223AAE0(u8);

typedef struct {
    u8 arc[0x94];
    u8 handle[4];
    void *data;
    void *task;
} SoundWork;

typedef struct {
    void *obj[5];
    void *task;
} TitleObjs;

typedef struct {
    int unk0;
    TitleObjs *objs;
} TitleWork;

typedef struct {
    u16 x;
    u16 y;
} TitlePos;

extern SoundWork *ov13_0224F5A8;
extern TitleWork ov13_0224F5AC;
extern u8 ov13_022467D8[];
extern u8 ov13_022457EC[];
extern u8 ov13_022457FB[];
extern TitlePos ov13_0224580A[];
extern void *WifiSetupFile_Load(void *, u32 *, int);
extern void NNS_SndArcInitOnMemory(void *, void *);
extern int NNS_SndArcPlayerStartSeqArc(void *, int, int);
extern void NNS_SndPlayerSetVolume(void *, int);
extern void NNS_SndPlayerSetTrackPitch(void *, u32, int);
extern void NNS_SndPlayerStopSeq(void *, int);
extern void WifiSetupOamBlock_SetModeAndPalette(void *, int, int, int);
extern void WifiSetupOamBlock_GetPosition(void *, int, int *, int *);
extern void WifiSetupTask_SetFunc(void *, TaskFunc);
extern void ov13_0223B6F4(int);

void ov13_0223AC40(void *, void *);
void ov13_0223AD48(void *, void *);
void ov13_0223AE2C(void *, void *);
void ov13_0223AF10(void *, void *);
void ov13_0223AFF4(void *, void *);
extern void ov13_0223B0D8(void *, void *);

u8 ov13_02239550(void);
u8 ov13_02239564(void);
void ov13_02239578(int value);
void ov13_02239584(void);
void ov13_0223959C(void);
void ov13_02239B00(void);

u8 ov13_02239550(void) {
    return ov13_0224F59C->value;
}

u8 ov13_02239564(void) {
    return ov13_0224F59C->result;
}

void ov13_02239578(int value) {
    ov13_02239AC4(value);
}

void ov13_02239584(void) {
    ov13_0224F59C->disabled = 0;
}

void ov13_0223959C(void) {
    ov13_0224F59C->disabled = 1;
}

void ov13_022395B4(void *task, void *arg) {
    ov13_0224F59C->result = 0;
    switch (ov13_0224F59C->state) {
    case 0:
        if (ov13_0224F59C->disabled) {
            return;
        }
        switch (ov13_02239914()) {
        case 1:
            if (ov13_0224F59C->kind == 0) {
                return;
            }
            WifiSetupSound_Play(0x16);
            ov13_0223ABDC(0);
            ov13_0224F59C->result = 1;
            WifiSetupInput_GetTouchPos(&ov13_0224F59C->touch);
            ov13_0224F59C->start = ov13_0224F59C->value;
            ov13_0224F59C->state = 1;
            return;
        case 2:
            ov13_02239880(2);
            return;
        case 3:
            ov13_02239880(3);
            return;
        case 4:
            ov13_022398AC();
            return;
        }
        break;
    case 1:
        ov13_02239738();
        break;
    case 2:
        if (ov13_022399A0(2) != 2) {
            ov13_0224F59C->result = 5;
            ov13_0224F59C->state = 0;
            return;
        }
        if (ov13_02239914() == 2) {
            ov13_02239880(2);
        }
        break;
    case 3:
        if (ov13_022399A0(3) != 3) {
            ov13_0224F59C->result = 7;
            ov13_0224F59C->state = 0;
            return;
        }
        if (ov13_02239914() == 3) {
            ov13_02239880(3);
        }
        break;
    }
}

void ov13_02239738(void) {
    Point p;
    Slider *s;
    int v;
    int max;

    if (WifiSetupInput_IsTouchHeld(ov13_02245958)) {
        WifiSetupInput_GetTouchPos(&p);
        s = ov13_0224F59C;
        if (p.x >= s->pos.x - 0x1e) {
            v = s->start + (p.y - s->touch.y);
            if (v < 0) {
                v = 0;
            } else {
                max = s->range - ov13_02245794[s->kind];
                if (v >= max) {
                    v = max;
                }
            }
            ov13_02239808(v);
            ov13_02239AC4(v);
            ov13_0224F59C->result = 2;
            return;
        }
    }
    ov13_0223AC20();
    ov13_0224F59C->state = 0;
    ov13_0224F59C->result = 3;
}

void ov13_02239808(int v) {
    int d = ov13_0224F59C->value - v;
    int a;
    if (d < 0) {
        d = -d;
    }
    if (d < 2) {
        a = 0;
    } else if (d >= 6) {
        a = 0x7f;
    } else {
        a = FX_DivS32(0x7f, 6 - d);
    }
    ov13_0223ABDC(a);
    if (d < 2) {
        a = -0x100;
    } else if (d >= 6) {
        a = 0x100;
    } else {
        a = FX_DivS32(0x200, 6 - d) - 0x100;
    }
    ov13_0223ABFC(0xFFFF, a);
}

void ov13_02239880(int state) {
    ov13_0224F59C->state = state;
    ov13_0224F59C->result = (state == 2) ? 4 : 6;
}

void ov13_022398AC(void) {
    Point p;
    Slider *s;
    u32 w;
    int v;
    int max;

    WifiSetupInput_GetTouchPos(&p);
    s = ov13_0224F59C;
    w = ov13_02245794[s->kind];
    v = p.y - s->pos.y - (w >> 1);
    if (v < 0) {
        v = 0;
    } else {
        max = s->range - w;
        if (v >= max) {
            v = max;
        }
    }
    ov13_02239AC4(v);
    ov13_0224F59C->result = 3;
}

int ov13_02239914(void) {
    Rect r;
    int i;
    ov13_022399EC(1, &r);
    if (WifiSetupInput_IsTouchRepeated(&r)) {
        return 1;
    }
    for (i = 2; i <= 3; i++) {
        ov13_022399EC(i, &r);
        if (WifiSetupInput_IsTouchRepeated(&r)) {
            return i;
        }
    }
    ov13_022399EC(4, &r);
    if (WifiSetupInput_IsTouchPressed(&r)) {
        return 4;
    }
    return 0;
}

int ov13_022399A0(int n) {
    Rect r;
    int i;
    for (i = 2; i <= 3; i++) {
        ov13_022399EC(i, &r);
        if (WifiSetupInput_IsTouchHeld(&r)) {
            return i;
        }
    }
    return 0;
}

void ov13_022399EC(int kind, Rect *r) {
    u16 x = ov13_0224F59C->pos.x;
    u16 y;
    r->x0 = x;
    r->x1 = x + 12;
    switch (kind) {
    case 1:
        r->y0 = ov13_0224F59C->pos.y + ov13_0224F59C->value;
        r->y1 = r->y0 + ov13_02245794[ov13_0224F59C->kind];
        break;
    case 2:
        r->y0 = ov13_0224F59C->pos.y - 13;
        r->y1 = ov13_0224F59C->pos.y;
        break;
    case 3:
        r->y0 = ov13_0224F59C->pos.y + ov13_0224F59C->range;
        r->y1 = r->y0 + 13;
        break;
    case 4:
        y = ov13_0224F59C->pos.y;
        r->y0 = y;
        r->y1 = y + ov13_0224F59C->range;
        break;
    }
}

void ov13_02239AC4(int v) {
    WifiSetupOamBlock_SetPosition(ov13_0224F59C->obj, -1, ov13_0224F59C->pos.x, v + ov13_0224F59C->pos.y);
    ov13_0224F59C->value = v;
}

void ov13_02239B00(void) {
    if (ov13_0224F5A0 != NULL) {
        return;
    }
    ov13_0224F5A0 = WifiSetupHeap_AllocZeroed(0x1E60, 0x20);
}
