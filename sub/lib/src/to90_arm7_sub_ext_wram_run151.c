#pragma force_active on
#pragma dollar_identifiers on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#define BASE    (*(u8 **)0x0380fff4)
#define H(p, o) (*(u16 *)((u8 *)(p) + (o)))
#define P(p, o) (*(u8 **)((u8 *)(p) + (o)))
#define HW(a)   (*(volatile u16 *)(a))
void __VENEER_OS_CancelAlarm(void *);
void WShutdown(void);
void WStop(void);
void WWakeUp(void);
void WEnableTmpttPowerSave(void);
void WIntervalTimer(void);
void SetupPeriodicTimeOut(u32, void (*)(void));
void WSetStaState(u32 state);

void WSetStaState(u32 state) {
    u8 *w = BASE;
    u8 *s = w + 0x344;
    if (H(s, 8) == state) {
        return;
    }
    if (H(s, 8) == 64) {
        __VENEER_OS_CancelAlarm(w + 0x634);
    }
    switch (state) {
    case 0:
        WShutdown();
        break;
    case 16:
        HW(0x04808040) = 0;
        WStop();
        WWakeUp();
        break;
    case 64:
        if (H(s, 12) == 2) {
            WEnableTmpttPowerSave();
        }
        SetupPeriodicTimeOut(100, WIntervalTimer);
        break;
    }
    H(s, 8) = state;
}
