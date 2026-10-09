#pragma force_active on
#pragma dollar_identifiers on
#include <nitro/types.h>
#include <OS_valarm.h>
#include <OS_system.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
extern int SPIi_CheckException(u32);
extern void SPIi_GetException(u32);
extern void SPIi_ReleaseException(u32);
extern void SPIi_ReturnResult(u16,u16);
extern int SPIi_SetEntry(u32,u32,u16,...);
typedef struct{u32 unused,cmd,count,start;}W40748Request;
typedef OSVAlarm W40748Alarm;
typedef struct{u8 prefix[32];u32 state;s32 samples,minimum;W40748Alarm alarms[4];s16 lines[4];}W40748Work;
typedef char W40748WorkSize[(sizeof(W40748Work)==0xd4)?1:-1];
typedef union {u32 word;struct{u32 x:12,y:12,touch:1,invalid:2,rest:5;} b;} W35038Sample;
typedef union{W35038Sample sample;u16 halves[2];}W40748Sample;
extern W40748Work tpw;
static u8 invalid_cnt$3730;
static u8 valid_cnt$3731;
extern void TP_ExecSampling(W35038Sample*,s32,u16*);
extern void TpVAlarmHandler(void*);
extern void OS_SetPeriodicVAlarm(OSVAlarm*,s16,s16,OSAlarmHandler,void*);
extern void OS_CancelVAlarms(u32);
void TP_ExecuteProcess(void*);
static inline void UpdateTouchThreshold09(const W35038Sample*sample,u16 deviation){if(!sample->b.touch){invalid_cnt$3730=0;valid_cnt$3731=0;}
else if(sample->b.invalid){valid_cnt$3731=0;if(++invalid_cnt$3730>=4){invalid_cnt$3730=0;if(tpw.samples<35)tpw.samples++;}}
else{invalid_cnt$3730=0;if((s32)deviation>=(tpw.samples>>1))valid_cnt$3731=0;else if(++valid_cnt$3731>=4){valid_cnt$3731=0;if(tpw.samples>tpw.minimum){tpw.samples--;invalid_cnt$3730=3;}}}}
// @fn TP_ExecuteProcess
void TP_ExecuteProcess(void*request){W40748Request*q=request;
switch(q->cmd){
case 0x10:if(tpw.state!=2)break;
case 0:{OSIntrMode irq;W40748Sample result;u16 deviation;
irq=OS_DisableInterrupts();
if(!SPIi_CheckException(0)){OS_RestoreInterrupts(irq);SPIi_ReturnResult((u16)q->cmd,4);break;}
SPIi_GetException(0);OS_RestoreInterrupts(irq);
TP_ExecSampling(&result.sample,tpw.samples,&deviation);
UpdateTouchThreshold09(&result.sample,deviation);
*(volatile u16*)0x027fffaa=result.halves[0];*(volatile u16*)0x027fffac=result.halves[1];
if(q->cmd==0)SPIi_ReturnResult(q->cmd,0);else SPIi_ReturnResult(q->cmd,q->count&255);
SPIi_ReleaseException(0);break;}
case 1:if(tpw.state==1){u32 i;for(i=0;i<q->count;i++){u16 line=(u16)((q->start+(i*263u/q->count))%263u);if(line>=200&&line<215)line=215;tpw.lines[i]=(s16)line;OS_SetPeriodicVAlarm(&tpw.alarms[i],tpw.lines[i],10,TpVAlarmHandler,(void*)i);}SPIi_ReturnResult((u16)q->cmd,0);tpw.state=2;}else SPIi_ReturnResult((u16)q->cmd,3);break;
case 2:if(tpw.state==3){OS_CancelVAlarms(0x54505641);SPIi_ReturnResult((u16)q->cmd,0);tpw.state=0;}else SPIi_ReturnResult((u16)q->cmd,3);break;
}}
