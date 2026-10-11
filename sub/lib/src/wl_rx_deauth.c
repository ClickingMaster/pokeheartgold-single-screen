#pragma force_active on
#include "wl_internal.h"
// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

// -ipa file would otherwise inline the small functions of this file into their callers here.
#pragma dont_inline on

// Deauthentication from a station (parent) or from the parent (child modes).
void RxDeAuthFrame(WlRxFrame *frame) {
    WlBss *bss = &wlWorkPlain->bss;
    u32 station = frame->station;
    switch (bss->mode) {
    case 1: {
        if (CAM_GetStaState(station) > 32) {
            CAM_SetStaState(station, 32);
            MLME_IssueDeAuthIndication(frame->addr2, *(u16 *)((u8 *)frame + 0x2C));
            DeleteTxFrames(station);
        }
    } break;
    case 2:
    case 3: {
        if (bss->state > 32 && MatchMacAdrs(frame->addr2, bss->unk_82)) {
            WSetStaState(32);
            WClearAids();
            MLME_IssueDeAuthIndication(frame->addr2, *(u16 *)((u8 *)frame + 0x2C));
        }
    } break;
    }
}

// Walks the elements of a management frame body: records where they are (found, ssid, cf, tim, gameInfo),
// counts the unknown ones, and sets the match bits (1: SSID, 2: channel of the join request, 4: rates, 8: BSSID,
// 16 / 32: capability bits as the own).
void ElementChecker(WlElements *info) {
    WlBss *bss = &wlWork->bss;
    u8 *p = info->elements;
    int remaining;
    u32 length, id;
    info->channel = bss->channel;
    if (info->found & 0x800) {
        info->match |= 1;
    }
    remaining = info->length;
    while (remaining > 0) {
        id = WL_ReadByte(p);
        {
            u8 *l = p + 1;
            p += 2;
            length = WL_ReadByte(l);
        }
        switch (id) {
        case 0: // SSID
            if (length <= 32) {
                info->found |= 1;
                info->ssid = p - 2;
                if (length == 0 && (info->found & 0x800)) {
                    info->match |= 1;
                } else {
                    info->match &= ~1u;
                    if (WCheckSSID((u16)length, p)) {
                        info->match |= 1;
                    }
                }
            }
            break;
        case 1: // supported rates
            if (length >= 1) {
                u32 basic, supported, actual;
                info->found |= 4;
                WElement2RateSet(p - 2, &info->rateSet);
                basic = bss->rateSet.basic;
                supported = bss->rateSet.supported;
                actual = info->rateSet.basic;
                if (!(actual & ~(basic | supported)) && (((actual | info->rateSet.supported) & basic) == basic)) {
                    info->match |= 4;
                } else {
                    info->match &= ~4u;
                }
            }
            break;
        case 2:
        case 6:
            break;
        case 3: // DS parameter set
            if (length >= 1) {
                info->found |= 2;
                info->channel = WL_ReadByte(p);
                if (info->channel == wlWork->mlme.req.join->channel) {
                    info->match |= 2;
                } else {
                    info->match &= ~2u;
                }
            }
            break;
        case 5: // TIM
            if (length >= 3) {
                info->found |= 0x100;
                info->tim = p - 2;
            }
            break;
        case 4: // CF parameter set
            if (length >= 6) {
                info->found |= 0x200;
                info->cf = p - 2;
            }
            break;
        case 221: // vendor specific: the game information element (00 09 BF 00)
            if (length < 8 || WL_ReadByte(p) != 0 || WL_ReadByte(p + 1) != 9 || WL_ReadByte(p + 2) != 0xBF || WL_ReadByte(p + 3) != 0) {
                info->otherCount++;
                info->otherLength += length + 2;
            } else {
                info->found |= 0x400;
                info->gameInfo = p - 2;
            }
            break;
        default:
            info->otherCount++;
            info->otherLength += length + 2;
            break;
        }
        p += length;
        remaining -= length + 2;
    }
    if ((info->found & 8) && ((bss->bssid[0] & 1) || (info->unk_04 & 0x8000))) {
        info->match |= 8;
    }
    if (info->found & 0x30) {
        if ((info->capaInfo & 3) == (bss->capability & 3)) {
            info->match |= 16;
        }
        if ((info->capaInfo & 16) == (bss->capability & 16)) {
            info->match |= 32;
        }
    }
}

#pragma dont_inline reset
