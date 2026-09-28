#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on

typedef void (*Func)(void);
typedef void (*TaskFunc)(void *, void *);

extern void ov13_022403B8(int, int, int, int);
extern void ov13_02240EA8(int, int);
extern void ov13_02240EFC(int, int);
extern void ov13_0222B8E8(Func);
extern void ov13_0222B8F8(int, int);
extern int ov13_02240394(int);
extern void ov13_022279B4(int);
extern int ov13_02227258(void);
extern void ov13_022272CC(void);
extern void ov13_022272E4(void);
extern void ov13_02227228(void);
extern int ov13_022272A0(void);
extern void ov13_02227028(void);
extern void ov13_0222788C(void);
extern int ov13_02241694(int);
extern void ov13_0222726C(int);
extern int ov13_02238C2C(void);
extern void ov13_0223AC20(void);
extern void ov13_0223ABB8(int);
extern void ov13_022405B8(int);
extern void ov13_0223B940(void);
extern void ov13_02240B04(int);
extern void ov13_0223BE6C(int, int, int, int, int);
extern void ov13_0223BA24(void *);
extern int ov13_0223C1DC(void);
extern int ov13_0223C1F0(void);
extern void ov13_0223C198(void);
extern void ov13_022368D8(void);
extern void ov13_022388A0(void);
extern void ov13_02238694(void);
extern void *ov13_0224128C(u32, u32);
extern void ov13_022412B0(void *);
extern void *ov13_02242528(int, TaskFunc, void *, int);
extern void ov13_022425F0(int, void *);
extern void ov13_022425D0(int, void *);

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
extern void ov13_02240E50(int, int, Point *);
extern void *ov13_022401B0(int, int, int);
extern void ov13_0223FF18(void *, int, int, int);
extern void ov13_02240008(void *, int, int);
extern void ov13_0223FD60(void *);
extern void ov13_022419DC(Point *);
extern void ov13_0223ABDC(int);
extern void ov13_0223ABFC(int, int);
extern int ov13_02241754(void *);
extern int ov13_02241864(Rect *);
extern int ov13_022417DC(Rect *);
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
extern int ov13_02242074(const char *, int);
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
extern void *ov13_0223F95C(void *, u32 *, int);
extern void NNS_SndArcInitOnMemory(void *, void *);
extern int NNS_SndArcPlayerStartSeqArc(void *, int, int);
extern void NNS_SndPlayerSetVolume(void *, int);
extern void NNS_SndPlayerSetTrackPitch(void *, u32, int);
extern void NNS_SndPlayerStopSeq(void *, int);
extern void ov13_0223FE98(void *, int, int, int);
extern void ov13_02240068(void *, int, int *, int *);
extern void ov13_022425C8(void *, TaskFunc);
extern void ov13_0223B6F4(int);

void ov13_0223AC40(void *, void *);
void ov13_0223AD48(void *, void *);
void ov13_0223AE2C(void *, void *);
void ov13_0223AF10(void *, void *);
void ov13_0223AFF4(void *, void *);
extern void ov13_0223B0D8(void *, void *);


void ov13_0223AAFC(void);

// @fn ov13_0223AA58
void ov13_0223AA58(const char *str, u8 *out) {
    u8 buf[4];
    int i;
    int j;
    int off;
    MI_CpuFill8(buf, 0, 4);
    for (i = 0, off = 0; i < 4; i++, off += 3) {
        MI_CpuCopy8(str + off, buf, 3);
        for (j = 0; j < 3; j++) {
            if (buf[j] != 0) {
                break;
            }
            buf[j] = ' ';
        }
        out[i] = atol((char *)buf);
    }
}

// @fn ov13_0223AAE0
int ov13_0223AAE0(u8 c) {
    if (c <= '9') {
        return c - '0';
    }
    if (c <= 'F') {
        return c - 0x37;
    }
    return c - 0x57;
}

// @fn ov13_0223AAFC
void ov13_0223AAFC(void) {
    u32 size;
    ov13_0224F5A8 = ov13_0224128C(0xa0, 4);
    ov13_0224F5A8->data = ov13_0223F95C(ov13_022467D8, &size, 0x20);
    NNS_SndInit();
    NNS_SndArcInitOnMemory(ov13_0224F5A8, ov13_0224F5A8->data);
    NNS_SndArcPlayerSetup(0);
    NNS_SndHandleInit((NNSSndHandle *)ov13_0224F5A8->handle);
    ov13_0224F5A8->task = ov13_02242528(0, ov13_0223AC40, NULL, 0xc8);
}
