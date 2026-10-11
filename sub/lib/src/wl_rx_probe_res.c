#pragma force_active on
#include "wl_internal.h"
// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

// Active scan (MLME state 0x13): a probe response, or a beacon (RxBeaconFrame passes its elements). A BSS listed in
// the scan request only sets its bit in the confirm; another one not described yet gets a WlBssDesc in the confirm.
// The scan ends (state 0x15) when the confirm has less than 32 halfwords left.
void RxProbeResFrame(WlRxFrame *frame, WlElements *info) {
    WlWork *w = wlWorkPlain;
    WlMlme *mlme = &w->mlme;
    WlMlmeScanCfm *cfm;
    WlBssDesc *desc;
    u8 *src;
    u8 *dst;
    u32 i, k, n, length;
    u16 *known;
    u32 id, len;
    WlElements elements;

    if (w->mlme.state != 0x13) {
        return;
    }
    cfm = mlme->cfm.scan;
    known = mlme->req.scan->knownBssid[0];
    for (i = 0; i < mlme->req.scan->knownBssidCount; i++) {
        if (MatchMacAdrs(known, frame->addr3)) {
            cfm->knownBssidFound |= 1 << i;
            return;
        }
        known += 3;
    }
    desc = (WlBssDesc *)cfm->bssDesc;
    for (i = 0; i < cfm->bssCount; i++) {
        if (MatchMacAdrs(frame->addr3, desc->bssid)) {
            return;
        }
        desc = (WlBssDesc *)((u16 *)desc + desc->length);
    }
    __VENEER_MIi_CpuClear16(0, desc, sizeof(WlBssDesc));
    length = frame->length;
    if (length <= 12) {
        return;
    }
    if (info == NULL) {
        __VENEER_MIi_CpuClear32(0, &elements, sizeof(elements));
        elements.elements = WL_RX_BODY(frame, WlBeaconBody)->elements;
        elements.length = length - 12;
        elements.match = 3;
        elements.found = 0x38;
        elements.unk_04 = frame->status;
        elements.capaInfo = WL_RX_BODY(frame, WlBeaconBody)->capaInfo;
        ElementChecker(&elements);
        info = &elements;
    }
    if (info->gameInfo) {
        desc->gameInfoLength = WL_ReadByte(info->gameInfo + 1) - 8;
        n = desc->gameInfoLength;
    } else {
        n = info->otherLength;
    }
    desc->length = (n + sizeof(WlBssDesc) + 1) >> 1;
    if ((info->match & 1) == 1 && wlWorkPlain->mlme.work.scan.cfmSpace >= desc->length) {
        desc->capaInfo = WL_RX_BODY(frame, WlBeaconBody)->capaInfo;
        WSetMacAdrs1(desc->bssid, frame->addr3);
        desc->beaconPeriod = WL_RX_BODY(frame, WlBeaconBody)->beaconPeriod;
        desc->rssi = frame->rssi & 0xFF;
        if (info->gameInfo) {
            // game information element: id, length, 8 bytes, the game information
            for (i = 0; i < desc->gameInfoLength; i++) {
                WL_WriteByte(desc->data + i, WL_ReadByte(info->gameInfo + 10 + i));
            }
        } else {
            // the unknown elements (id above 6), whole
            desc->otherElementCount = info->otherCount;
            if (info->otherCount) {
                src = WL_RX_BODY(frame, WlBeaconBody)->elements;
                dst = desc->data;
                k = 0;
                while (k < info->otherCount) {
                    id = WL_ReadByte(src);
                    len = WL_ReadByte(src + 1);
                    if (id > 6 && src != info->gameInfo) {
                        n = len + 2;
                        for (i = 0; i < n; i++) {
                            WL_WriteByte(dst, WL_ReadByte(src));
                            dst++;
                            src++;
                        }
                        k++;
                    } else {
                        src += len + 2;
                    }
                }
            }
        }
        if (info->ssid) {
            desc->ssidLength = WL_ReadByte(info->ssid + 1);
            for (i = 0; i < desc->ssidLength; i++) {
                WL_WriteByte(desc->ssid + i, WL_ReadByte(info->ssid + 2 + i));
            }
        } else {
            desc->ssidLength = 0;
            for (i = 0; i < 32; i++) {
                WL_WriteByte(desc->ssid + i, 0);
            }
        }
        desc->rateSet.basic = info->rateSet.basic;
        desc->rateSet.supported = info->rateSet.supported;
        desc->channel = info->channel;
        if (info->cf) {
            desc->cfpPeriod = WL_ReadByte(info->cf + 3);
        }
        if (info->tim) {
            desc->dtimPeriod = WL_ReadByte(info->tim + 3);
        }
        cfm->header.length += desc->length;
        cfm->bssCount++;
        mlme->work.scan.cfmSpace -= desc->length;
        if (mlme->work.scan.cfmSpace < 32) {
            ClearTimeOut();
            mlme->state = 0x15;
            AddTask(2, 0);
        }
    } else {
        WSetMacAdrs1(desc->bssid, (const u16 *)NULL_ADRS);
    }
}
