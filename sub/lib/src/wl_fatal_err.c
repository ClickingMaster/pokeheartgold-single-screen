#pragma force_active on
#include "wl_internal.h"
// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

// -ipa file would otherwise inline the small functions of this file into their callers here.
#pragma dont_inline on

#define WL_IRQ_MASK 0x01000000 // interrupt bit 24: wireless

void SetFatalErr(u32 bits) {
    u32 mask = __VENEER_OS_DisableIrqMask(WL_IRQ_MASK);
    wlWorkPlain->bss.fatalErr |= bits;
    __VENEER_OS_EnableIrqMask(mask);
    AddTask(2, 21);
}

void SendFatalErrMsgTask(void) {
    WlWork *w = wlWorkPlain;
    WlBss *bss = &w->bss;
    u8 *msg;
    u32 mask;
    if (!bss->fatalErr) {
        return;
    }
    msg = AllocateHeapBuf(&w->heap.bufMan[0], 18);
    if (!msg) {
        return;
    }
    *(u16 *)(msg + 12) = 390;
    *(u16 *)(msg + 14) = 1;
    mask = __VENEER_OS_DisableIrqMask(WL_IRQ_MASK);
    *(u16 *)(msg + 16) = bss->fatalErr;
    bss->fatalErr = 0;
    __VENEER_OS_EnableIrqMask(mask);
    SendMessageToWmDirect(&wlWorkPlain->heap.bufMan[0], msg);
}

void TerminateWlTask(void) {
    wlWorkPlain->param.errors |= 0x8000;
    if (wlWorkPlain->bss.state) {
        WStop();
        if (wlWorkPlain->unk_404) {
            wlWorkPlain->unk_404 = 0;
            u8 *msg = wlWorkPlain->unk_420;
            *(u16 *)(msg + 4) = 6;
            IssueMlmeConfirm();
        }
        WShutdown();
    }
    while (DeleteTask(3) != 0xFFFF) {}
    AddTask(3, 23);
}

void ReleaseWlTask(void) {
    WlWork *w = wlWorkPlain;
    ReleaseIntr();
    ReleaseHeapBuf(&w->heap.bufMan[0], (WlHeapBuf *)(wlWorkPlain->unk_318 - 12));
    ReleaseHeapBuf(&w->heap.bufMan[0], (WlHeapBuf *)((u32)wlWorkPlain->bss.gameInfo - 12));
    __VENEER_OS_ExitThread();
}
