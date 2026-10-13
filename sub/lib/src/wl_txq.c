#pragma force_active on
#include "wl_internal.h"
// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

// Starts the TX of the next frame of priority queue `pri` (WlHeap.bufMan[1 + pri]) when its MAC slot is
// free: frames that timed out or whose station is gone are completed on the way.
void TxqPri(u32 pri) {
    WlBss *bss;
    WlWork *w = wlWorkPlain;
    WlTxCtrl *txCtrl;
    WlTxDesc *desc;
    WlHeapBufMan *queue = &w->heap.bufMan[1] + pri;
    u32 cur, irq, next;
    txCtrl = &w->txCtrl;
    desc = &txCtrl->txq[0] + pri;
    bss = &w->bss;
    WlTxHeapFrame *frame;
    u8 *buf;
    s32 aid, frameCtrl;
    vu16 *reg;
    u32 offset;
    if (queue->count == 0) {
        return;
    }
    irq = __VENEER_OS_DisableIrqMask(0x1000000);
    if (desc->active) {
        __VENEER_OS_EnableIrqMask(irq);
        return;
    }
    next = (u32)queue->first;
    while (1) {
        if (next == 0xFFFFFFFF) {
            __VENEER_OS_EnableIrqMask(irq);
            return;
        }
        cur = next;
        next = (u32)GetHeapBufNextAdrs((WlHeapBuf *)cur);
        frame = (WlTxHeapFrame *)(cur + 0x10);
        aid = frame->station;
        if (CheckFrameTimeout(frame)) {
            txCtrl->txTimeOutCount++;
            frame->frame.status = 2;
            desc->count++;
            desc->callback(frame, 0);
            continue;
        }
        if (0 == pri || (pri == 1 && CAM_GetStaState(aid) == 0x40)) {
            if (!CAM_IsActive(aid)) {
                continue;
            }
            if (CAM_GetStaState(aid) != 0x40) {
                frame->frame.status = 2;
                IssueMaDataConfirm(queue, (WlMaDataReq *)((u8 *)frame - 0x10));
                CAM_DecFrameCount(frame);
                continue;
            }
        }
        break;
    }
    desc->active = 1;
    desc->pendingCount++;
    desc->heapFrame = frame;
    buf = desc->frame;
    if (bss->powerState == 0) {
        WSetPowerState(2);
    }
    CopyTxFrmToMacBuf(buf, (void *)cur);
    // more data: further frames wait for the station
    if (bss->mode == 1 && CAM_GetFrameCount(aid) > 1) {
        ((WlTxFrame *)buf)->frameCtrl = ((WlTxFrame *)buf)->frameCtrl | 0x2000;
    }
    frameCtrl = frame->frame.frameCtrl;
    u32 bufAdrs = (u32)buf;
    reg = (vu16 *)(0x048080A0 + pri * 4);
    offset = (bufAdrs & 0x3FFF) >> 1;
    if (4 == (frameCtrl & 0xC)) {
        *reg = (u16)offset | 0xA000;
    } else {
        if ((frameCtrl & 0xFC) == 0x50) {
            *reg = (u16)offset | 0x9000;
        } else {
            *reg = (u16)offset | 0x8000;
        }
    }
    __VENEER_OS_EnableIrqMask(irq);
}
