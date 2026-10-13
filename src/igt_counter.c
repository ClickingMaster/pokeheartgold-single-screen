#include "igt_counter.h"

#include "global.h"

#include "timer_3.h"

static IGTCounter sIGTCounter;

void IGTCounter_Init(void) {
    sIGTCounter.active = 0;
}

void IGTCounter_Start(IGT *igt) {
    sIGTCounter.active = 1;
    sIGTCounter.unk10 = 0;
    sIGTCounter.unk14 = 0;
    sIGTCounter.countedSeconds = 0;
    sIGTCounter.igt = igt;
    sIGTCounter.startTicks = GetTimer3Ticks();
}

void IGTCounter_Update(void) {
    if (sIGTCounter.active == 0) {
        return;
    }

    u64 val = GetTimer3Ticks();
    u64 val2 = Timer3TicksToSeconds(val - sIGTCounter.startTicks);
    if (sIGTCounter.countedSeconds >= val2) {
        return;
    }

    AddIGTSeconds(sIGTCounter.igt, val2 - sIGTCounter.countedSeconds);
    sIGTCounter.countedSeconds = val2;
}
