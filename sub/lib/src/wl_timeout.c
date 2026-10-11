#pragma force_active on
#include "wl_internal.h"
// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

// Runs `handler` once after `ms` milliseconds.
void SetupTimeOut(u32 ms, OSAlarmHandler handler) {
    __VENEER_OS_CancelAlarm(&wlWorkPlain->timeOutAlarm);
    __VENEER_OS_SetAlarm(&wlWorkPlain->timeOutAlarm, ((u64)ms * 33514) >> 6, handler, NULL);
}

// Runs `handler` once after `us` microseconds.
void SetupUsTimeOut(u32 us, OSAlarmHandler handler) {
    __VENEER_OS_CancelAlarm(&wlWorkPlain->timeOutAlarm);
    __VENEER_OS_SetAlarm(&wlWorkPlain->timeOutAlarm, __VENEER__ll_udiv(((u64)us * 33514) >> 6, 1000), handler, NULL);
}
