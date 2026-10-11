#pragma force_active on
#include "wl_internal.h"
// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

// -ipa file would otherwise inline the small functions of this file into their callers here.
#pragma dont_inline on

// clang-format off
asm void __VENEER_MI_WaitDma(u32 dmaNo) {
    ldr pc, =MI_WaitDma
}
// clang-format on

// Counts down the fragment reassembly slots; a slot that runs out drops its frame.
void DefragTimerTask(void) {
    WlDefrag *defrag = wlWorkPlain->rxCtrl.defrag;
    u32 i;
    for (i = 0; i < 3; i++) {
        WlDefrag *slot = defrag + i;
        if (slot->timer) {
            slot->timer--;
            if (!slot->timer) {
                ReleaseHeapBuf(&wlWorkPlain->heap.bufMan[0], slot->buf);
            }
        }
    }
}

// Sets up the RX ring in the MAC memory for the mode (it starts after the mode's TX slots).
void InitRxCtrl(void) {
    WlWork *w = wlWorkPlain;
    WlBss *bss = &w->bss;
    WlRxCtrl *rx = &w->rxCtrl;
    u32 start;
    __VENEER_MIi_CpuClear32(0, rx, 0x50);
    WL_REG(0x030) = 0x8000;
    switch (wlWorkPlain->bss.mode) {
    case 0:
        start = 0x794;
        break;
    case 1:
        start = 0x10C4;
        break;
    case 2:
        start = 0xBFC;
        break;
    case 3:
        start = 0x794;
        break;
    }
    WL_REG(0x050) = (u16)(start + 0x04804000);
    WL_REG(0x056) = (u16)(start >> 1);
    WL_REG(0x052) = 0x5F60;
    WL_REG(0x05A) = (u16)(start >> 1);
    rx->unk_04 = (u16)(start >> 1);
    rx->lastMpSeqCtrl = 0xFFFF;
    bss->unk_9a = (u16)(0x1F60 - start);
    WL_REG(0x062) = 0x5F5E;
    WL_REG(0x030) = 0x8001;
    WL_REG(0x24C) = 0xFFFF;
    WL_REG(0x24E) = 0xFFFF;
    *(vu16 *)0x04805F70 = 0xFFFF;
    *(vu16 *)0x04805F72 = 0xFFFF;
    *(vu16 *)0x04805F7E = 0xFFFF;
    *(vu16 *)0x04805F76 = 0xFFFF;
}

#pragma dont_inline reset
