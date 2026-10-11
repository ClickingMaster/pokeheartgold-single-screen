#pragma force_active on
#include "wl_internal.h"
// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

// Runs `handler` every `ms` milliseconds (OS ticks run at 33.514 MHz / 64: 33514 / 64 per ms).
void SetupPeriodicTimeOut(u32 ms, OSAlarmHandler handler) {
    u32 period;
    u64 start;
    __VENEER_OS_CancelAlarm(&wlWorkPlain->periodicTimeOutAlarm);
    period = ((u64)ms * 33514) >> 6;
    start = __VENEER_OS_GetTick() + period;
    __VENEER_OS_SetPeriodicAlarm(&wlWorkPlain->periodicTimeOutAlarm, start, period, handler, NULL);
}
