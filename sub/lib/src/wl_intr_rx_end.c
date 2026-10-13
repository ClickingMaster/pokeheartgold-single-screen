#pragma force_active on
#include "wl_internal.h"
// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

// RX end: walks the frames the MAC added to the RX ring (time stamps them, drops broken ones, follows the
// sequence of the MP frames) and queues task 15 for the rest of the work.
void WlIntrRxEnd(void) {
    WlWork *w = wlWorkPlain;
    u32 checkRing;
    u16 *nextp;
    WlBss *bss;
    WlRxCtrl *rx;
    u16 *packet;
    u32 current, end;
    bss = &w->bss;
    rx = &w->rxCtrl;
    checkRing = w->hwWorkarounds;
    WL_REG(0x010) = 1;
    if (!bss->mode) {
        WL_REG(0x05A) = WL_REG(0x054);
    }
    checkRing &= 1;
    for (;;) {
        u32 tick1, tick2, hi1, hi2, ticks;
        u16 *timestampp, *lengthp;
        u16 *durationp;
        u32 len;
        u32 frameCtrl;
        current = rx->readIndex;
        end = WL_REG(0x054);
        if (current == end) {
            break;
        }
        tick1 = WL_REG(0x0F8);
        hi1 = WL_REG(0x0FA);
        tick2 = WL_REG(0x0F8);
        hi2 = WL_REG(0x0FA);
        if (tick1 > tick2) {
            ticks = (tick2 >> 4) | (hi2 << 12);
        } else {
            ticks = (tick1 >> 4) | (hi1 << 12);
        }
        if (current >= 0x8C6 && current <= 0x8EF) {
            WUpdateCounter();
        }
        packet = (u16 *)(0x04804000 + current * 2);
        nextp = AdjustRingPointer(packet + 1);
        timestampp = AdjustRingPointer(nextp + 1);
        lengthp = AdjustRingPointer(timestampp + 2);
        durationp = AdjustRingPointer(packet + 7);
        {
            u32 part = *nextp;
            packet[0] |= (part << 1) & 0x400;
        }
        *timestampp = (u16)ticks;
        len = *lengthp;
        current = ((len + (current << 1) + 15) >> 2) * 2;
        if (current >= 0xFB0) {
            current -= (u32)bss->rxBufSize >> 1;
        }
        if (len > 0x92C) {
            current = end;
            packet[0] = 0xFFFF;
            bss->rxDropCount++;
        } else if (checkRing && current != end) {
            u32 next = 0x04804000 + current * 2;
            u32 rate;
            u32 status = *(u16 *)((u8 *)0x04804000 + current * 2);
            if (next < 0x04805F5A) {
                rate = *(u16 *)(next + 6) & 255;
            } else {
                rate = *(u16 *)(next - bss->rxBufSize + 6);
            }
            if ((status & 0x7C00) || (rate != 10 && rate != 20) || len > 0xFFF) {
                bss->rxDropCount++;
                packet[0] = 0xFFFF;
                rx->readIndex = (u16)end;
                *nextp = (u16)end;
                break;
            }
        }
        {
            u32 type = packet[0] & 15;
            if (type == 12) {
                frameCtrl = *AdjustRingPointer(packet + 6);
                u32 seqCtrl = *AdjustRingPointer(packet + 17);
                if (rx->lastMpSeqCtrl == seqCtrl && (frameCtrl & 0x800)) {
                    WlWork *w2 = wlWorkPlain;
                    w2->counter[16]++;
                    packet[0] = 0xFFFF;
                } else {
                    WlWork *w2 = wlWorkPlain;
                    if (!w2->param.flags.bits.nullKeyMode && w2->bss.state == 0x40) {
                        if (WL_REG(0x028) && (WL_REG(0x098) & 0x8000)) {
                            u64 ticks;
                            __VENEER_OS_CancelAlarm(&w2->ksidAlarm);
                            ticks = __VENEER__ll_udiv(((u64)*durationp * 0x82EA) >> 6, 1000);
                            __VENEER_OS_SetAlarm(&wlWorkPlain->ksidAlarm, ticks, (OSAlarmHandler)WClearKSID, 0);
                        } else {
                            seqCtrl = 0xFFFF;
                            *(vu16 *)0x04805F7E = 0xFFFF;
                            WL_REG(0x24C) = 0xFFFF;
                            WL_REG(0x24E) = 0xFFFF;
                            packet[0] = 0xFFFF;
                        }
                    }
                }
                rx->lastMpSeqCtrl = (u16)seqCtrl;
                if (CheckKeyTxEnd() & 1) {
                    WlWork *w2 = wlWorkPlain;
                    w2->counter[27]++;
                }
            } else if (type == 13) {
                WlWork *w2 = wlWorkPlain;
                if (!w2->param.flags.bits.nullKeyMode && w2->bss.state == 0x40) {
                    if (!WL_REG(0x028) || !(WL_REG(0x098) & 0x8000)) {
                        packet[0] = 0xFFFF;
                    }
                }
            }
        }
        rx->readIndex = (u16)current;
        *nextp = (u16)current;
    }
    if (checkRing) {
        u32 prior = WL_REG(0x054);
        u16 key = CheckKeyTxEnd();
        if (key && prior == WL_REG(0x054)) {
            if (key & 2) {
                SetFatalErr(0x80);
            } else if (key & 1) {
                SetFatalErr(0x100);
            }
        }
    }
    if (WL_REG(0x05A) != WL_REG(0x054)) {
        AddTask(0, 15);
    }
}
