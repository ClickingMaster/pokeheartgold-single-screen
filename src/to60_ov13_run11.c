#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

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

int ov13_02239B68(void);
int ov13_02239C68(void);
int ov13_02239D04(ScanEntry **out);
void ov13_02239F98(void);
void ov13_02239FDC(void);
BackupWork *ov13_02239FF0(void);
void ov13_0223A004(u8 v);
void ov13_0223A018(u8 v);
void ov13_0223A02C(const void *ssid);
void ov13_0223A05C(const char *key);
void ov13_0223A1F4(const char *str);
void ov13_0223A210(const char *str);
void ov13_0223A22C(const char *str);
void ov13_0223A24C(const char *str);
void ov13_0223A26C(const char *str);
void ov13_0223A28C(void *dst);
void ov13_0223A2B0(char *buf);
void ov13_0223A2F0(char *buf);
void ov13_0223A330(char *buf);
void ov13_0223A374(char *buf);
void ov13_0223A3B8(char *buf);
u8 ov13_0223A3FC(int idx);
void ov13_0223A414(int idx);
void ov13_0223A5D0(WepSetting *src);
void ov13_0223A6E4(u8 *src);
BackupData *ov13_0223A7F4(void);
void ov13_0223A804(int idx);
void ov13_0223A840(void);

int ov13_02239B68(void) {
    WmWork *w;
    MIi_CpuClear16(0, (u16 *)ov13_0224F5A0->entries, sizeof(ov13_0224F5A0->entries));
    if (WM_Initialize(ov13_0224F5A0, ov13_02239D60, 3) != 2) {
        return 0;
    }
    do {
        WM_ReadStatus(&ov13_0224F5A0->status);
        w = ov13_0224F5A0;
    } while (w->status.state != 2);
    w->param = ov13_022457A4;
    w->param.w[0] = (u32)w->scanBuf;
    *(u16 *)&ov13_0224F5A0->param.w[2] = WM_GetDispersionScanPeriod();
    return ov13_02239C34() ? 1 : 0;
}

int ov13_02239C34(void) {
    return WM_StartScanEx(ov13_02239D60, &ov13_0224F5A0->param) == 2 ? 1 : 0;
}

int ov13_02239C68(void) {
    ov13_0224F5A0->ending = 1;
    WM_ReadStatus(&ov13_0224F5A0->status);
    if (ov13_0224F5A0->status.state != 2) {
        if (WM_Reset(ov13_02239D60) != 2) {
            return 0;
        }
        do {
            WM_ReadStatus(&ov13_0224F5A0->status);
        } while (ov13_0224F5A0->status.state != 2);
    }
    if (WM_End(ov13_02239D60) != 2) {
        return 0;
    }
    return 1;
}

int ov13_02239D04(ScanEntry **out) {
    ScanEntry *e;
    int count = 0;
    int i;
    *out = e = ov13_0224F5A0->entries;
    for (i = 0; i < 20; i++) {
        if (memcmp(e[i].bssid, ov13_0224579C, 6) != 0) {
            count++;
        }
    }
    return count;
}

void ov13_02239D60(void *arg) {
    WmCallbackHead *cb = arg;
    if (cb->errcode != 0) {
        return;
    }
    if (ov13_0224F5A0->ending != 0 || cb->apiid != 0x26) {
        return;
    }
    switch (cb->state) {
    case 5:
        ov13_02239DC4(arg);
        ov13_02239C34();
        break;
    case 4:
        ov13_02239C34();
        break;
    default:
        OS_Terminate();
        break;
    }
}

void ov13_02239DC4(ScanCallback *cb) {
    OtherElements elems;
    ScanEntry *entries;
    BssDesc *desc;
    int i;
    int j;
    int k;
    int n;

    entries = ov13_0224F5A0->entries;
    DC_InvalidateRange(ov13_0224F5A0->scanBuf, 0x400);
    for (i = 0; i < cb->bssDescCount; i++) {
        desc = cb->bssDesc[i];
        if (desc->ssid[0] == 0 || desc->gameInfoLength != 0) {
            continue;
        }
        for (j = 0; j < 20; j++) {
            if (memcmp(desc->bssid, entries[j].bssid, 6) == 0) {
                break;
            }
        }
        if (j == 20) {
            for (j = 0; j < 20; j++) {
                if (memcmp(entries[j].bssid, ov13_0224579C, 6) == 0) {
                    break;
                }
            }
            if (j == 20) {
                return;
            }
        }
        MI_CpuCopy8(desc->bssid, entries[j].bssid, 6);
        MI_CpuCopy8(desc->ssid, entries[j].ssid, 0x20);
        entries[j].link = cb->linkLevel[i];
        if (!(desc->capaInfo & 0x10)) {
            entries[j].security = 0;
            continue;
        }
        entries[j].security = 1;
        WM_GetOtherElements(&elems, desc);
        n = elems.count;
        for (k = 0; k < n; k++) {
            if (elems.element[k].id == 0x30) {
                entries[j].security = 2;
                break;
            }
            if (elems.element[k].id == 0xdd && elems.element[k].length >= 4) {
                if (memcmp(elems.element[k].body, ov13_02245798, 4) == 0) {
                    entries[j].security = 2;
                    break;
                }
            }
        }
    }
}

