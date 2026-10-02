#pragma force_active on
// clang-format off
#define PM_GetBackLight PM_GetBackLight_HeaderDeclaration
#include "global.h"
#include <nitro/code32.h>
#undef PM_GetBackLight
// clang-format on
#define W(p,n) (*(int*)((u8*)(p)+(n)))
#define V(p,n) (*(volatile int*)((u8*)(p)+(n)))
#define H(p,n) (*(u16*)((u8*)(p)+(n)))
#define VH(p,n) (*(volatile u16*)((u8*)(p)+(n)))
#define P(p,n) (*(void**)((u8*)(p)+(n)))
typedef void (*Callback)(int,void*);
extern u8 _021E36B0[],_021E36EC[],_021E370C[],_021E3778[],_021E3750[],_021E3754[],_021E3738[];
extern volatile int _021E36F0,_021E3784,_021E3728;
extern void *_021E3718,*_021E3724;
void PXIi_HandlerRecvFifoNotEmpty(void);
int PM_GetLCDPower(void);int PM_ForceToPowerOffAsync(Callback,void*);
void TPi_TpCallback(PXIFifoTag,u32,BOOL);void MicCommonCallback(PXIFifoTag,u32,BOOL);void RtcCommonCallback(PXIFifoTag,u32,BOOL);int RTCi_ReadRawDateAsync(void);int RTCi_ReadRawTimeAsync(void);int RTCi_ReadRawDateTimeAsync(void);
int MIC_StartAutoSamplingAsync(void*,Callback,void*);
int PMi_SetLCDPower(int,int,int,int);
void PMi_CommonCallback(PXIFifoTag,u32,BOOL);
void TP_WaitBusy(unsigned flags);
unsigned TP_CheckBusy(unsigned flags);
void *MIC_GetLastSamplingAddress(void);
int MicStartAutoSampling(unsigned a,unsigned b,unsigned c,unsigned d);
int MicStopAutoSampling(void);
void MicGetResultCallback(int x,void*p);
int MicWaitBusy(void);
int PMi_Lock(void);
void PMi_WaitBusy(void);
void PMi_DummyCallback(int x,void*p);
void PMi_CallCallbackAndUnlock(int x);
void PMi_SendPxiData(unsigned x);
int PM_SendUtilityCommandAsync(unsigned cmd,Callback cb,void*p);
int PMi_ReadRegisterAsync(unsigned id,u16*out,Callback cb,void*p);
int PMi_WriteRegisterAsync(unsigned id,unsigned value,Callback cb,void*p);
int PMi_ReadRegister(unsigned id,u16*out);
int PMi_WriteRegister(unsigned id,unsigned v);
int PMi_SetLED(int mode);
u32 PM_SetBackLight(PMLCDTarget screen,PMBackLightSwitch on);
u32 PM_ForceToPowerOff(void);
int PMi_SendLEDPatternCommand(unsigned pattern);
int PM_GetLEDPattern(unsigned*out);
int PMi_SetLEDAsync(int mode,Callback cb,void*p);
int PM_SetBackLightAsync(int screen,int on,Callback cb,void*p);
int PMi_SetAmp(unsigned x);
u32 PM_SetAmp(PMAmpSwitch x);
u32 PM_SetAmpGain(PMAmpGain x);
int PM_GetBackLight(PMBackLightSwitch*a,PMBackLightSwitch*b);
int PMi_SendLEDPatternCommandAsync(unsigned pattern,Callback cb,void*p);
int PM_GetLEDPatternAsync(unsigned*out,Callback cb,void*p);
void PMi_PrependList(void**head,void*item);
void PMi_AppendList(void**head,void*item);
void PMi_DeleteList(void**head,void*item);
void PMi_ExecuteList(void*p);
void PM_PrependPreSleepCallback(void*p);
void PM_AppendPostSleepCallback(void*p);
void PM_DeletePreSleepCallback(void*p);
void PM_DeletePostSleepCallback(void*p);
void RtcGetResultCallback(RTCResult x,void*p);
int RtcWaitBusy(void);
unsigned TP_GetLatestIndexInAuto(void);
u32 TP_WaitRawResult(TPData*out);
void TP_Init(void);
void MIC_Init(void);
void RTC_Init(void);
int MIC_StopAutoSamplingAsync(Callback cb,void*p);
int MIC_StartAutoSampling(void*p);
int MIC_StopAutoSampling(void);
RTCResult RTC_GetDateAsync(RTCDate*out,RTCCallback cb,void*p);
RTCResult RTC_GetTimeAsync(RTCTime*out,RTCCallback cb,void*p);
RTCResult RTC_GetDateTimeAsync(RTCDate*date,RTCTime*time,RTCCallback cb,void*p);
RTCResult RTC_GetDate(RTCDate*p);
RTCResult RTC_GetTime(RTCTime*p);
RTCResult RTC_GetDateTime(RTCDate*d,RTCTime*t);
int PM_GetLCDPower(void);
int PM_SetLCDPower(int mode);
int MIC_StartAutoSamplingAsync(void*params,Callback cb,void*p);
void PM_Init(void);
int PMi_SetLCDPower(int mode,int led,int force,int sync);
int PMi_SendSleepStart(unsigned a,unsigned b);
int PM_ForceToPowerOffAsync(Callback cb,void*p);
void TP_RequestSamplingAsync(void);
void TP_RequestAutoSamplingStopAsync(void);
void MicCommonCallback(PXIFifoTag tag,u32 data,BOOL err);
void PMi_CommonCallback(PXIFifoTag tag,u32 data,BOOL err);
int PMi_SetLCDPower(int mode,int led,int force,int sync){if(mode!=0){if(mode==1){if(!force){if((unsigned)(*(int*)0x027ffc3c-W(_021E370C,16))<=7)return 0;}if(led){if(sync)PMi_SetLED(led);else PMi_SetLEDAsync(led,0,0);}VH((void*)0x04000304,0)|=1;PMi_SetAmp(W(_021E370C,20));}}else{PMi_SetAmp(0);VH((void*)0x04000304,0)&=~1;W(_021E370C,16)=*(int*)0x027ffc3c;if(led){if(sync)PMi_SetLED(led);else PMi_SetLEDAsync(led,0,0);}}return 1;}

