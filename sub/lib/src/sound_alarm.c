#pragma force_active on
#include <nitro/types.h>
// The SDK headers below expect <nitro/types.h> to be included first.
#include <nitro/SND_work_shared.h>

#include "OS_alarm.h"
#include "OS_tick.h"
#include "PXI_fifo.h"
// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

// Alarms the ARM9 sound library sets up; each one posts (index | id << 8) back when it fires.

// Not declared by the OS_*.h headers used with the SND_*_shared.h types.
void OS_SetPeriodicAlarm(OSAlarm *alarm, OSTick start, OSTick period, OSAlarmHandler handler, void *arg);

void SND_AlarmInit(void);
void SND_SetupAlarm(int index, OSTick tick, OSTick period, int id);
void SND_StartAlarm(int index);
void SND_StopAlarm(int index);
void AlarmHandler(void *arg);

// -ipa file would otherwise inline these small functions into their callers in this file.
#pragma dont_inline on

void SND_AlarmInit(void) {
    int i;
    for (i = 0; i < 8; i++) {
        SNDi_Work.alarms[i].enable = 0;
        SNDi_Work.alarms[i].id = 0;
    }
}

void SND_SetupAlarm(int index, OSTick tick, OSTick period, int id) {
    struct SNDAlarm *alarm = &SNDi_Work.alarms[index];
    if (alarm->enable) {
        OS_CancelAlarm(&alarm->alarm);
        alarm->enable = 0;
    }
    alarm->tick = tick;
    alarm->period = period;
    alarm->id = id;
}

void SND_StartAlarm(int index) {
    OSTick tick, period;
    struct SNDAlarm *alarm = &SNDi_Work.alarms[index];
    u32 arg;
    if (alarm->enable) {
        OS_CancelAlarm(&alarm->alarm);
        alarm->enable = 0;
    }
    period = alarm->period;
    tick = alarm->tick;
    arg = index | ((u32)alarm->id << 8);
    OS_CreateAlarm(&alarm->alarm);
    if (!period) {
        OS_SetAlarm(&alarm->alarm, tick, AlarmHandler, (void *)arg);
    } else {
        OS_SetPeriodicAlarm(&alarm->alarm, OS_GetTick() + tick, period, AlarmHandler, (void *)arg);
    }
    alarm->enable = 1;
}

void SND_StopAlarm(int index) {
    struct SNDAlarm *alarm = &SNDi_Work.alarms[index];
    if (alarm->enable) {
        OS_CancelAlarm(&alarm->alarm);
        alarm->id++;
        alarm->enable = 0;
    }
}

void AlarmHandler(void *arg) {
    while (PXI_SendWordByFifo(PXI_FIFO_TAG_SOUND, (u32)arg, 0) < 0) {}
}

#pragma dont_inline reset
