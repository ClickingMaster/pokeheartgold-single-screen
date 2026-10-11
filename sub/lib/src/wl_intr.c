#pragma force_active on
#include "wl_internal.h"
// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

// -ipa file would otherwise inline the small interrupt handlers of this file into WlIntr.
#pragma dont_inline on

// Wireless interrupt: runs the handler of every pending and enabled source until none is left.
void WlIntr(void) {
    u32 pending;
    for (;;) {
        pending = WL_REG(0x010) & WL_REG(0x012);
        if (!pending) {
            break;
        }
        if (pending & 128) {
            WlIntrStartTx();
        }
        if (pending & 64) {
            WlIntrStartRx();
        }
        if (pending & 0x8000) {
            WlIntrPreTbtt();
        }
        if (pending & 0x4000) {
            WlIntrTbtt();
        }
        if (pending & 0x2000) {
            WlIntrActEnd();
        }
        if (pending & 0x800) {
            WlIntrRfWakeup();
        }
        if (pending & 8) {
            WlIntrTxErr();
        }
        if (pending & 4) {
            WlIntrRxCntup();
        }
        if (pending & 1) {
            WlIntrRxEnd();
        }
        if (pending & 0x30) {
            WlIntrCntOvf();
        }
        if (pending & 2) {
            WlIntrTxEnd();
        }
        if (pending & 0x1000) {
            WlIntrMpEnd(1);
        }
    }
    // the interrupt check flag of the wireless interrupt (OS_IE_WIRELESS)
    *(vu32 *)0x0380FFF8 |= 0x1000000;
}

// Before the target beacon time: counts the lost beacons of the joined BSS (task 13 when too many).
void WlIntrPreTbtt(void) {
    WlBss *bss = &wlWorkPlain->bss;
    WL_REG(0x010) = 0x8000;
    if (bss->state == 64 && bss->beaconLostThreshold && bss->listenCount == bss->listenInterval) {
        bss->beaconLostCount++;
        if (bss->beaconLostCount > bss->beaconLostThreshold) {
            bss->beaconLostCount = 0;
            AddTask(1, 13);
        }
    }
    bss->powerState = 1;
}

// Target beacon time: parent (mode 1) updates the beacon and the TX queues; child modes handle power
// save, the listen/DTIM counts and frames that timed out in the queues.
void WlIntrTbtt(void) {
    WlWork *w = wlWork;
    WlBss *bss = &w->bss;
    WlParam *param = &w->param;
    WlTxCtrl *txCtrl = &w->txCtrl;
    u8 *frame;
    u32 power, i;
    WlTxDesc *desc;
    WL_REG(0x010) = 0x4000;
    switch (bss->mode) {
    case 1: {
        u32 timestamp = *(vu16 *)0x0380FFF0;
        frame = txCtrl->beacon.frame + 0x24 + bss->gameInfoOffset;
        WL_WriteByte(frame + 8, timestamp & 255);
        WL_WriteByte(frame + 9, (timestamp >> 8) & 255);
        if (bss->powerMgtMode == 1) {
            WL_REG(0x134) = param->activeZoneTime + WL_REG(0x134) + 1;
        }
        {
            WlWork *w2 = wlWork;
            w2->cam.activeMap = ~w2->cam.powerMgtMap | w2->cam.unk_06;
        }
        {
            u32 flags = WL_REG(0x0B6);
            if ((flags & 0x18) || (flags & 6) == 2) {
                txCtrl->unk_8c &= ~2u;
                SetParentTbttTxq();
            } else {
                txCtrl->unk_8c |= 2;
            }
        }
        break;
    }
    case 2:
        if (bss->unk_12 == 0) {
            WL_REG(0x134) = 0xFFFF;
        } else {
            WL_REG(0x134) = param->activeZoneTime + WL_REG(0x134) + 1;
        }
        if (bss->unk_1a == 2) {
            WSetPowerState(2);
        }
    case 3:
        if (bss->state != 64) {
            power = 1;
        } else {
            power = 0;
            if (bss->listenCount == 1) {
                power = 1;
            }
            if (bss->receiveDtims) {
                if (bss->dtimCount == 1 || (bss->dtimCount == 0 && bss->dtimPeriod == 1)) {
                    power = 1;
                }
            }
        }
        if (power) {
            WL_REG(0x038) |= 1;
        } else {
            WL_REG(0x038) &= ~1u;
        }
        if (WL_REG(0x118) > 10) {
            WL_REG(0x048) = 0;
        }
        bss->listenCount--;
        if (bss->listenCount == 0) {
            bss->listenCount = bss->listenInterval;
        }
        {
            u16 old = bss->dtimCount;
            bss->dtimCount = old - 1;
            if (old == 0) {
                bss->dtimCount = bss->dtimPeriod - 1;
            }
        }
        for (i = 0; i < 2; i++) {
            desc = &txCtrl->txq[i];
            if (desc->active) {
                frame = (u8 *)desc->heapFrame;
                if (((WlTxHeapFrame *)frame)->frame.status == 0 && CheckFrameTimeout((WlTxHeapFrame *)frame)) {
                    ResetTxqPri(i);
                    *(u16 *)desc->frame = 2;
                    AddTask(0, 14);
                    txCtrl->unk_ae++;
                }
            }
        }
        WL_REG(0x0AE) = 13;
        break;
    }
}

