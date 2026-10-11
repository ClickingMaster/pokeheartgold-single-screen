#pragma force_active on
#include "wl_internal.h"
// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

// -ipa file would otherwise inline the small functions of this file into their callers here.
#pragma dont_inline on

// clang-format off
asm void __VENEER_OS_SetPeriodicAlarm(OSAlarm *alarm, OSTick start, OSTick period, OSAlarmHandler handler, void *arg) {
    ldr pc, =OS_SetPeriodicAlarm
}
// clang-format on

void ClearPeriodicTimeOut(void) {
    OS_CancelAlarm(&wlWorkPlain->periodicTimeOutAlarm);
}

// Periodic timer of a running BSS (SetupPeriodicTimeOut from WSetStaState): queues the periodic tasks.
void WIntervalTimer(void) {
    WlWork *w = wlWorkPlain;
    w->bss.intervalCount++;
    AddTask(1, 10);
    AddTask(2, 18);
    AddTask(1, 17);
    if (w->heap.bufMan[9].count) {
        AddTask(2, 19);
    }
    if (w->bss.fatalErr) {
        AddTask(2, 21);
    }
}

#pragma dont_inline reset
