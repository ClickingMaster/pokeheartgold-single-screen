#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

typedef struct W35232Alarm {
    u8 active, id;
    u8 pad[2];
    u64 delay, period;
    u8 alarm[44];
} W35232Alarm;

extern u8 SNDi_Work[];
extern void OS_CancelAlarm(void *);
void SND_SetupAlarm(int i, u64 delay, u64 period, int id);

void SND_SetupAlarm(int i, u64 delay, u64 period, int id) {
    W35232Alarm *a = (W35232Alarm *)(SNDi_Work + 0xf80) + i;
    if (a->active) {
        OS_CancelAlarm(a->alarm);
        a->active = 0;
    }
    a->delay = delay;
    a->period = period;
    a->id = id;
}
