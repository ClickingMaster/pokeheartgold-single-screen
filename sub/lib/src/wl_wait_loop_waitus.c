#pragma force_active on
#include "wl_internal.h"
// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

// Busy-waits `us` microseconds: the alarm handler clears the flag it gets as its argument.
void WaitLoop_Waitus(u32 us, OSAlarmHandler handler) {
    volatile u32 waiting;
    u64 tick;
    waiting = 1;
    tick = __VENEER__ll_udiv(((u64)us * 33514) >> 6, 1000);
    __VENEER_OS_SetAlarm(&wlWorkPlain->timeOutAlarm, tick, handler, (void *)&waiting);
    while (waiting) {}
}
