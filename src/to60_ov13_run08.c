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
extern void ov13_0223ABB8(int);
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

void ov13_02238D88(void);
void ov13_02239228(void *src);

void ov13_02238D88(void) {
    WifiSetupFade_Start(2, 0, 0x15, 8);
    WifiSetupDisplay_ShowPlanes(0, 0x15);
    ov13_0222B8E8(ov13_02238DBC);
}

void ov13_02238DBC(void) {
    if (WifiSetupFade_IsActive(0) != 0) {
        return;
    }
    ov13_022279B4(2);
    ov13_0222B8E8(ov13_02238DE8);
}

void ov13_02238DE8(void) {
    if (WifiSetupButtonBar_GetSelected() == -2) {
        return;
    }
    WifiSetupButtonBar_Unlock();
    ov13_0222B8E8(ov13_02238E10);
}

void ov13_02238E10(void) {
    ov13_02238E24();
    ov13_02238E5C();
    ov13_02238E60();
}

void ov13_02238E24(void) {
    if (WifiSetupInput_IsKeyPressed(2)) {
        WifiSetupButtonBar_SelectIfNone(0);
        return;
    }
    if (ov13_02238C2C()) {
        WifiSetupButtonBar_SelectIfNone(0);
    }
}

void ov13_02238E5C(void) {
}

void ov13_02238E60(void) {
    if (ov13_0224F58C != 0) {
        return;
    }
    if (WifiSetupButtonBar_GetSelected() != 0) {
        return;
    }
    ov13_0224F58C = 2;
    ov13_0223AC20();
    ov13_0223ABB8(7);
    ov13_0222B8E8(ov13_02238EAC);
}

void ov13_02238EAC(void) {
    WifiSetupButtonBar_Lock();
    WifiSetupFade_StartDelay(8);
    ov13_0222B8E8(ov13_02238ECC);
}

void ov13_02238ECC(void) {
    if (WifiSetupFade_IsActive(1) != 0) {
        return;
    }
    WifiSetupButtonBar_Close();
    if (ov13_0224F58C == 2 || ov13_0224F58C == 3) {
        WifiSetupFade_Start(3, 1, 1, 8);
    }
    WifiSetupFade_Start(3, 0, 0x15, 8);
    ov13_0222B8E8(ov13_02238F38);
}

void ov13_02238F38(void) {
    if (WifiSetupFade_IsActive(0) != 0) {
        return;
    }
    if (ov13_0224F58C == 2) {
        if (WifiSetupFade_IsActive(1) != 0) {
            return;
        }
    }
    if (ov13_022272A0() == 0) {
        return;
    }
    ov13_0223B940();
    WifiSetupBusyIcon_Destroy();
    WifiSetupText_FreeBgCanvas(0);
    if (ov13_0224F58C == 2 || ov13_0224F58C == 3) {
        WifiSetupMessage_Hide();
        WifiSetupDisplay_HidePlanes(1, 1);
    }
    WifiSetupDisplay_HidePlanes(0, 0x15);
    if (ov13_0224F58C == 2) {
        ov13_0222B8F8(2, 1);
        ov13_0222B8E8(ov13_022368D8);
    } else if (ov13_0224F58C == 3) {
        ov13_0222B8F8(2, 1);
        ov13_0222B8E8(ov13_022388A0);
    } else {
        ov13_0222B8F8(0, 0);
        ov13_0222B8E8(ov13_02238694);
    }
}

void ov13_0223902C(int a) {
    if (ov13_0224F58C != 0) {
        return;
    }
    switch (a) {
    case 0:
        ov13_0224F58C = 3;
        ov13_0223AC20();
        ov13_0223ABB8(0x12);
        ov13_0223BE6C(0x10, 1, 1, -1, 0);
        WifiSetupButtonBar_Lock();
        ov13_0222B8E8(ov13_02239118);
        break;
    case 1:
        ov13_0224F58C = 1;
        ov13_0223AC20();
        ov13_0222B8E8(ov13_02238EAC);
        break;
    case 2:
        ov13_0223AC20();
        ov13_0223BA24(NULL);
        ov13_0223BE6C(0x11, 0, 1, -1, 0);
        WifiSetupButtonBar_Lock();
        ov13_0222B8E8(ov13_02239164);
        break;
    case 3:
        ov13_0224F58C = 2;
        ov13_0223AC20();
        ov13_0223ABB8(9);
        ov13_0222B8E8(ov13_02238EAC);
        break;
    }
}

void ov13_02239118(void) {
    if (ov13_0223C1DC() != 0) {
        return;
    }
    ov13_0223ABB8(6);
    ov13_0223C198();
    ov13_0222B8E8(ov13_02239144);
}

void ov13_02239144(void) {
    if (ov13_0223C1F0() != 0) {
        return;
    }
    ov13_0222B8E8(ov13_02238EAC);
}

void ov13_02239164(void) {
    int r = ov13_0223C1DC();
    if (r != 0) {
        if (r != 1) {
            return;
        }
        ov13_0224F58C = 3;
        ov13_0223ABB8(6);
    } else {
        ov13_0224F58C = 1;
        ov13_0223ABB8(7);
    }
    ov13_0223C198();
    ov13_0222B8E8(ov13_022391C0);
}

void ov13_022391C0(void) {
    if (ov13_0223C1F0() != 0) {
        return;
    }
    if (ov13_0224F58C == 1) {
        ov13_0222B8E8(ov13_02238EAC);
        return;
    }
    ov13_0223ABB8(0xb);
    WifiSetupButtonBar_Unlock();
    ov13_0224F58C = 0;
    ov13_0223BA24(ov13_0223902C);
    ov13_0222B8E8(ov13_02238E10);
}

void ov13_02239228(void *src) {
    BgBuf *p = WifiSetupHeap_AllocZeroed(0x608, 4);
    ov13_0224F590.buf = p;
    MIi_CpuCopyFast(src, (u32 *)p->data, 0x600);
    ov13_0224F590.buf->task = WifiSetupTask_Create(1, ov13_02239324, NULL, 0x78);
}
