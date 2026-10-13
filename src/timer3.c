#include "global.h"

#include "timer_3.h"

vu64 gTimer3Overflows;
int gTimer3NeedsReset;

static void Timer3_IrqHandler(void);

void Init_Timer3(void) {
    gTimer3Overflows = 0;
    gTimer3NeedsReset = 0;
    OS_SetTimerControl(OS_TIMER_3, 0);
    OS_SetTimerCount(OS_TIMER_3, 0);
    OS_SetTimerControl(OS_TIMER_3, (OS_TIMER_PRESCALER_64 << REG_OS_TM3CNT_H_PS_SHIFT) | (1 << REG_OS_TM3CNT_H_I_SHIFT) | (1 << REG_OS_TM3CNT_H_E_SHIFT));
    OS_SetIrqFunction(OS_IE_TIMER3, Timer3_IrqHandler);
    OS_EnableIrqMask(OS_IE_TIMER3);
}

static void Timer3_IrqHandler(void) {
    gTimer3Overflows++;
    if (gTimer3NeedsReset) {
        OS_SetTimerControl(OS_TIMER_3, 0);
        OS_SetTimerCount(OS_TIMER_3, 0);
        OS_SetTimerControl(OS_TIMER_3, (OS_TIMER_PRESCALER_64 << REG_OS_TM3CNT_H_PS_SHIFT) | (1 << REG_OS_TM3CNT_H_I_SHIFT) | (1 << REG_OS_TM3CNT_H_E_SHIFT));
        gTimer3NeedsReset = 0;
    }
    OS_SetIrqCheckFlag(OS_IE_TIMER3);
    OS_SetIrqFunction(OS_IE_TIMER3, Timer3_IrqHandler);
}

u64 Timer3_GetTicks(void) {
    vu16 count_lo;
    vu64 count_hi;
    OSIntrMode bak_psr;

    bak_psr = OS_DisableInterrupts();
    count_lo = *(REGType16 *)((u32)(REG_TM0CNT_L_ADDR + 4 * OS_TIMER_3));
    count_hi = gTimer3Overflows & 0x0000FFFFFFFFFFFFull;
    if ((OS_GetRequestIrqMask() & OS_IE_TIMER3) && !(count_lo & 0x8000)) {
        count_hi++;
    }
    OS_RestoreInterrupts(bak_psr);

    return (count_hi << 16) | count_lo;
}

u64 GetTimer3Ticks(void) {
    return Timer3_GetTicks();
}

u64 Timer3TicksToSeconds(u64 ticks) {
    return (ticks * 64) / OS_SYSTEM_CLOCK;
}