void WlIntrActEnd(void) {
    WlBss *bss = &wlWorkPlain->bss;
    WL_REG(0x010) = 0x2000;
    WL_REG(0x0AC) = 13;
    if (bss->unk_1a == 1) {
        bss->unk_1a = 2;
        return;
    }
    if (bss->unk_1a == 2) {
        bss->unk_1a = 0;
        return;
    }
    if (bss->mode == 2 && bss->state != 64) {
        WL_REG(0x048) = 0;
    }
}

void WlIntrRfWakeup(void) {
    WL_REG(0x010) = 0x800;
}

// Counter overflow: adds the hardware counters to WlWork.counter.
void WlIntrCntOvf(void) {
    WUpdateCounter();
    WL_REG(0x1AC) = 0xFFFF;
    WL_REG(0x010) = 0x30;
}

// TX error: switches the antenna (diversity), and resets the TX side when a queued WEP frame has an
// empty ICV (unk_690 bit 3).
void WlIntrTxErr(void) {
    WlWork *w;
    WL_REG(0x010) = 8;
    if (wlWorkPlain->param.flags.bits.diversity) {
        if (!(WL_REG(0x19C) & 1)) {
            WL_REG(0x290) ^= 1;
        }
    }
    w = wlWorkPlain;
    if (w->unk_690 & 8) {
        u32 i = 0;
        WlBss *bss = &w->bss;
        WlTxDesc *txq = w->txCtrl.txq;
        for (; i < 3; i++) {
            WlTxDesc *desc = &txq[i];
            if (desc->active) {
                WlTxFrame *frame = (WlTxFrame *)desc->frame;
                if ((frame->frameCtrl & 0x4000) && (frame->retry & 255)) {
                    u16 *icv = (u16 *)(((u32)((u32)&frame->frameCtrl + frame->length - 7)) & ~1u);
                    if (icv[0] == 0 && icv[1] == 0) {
                        frame->retry = 0;
                        WL_REG(0x032) = 0;
                        WL_REG(0x032) = 0x8000;
                        bss->txResetCount++;
                    }
                }
            }
        }
    }
}

