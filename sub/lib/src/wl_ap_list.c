#pragma force_active on
#include "wl_internal.h"
// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

// -ipa file would otherwise inline the small functions of this file into their callers here.
#pragma dont_inline on

// Records the sender of a received beacon / probe response in the AP list (a known BSSID updates its
// entry, else a free one or the oldest is used). `ssid`: the SSID element (id, length, SSID).
void UpdateApList(u32 strength, const WlRxFrame *frame, const u8 *ssid) {
    WlApEntry *entry = wlWorkPlain->apList;
    u32 i, empty, oldest, min;
    if (WL_ReadByte(ssid + 1) > 32) {
        return;
    }
    empty = 4;
    oldest = empty;
    min = 0x400;
    for (i = 0; i < 4; i++, entry++) {
        if (entry->rssi) {
            if (MatchMacAdrs(entry->bssid, frame->addr3)) {
                goto filled;
            }
            if (entry->lifeTime < min) {
                min = entry->lifeTime;
                oldest = i;
            }
        } else {
            empty = i;
        }
    }
    if (empty != 4) {
        i = empty;
    } else if (oldest != 4) {
        i = oldest;
    } else {
        return;
    }
filled:
    entry = &wlWorkPlain->apList[i];
    __VENEER_MIi_CpuClear16(0, entry, sizeof(WlApEntry));
    entry->lifeTime = 0x400;
    entry->rssi = (u16)(frame->rssi & 255);
    entry->strength = (u16)strength;
    WSetMacAdrs1(entry->bssid, frame->addr3);
    entry->ssidLength = WL_ReadByte(ssid + 1);
    for (i = 0; i < entry->ssidLength; i++) {
        u32 byte = WL_ReadByte(ssid + 2 + i);
        WL_WriteByte(entry->ssid + i, byte);
    }
    // the frame body starts with the 8-byte timestamp, the beacon interval and the capability
    entry->beaconPeriod = *(u16 *)((u8 *)frame + 0x34);
    entry->capaInfo = *(u16 *)((u8 *)frame + 0x36);
}

void InitApList(void) {
    MIi_CpuClear16(0, (u16 *)wlWorkPlain->apList, sizeof(wlWorkPlain->apList));
}

// Ages the AP list (only the first entry's life time counts down, as in the original).
void UpdateApListTask(void) {
    vu16 *entry = (vu16 *)wlWorkPlain->apList;
    u32 i;
    for (i = 0; i < 4; i++) {
        u32 life = entry[24];
        if (life) {
            entry[24] = (u16)(life - 1);
            if (!entry[24]) {
                entry[0] = 0;
            }
        }
    }
}

#pragma dont_inline reset