int PM_SetLCDPower(int mode){if(mode!=1)mode=0;return PMi_SetLCDPower(mode,0,0,1);}

int PM_GetLCDPower(void){return (*(u16*)0x04000304&1)?1:0;}

int PMi_SendLEDPatternCommandAsync(unsigned pattern,Callback cb,void*p){if(!PMi_Lock())return 1;P(_021E370C,32)=(void*)cb;P(_021E370C,36)=p;PMi_SendPxiData((pattern&255)|0x6600|0x3000000);return 0;}

int PMi_SendLEDPatternCommand(unsigned pattern){int result,x=PMi_SendLEDPatternCommandAsync(pattern,PMi_DummyCallback,&result);if(x)return x;PMi_WaitBusy();return result;}

int PM_GetLEDPatternAsync(unsigned*out,Callback cb,void*p){if(!PMi_Lock())return 1;P(_021E370C,32)=(void*)cb;P(_021E370C,36)=p;P(_021E370C,40)=out;PMi_SendPxiData(0x03006700);return 0;}

int PM_GetLEDPattern(unsigned*out){int result,x=PM_GetLEDPatternAsync(out,PMi_DummyCallback,&result);if(x)return x;PMi_WaitBusy();return result;}

void PMi_PrependList(void**head,void*item){if(head){P(item,8)=*head;*head=item;}}

void PMi_AppendList(void**head,void*item){void*p,*next;if(!head)return;p=*head;if(!p){P(item,8)=0;*head=item;return;}while((next=P(p,8))!=0)p=next;P(item,8)=next;P(p,8)=item;}

