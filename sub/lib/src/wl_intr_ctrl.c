#pragma force_active on
#include "wl_internal.h"
// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

// Index of the TX queue control register (0x04808090 + 4 * index) of each priority's queue.
#define WL_TXQ_REG(index) (*(vu16 *)(0x048080A0 + (index) * 4))

// -ipa file would otherwise inline the small functions of this file into their callers here.
#pragma dont_inline on

// RX start (hwWorkarounds bit 5): when an MP frame for this station starts arriving while the TX is busy,
// stops the TX of the key data frame (its pointer is saved in WlTxCtrl.savedTxPointer and restored at the end).
void WlIntrStartRx(void) {
    u16 *cursor = 0;
    u32 startTime = 0, start = 0, index = 0;
    u16 distance = 0;
    WlWork *w = wlWork;
    WlBss *bss = &w->bss;
    WlTxCtrl *txCtrl = &w->txCtrl;
    WL_REG(0x010) = 0x40;
    if (!(wlWork->hwWorkarounds & 0x20)) {
        goto done;
    }
    if (txCtrl->savedTxPointer != 0xFFFF || (WL_REG(0x19C) & 3) != 3) {
        goto done;
    }
    if ((int)WL_REG(0x268) < (int)(((u32)WL_REG(0x050) >> 1) & 0xFFF)) {
        goto done;
    }
    start = WL_REG(0x054);
    cursor = ((u16 *)0x04804000) + start;
    cursor += 4;
    cursor = AdjustRingPointer(cursor);
    cursor = AdjustRingPointer(cursor + 2);
    if ((*cursor & 0xE7FF) != 0x228) {
        goto done;
    }
    cursor = AdjustRingPointer(cursor + 1);
    startTime = (u32)WL_REG(0x0F8) - 0x10000u;
    for (;;) {
        distance = (u16)(WL_REG(0x268) - start);
        if (distance & 0x8000) {
            distance = (u16)(distance + ((u32)bss->rxBufSize >> 1));
        }
        if (distance > 14) {
            break;
        }
        if ((u16)(WL_REG(0x0F8) - startTime) > 64) {
            goto done;
        }
    }
    cursor += 4;
    for (index = 0; index < 3; index++) {
        cursor = AdjustRingPointer(cursor);
        if (*cursor++ != bss->bssid[index]) {
            goto done;
        }
    }
    cursor = AdjustRingPointer(cursor + 5);
    for (;;) {
        distance = (u16)(WL_REG(0x268) - start);
        if (distance & 0x8000) {
            distance = (u16)(distance + ((u32)bss->rxBufSize >> 1));
        }
        if (distance > 20) {
            break;
        }
        if ((u16)(WL_REG(0x0F8) - startTime) > 112) {
            goto done;
        }
    }
    // the hardware key station ID (register 0x028) is in the 0..15 range
    if (*cursor & (1u << WL_REG(0x028))) {
        goto done;
    }
    txCtrl->savedTxPointer = WL_REG(0x098);
    WL_REG(0x0B4) = 0x40;
    bss->keyRestoreCount++;
    while ((WL_REG(0x19C) & 3) == 3) {}
    WL_REG(0x244) |= 0x40;
    WL_REG(0x244) &= ~0x40;
    WL_REG(0x228) = 8;
    WL_REG(0x228) = 0;
    {
        WlTxCtrl *txCtrl2 = &wlWork->txCtrl;
        u16 saved = txCtrl2->savedTxPointer;
        if (saved != 0xFFFF) {
            u16 old = WL_REG(0x094);
            WL_REG(0x094) = saved;
            WL_REG(0x030) |= 0x80;
            WL_REG(0x094) = old;
            txCtrl2->savedTxPointer = 0xFFFF;
        }
    }
done:;
}

// Parent TBTT: resets the three priority queues, drops the ones whose frame is done; queues task 14 (TX)
// when one still has a frame, and task 20.
void SetParentTbttTxq(void) {
    WlTxDesc *txq = wlWorkPlain->txCtrl.txq;
    int pending = 0;
    ResetTxqPri(2);
    ResetTxqPri(1);
    ResetTxqPri(0);
    if (txq[2].active) {
        if (*(u16 *)txq[2].frame) {
            pending = 1;
        } else {
            txq[2].active = 0;
        }
    }
    if (txq[1].active) {
        if (*(u16 *)txq[1].frame == 0) {
            txq[1].active = 0;
        } else {
            pending = 1;
        }
    }
    if (txq[0].active) {
        if (*(u16 *)txq[0].frame == 0) {
            txq[0].active = 0;
        } else {
            pending = 1;
        }
    }
    if (pending) {
        AddTask(0, 14);
    }
    AddTask(0, 20);
}

