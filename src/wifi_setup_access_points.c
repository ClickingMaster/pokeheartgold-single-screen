#pragma force_active on
#include "wifi_setup_access_points.h"

#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

// Access-point data (0x02239B68-0x0223ABB4): the scan for access points over WM, the saved
// connection settings (read with DWCi_BACKUPlRead, one page copied to an edit copy and written back
// with a CRC per page), helpers that parse an address from the address entry's 12 characters, and
// the start and end of the sound archive (its task and the sound functions follow in the next file).

typedef void (*WifiSetupTaskFunc2)(void *, void *);

typedef struct WifiSetupScanParam {
    u32 words[17];
} WifiSetupScanParam;

typedef struct WifiSetupWmStatus {
    u16 state;
} WifiSetupWmStatus;

// WM work of the scan, followed by the scan state.
typedef struct WifiSetupApScan {
    u8 wmWork[0xF00];
    u8 scanBuf[0x400];
    WifiSetupScanEntry entries[WIFI_SETUP_NUM_SCAN_ENTRIES];
    WifiSetupScanParam param;
    WifiSetupWmStatus status;
    u8 unk_168E[0x1E5C - 0x168E];
    u8 ending;
} WifiSetupApScan;

typedef struct WifiSetupWmCallback {
    u16 apiid;
    u16 errcode;
    u16 unk4;
    u16 unk6;
    u16 state;
} WifiSetupWmCallback;

typedef struct WifiSetupBssDesc {
    u16 length;
    u16 rssi;
    u8 bssid[6];
    u16 ssidLength;
    u8 ssid[0x20];
    u16 capaInfo;
    u8 unk2E[0x3C - 0x2E];
    u16 gameInfoLength;
} WifiSetupBssDesc;

typedef struct WifiSetupScanCallback {
    u16 apiid;
    u16 errcode;
    u16 unk4;
    u16 unk6;
    u16 state;
    u8 unkA[4];
    u16 bssDescCount;
    WifiSetupBssDesc *bssDesc[16];
    u16 linkLevel[16];
} WifiSetupScanCallback;

typedef struct WifiSetupOtherElement {
    u8 id;
    u8 length;
    u8 pad[2];
    u8 *body;
} WifiSetupOtherElement;

typedef struct WifiSetupOtherElements {
    u8 count;
    u8 pad[3];
    WifiSetupOtherElement element[16];
} WifiSetupOtherElements;

typedef struct WifiSetupConnectionBody {
    u8 bytes[0xF0];
} WifiSetupConnectionBody;

extern WifiSetupApScan *gWifiSetupApScan;
extern u8 gWifiSetupWpaVendorPrefix[]; // vendor element prefix that marks WPA
extern u8 gWifiSetupApScanEmptyBssid[]; // empty BSSID
extern WifiSetupScanParam gWifiSetupApScanParam;
extern u8 gWifiSetupZeroAddress[]; // address 0.0.0.0
extern u8 gWifiSetupConnectionsAddressFormat[]; // address format
extern u8 gWifiSetupSoundArchivePath[]; // sound archive path

extern int WM_Initialize(void *, void *, u16);
extern int WM_ReadStatus(void *);
extern u16 WM_GetDispersionScanPeriod(void);
extern int WM_StartScanEx(void *, void *);
extern int WM_Reset(void *);
extern int WM_End(void *);
extern void WM_GetOtherElements(WifiSetupOtherElements *, WifiSetupBssDesc *);
extern int memcmp(const void *, const void *, u32);
extern void DC_InvalidateRange(void *, u32);
extern void DWCi_BACKUPlRead(WifiSetupConnections *);
extern void DWCi_BACKUPlConvMaskAddr(int, u8 *);
extern int DWCi_BACKUPlConvMaskCidr(u8 *);
extern void DWCi_AUTH_GetNewWiFiInfo(void *);
extern void *DWCi_BACKUPlConvWifiInfo(void *);
extern void DWCi_BACKUPlWritePage(WifiSetupConnections *, int *, void *);
extern u16 MATH_CalcCRC16(const MATHCRC16Table *, const void *, u32);
extern long atol(const char *);
extern void NNS_SndArcInitOnMemory(void *, void *);