void ov13_02239F98(void) {
    BackupData *d = ov13_0224128C(sizeof(BackupData), 0x20);
    ov13_0224F5A4 = d;
    MATHi_CRC16InitTableRev(&d->crcTable, 0xA001);
    DWCi_BACKUPlRead(ov13_0224F5A4);
}

void ov13_02239FDC(void) {
    ov13_022412B0(&ov13_0224F5A4);
}

BackupWork *ov13_02239FF0(void) {
    return &ov13_0224F5A4->work;
}

void ov13_0223A004(u8 v) {
    ov13_0224F5A4->work.useDhcp = v;
}

void ov13_0223A018(u8 v) {
    ov13_0224F5A4->work.autoDns = v;
}

void ov13_0223A02C(const void *ssid) {
    MI_CpuCopy8(ssid, ov13_0224F5A4->work.ssid, 0x20);
    ov13_0224F5A4->work.state = 0;
}

void ov13_0223A05C(const char *key) {
    int i;
    int len;
    u8 *p;
    int hi;

    MI_CpuFill8(ov13_0224F5A4->work.wep[0], 0, 0x10);
    len = ov13_02242074(key, 0x20);
    switch (len) {
    case 0:
    case 10:
    case 26:
    case 32:
        ov13_0224F5A4->work.modeHi = 0;
        p = ov13_0224F5A4->work.wep[0];
        for (i = 0; i < len; i += 2) {
            hi = ov13_0223AAE0(key[i]);
            *p++ = ov13_0223AAE0(key[i + 1]) + (hi << 4);
        }
        break;
    default:
        ov13_0224F5A4->work.modeHi = 1;
        MI_CpuCopy8(key, ov13_0224F5A4->work.wep[0], 0x10);
        break;
    }
    switch (len) {
    case 0:
        ov13_0224F5A4->work.mode = 0;
        break;
    case 5:
    case 10:
        ov13_0224F5A4->work.mode = 1;
        break;
    case 13:
    case 26:
        ov13_0224F5A4->work.mode = 2;
        break;
    default:
        ov13_0224F5A4->work.mode = 3;
        break;
    }
}

void ov13_0223A1F4(const char *str) {
    ov13_0223AA58(str, ov13_0224F5A4->work.ip);
}

void ov13_0223A210(const char *str) {
    ov13_0223AA58(str, ov13_0224F5A4->work.mask);
}

void ov13_0223A22C(const char *str) {
    ov13_0223AA58(str, ov13_0224F5A4->work.gateway);
}

void ov13_0223A24C(const char *str) {
    ov13_0223AA58(str, ov13_0224F5A4->work.dns1);
}

void ov13_0223A26C(const char *str) {
    ov13_0223AA58(str, ov13_0224F5A4->work.dns2);
}

void ov13_0223A28C(void *dst) {
    MI_CpuCopy8(ov13_0224F5A4->work.ssid, dst, 0x20);
}

void ov13_0223A2B0(char *buf) {
    BackupData *d = ov13_0224F5A4;
    OS_SPrintf(buf, (char *)ov13_022467C8, d->work.ip[0], d->work.ip[1], d->work.ip[2], d->work.ip[3]);
}

void ov13_0223A2F0(char *buf) {
    BackupData *d = ov13_0224F5A4;
    OS_SPrintf(buf, (char *)ov13_022467C8, d->work.mask[0], d->work.mask[1], d->work.mask[2], d->work.mask[3]);
}

void ov13_0223A330(char *buf) {
    BackupData *d = ov13_0224F5A4;
    u8 *a = d->work.gateway;
    OS_SPrintf(buf, (char *)ov13_022467C8, a[0], a[1], a[2], a[3]);
}

void ov13_0223A374(char *buf) {
    BackupData *d = ov13_0224F5A4;
    u8 *a = d->work.dns1;
    OS_SPrintf(buf, (char *)ov13_022467C8, a[0], a[1], a[2], a[3]);
}

void ov13_0223A3B8(char *buf) {
    BackupData *d = ov13_0224F5A4;
    u8 *a = d->work.dns2;
    OS_SPrintf(buf, (char *)ov13_022467C8, a[0], a[1], a[2], a[3]);
}

u8 ov13_0223A3FC(int idx) {
    return ov13_0224F5A4->pages[idx].state;
}

