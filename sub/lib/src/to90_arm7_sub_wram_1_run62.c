#pragma force_active on
#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
typedef struct W35231Alarm { u8 active,id; u8 pad[2]; u64 delay,period; u8 alarm[44]; } W35231Alarm;
extern u8 SNDi_Work[];
extern void OS_CancelAlarm(void *);
void SND_AlarmInit(void);
typedef struct W35231Work {u8 pad[0xf80];W35231Alarm alarms[8];} W35231Work;
// @fn SND_AlarmInit
void SND_AlarmInit(void){int i;for(i=0;i<8;i++){((W35231Work *)SNDi_Work)->alarms[i].active=0;((W35231Work *)SNDi_Work)->alarms[i].id=0;}}
