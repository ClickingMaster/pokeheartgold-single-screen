#pragma force_active on
#include "wl_internal.h"
// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

// The capability information of a reassociation request.
#define CAPA(buf) ((WlCapaBits *)&WL_RX_BODY(&(buf)->frame, WlReAssReqBody)->capaInfo)

// Parent: reassociation request of an authenticated station; answered with a reassociation response
// (status 10: capability not supported, 1: no SSID, 18: rates not supported).
static inline void RxReAssReqFrame(WlRxBuf *buf) {
    WlWork *w = wlWorkPlain;
    u32 length = buf->frame.length;
    u16 station;
    u32 reason;
    WlTxHeapFrame *reply;
    WlElements info;
    if (length <= 10) {
        return;
    }
    if (w->bss.mode != 1) {
        return;
    }
    if (IsExistManFrame(buf->frame.addr2, 0x30)) {
        return;
    }
    station = buf->frame.station;
    if (CAM_GetStaState(station) < 48) {
        if (IsExistManFrame(buf->frame.addr2, 0xC0)) {
            return;
        }
        reply = MakeDeAuthFrame(buf->frame.addr2, 6, 1);
        if (reply) {
            TxManCtrlFrame(reply);
        }
        return;
    }
    if (CAM_GetStaState(station) == 64) {
        CAM_SetStaState(station, 48);
        MLME_IssueDisAssIndication(buf->frame.addr2, 1);
    } else if (CAM_GetAID(station)) {
        return;
    }
    __VENEER_MIi_CpuClear32(0, &info, sizeof(info));
    info.elements = WL_RX_BODY(&buf->frame, WlReAssReqBody)->elements;
    info.length = length - 10;
    info.found = 0x800;
    ElementChecker(&info);
    {
        u16 capaInfo = WL_RX_BODY(&buf->frame, WlReAssReqBody)->capaInfo;
        u16 wep;
        if ((capaInfo & 0xFFC2) || ((wep = w->param.wepMode) == 0 && CAPA(buf)->wep == 1) || (wep != 0 && CAPA(buf)->wep == 0)) {
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
    reply = MakeReAssResFrame(station, reason, info.ssid);
    if (reply) {
        TxManCtrlFrame(reply);
    }
}

#undef CAPA

// Parent: PS-Poll of an associated station; it is awake now, send what is queued for it.
static inline void RxPsPollFrame(WlRxBuf *buf) {
    WlHeap *heap;
    u32 station = buf->frame.station;
    heap = &wlWorkPlain->heap;
    if (CAM_GetStaState(station) != 64) {
        return;
    }
    CAM_SetAwake(station);
    if (heap->bufMan[2].count) {
        TxqPri(1);
    }
    if (heap->bufMan[1].count) {
        TxqPri(0);
    }
}

// The body of an association / reassociation response.
#define BODY(buf) WL_RX_BODY(&(buf)->frame, WlAssResBody)

// Child: association response to the MLME association request in progress (state 0x51).
static inline void RxAssResFrame(WlRxBuf *buf) {
    WlWork *w = wlWorkPlain;
    WlBss *bss = &w->bss;
    WlMlme *mlme = &w->mlme;
    switch (bss->mode) {
    case 2:
    case 3:
        break;
    default:
        return;
    }
    if (mlme->state != 0x51) {
        return;
    }
    if (!MatchMacAdrs(mlme->req.peer->peerMacAdrs, buf->frame.addr2)) {
        return;
    }
    ClearTimeOut();
    if (BODY(buf)->statusCode == 0) {
        WSetAids(BODY(buf)->aid & 0xFFF);
        MakePsPollFrame(bss->aid);
        WSetMacAdrs1(bss->parentMacAdrs, buf->frame.addr2);
        bss->parentStation = CAM_Search(buf->frame.addr2);
        CAM_SetStaState(bss->parentStation, 64);
    }
    if (BODY(buf)->statusCode == 0) {
        mlme->cfm.ass->header.result = 0;
        mlme->cfm.ass->statusCode = 0;
        WSetStaState(64);
    } else {
        mlme->cfm.ass->header.result = 12;
        mlme->cfm.ass->statusCode = BODY(buf)->statusCode;
    }
    mlme->cfm.ass->aid = bss->aid;
    mlme->state = 0x53;
    AddTask(2, 3);
}

// Child: reassociation response to the MLME reassociation request in progress (state 0x61).
static inline void RxReAssResFrame(WlRxBuf *buf) {
    WlWork *w = wlWorkPlain;
    WlBss *bss = &w->bss;
    WlMlme *mlme = &w->mlme;
    switch (bss->mode) {
    case 2:
    case 3:
        break;
    default:
        return;
    }
    if (mlme->state != 0x61) {
        return;
    }
    if (!MatchMacAdrs(mlme->req.peer->peerMacAdrs, buf->frame.addr2)) {
        return;
    }
    ClearTimeOut();
    if (BODY(buf)->statusCode == 0) {
        WSetAids(BODY(buf)->aid & 0xFFF);
        MakePsPollFrame(bss->aid);
        WSetMacAdrs1(bss->parentMacAdrs, buf->frame.addr2);
        bss->parentStation = CAM_Search(buf->frame.addr2);
        CAM_SetStaState(bss->parentStation, 64);
        WSetStaState(64);
    }
    if (BODY(buf)->statusCode == 0) {
        mlme->cfm.ass->header.result = 0;
        mlme->cfm.ass->statusCode = 0;
        WSetStaState(64);
    } else {
        mlme->cfm.ass->header.result = 12;
        mlme->cfm.ass->statusCode = BODY(buf)->statusCode;
    }
    mlme->cfm.ass->aid = bss->aid;
    mlme->state = 0x63;
    AddTask(2, 4);
}

#undef BODY

// Task 7: the next received management or control frame (WlHeap.bufMan[7]); counted, checked against the
// station table and the last sequence number, and handed to its handler for the current mode (1: parent,
// 2 / 3: child). Frames of unknown senders only reach the parent's auth, probe request and association handlers.
void RxManCtrlTask(void) {
    WlWork *w = wlWorkPlain;
    WlHeap *heap = &w->heap;
    u16 mode;
    WlRxBuf *buf;
    buf = (WlRxBuf *)heap->bufMan[7].first;
    mode = w->bss.mode;
    u32 station, type, subtype;
    u16 seqCtrl;

    if (buf == (WlRxBuf *)0xFFFFFFFF) {
        return;
    }
    if (buf->frame.addr1[0] & 1) {
        w->counter[11]++;
    } else {
        w->counter[10]++;
    }
    w->counter[9] += (int)(buf->frame.status & 0xF0) / 16 - 1;
    type = ((WlFrameCtrlBits *)&buf->frame.frameCtrl)->type;
    subtype = ((WlFrameCtrlBits *)&buf->frame.frameCtrl)->subtype;
    station = CAM_SearchAdd(buf->frame.addr2);
    buf->frame.station = station;
    if (station == 255) {
        buf->frame.station = 0;
        if (mode == 1) {
            switch (type) {
            case 0:
                switch (subtype) {
                case 11:
                    RxAuthFrame(&buf->frame);
                    break;
                case 4:
                    RxProbeReqFrame(&buf->frame);
                    break;
                case 0:
                    RxAssReqFrame(&buf->frame);
                    break;
                }
                break;
            }
        }
        goto end;
    }
    CAM_UpdateLifeTime(station);
    CAM_SetRSSI(station, buf->frame.rssi & 0xFF);
    if (type == 0) {
        seqCtrl = buf->frame.seqCtrl;
        if (seqCtrl == CAM_GetLastSeqCtrl(station)) {
            w->counter[15]++;
            goto end;
        }
        CAM_SetLastSeqCtrl(station, seqCtrl);
    }
    switch (mode) {
    case 1:
        CAM_SetPowerMgtMode(station, ((WlFrameCtrlBits *)&buf->frame.frameCtrl)->pwrMgt);
        if (type == 0) {
            switch (subtype) {
            case 8:
                RxBeaconFrame(&buf->frame);
                break;
            case 0:
                RxAssReqFrame(&buf->frame);
                break;
            case 2:
                RxReAssReqFrame(buf);
                break;
            case 4:
                RxProbeReqFrame(&buf->frame);
                break;
            case 5:
                RxProbeResFrame(&buf->frame, NULL);
                break;
            case 10:
                RxDisAssFrame(&buf->frame);
                break;
            case 11:
                RxAuthFrame(&buf->frame);
                break;
            case 12:
                RxDeAuthFrame(&buf->frame);
                break;
            }
        } else if (type == 1) {
            switch (subtype) {
            case 10:
                RxPsPollFrame(buf);
                break;
            }
        }
        break;
    case 2:
    case 3:
        if (type == 0) {
            switch (subtype) {
            case 8:
                RxBeaconFrame(&buf->frame);
                break;
            case 1:
                RxAssResFrame(buf);
                break;
            case 3:
                RxReAssResFrame(buf);
                break;
            case 5:
                RxProbeResFrame(&buf->frame, NULL);
                break;
            case 10:
                RxDisAssFrame(&buf->frame);
                break;
            case 11:
                RxAuthFrame(&buf->frame);
                break;
            case 12:
                RxDeAuthFrame(&buf->frame);
                break;
            }
        }
        break;
    }
end:
    ReleaseHeapBuf(&heap->bufMan[7], &buf->header);
    if (heap->bufMan[7].count) {
        AddTask(1, 7);
    }
}