extern void *WifiSetupHeap_AllocZeroed(u32, u32);
extern void WifiSetupHeap_FreeAndClear(void *);
extern void *WifiSetupFile_Load(void *, u32 *, int);
extern int WifiSetup_StrNLen(const char *, int);
extern void *WifiSetupTask_Create(int, WifiSetupTaskFunc2, void *, int);
extern void WifiSetupTask_DestroyLater(int, void *);
extern void WifiSetupSound_UpdateTask(void *, void *);

void WifiSetupApScan_StoreResults(WifiSetupScanCallback *cb);

int WifiSetupApScan_Start(void) {
    WifiSetupApScan *w;
    MIi_CpuClear16(0, (u16 *)gWifiSetupApScan->entries, sizeof(gWifiSetupApScan->entries));
    if (WM_Initialize(gWifiSetupApScan, WifiSetupApScan_Callback, 3) != 2) {
        return 0;
    }
    do {
        WM_ReadStatus(&gWifiSetupApScan->status);
        w = gWifiSetupApScan;
    } while (w->status.state != 2);
    w->param = gWifiSetupApScanParam;
    w->param.words[0] = (u32)w->scanBuf;
    *(u16 *)&gWifiSetupApScan->param.words[2] = WM_GetDispersionScanPeriod();
    return WifiSetupApScan_Continue() ? 1 : 0;
}

int WifiSetupApScan_Continue(void) {
    return WM_StartScanEx(WifiSetupApScan_Callback, &gWifiSetupApScan->param) == 2 ? 1 : 0;
}

int WifiSetupApScan_End(void) {
    gWifiSetupApScan->ending = 1;
    WM_ReadStatus(&gWifiSetupApScan->status);
    if (gWifiSetupApScan->status.state != 2) {
        if (WM_Reset(WifiSetupApScan_Callback) != 2) {
            return 0;
        }
        do {
            WM_ReadStatus(&gWifiSetupApScan->status);
        } while (gWifiSetupApScan->status.state != 2);
    }
    if (WM_End(WifiSetupApScan_Callback) != 2) {
        return 0;
    }
    return 1;
}

int WifiSetupApScan_GetEntries(WifiSetupScanEntry **out) {
    WifiSetupScanEntry *e;
    int count = 0;
    int i;
    *out = e = gWifiSetupApScan->entries;
    for (i = 0; i < 20; i++) {
        if (memcmp(e[i].bssid, gWifiSetupApScanEmptyBssid, 6) != 0) {
            count++;
        }
    }
    return count;
}

void WifiSetupApScan_Callback(void *arg) {
    WifiSetupWmCallback *cb = arg;
    if (cb->errcode != 0) {
        return;
    }
    if (gWifiSetupApScan->ending != 0 || cb->apiid != 0x26) {
        return;
    }
    switch (cb->state) {
    case 5:
        WifiSetupApScan_StoreResults(arg);
        WifiSetupApScan_Continue();
        break;
    case 4:
        WifiSetupApScan_Continue();
        break;
    default:
        OS_Terminate();
        break;
    }
}

void WifiSetupApScan_StoreResults(WifiSetupScanCallback *cb) {
    WifiSetupOtherElements elems;
    WifiSetupScanEntry *entries;
    WifiSetupBssDesc *desc;
    int i;
    int j;
    int k;
    int n;

    entries = gWifiSetupApScan->entries;
    DC_InvalidateRange(gWifiSetupApScan->scanBuf, 0x400);
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
                if (memcmp(entries[j].bssid, gWifiSetupApScanEmptyBssid, 6) == 0) {
                    break;
                }
            }
            if (j == 20) {
                return;
            }
        }
        MI_CpuCopy8(desc->bssid, entries[j].bssid, 6);
        MI_CpuCopy8(desc->ssid, entries[j].ssid, 0x20);
        entries[j].linkLevel = cb->linkLevel[i];
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
                if (memcmp(elems.element[k].body, gWifiSetupWpaVendorPrefix, 4) == 0) {
                    entries[j].security = 2;
                    break;
                }
            }
        }
    }
}