// RX counter interrupt: resets the TX side when it stays busy (unk_690 bit 3), and repairs the RX ring
// read pointer (unk_690 bit 0).
void WlIntrRxCntup(void) {
    WlWork *w = wlWorkPlain;
    WlRxCtrl *rx = &w->rxCtrl;
    WlTxDesc *txq = w->txCtrl.txq;
    u16 status;
    WL_REG(0x010) = 4;
    status = WL_REG(0x1A8);
    if ((wlWorkPlain->unk_690 & 8) && (status & 0x400)) {
        u16 busy = WL_REG(0x0B0);
        if (!((busy & 1) && txq[0].active) && !((busy & 4) && txq[1].active) && !((busy & 8) && txq[2].active) && !(WL_REG(0x19C) & 1)) {
            WL_REG(0x032) = 0;
            WL_REG(0x032) = 0x8000;
            rx->unk_02 = 0;
        } else {
            u32 previous = rx->unk_02;
            rx->unk_02 = (u16)(previous + 1);
            if (previous > 12) {
                rx->unk_02 = 0;
                WL_REG(0x032) = 0;
                WL_REG(0x032) = 0x8000;
                wlWorkPlain->bss.txResetCount++;
            }
        }
    }
    if ((wlWorkPlain->unk_690 & 1) && (status & 0x60)) {
        s32 position = WL_REG(0x054);
        if (position >= (s32)(WL_REG(0x052) - 0x4000) / 2 || position < (s32)(WL_REG(0x050) - 0x4000) / 2) {
            WL_REG(0x056) = WL_REG(0x05A);
            WL_REG(0x030) = 0x8001;
        }
        WCheckTxBuf();
    }
}

// TX end: by the TX status, the beacon (0x300), the MP frame (0x800: retries, 0xB00: answers) ...; queues
// task 14 (the next TX).
void WlIntrTxEnd(void) {
    WlWork *w = wlWorkPlain;
    WlTxCtrl *q;
    u32 status;
    WL_REG(0x010) = 2;
    q = &w->txCtrl;
    if (wlWorkPlain->bss.state == 0x12) {
        IntrCarrierSuppresionSignal();
        return;
    }
    status = WL_REG(0x0B8) & 0xF00;
    switch (status) {
    case 0x300: {
        WlWork *w2;
        if (q->unk_8c & 2) {
            SetParentTbttTxq();
        }
        w2 = wlWorkPlain;
        w2->counter[7]++;
        AddTask(0, 8);
        break;
    }
    case 0x800: {
        u16 retry = ((WlTxFrame *)q->txq[3].frame)->retry & 255;
        WlMpState *mp;
        if (q->unk_9e <= retry) {
            WL_REG(0x0AC) = 2;
            q->unk_a8++;
        }
        mp = q->mpState;
        mp->unk_18++;
        break;
    }
    case 0xB00: {
        u16 mask, bits;
        u8 *entry;
        *(vu16 *)0x04805F70 = 0xFFFF;
        *(vu16 *)0x04805F72 = 0xFFFF;
        WL_REG(0x24C) = 0xFFFF;
        WL_REG(0x24E) = 0xFFFF;
        if (q->txq[3].active && ((WlTxFrame *)q->txq[3].frame)->seqCtrl == 0xFFFF) {
            if (((WlTxFrame *)q->txq[3].frame)->retry) {
                ((WlTxFrame *)q->txq[3].frame)->retry = 0;
                ((WlTxFrame *)q->txq[3].frame)->unk_02 = q->unk_98;
            }
        }
        bits = ((WlTxFrame *)q->txq[3].frame)->unk_02;
        mask = q->unk_98;
        entry = q->mpState->entries;
        if (bits > 1 && wlWorkPlain->param.flags.bits.diversity) {
            if (!(WL_REG(0x19C) & 1)) {
                WL_REG(0x290) ^= 1;
            }
        }
        if (wlWorkPlain->unk_690 & 64) {
            while (bits > 1) {
                bits = (u16)(bits >> 1);
                if (bits & 1) {
                    (*(u16 *)(entry + 6))++;
                }
                mask = (u16)(mask >> 1);
                if (mask & 1) {
                    entry += q->mpState->entrySize;
                }
            }
        }
        break;
    }
    }
    if (status == 0x800) {
        goto scheduled;
    }
    if (WL_REG(0x0B0) & 2) {
        goto scheduled;
    }
    {
        if (q->txq[3].active) {
            WL_REG(0x0B4) = 2;
            WL_REG(0x048) = 0;
            if (WL_REG(0x010) & 0x1000) {
                q->unk_ac++;
            } else {
                WlIntrMpEnd(2);
            }
            q->unk_aa++;
        }
        WL_REG(0x0AE) = 2;
    }
scheduled:
    AddTask(0, 14);
}

#pragma dont_inline reset
