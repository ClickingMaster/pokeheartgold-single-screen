#include "wifi_setup/wifi_setup_access_points.h"

#include "global.h"

#include "dwc/dwc_bm_internal.h"
#include "nnsys/snd_arc_internal.h"
#include "wifi_setup/wifi_setup_local_wireless.h"
#include "wifi_setup/wifi_setup_ui.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

// Access-point data: the scan for access points over WM, the saved
// connection settings (read with DWCi_BACKUPlRead, one page copied to an edit copy and written back
// with a CRC per page), helpers that parse an address from the address entry's 12 characters, and
// the start and end of the sound archive (its task and the sound functions follow in the next file).

typedef void (*WifiSetupTaskFunc2)(void *, void *);

typedef struct WifiSetupConnectionBody {
    u8 bytes[0xF0];
} WifiSetupConnectionBody;

extern u8 gWifiSetupWpaVendorPrefix[];  // vendor element prefix that marks WPA
extern u8 gWifiSetupApScanEmptyBssid[]; // empty BSSID
extern WmScanExParam gWifiSetupApScanParam;
extern u8 gWifiSetupZeroAddress[];              // address 0.0.0.0
extern u8 gWifiSetupConnectionsAddressFormat[]; // address format
extern u8 gWifiSetupSoundArchivePath[];         // sound archive path

extern long atol(const char *);

void WifiSetupApScan_StoreResults(WMStartScanExCallback *cb);

int WifiSetupApScan_Start(void) {
    WifiSetupApScan *w;
    MIi_CpuClear16(0, (u16 *)gWifiSetupApScan->entries, sizeof(gWifiSetupApScan->entries));
    if (WM_Initialize(gWifiSetupApScan, WifiSetupApScan_Callback, 3) != WM_ERRCODE_OPERATING) {
        return 0;
    }
    do {
        WM_ReadStatus(&gWifiSetupApScan->status);
        w = gWifiSetupApScan;
    } while (w->status.state != 2);
    w->param = gWifiSetupApScanParam;
    w->param.buffer = w->scanBuf;
    gWifiSetupApScan->param.dwell = WM_GetDispersionScanPeriod();
    return WifiSetupApScan_Continue() ? 1 : 0;
}

int WifiSetupApScan_Continue(void) {
    return WM_StartScanEx(WifiSetupApScan_Callback, &gWifiSetupApScan->param) == WM_ERRCODE_OPERATING ? 1 : 0;
}