void WifiSetupConnections_Load(void) {
    WifiSetupConnections *d = WifiSetupHeap_AllocZeroed(sizeof(WifiSetupConnections), 0x20);
    gWifiSetupConnections = d;
    MATHi_CRC16InitTableRev(&d->crcTable, 0xA001);
    DWCi_BACKUPlRead(gWifiSetupConnections);
}

void WifiSetupConnections_Free(void) {
    WifiSetupHeap_FreeAndClear(&gWifiSetupConnections);
}

WifiSetupConnection *WifiSetupConnections_GetEdit(void) {
    return &gWifiSetupConnections->edit;
}

void WifiSetupConnections_SetAutoAddress(u8 v) {
    gWifiSetupConnections->edit.autoAddress = v;
}

void WifiSetupConnections_SetAutoDns(u8 v) {
    gWifiSetupConnections->edit.autoDns = v;
}

void WifiSetupConnections_SetSsid(const void *ssid) {
    MI_CpuCopy8(ssid, gWifiSetupConnections->edit.ssid, 0x20);
    gWifiSetupConnections->edit.setupMethod = 0;
}

void WifiSetupConnections_SetWepKey(const char *key) {
    int i;
    int len;
    u8 *p;
    int hi;

    MI_CpuFill8(gWifiSetupConnections->edit.wepKeys[0], 0, 0x10);
    len = WifiSetup_StrNLen(key, 0x20);
    switch (len) {
    case 0:
    case 10:
    case 26:
    case 32:
        gWifiSetupConnections->edit.keyFormat = 0;
        p = gWifiSetupConnections->edit.wepKeys[0];
        for (i = 0; i < len; i += 2) {
            hi = WifiSetup_HexDigitValue(key[i]);
            *p++ = WifiSetup_HexDigitValue(key[i + 1]) + (hi << 4);
        }
        break;
    default:
        gWifiSetupConnections->edit.keyFormat = 1;
        MI_CpuCopy8(key, gWifiSetupConnections->edit.wepKeys[0], 0x10);
        break;
    }
    switch (len) {
    case 0:
        gWifiSetupConnections->edit.keyType = 0;
        break;
    case 5:
    case 10:
        gWifiSetupConnections->edit.keyType = 1;
        break;
    case 13:
    case 26:
        gWifiSetupConnections->edit.keyType = 2;
        break;
    default:
        gWifiSetupConnections->edit.keyType = 3;
        break;
    }
}

void WifiSetupConnections_SetIpAddress(const char *str) {
    WifiSetup_ParseAddress(str, gWifiSetupConnections->edit.ip);
}

void WifiSetupConnections_SetSubnetMask(const char *str) {
    WifiSetup_ParseAddress(str, gWifiSetupConnections->edit.mask);
}

void WifiSetupConnections_SetGateway(const char *str) {
    WifiSetup_ParseAddress(str, gWifiSetupConnections->edit.gateway);
}

void WifiSetupConnections_SetPrimaryDns(const char *str) {
    WifiSetup_ParseAddress(str, gWifiSetupConnections->edit.dns1);
}

void WifiSetupConnections_SetSecondaryDns(const char *str) {
    WifiSetup_ParseAddress(str, gWifiSetupConnections->edit.dns2);
}

void WifiSetupConnections_GetSsid(void *dst) {
    MI_CpuCopy8(gWifiSetupConnections->edit.ssid, dst, 0x20);
}

void WifiSetupConnections_FormatIpAddress(char *buf) {
    WifiSetupConnections *d = gWifiSetupConnections;
    OS_SPrintf(buf, (char *)gWifiSetupConnectionsAddressFormat, d->edit.ip[0], d->edit.ip[1], d->edit.ip[2], d->edit.ip[3]);
}

void WifiSetupConnections_FormatSubnetMask(char *buf) {
    WifiSetupConnections *d = gWifiSetupConnections;
    OS_SPrintf(buf, (char *)gWifiSetupConnectionsAddressFormat, d->edit.mask[0], d->edit.mask[1], d->edit.mask[2], d->edit.mask[3]);
}

void WifiSetupConnections_FormatGateway(char *buf) {
    WifiSetupConnections *d = gWifiSetupConnections;
    u8 *a = d->edit.gateway;
    OS_SPrintf(buf, (char *)gWifiSetupConnectionsAddressFormat, a[0], a[1], a[2], a[3]);
}

