#include "igt_counter.h"

#include "global.h"

#include "timer_3.h"

static IGTCounter sIGTCounter;

void IGTCounter_Init(void) {
    sIGTCounter.unk0 = 0;
}

void IGTCounter_Start(IGT *igt) {
    sIGTCounter.unk0 = 1;
    sIGTCounter.unk10 = 0;
    sIGTCounter.unk14 = 0;
    sIGTCounter.unk8 = 0;
    sIGTCounter.igt = igt;
    sIGTCounter.unk18 = sub_020254FC();
}

void IGTCounter_Update(void) {
    if (sIGTCounter.unk0 == 0) {
        return;
    }

    u64 val = sub_020254FC();
    u64 val2 = sub_02025504(val - sIGTCounter.unk18);
    if (sIGTCounter.unk8 >= val2) {
        return;
    }

    AddIGTSeconds(sIGTCounter.igt, val2 - sIGTCounter.unk8);
    sIGTCounter.unk8 = val2;
}
