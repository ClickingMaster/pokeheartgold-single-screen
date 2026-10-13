#pragma force_active on
#include "wl_internal.h"
// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

// Beacon. Passive scan: handed to RxProbeResFrame. From the own BSS: completes the MLME join request in progress
// (state 0x21: takes over channel, rates, DTIM and beacon period); children take the parent's game information
// (active zone time, V-sync time), follow its TSF (next TBTT) and, in power save mode, its TIM (PS-Poll when frames
// are buffered for the own AID). Every beacon with an SSID goes to the AP list.
void RxBeaconFrame(WlRxFrame *frame) {
    WlWork *w;
    WlBss *bss;
    WlMlme *mlme;
    WlParam *param;
    WlHeap *heap;
    u32 station;
    u32 length;
    u16 state;
    u64 ts;
    u64 t1;
    u64 t0;
    WlElements info;
    u64 interval;
    u64 q;

    w = wlWorkPlain;
    w->counter[13]++;
    bss = &w->bss;
    mlme = &w->mlme;
    param = &w->param;
    heap = &w->heap;
    station = CAM_SearchAdd(frame->addr2);
    frame->station = (u16)station;
    if (station == 255) {
        return;
    }
    CAM_SetRSSI(station, frame->rssi);
    length = frame->length;
    if (length <= 12) {
        return;
    }
    __VENEER_MIi_CpuClear32(0, &info, sizeof(info));
    info.elements = WL_RX_BODY(frame, WlBeaconBody)->elements;
    info.length = length - 12;
    info.match = 2;
    if (bss->ssidLength == 0) {
        info.match |= 1;
    }
    info.found = 0x38;
    info.rxStatus = frame->status;
    info.capaInfo = WL_RX_BODY(frame, WlBeaconBody)->capaInfo;
    ElementChecker(&info);
    if (info.cf && (frame->duration & 0x8000)) {
        // CF parameter set: the remaining duration of the contention free period
        u8 *cf = info.cf;
        WL_REG(0x10C) = WL_ReadByte(cf + 6) + (WL_ReadByte(cf + 7) << 8);
    }
    state = mlme->state;
    if (state == 0x13 && mlme->req.scan->scanType == 1) {
        if ((info.match & 9) == 9) {
            RxProbeResFrame(frame, &info);
        }
        goto tail;
    }
    if (!(info.match & 8)) {
        goto tail;
    }
    if (state == 0x21) {
        ClearTimeOut();
        if ((info.match & 0x30) != 0x30) {
            mlme->work.join.result = 12;
            mlme->work.join.reason = 10;
        } else if (WL_RX_BODY(frame, WlBeaconBody)->beaconPeriod > 1000) {
            mlme->work.join.result = 12;
            mlme->work.join.reason = 1;
        } else {
            mlme->work.join.result = 0;
            if ((info.found & 2) && !(info.match & 2)) {
                WSetChannel(info.channel, 0);
            }
            CAM_SetSupRate(station, info.rateSet.supported);
            if (bss->mode == 2) {
                u8 *gameInfo = info.gameInfo;
                if (gameInfo) {
                    u8 lo = WL_ReadByte(gameInfo + 6);
                    WSetActiveZoneTime(lo + (WL_ReadByte(gameInfo + 7) << 8), 1);
                    lo = WL_ReadByte(info.gameInfo + 8);
                    WL_VSYNC_TIME = lo + (WL_ReadByte(info.gameInfo + 9) << 8);
                } else {
                    WSetActiveZoneTime(0xFFFF, 1);
                    WL_VSYNC_TIME = 0;
                }
            }
            WSetDTIMPeriod(WL_ReadByte(info.tim + 3));
            bss->dtimCount = WL_ReadByte(info.tim + 2);
            WSetBeaconPeriod(WL_RX_BODY(frame, WlBeaconBody)->beaconPeriod);
            bss->joined = 1;
            bss->unk_1a = 1;
            if (bss->mode == 2) {
                WL_REG(0x048) = 3;
            }
            WL_REG(0x038) |= 1;
        }
        WSetMacAdrs1(mlme->cfm.join->peerMacAdrs, frame->addr2);
        mlme->state = 0x25;
        AddTask(2, 1);
    }
    switch (bss->mode) {
    case 2:
        if (info.gameInfo) {
            // game information element: id, length, active zone time, V-sync time, game information
            u8 *gameInfo = info.gameInfo;
            u8 lo = WL_ReadByte(gameInfo + 6);
            WSetActiveZoneTime(lo + (WL_ReadByte(gameInfo + 7) << 8), 0);
            lo = WL_ReadByte(info.gameInfo + 8);
            WL_VSYNC_TIME = lo + (WL_ReadByte(info.gameInfo + 9) << 8);
            bss->gameInfoLength = WL_ReadByte(info.gameInfo + 1) - 8;
            if (bss->gameInfoLength) {
                // copied by halfwords: from one byte earlier when the game information is not aligned
                if ((u32)info.gameInfo & 1) {
                    __VENEER_MIi_CpuCopy16(info.gameInfo + 9, bss->gameInfo, bss->gameInfoLength + 2);
                    bss->gameInfoOdd = 1;
                } else {
                    __VENEER_MIi_CpuCopy16(info.gameInfo + 10, bss->gameInfo, bss->gameInfoLength + 1);
                    bss->gameInfoOdd = 0;
                }
            }
        }
        // fallthrough
    case 3:
        bss->beaconLostCount = 0;
        CAM_UpdateLifeTime(station);
        // next TBTT: the parent's time stamp rounded up to the next multiple of the beacon period (in us)
        ts = *(u64 *)WL_RX_BODY(frame, WlBeaconBody)->timeStamp;
        interval = (u32)(bss->beaconPeriod << 10);
        q = __VENEER__ll_udiv(ts, interval) + 1;
        ts = q * interval;
        {
            u16 *p = (u16 *)&ts;
            WL_REG(0x0F6) = p[3];
            WL_REG(0x0F4) = p[2];
            WL_REG(0x0F2) = p[1];
            WL_REG(0x0F0) = p[0] | 1;
        }
        if (bss->mode == 2 && bss->unk_1a) {
            // the rest of the active zone, from the TSF (read twice: the low halfword may wrap in between)
            OSIntrMode e;
            u32 elapsed;
            ts -= interval;
            e = __VENEER_OS_DisableInterrupts();
            ((u16 *)&t1)[0] = WL_REG(0x0F8);
            ((u16 *)&t1)[1] = WL_REG(0x0FA);
            ((u16 *)&t1)[2] = WL_REG(0x0FC);
            ((u16 *)&t1)[3] = WL_REG(0x0FE);
            ((u16 *)&t0)[0] = WL_REG(0x0F8);
            ((u16 *)&t0)[1] = WL_REG(0x0FA);
            ((u16 *)&t0)[2] = WL_REG(0x0FC);
            ((u16 *)&t0)[3] = WL_REG(0x0FE);
            __VENEER_OS_RestoreInterrupts(e);
            if (((u16 *)&t1)[0] < ((u16 *)&t0)[0]) {
                elapsed = (t1 - ts) >> 10;
            } else {
                elapsed = (t0 - ts) >> 10;
            }
            if (elapsed < param->activeZoneTime) {
                WL_REG(0x134) = param->activeZoneTime - elapsed;
            } else {
                WL_REG(0x134) = 0;
            }
        }
        if (bss->state == 0x40 && info.tim && bss->powerMgtMode == 1) {
            // TIM element: id, length, DTIM count, DTIM period, bitmap control, partial virtual bitmap
            u32 count = WL_ReadByte(info.tim + 2);
            u32 first;
            u32 offset;
            u32 bit;
            u32 last;
            if (bss->dtimCount != count) {
                bss->dtimCount = count;
            }
            bss->parentBuffered = 0;
            if (count == 0 && (WL_ReadByte(info.tim + 4) & 1)) {
                bss->parentBuffered |= 1; // broadcast frames buffered
            }
            offset = WL_ReadByte(info.tim + 4) & 0xFE;
            first = offset * 8;
            last = offset + WL_ReadByte(info.tim + 1) - 3;
            if (first <= bss->aid && bss->aid <= last * 8) {
                bit = bss->aid - first;
                if (WL_ReadByte(info.tim + 5 + (bit >> 3)) & (1 << (bit & 7))) {
                    bss->parentBuffered |= 2; // frames buffered for the own AID
                    TxPsPollFrame();
                }
            }
            if (heap->bufMan[1].count == 0 && heap->bufMan[2].count == 0 && bss->parentBuffered == 0) {
                WSetPowerState(1);
            }
        }
        break;
    }
    if (param->flags.bits.beaconIndicate) {
        MLME_IssueBeaconRecvIndication(frame);
    }
tail:
    if (info.ssid) {
        UpdateApList(info.channel, frame, info.ssid);
    }
}