void WifiSetupConnections_FormatPrimaryDns(char *buf) {
    WifiSetupConnections *d = gWifiSetupConnections;
    u8 *a = d->edit.dns1;
    OS_SPrintf(buf, (char *)gWifiSetupConnectionsAddressFormat, a[0], a[1], a[2], a[3]);
}

void WifiSetupConnections_FormatSecondaryDns(char *buf) {
    WifiSetupConnections *d = gWifiSetupConnections;
    u8 *a = d->edit.dns2;
    OS_SPrintf(buf, (char *)gWifiSetupConnectionsAddressFormat, a[0], a[1], a[2], a[3]);
}

u8 WifiSetupConnections_GetSetupMethod(int idx) {
    return gWifiSetupConnections->pages[idx].setupMethod;
}

void WifiSetupConnections_Edit(int idx) {
    WifiSetupConnections *d = gWifiSetupConnections;
    WifiSetupConnectionPage *page = &d->pages[idx];
    *(WifiSetupConnectionBody *)&d->edit = *(WifiSetupConnectionBody *)page;
    d->edit.index = idx;
    if (memcmp(page->ip, gWifiSetupZeroAddress, 4) != 0) {
        gWifiSetupConnections->edit.autoAddress = 0;
    } else {
        gWifiSetupConnections->edit.autoAddress = 1;
    }
    if (memcmp(page->dns1, gWifiSetupZeroAddress, 4) != 0 || memcmp(page->dns2, gWifiSetupZeroAddress, 4) != 0) {
        gWifiSetupConnections->edit.autoDns = 0;
    } else {
        gWifiSetupConnections->edit.autoDns = 1;
    }
    DWCi_BACKUPlConvMaskAddr(page->cidr, gWifiSetupConnections->edit.mask);
}

