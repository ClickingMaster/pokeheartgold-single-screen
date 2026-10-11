#pragma force_active on
#include "wl_internal.h"
// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

// MP end: queues task 16, or (unk_690 bit 4, timeout != 0) first waits for the answers of the addressed
// stations when the MAC ended the MP sequence without an ACK (MacBugTxMp).
void WlIntrMpEnd(int timeout) {
    WlTxCtrl *txCtrl = &wlWorkPlain->txCtrl;
    WL_REG(0x010) = 0x1000;
    if (txCtrl->txq[3].active) {
        if ((wlWorkPlain->unk_690 & 16) && timeout) {
            u16 ack = WL_REG(0x0B6);
            u16 state = WL_REG(0x214);
            if ((state == 3 || state == 5) && ack == 0) {
                u32 count = 0;
                WlTxFrame *frame = (WlTxFrame *)txCtrl->txq[3].frame;
                u16 stations = frame->unk_02;
                while (stations) {
                    count += stations & 1;
                    stations = (u16)(stations >> 1);
                }
                SetupUsTimeOut((u16)(count * (frame->unk_24 + 10) + 0xC0 + (frame->length << 2)), (OSAlarmHandler)MacBugTxMp);
                wlWorkPlain->bss.mpTimeOutCount++;
                return;
            }
        }
        AddTask(0, 16);
    }
}
