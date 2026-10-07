#pragma force_active on
#include <nitro/types.h>
#include <nitro/os/message.h>
#include <nitro/exi/ARM7/genPort.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
typedef struct {u16 mode:1,b:3,irq1:1,irq2:1,low:1,power:1,:8;} Status37003;
typedef struct {u16 irq:4,pad:2,bit:1,power:1,:8;} Status237003;
typedef struct {u32 year:8,month:5,pad:3,day:6,pad2:2,weekday:3,pad3:5;u32 time;} Date37003;
#include "to91_arm7_rtc_work_private.h"
extern u16 rtcInitialized;
extern void RTC_ReadStatus1(void *),RTC_ReadStatus2(void *),RTC_ReadDateTime(void *),RTC_WriteDateTime(void *),RTC_SetHourFormat(int);
extern void RTC_WriteStatus1(const void *),RTC_WriteStatus2(const void *);
extern u32 RtcBCD2HEX(u32);
extern void PXI_Init(void),RtcAlarmIntr(void);

extern void OS_SetIrqFunction(u32,void (*)(void));extern u32 OS_EnableIrqMask(u32);
static inline void InitializeHardwareStatuses(Status37003 *first,Status237003 *second){rtcInitialized=1;rtcWork.busy=1;RTC_ReadStatus1(first);RTC_ReadStatus2(second);}
void RTC_Init(u32 priority);
void RTC_Init(u32 priority){Status237003 s2;Status37003 s1;Date37003 *date=(Date37003 *)0x027ffde8;u32 day,month,year,weekday;OSIntrMode irq;
 if(rtcInitialized)return;InitializeHardwareStatuses(&s1,&s2);
if(s1.power||s1.low||s2.power){s1.mode=1;RTC_WriteStatus1(&s1);}
if(s1.irq1||s1.irq2){s2.irq=0;s2.bit=0;RTC_WriteStatus2(&s2);}
RTC_ReadDateTime(date);day=RtcBCD2HEX(date->day);month=RtcBCD2HEX(date->month);year=RtcBCD2HEX(date->year)+2000;
if(month-1<=1){year--;month+=12;}
weekday=(year+year/4-year/100+year/400+(13*month+8)/5+day)%7;
if(date->weekday!=weekday){date->weekday=weekday;RTC_WriteDateTime(date);}
RTC_SetHourFormat(1);rtcWork.busy=0;PXI_Init();PXI_SetFifoRecvCallback(5,RtcPxiCallback);
OS_InitMessageQueue(&rtcWork.queue,rtcWork.messages,4);OS_CreateThread(&rtcWork.thread,RtcThread,0,rtcWork.stack+sizeof rtcWork.stack,256,priority);OS_WakeupThreadDirect(&rtcWork.thread);
EXIi_SelectRcnt(EXI_GPIOIF_GPIO);EXIi_SetBitRcnt0L(0x40,0);EXIi_SetBitRcnt0L(0x100,0x100);irq=OS_DisableInterrupts();OS_SetIrqFunction(0x80,RtcAlarmIntr);OS_EnableIrqMask(0x80);OS_RestoreInterrupts(irq);}
