#pragma force_active on
#include "wl_internal.h"
// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

// -ipa file would otherwise inline the small functions of this file into their callers here.
#pragma dont_inline on

// Every frame in the MAC memory is preceded by the id words 0xB6B8 0x1D46; if they were overwritten, restore
// them (and the frame's copy at +12) and return 1.
int WCheckTxBufIdBeforeFrame(WlTxDesc *desc) {
    vu16 *id;
    u8 *frame = desc->frame;
    id = (vu16 *)((u32)frame - 4);
    if (*id == 0xB6B8 && *(vu16 *)((u32)id + 2) == 0x1D46) {
        return 0;
    }
    *(vu16 *)((u32)frame + 10) = 1;
    *id = 0xB6B8;
    *(vu16 *)((u32)id + 2) = 0x1D46;
    frame = desc->frame;
    *(vu16 *)((u32)frame + 12) = 0xB6B8;
    *(vu16 *)((u32)frame + 14) = 0x1D46;
    ++wlWorkPlain->bss.txBufRepairCount;
    return 1;
}

void WaitMacStop(void) {
    int count = 16;
    WL_REG(0x004) = 0;
    while (count) {
        u16 state = WL_REG(0x214);
        if (state == 0 || state == 9) {
            return;
        }
        count--;
    }
}

void RestoreTxFrame(WlTxDesc *desc) {
    if (desc->active) {
        WaitMacStop();
        CopyTxFrmToMacBuf(desc->frame, (void *)((u8 *)desc->heapFrame - 16));
        wlWorkPlain->bss.txFrameRetryCount++;
    }
}

void WCheckTxBuf(void) {
    WlWork *w = wlWorkPlain;
    WlBss *bss = &w->bss;
    WlTxDesc *desc = w->txCtrl.txq;
    switch (w->bss.mode) {
    case 1:
        if (WCheckTxBufIdBeforeFrame(&desc[6])) {
            WaitMacStop();
            MakeBeaconFrame();
        }
        if (WCheckTxBufIdBeforeFrame(&desc[2])) {
            RestoreTxFrame(&desc[2]);
        }
        break;
    case 2:
        WCheckTxBufIdBeforeFrame(&desc[5]);
        if (WCheckTxBufIdBeforeFrame(&desc[2])) {
            if (desc[2].active) {
                WaitMacStop();
            }
            MakePsPollFrame(bss->aid);
            bss->txFrameRetryCount++;
        }
        break;
    case 3:
        break;
    }
    if (WCheckTxBufIdBeforeFrame(&desc[1])) {
        RestoreTxFrame(&desc[1]);
    }
    if (WCheckTxBufIdBeforeFrame(&desc[0])) {
        RestoreTxFrame(&desc[0]);
    }
    if (!WL_REG(0x004)) {
        WL_REG(0x004) = 1;
    }
}