// SetupUsTimeOut handler of WlIntrMpEnd: kicks the MAC out of the MP sequence, then ends it.
void MacBugTxMp(void) {
    u32 mask = __VENEER_OS_DisableIrqMask(0x1000000);
    u16 original = WL_REG(0x210);
    int limit = 100;
    WL_REG(0x244) = 0x1000;
    while (limit) {
        if (original != WL_REG(0x210)) {
            break;
        }
        limit--;
    }
    WL_REG(0x244) = 0;
    WlIntrMpEnd(0);
    __VENEER_OS_EnableIrqMask(mask);
}

// Wraps a pointer that ran past the end of the RX ring in the MAC memory.
u16 *AdjustRingPointer(u16 *pointer) {
    if ((u32)pointer >= 0x04805F60) {
        pointer = (u16 *)((u32)pointer - wlWorkPlain->bss.rxBufSize);
    }
    return pointer;
}

// Bit 0: a key data frame was given up, bit 1: one was sent (txq[4] and txq[5]).
u32 CheckKeyTxEnd(void) {
    WlTxDesc *txq = wlWorkPlain->txCtrl.txq;
    u32 result = CheckKeyTxEndMain(&txq[4]);
    return result | CheckKeyTxEndMain(&txq[5]);
}

u32 CheckKeyTxEndMain(WlTxDesc *desc) {
    u32 result = 0;
    if (desc->active == 2) {
        if ((((u32)desc->frame & 0x3FFF) >> 1 | 0x8000) != WL_REG(0x094)) {
            desc->active = 1;
            result |= 1;
        }
    }
    if (desc->active == 1) {
        if (*(u16 *)desc->frame & 1) {
            TxEndKeyData(desc);
            result |= 2;
        }
    }
    return result;
}

void InitializeIntr(void) {
    __VENEER_OS_SetIrqFunction(0x1000000, WlIntr);
    __VENEER_OS_EnableIrqMask(0x1000000);
}

// clang-format off
asm void __VENEER_OS_SetIrqFunction(OSIrqMask intrBit, OSIrqFunction function) {
    ldr pc, =OS_SetIrqFunction
}
// clang-format on

void ReleaseIntr(void) {
    __VENEER_OS_DisableIrqMask(0x1000000);
    __VENEER_OS_SetIrqFunction(0x1000000, 0);
}

void WlIntrTxBeaconTask(void) {
    if (wlWorkPlain->bss.gameInfoUpdated) {
        UpdateGameInfoElement();
    }
    if (wlWorkPlain->param.flags.bits.beaconIndicate) {
        MLME_IssueBeaconSendIndication();
    }
}

// Completes the frames the MAC has sent from the three priority queues (highest first): updates the heap
// frame's status and retry count and calls the queue's callback. A WEP frame with an empty ICV resets the
// TX side (hwWorkarounds bit 3) and is sent again unless it timed out.
void WlIntrTxEndTask(void) {
    WlWork *w = wlWorkPlain;
    WlTxCtrl *txCtrl = &w->txCtrl;
    WlBss *bss = &w->bss;
    WlTxDesc *desc;
    int i;
    for (i = 2; i >= 0; i--) {
        desc = &txCtrl->txq[i];
        if (WL_TXQ_REG(i) & 0x8000) {
            continue;
        }
        if (!desc->active) {
            continue;
        }
        if (desc->heapFrame) {
            desc->heapFrame->frame.status = ((WlTxFrame *)desc->frame)->status;
            desc->heapFrame->frame.frameCtrl = ((WlTxFrame *)desc->frame)->frameCtrl;
            if (((WlTxFrame *)desc->frame)->frameCtrl & 0x4000) {
                WlTxFrame *frame = (WlTxFrame *)desc->frame;
                if (wlWorkPlain->hwWorkarounds & 8) {
                    u16 *icv = (u16 *)((u32)((u32)&frame->frameCtrl + frame->length - 7) & ~1u);
                    if (icv[0] == 0 && icv[1] == 0) {
                        WL_REG(0x032) = 0;
                        WL_REG(0x032) = 0x8000;
                        bss->txResetCount++;
                        if (CheckFrameTimeout(desc->heapFrame)) {
                            desc->heapFrame->frame.status = 2;
                            desc->count++;
                            txCtrl->txTimeOutCount++;
                            desc->callback(desc->heapFrame, 1);
                        } else {
                            desc->heapFrame->frame.retry = 0;
                            WL_TXQ_REG(i) |= 0x8000;
                        }
                        continue;
                    }
                }
                desc->heapFrame->frame.retry = (u16)(desc->heapFrame->frame.retry + (frame->retry & 255));
            } else {
                desc->heapFrame->frame.retry = ((WlTxFrame *)desc->frame)->retry & 255;
            }
            desc->count++;
            desc->callback(desc->heapFrame, 2);
        } else {
            desc->count++;
            desc->callback(desc->frame, 3);
        }
    }
}

#pragma dont_inline reset