int WifiSetupApScan_End(void) {
    gWifiSetupApScan->ending = 1;
    WM_ReadStatus(&gWifiSetupApScan->status);
    if (gWifiSetupApScan->status.state != 2) {
        if (WM_Reset(WifiSetupApScan_Callback) != WM_ERRCODE_OPERATING) {
            return 0;
        }
        do {
            WM_ReadStatus(&gWifiSetupApScan->status);
        } while (gWifiSetupApScan->status.state != 2);
    }
    if (WM_End(WifiSetupApScan_Callback) != WM_ERRCODE_OPERATING) {
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
    WMStartScanExCallback *cb = arg;
    if (cb->errcode != WM_ERRCODE_SUCCESS) {
        return;
    }
    if (gWifiSetupApScan->ending != 0 || cb->apiid != 0x26) {
        return;
    }
    switch (cb->state) {
    case 5:
        WifiSetupApScan_StoreResults(cb);
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

void WifiSetupApScan_StoreResults(WMStartScanExCallback *cb) {
    WMOtherElementResults elems;
    WifiSetupScanEntry *entries;
    WMBssDesc *desc;
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
        elems = WM_GetOtherElements(desc);
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
    gWifiSetupConnections->edit.setupMethod = WIFI_SETUP_METHOD_MANUAL;
}

#pragma force_active on

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
        gWifiSetupConnections->edit.keyType = WM_WEPMODE_NO;
        break;
    case 5:
    case 10:
        gWifiSetupConnections->edit.keyType = WM_WEPMODE_40BIT;
        break;
    case 13:
    case 26:
        gWifiSetupConnections->edit.keyType = WM_WEPMODE_104BIT;
        break;
    default:
        gWifiSetupConnections->edit.keyType = WM_WEPMODE_128BIT;
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

#pragma force_active reset

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
    case WM_WEPMODE_40BIT:
        w->keyType = WM_WEPMODE_40BIT;
        len = 5;
        break;
    case WM_WEPMODE_104BIT:
        w->keyType = WM_WEPMODE_104BIT;
        len = 13;
        break;
    case WM_WEPMODE_128BIT:
        w->keyType = WM_WEPMODE_128BIT;
        len = 16;
        break;
    default:
        w->keyType = WM_WEPMODE_NO;
        len = 0;
        break;
    }
    w->keyFormat = 0;
    for (i = 0; i < 4; i++) {
        MI_CpuCopy8(src->wepKeys[i], w->wepKeys[i], len);
    }
    w->setupMethod = WIFI_SETUP_METHOD_AUTO;
    MI_CpuFill8(w->mask, 0, 4);
    w->autoAddress = 1;
    w->autoDns = 1;
    WifiSetupConnections_SaveEdit();
}

void WifiSetupConnections_SetFromInfo(u8 *src) {
    WifiSetupConnection *work = &gWifiSetupConnections->edit;
    MI_CpuFill8(work, 0, 0xef);
    MI_CpuCopy8(src, &work->wep40Keys[0], 5);
    MI_CpuCopy8(src + 6, &work->wep40Keys[5], 5);
    MI_CpuCopy8(src + 0xc, &work->wep40Keys[10], 5);
    MI_CpuCopy8(src + 0x12, &work->wep40Keys[15], 5);
    MI_CpuCopy8(src + 0x18, work->unk_60, 0x20);
    MI_CpuCopy8(src + 0x39, work->wepKeys[0], 0xd);
    MI_CpuCopy8(src + 0x47, work->wepKeys[1], 0xd);
    MI_CpuCopy8(src + 0x55, work->wepKeys[2], 0xd);
    MI_CpuCopy8(src + 0x63, work->wepKeys[3], 0xd);
    MI_CpuCopy8(src + 0x71, work->ssid, 0x20);
    work->keyType = WM_WEPMODE_104BIT;
    work->keyFormat = 0;
    work->setupMethod = WIFI_SETUP_METHOD_COPIED;
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
    page->setupMethod = WIFI_SETUP_METHOD_ERASED;
    WifiSetupConnections_WritePage(idx);
}

void WifiSetupConnections_Reset(void) {
    DwcWiFiInfo info;
    u8 *conv;
    int i;
    MIi_CpuClear16(0, (u16 *)gWifiSetupConnections, 0x400);
    for (i = 0; i < 3; i++) {
        gWifiSetupConnections->pages[i].setupMethod = WIFI_SETUP_METHOD_ERASED;
    }
    DWCi_AUTH_GetNewWiFiInfo(&info);
    conv = DWCi_BACKUPlConvWifiInfo(&info);
    for (i = 0; i < 2; i++) {
        MI_CpuCopy8(conv, gWifiSetupConnections->pages[i].tail.page.info, 0xe);
    }
    for (i = 0; i < 4; i++) {
        WifiSetupConnections_WritePage(i);
    }
}

void WifiSetupConnections_WritePage(int index) {
    BOOL changed[4];
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
    gWifiSetupSound = WifiSetupHeap_AllocZeroed(sizeof(WifiSetupSound), 4);
    gWifiSetupSound->data = WifiSetupFile_Load(gWifiSetupSoundArchivePath, &size, 0x20);
    NNS_SndInit();
    NNS_SndArcInitOnMemory(&gWifiSetupSound->arc, gWifiSetupSound->data);
    NNS_SndArcPlayerSetup(0);
    NNS_SndHandleInit(&gWifiSetupSound->handle);
    gWifiSetupSound->task = WifiSetupTask_Create(0, WifiSetupSound_UpdateTask, NULL, 0xc8);
}

WifiSetupSound *gWifiSetupSound;

void WifiSetupSound_Free(void) {
    WifiSetupTask_DestroyLater(0, gWifiSetupSound->task);
    WifiSetupHeap_FreeAndClear(&gWifiSetupSound);
}
