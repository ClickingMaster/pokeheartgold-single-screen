#pragma force_active on
#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
typedef void (*AlarmHandler)(void *);
typedef struct {
    u8 pad[0x634];
    u8 alarm[0x2c];
} WlWork;
#define ROOT (*(WlWork **)0x0380fff4)
extern void __VENEER_OS_CancelAlarm(void *alarm);
extern void __VENEER_OS_SetAlarm(void *alarm, u64 tick, AlarmHandler handler, void *arg);
extern u64 __VENEER__ll_udiv(u64, u64);
void SetupTimeOut(u32 ms, AlarmHandler handler);
void SetupUsTimeOut(u32 us, AlarmHandler handler);
// @fn SetupTimeOut
void SetupTimeOut(u32 ms, AlarmHandler handler) {
    __VENEER_OS_CancelAlarm(ROOT->alarm);
    __VENEER_OS_SetAlarm(ROOT->alarm, ((u64)ms * 33514) >> 6, handler, NULL);
}
// @fn SetupUsTimeOut
void SetupUsTimeOut(u32 us, AlarmHandler handler) {
    __VENEER_OS_CancelAlarm(ROOT->alarm);
    __VENEER_OS_SetAlarm(ROOT->alarm, __VENEER__ll_udiv(((u64)us * 33514) >> 6, 1000), handler, NULL);
}