void ov13_0223A414(int idx) {
    BackupData *d = ov13_0224F5A4;
    BackupPage *page = &d->pages[idx];
    *(BackupBody *)&d->work = *(BackupBody *)page;
    d->work.index = idx;
    if (memcmp(page->ip, ov13_022457E8, 4) != 0) {
        ov13_0224F5A4->work.useDhcp = 0;
    } else {
        ov13_0224F5A4->work.useDhcp = 1;
    }
    if (memcmp(page->dns1, ov13_022457E8, 4) != 0 || memcmp(page->dns2, ov13_022457E8, 4) != 0) {
        ov13_0224F5A4->work.autoDns = 0;
    } else {
        ov13_0224F5A4->work.autoDns = 1;
    }
    DWCi_BACKUPlConvMaskAddr(page->cidr, ov13_0224F5A4->work.mask);
}

void ov13_0223A4F4(void) {
    BackupData *d = ov13_0224F5A4;
    BackupWork *work = &d->work;
    BackupPage *page = &d->pages[work->index];
    int cidr;
    *(BackupBody *)page = *(BackupBody *)work;
    if (work->useDhcp) {
        MI_CpuFill8(page->ip, 0, 4);
        MI_CpuFill8(page->gateway, 0, 4);
        cidr = 0;
    } else {
        MI_CpuCopy8(work->ip, page->ip, 4);
        MI_CpuCopy8(work->gateway, page->gateway, 4);
        cidr = DWCi_BACKUPlConvMaskCidr(work->mask);
    }
    page->cidr = cidr;
    if (work->autoDns) {
        MI_CpuFill8(page->dns1, 0, 8);
    } else {
        MI_CpuCopy8(work->dns1, page->dns1, 8);
    }
    ov13_0223A8F0(work->index);
}

void ov13_0223A5D0(WepSetting *src) {
    BackupData *d = ov13_0224F5A4;
    BackupWork *w = &d->work;
    u32 len;
    int i;
    MI_CpuFill8(w, 0, 0xef);
    MI_CpuCopy8(src, w->ssid, 0x20);
    switch (src->mode) {
    case 1:
        w->mode = 1;
        len = 5;
        break;
    case 2:
        w->mode = 2;
        len = 13;
        break;
    case 3:
        w->mode = 3;
        len = 16;
        break;
    default:
        w->mode = 0;
        len = 0;
        break;
    }
    w->modeHi = 0;
    for (i = 0; i < 4; i++) {
        MI_CpuCopy8(src->keys[i], w->wep[i], len);
    }
    w->state = 2;
    MI_CpuFill8(w->mask, 0, 4);
    w->useDhcp = 1;
    w->autoDns = 1;
    ov13_0223A4F4();
}

void ov13_0223A6E4(u8 *src) {
    BackupWork *work = &ov13_0224F5A4->work;
    MI_CpuFill8(work, 0, 0xef);
    MI_CpuCopy8(src, &work->unkD1[0], 5);
    MI_CpuCopy8(src + 6, &work->unkD1[5], 5);
    MI_CpuCopy8(src + 0xc, &work->unkD1[10], 5);
    MI_CpuCopy8(src + 0x12, &work->unkD1[15], 5);
    MI_CpuCopy8(src + 0x18, work->unk60, 0x20);
    MI_CpuCopy8(src + 0x39, work->wep[0], 0xd);
    MI_CpuCopy8(src + 0x47, work->wep[1], 0xd);
    MI_CpuCopy8(src + 0x55, work->wep[2], 0xd);
    MI_CpuCopy8(src + 0x63, work->wep[3], 0xd);
    MI_CpuCopy8(src + 0x71, work->ssid, 0x20);
    work->mode = 2;
    work->modeHi = 0;
    work->state = 1;
    MI_CpuFill8(work->mask, 0, 4);
    work->useDhcp = 1;
    work->autoDns = 1;
    ov13_0223A4F4();
}

BackupData *ov13_0223A7F4(void) {
    return ov13_0224F5A4;
}

void ov13_0223A804(int idx) {
    BackupPage *page = &ov13_0224F5A4->pages[idx];
    MI_CpuFill8(page, 0, 0xef);
    page->state = 0xff;
    ov13_0223A8F0(idx);
}

void ov13_0223A840(void) {
    u8 info[0x14];
    void *conv;
    int i;
    MIi_CpuClear16(0, (u16 *)ov13_0224F5A4, 0x400);
    for (i = 0; i < 3; i++) {
        ov13_0224F5A4->pages[i].state = 0xff;
    }
    DWCi_AUTH_GetNewWiFiInfo(info);
    conv = DWCi_BACKUPlConvWifiInfo(info);
    for (i = 0; i < 2; i++) {
        MI_CpuCopy8(conv, ov13_0224F5A4->pages[i].tail.page.info, 0xe);
    }
    for (i = 0; i < 4; i++) {
        ov13_0223A8F0(i);
    }
}
