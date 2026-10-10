#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

typedef struct W35234Alarm {
    u8 active, id;
    u8 pad[2];
    u64 delay, period;
    u8 alarm[44];
} W35234Alarm;

extern u8 SNDi_Work[];
extern void OS_CancelAlarm(void *);
void SND_StopAlarm(int i);

void SND_StopAlarm(int i) {
    W35234Alarm *a = (W35234Alarm *)(SNDi_Work + 0xf80) + i;
    if (a->active) {
        OS_CancelAlarm(a->alarm);
        a->id++;
        a->active = 0;
    }
}