void WifiSetupConnections_SaveEdit(void) {
    WifiSetupConnections *d = gWifiSetupConnections;
    WifiSetupConnection *work = &d->edit;
    WifiSetupConnectionPage *page = &d->pages[work->index];
    int cidr;
    *(WifiSetupConnectionBody *)page = *(WifiSetupConnectionBody *)work;
    if (work->autoAddress) {
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
    WifiSetupConnections_WritePage(work->index);
}

void WifiSetupConnections_SetFromClientResult(WifiSetupClientResultData *src) {
    WifiSetupConnections *d = gWifiSetupConnections;
    WifiSetupConnection *w = &d->edit;
    u32 len;
    int i;
    MI_CpuFill8(w, 0, 0xef);
    MI_CpuCopy8(src, w->ssid, 0x20);
    switch (src->keyType) {
    case 1:
        w->keyType = 1;
        len = 5;
        break;
    case 2:
        w->keyType = 2;
        len = 13;
        break;
    case 3:
        w->keyType = 3;
        len = 16;
        break;
    default:
        w->keyType = 0;
        len = 0;
        break;
    }
    w->keyFormat = 0;
    for (i = 0; i < 4; i++) {
        MI_CpuCopy8(src->wepKeys[i], w->wepKeys[i], len);
    }
    w->setupMethod = 2;
    MI_CpuFill8(w->mask, 0, 4);
    w->autoAddress = 1;
    w->autoDns = 1;
    WifiSetupConnections_SaveEdit();
}

void WifiSetupConnections_SetFromInfo(u8 *src) {
    WifiSetupConnection *work = &gWifiSetupConnections->edit;
    MI_CpuFill8(work, 0, 0xef);
    MI_CpuCopy8(src, &work->unk_D1[0], 5);
    MI_CpuCopy8(src + 6, &work->unk_D1[5], 5);
    MI_CpuCopy8(src + 0xc, &work->unk_D1[10], 5);
    MI_CpuCopy8(src + 0x12, &work->unk_D1[15], 5);
    MI_CpuCopy8(src + 0x18, work->unk_60, 0x20);
    MI_CpuCopy8(src + 0x39, work->wepKeys[0], 0xd);
    MI_CpuCopy8(src + 0x47, work->wepKeys[1], 0xd);
    MI_CpuCopy8(src + 0x55, work->wepKeys[2], 0xd);
    MI_CpuCopy8(src + 0x63, work->wepKeys[3], 0xd);
    MI_CpuCopy8(src + 0x71, work->ssid, 0x20);
    work->keyType = 2;
    work->keyFormat = 0;
    work->setupMethod = 1;
    MI_CpuFill8(work->mask, 0, 4);
    work->autoAddress = 1;
    work->autoDns = 1;
    WifiSetupConnections_SaveEdit();
}

WifiSetupConnections *WifiSetupConnections_Get(void) {
    return gWifiSetupConnections;
}

void WifiSetupConnections_Erase(int idx) {
    WifiSetupConnectionPage *page = &gWifiSetupConnections->pages[idx];
    MI_CpuFill8(page, 0, 0xef);
    page->setupMethod = 0xff;
    WifiSetupConnections_WritePage(idx);
}

void WifiSetupConnections_Reset(void) {
    u8 info[0x14];
    void *conv;
    int i;
    MIi_CpuClear16(0, (u16 *)gWifiSetupConnections, 0x400);
    for (i = 0; i < 3; i++) {
        gWifiSetupConnections->pages[i].setupMethod = 0xff;
    }
    DWCi_AUTH_GetNewWiFiInfo(info);
    conv = DWCi_BACKUPlConvWifiInfo(info);
    for (i = 0; i < 2; i++) {
        MI_CpuCopy8(conv, gWifiSetupConnections->pages[i].tail.page.info, 0xe);
    }
    for (i = 0; i < 4; i++) {
        WifiSetupConnections_WritePage(i);
    }
}

void WifiSetupConnections_WritePage(int index) {
    int changed[4];
    void *temp;
    int status = gWifiSetupConnections->pages[index].setupMethod;
    int active = 0, i, offset;
    int value = 1 << index;
    MI_CpuFill8(changed, 0, 16);
    changed[index] = 1;
    if (index <= 2) {
        if (gWifiSetupConnections->pages[0].usedMask & value) {
            active = 1;
        }
        if (status == 255 && active) {
            gWifiSetupConnections->pages[0].usedMask &= ~value;
            gWifiSetupConnections->pages[1].usedMask &= ~value;
            changed[0] = changed[1] = 1;
        } else if (status != 255 && !active) {
            gWifiSetupConnections->pages[0].usedMask |= value;
            gWifiSetupConnections->pages[1].usedMask |= value;
            changed[0] = changed[1] = 1;
        }
    }
    i = 0;
    offset = 0;
    for (; i < 4; i++, offset += 256) {
        if (changed[i]) {
            gWifiSetupConnections->pages[i].tail.page.crc = MATH_CalcCRC16(&gWifiSetupConnections->crcTable, (u8 *)gWifiSetupConnections + offset, 254);
        }
    }
    temp = WifiSetupHeap_AllocZeroed(256, 32);
    DWCi_BACKUPlWritePage(gWifiSetupConnections, changed, temp);
    WifiSetupHeap_FreeAndClear(&temp);
}

void WifiSetup_ParseAddress(const char *str, u8 *out) {
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

int WifiSetup_HexDigitValue(u8 c) {
    if (c <= '9') {
        return c - '0';
    }
    if (c <= 'F') {
        return c - 0x37;
    }
    return c - 0x57;
}

void WifiSetupSound_Init(void) {
    u32 size;
    gWifiSetupSound = WifiSetupHeap_AllocZeroed(0xa0, 4);
    gWifiSetupSound->data = WifiSetupFile_Load(gWifiSetupSoundArchivePath, &size, 0x20);
    NNS_SndInit();
    NNS_SndArcInitOnMemory(gWifiSetupSound, gWifiSetupSound->data);
    NNS_SndArcPlayerSetup(0);
    NNS_SndHandleInit((NNSSndHandle *)gWifiSetupSound->handle);
    gWifiSetupSound->task = WifiSetupTask_Create(0, WifiSetupSound_UpdateTask, NULL, 0xc8);
}

WifiSetupSound *gWifiSetupSound;

void WifiSetupSound_Free(void) {
    WifiSetupTask_DestroyLater(0, gWifiSetupSound->task);
    WifiSetupHeap_FreeAndClear(&gWifiSetupSound);
}