void PMi_DeleteList(void**head,void*item){void*p,*prev;if(!head)return;p=*head;prev=p;if(!p)return;do{if(p==item){if(p==prev)*head=P(p,8);else P(prev,8)=P(p,8);return;}prev=p;p=P(p,8);}while(p);}

void PMi_ExecuteList(void*p){while(p){((void(*)(void*))P(p,0))(P(p,4));p=P(p,8);}}

void PM_PrependPreSleepCallback(void*p){PMi_PrependList(&_021E3718,p);}

void PM_AppendPostSleepCallback(void*p){PMi_AppendList(&_021E3724,p);}

void PM_DeletePreSleepCallback(void*p){PMi_DeleteList(&_021E3718,p);}

void PM_DeletePostSleepCallback(void*p){PMi_DeleteList(&_021E3724,p);}

void RTC_Init(void){if(H(_021E3778,0))return;H(_021E3778,0)=1;W(_021E3778,12)=0;W(_021E3778,16)=0;W(_021E3778,40)=0;W(_021E3778,20)=0;W(_021E3778,24)=0;PXI_Init();while(!PXI_IsCallbackReady(5,PXI_PROC_ARM7)){}PXI_SetFifoRecvCallback(5,RtcCommonCallback);}

RTCResult RTC_GetDateAsync(RTCDate*out,RTCCallback cb,void*p){OSIntrMode x=OS_DisableInterrupts();if(W(_021E3778,12)){OS_RestoreInterrupts(x);return RTC_RESULT_BUSY;}W(_021E3778,12)=1;OS_RestoreInterrupts(x);W(_021E3778,32)=0;W(_021E3778,36)=0;P(_021E3778,20)=out;P(_021E3778,16)=(void*)cb;P(_021E3778,28)=p;return RTCi_ReadRawDateAsync()?RTC_RESULT_SUCCESS:RTC_RESULT_SEND_ERROR;}

RTCResult RTC_GetDate(RTCDate*p){RTCResult x=RTC_GetDateAsync(p,RtcGetResultCallback,0);W(_021E3778,44)=x;if(!x)RtcWaitBusy();return (RTCResult)W(_021E3778,44);}

RTCResult RTC_GetTimeAsync(RTCTime*out,RTCCallback cb,void*p){OSIntrMode x=OS_DisableInterrupts();if(W(_021E3778,12)){OS_RestoreInterrupts(x);return RTC_RESULT_BUSY;}W(_021E3778,12)=1;OS_RestoreInterrupts(x);W(_021E3778,32)=1;W(_021E3778,36)=0;P(_021E3778,20)=out;P(_021E3778,16)=(void*)cb;P(_021E3778,28)=p;return RTCi_ReadRawTimeAsync()?RTC_RESULT_SUCCESS:RTC_RESULT_SEND_ERROR;}

RTCResult RTC_GetTime(RTCTime*p){RTCResult x=RTC_GetTimeAsync(p,RtcGetResultCallback,0);W(_021E3778,44)=x;if(!x)RtcWaitBusy();return (RTCResult)W(_021E3778,44);}

RTCResult RTC_GetDateTimeAsync(RTCDate*date,RTCTime*time,RTCCallback cb,void*p){OSIntrMode x=OS_DisableInterrupts();if(W(_021E3778,12)){OS_RestoreInterrupts(x);return RTC_RESULT_BUSY;}W(_021E3778,12)=1;OS_RestoreInterrupts(x);W(_021E3778,32)=2;W(_021E3778,36)=0;P(_021E3778,20)=date;P(_021E3778,24)=time;P(_021E3778,16)=(void*)cb;P(_021E3778,28)=p;return RTCi_ReadRawDateTimeAsync()?RTC_RESULT_SUCCESS:RTC_RESULT_SEND_ERROR;}

RTCResult RTC_GetDateTime(RTCDate*d,RTCTime*t){RTCResult x=RTC_GetDateTimeAsync(d,t,RtcGetResultCallback,0);W(_021E3778,44)=x;if(!x)RtcWaitBusy();return (RTCResult)W(_021E3778,44);}
