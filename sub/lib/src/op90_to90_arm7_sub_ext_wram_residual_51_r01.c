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
extern u64 __VENEER__ll_udiv(u64, u64);
extern void __VENEER_OS_SetAlarm(void *alarm, u64 tick, AlarmHandler handler, void *arg);
void WaitLoop_Waitus(u32 us, AlarmHandler handler);
// @fn WaitLoop_Waitus
void WaitLoop_Waitus(u32 us, AlarmHandler handler) {
    volatile u32 waiting; // cleared by the alarm handler (interrupt context)
    u64 tick;
    waiting = 1;
    tick = __VENEER__ll_udiv(((u64)us * 33514) >> 6, 1000);
    __VENEER_OS_SetAlarm(ROOT->alarm, tick, handler, (void *)&waiting);
    while (waiting) {
    }
}
