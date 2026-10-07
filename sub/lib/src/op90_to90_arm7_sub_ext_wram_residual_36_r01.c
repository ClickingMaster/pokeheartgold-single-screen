#pragma force_active on
#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
typedef void (*AlarmHandler)(void *);
typedef struct {
    u8 pad[0x608];
    u8 alarm[0x2c];
} WlWork;
#define ROOT (*(WlWork **)0x0380fff4)
extern void __VENEER_OS_CancelAlarm(void *alarm);
extern u64 __VENEER_OS_GetTick(void);
extern void __VENEER_OS_SetPeriodicAlarm(void *alarm, u64 start, u64 period, AlarmHandler handler, void *arg);
void SetupPeriodicTimeOut(u32 ms, AlarmHandler handler);
// @fn SetupPeriodicTimeOut
void SetupPeriodicTimeOut(u32 ms, AlarmHandler handler) {
    u32 period;
    u64 start;
    __VENEER_OS_CancelAlarm(ROOT->alarm);
    period = ((u64)ms * 33514) >> 6;
    start = __VENEER_OS_GetTick() + period;
    __VENEER_OS_SetPeriodicAlarm(ROOT->alarm, start, period, handler, NULL);
}
