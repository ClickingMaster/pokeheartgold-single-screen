#pragma force_active on
#include "wl_internal.h"
// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

#pragma force_active off

static inline void QueueRxPacket(WlHeapBufMan *from, WlHeapBufMan *to, WlHeapBuf *packet, u32 priority, u32 task) {
    MoveHeapBuf(from, to, packet);
    AddTask(priority, task);
}

#pragma force_active on

// Task 15: takes the received frames out of the RX ring and hands them to the task for their type
// (fragments, data, management, beacons, key data, MP and MP ACK frames).
void WlIntrRxEndTask(void) {
    WlWork *w = wlWorkPlain;
    WlRxCtrl *rx = &w->rxCtrl;
    for (;;) {
        u32 index;
        u32 next;
        u32 release;
        u16 *raw;
        u32 status;
        WlRxFrame *frame;
        index = WL_REG(0x05A);
        if (index == rx->readIndex) {
            return;
        }
        if (index >= 0x8C6) {
            WUpdateCounter();
        }
        raw = ((u16 *)0x04804000) + index;
        next = *AdjustRingPointer(raw + 1);
        if (*raw == 0xFFFF) {
            WL_REG(0x05A) = next;
            continue;
        }
        frame = TakeoutRxFrame(raw, *AdjustRingPointer(raw + 4));
        WL_REG(0x05A) = next;
        if (!frame) {
            if ((*raw & 15) == 12) {
                SetFatalErr(16);
            } else {
                SetFatalErr(8);
            }
            continue;
        }
        if ((wlWorkPlain->hwWorkarounds & 8) && (frame->frameCtrl & 0x4000)) {
            rx->cntupBusyCount = 0;
        }
        release = 1;
        status = frame->status;
        if (status & 0x200) {
            if (((WlFrameCtrlBits *)&frame->frameCtrl)->moreFrag == 1 || ((WlSeqCtrlBits *)&frame->seqCtrl)->fragNo != 0) {
                release = 0;
                QueueRxPacket(&w->heap.bufMan[0], &w->heap.bufMan[8], (WlHeapBuf *)((u8 *)frame - 16), 2, 9);
            }
        } else {
            switch (status & 15) {
            case 8:
                if ((frame->frameCtrl & 15) == 8) {
                    release = 0;
                    QueueRxPacket(&w->heap.bufMan[0], &w->heap.bufMan[5], (WlHeapBuf *)((u8 *)frame - 16), 2, 6);
                }
                break;
            case 1:
                if (frame->frameCtrl == 0x80) {
                    RxBeaconFrame(frame);
                }
                break;
            case 0:
                if (!(frame->frameCtrl & 15)) {
                    release = 0;
                    QueueRxPacket(&w->heap.bufMan[0], &w->heap.bufMan[7], (WlHeapBuf *)((u8 *)frame - 16), 1, 7);
                }
                break;
            case 5:
                if ((frame->frameCtrl & 0xE7FF) == 0xA4) {
                    release = 0;
                    QueueRxPacket(&w->heap.bufMan[0], &w->heap.bufMan[7], (WlHeapBuf *)((u8 *)frame - 16), 1, 7);
                }
                break;
            case 14:
            case 15:
                if ((frame->frameCtrl & 0xE7BF) == 0x118) {
                    RxKeyDataFrame(frame);
                }
                break;
            case 12:
                if ((frame->frameCtrl & 0xE7FF) == 0x228) {
                    if (wlWorkPlain->bss.powerState == 0) {
                        WL_REG(0x03C) = 1;
                    }
                    wlWorkPlain->counter[28]++;
                    release = RxMpFrame(frame);
                }
                break;
            case 13:
                if ((frame->frameCtrl & 0xE7FF) == 0x218) {
                    wlWorkPlain->counter[29]++;
                    release = RxMpAckFrame(frame);
                }
                break;
            }
        }
        if (release) {
            ReleaseHeapBuf(&w->heap.bufMan[0], (WlHeapBuf *)((u8 *)frame - 16));
        }
        if (wlWorkPlain->hwWorkarounds & 1) {
            u32 i;
            for (i = 0; i < 7; i++) {
                if ((u32)raw >= 0x04805F60) {
                    raw = (u16 *)((u8 *)raw - wlWorkPlain->bss.rxBufSize);
                }
                *raw = 0xFFFF;
                raw++;
            }
        }
    }
}
