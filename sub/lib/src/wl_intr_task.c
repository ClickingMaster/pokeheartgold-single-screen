#pragma force_active on
#include "wl_internal.h"
// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

// -ipa file would otherwise inline the small functions of this file into their callers here.
#pragma dont_inline on

// Task 16 (after the MP end interrupt): reads the answers still in the RX ring when some are missing,
// counts the retries, records which stations did not answer and sends the MP state to the WMSP.
void WlIntrMpEndTask(void) {
    WlTxCtrl *txCtrl = &wlWorkPlain->txCtrl;
    if (txCtrl->txq[3].active) {
        WlMpState *mp = txCtrl->mpState;
        if (mp->remainBitmap != ((WlTxFrame *)txCtrl->txq[3].frame)->clientBits) {
            WlIntrRxEndTask();
        }
        {
            u32 retries = ((WlTxFrame *)txCtrl->txq[3].frame)->retry & 255;
            if (retries) {
                wlWorkPlain->counter[25] += retries;
            } else {
                wlWorkPlain->counter[25]++;
            }
        }
        txCtrl->txq[3].count++;
        mp = txCtrl->mpState;
        mp->unk_12 = mp->remainBitmap ^ ((WlTxFrame *)txCtrl->txq[3].frame)->clientBits;
        txCtrl->mpLastRemainBitmap = txCtrl->mpState->remainBitmap;
        txCtrl->txq[3].active = 0;
        if (wlWorkPlain->bss.tmpttPowerSaveOff) {
            WDisableTmpttPowerSave();
        }
        SendMessageToWmDirect(&wlWorkPlain->heap.bufMan[0], txCtrl->mpState);
    }
}

// Task 20 (parent TBTT): restarts the TX of the priority queues that have frames; without stations in
// power save (or with an empty TX FIFO) only the highest one.
void SetParentTbttTxqTask(void) {
    WlWork *w = wlWorkPlain;
    WlHeap *heap = &w->heap;
    if (WL_REG(0x088) == 0 && heap->bufMan[3].count) {
        WL_REG(0x0AE) = 8;
        TxqPri(2);
    } else {
        if (!(w->cam.powerMgtMap & ~w->cam.notAssocMap)) {
            WL_REG(0x0AE) = 8;
            if (heap->bufMan[3].count) {
                TxqPri(2);
            }
        }
        WL_REG(0x0AE) = 5;
        if (heap->bufMan[2].count) {
            TxqPri(1);
        }
        if (heap->bufMan[1].count) {
            TxqPri(0);
        }
    }
}

// Copies a received frame of `size` bytes from the MAC memory into a new heap buffer (NULL: no memory).
WlRxFrame *TakeoutRxFrame(u16 *src, u32 size) {
    WlRxBuf *buf = AllocateHeapBuf(&wlWorkPlain->heap.bufMan[0], size + 0x22);
    if (buf == NULL) {
        return NULL;
    }
    DMA_Read(&buf->frame.status, src, size + 12);
    buf->frame.length = size - 24;
    buf->frame.unk_0e = (buf->frame.unk_0e & 0xFF) | ((u16)(buf->frame.rssi & 0xFF) << 8);
    return &buf->frame;
}

#pragma dont_inline reset
