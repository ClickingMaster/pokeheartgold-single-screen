#pragma force_active on
#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
typedef struct W35233Alarm { u8 active,id; u8 pad[2]; u64 delay,period; u8 alarm[44]; } W35233Alarm;
extern u8 SNDi_Work[];
extern void OS_CancelAlarm(void *);
extern void OS_CreateAlarm(void *),AlarmHandler(void *);
extern u64 OS_GetTick(void);
extern void OS_SetAlarm(void *,u64,void (*)(void *),void *),OS_SetPeriodicAlarm(void *,u64,u64,void (*)(void *),void *);
void SND_StartAlarm(int i);
// @fn SND_StartAlarm
void SND_StartAlarm(int i){u64 delay,period;W35233Alarm *a=(W35233Alarm *)(SNDi_Work+0xf80)+i;u32 arg;if(a->active){OS_CancelAlarm(a->alarm);a->active=0;}period=a->period;delay=a->delay;arg=i|((u32)a->id<<8);OS_CreateAlarm(a->alarm);if(!period)OS_SetAlarm(a->alarm,delay,AlarmHandler,(void*)arg);else OS_SetPeriodicAlarm(a->alarm,OS_GetTick()+delay,period,AlarmHandler,(void*)arg);a->active=1;}
