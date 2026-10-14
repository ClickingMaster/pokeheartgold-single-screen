#pragma force_active on
#include "wl_internal.h"
// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

// -ipa file would otherwise inline the small functions of this file into their callers here.
#pragma dont_inline on

// Disassociation from a station (parent: back to authenticated, answered when it was not associated) or
// from the parent (child modes).
void RxDisAssFrame(WlRxFrame *frame) {
    WlBss *bss = &wlWorkPlain->bss;
    u32 station = frame->station, state;
    switch (bss->mode) {
    case 1: {
        WlTxHeapFrame *reply;
        state = CAM_GetStaState(station);
        if (state == 64) {
            CAM_SetStaState(station, 48);
            MLME_IssueDisAssIndication(frame->addr2, *(u16 *)((u8 *)frame + 0x2C));
            DeleteTxFrames(station);
        } else {
            if (state == 48) {
                reply = MakeDisAssFrame(frame->addr2, 7);
            } else {
                reply = MakeDeAuthFrame(frame->addr2, 7, 1);
            }
            if (reply) {
                TxManCtrlFrame(reply);
            }
        }
    } break;
    case 2:
    case 3: {
        if (bss->state == 64 && MatchMacAdrs(frame->addr2, bss->parentMacAdrs)) {
            WSetStaState(48);
            WClearAids();
            MLME_IssueDisAssIndication(frame->addr2, *(u16 *)((u8 *)frame + 0x2C));
        }
    } break;
    }
}

#pragma opt_common_subs off

// The capability information at the start of the request body.
#define CAPA(frame) ((WlCapaBits *)((u8 *)(frame) + 0x2C))

// Parent: association request of an authenticated station; answered with an association response
// (status 10: capability not supported, 1: no SSID, 18: rates not supported).
void RxAssReqFrame(WlRxFrame *frame) {
    WlWork *w = wlWorkPlain;
    WlParam *param = &w->param;
    u32 length = frame->length;
    u16 station; // (the original passed it to CAM_SetStaState without a u16 conversion)
    u32 reason;
    WlTxHeapFrame *reply;
    WlElements info;
    if (length <= 4 || w->bss.mode != 1) {
        return;
    }
    if (IsExistManFrame(frame->addr2, 0x10)) {
        return;
    }
    station = frame->station;
    if (!station || CAM_GetStaState(station) < 48) {
        if (IsExistManFrame(frame->addr2, 0xC0)) {
            return;
        }
        reply = MakeDeAuthFrame(frame->addr2, 6, 1);
        if (reply) {
            TxManCtrlFrame(reply);
        }
        return;
    }
    if (CAM_GetStaState(station) == 64) {
        CAM_SetStaState(station, 48);
        MLME_IssueDisAssIndication(frame->addr2, 1);
    } else if (CAM_GetAID(station)) {
        return;
    }
    // body: capability, listen interval, elements
    __VENEER_MIi_CpuClear32(0, (u32 *)&info, sizeof(WlElements));
    info.elements = (u8 *)frame + 0x30;
    info.length = length - 4;
    ElementChecker(&info);
    {
        u16 capaInfo = *(u16 *)((u8 *)frame + 0x2C), wep;
        if ((capaInfo & 0xFFC2) || !CAPA(frame)->ess || ((wep = param->wepMode) == 0 && CAPA(frame)->wep == 1) || (wep != 0 && CAPA(frame)->wep == 0) || ((((u32)param->flags.raw << 29) >> 31) == 1 && !CAPA(frame)->shortPreamble)) {
            reason = 10;
        } else {
            CAM_SetCapaInfo(station, capaInfo);
            if (!(info.match & 1)) {
                reason = 1;
            } else if (!(info.match & 4)) {
                reason = 18;
            } else {
                CAM_SetSupRate(station, info.rateSet.supported);
                reason = 0;
            }
        }
    }
    reply = MakeAssResFrame(station, reason, info.ssid);
    if (reply) {
        TxManCtrlFrame(reply);
    }
}

#undef CAPA

#pragma opt_common_subs reset

// Parent: probe request for the own BSS (or broadcast); answered with a probe response unless the SSID does
// not match (an empty SSID only matches with broadcast SSID responses on).
void RxProbeReqFrame(WlRxFrame *frame) {
    WlElements info;
    WlTxHeapFrame *reply;
    if (IsExistManFrame(frame->addr2, 0x50)) {
        return;
    }
    if (!(frame->addr3[0] & 1) && !(frame->status & 0x8000)) {
        return;
    }
    __VENEER_MIi_CpuClear32(0, (u32 *)&info, sizeof(WlElements));
    info.elements = (u8 *)frame + 0x2C;
    info.length = frame->length;
    if (!wlWorkPlain->param.flags.bits.bcSsidResponse) {
        info.found = 0x800;
    }
    ElementChecker(&info);
    if ((info.match & 1) != 1) {
        return;
    }
    reply = MakeProbeResFrame(frame->addr2);
    if (reply) {
        TxManCtrlFrame(reply);
    }
}

#pragma dont_inline reset
