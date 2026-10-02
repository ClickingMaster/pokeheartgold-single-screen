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
void TP_WaitBusy(unsigned flags){while(VH(_021E36B0,0x3a)&flags){}}

unsigned TP_CheckBusy(unsigned flags){return H(_021E36B0,0x38)&flags;}

void MIC_Init(void){if(H(_021E36EC,0))return;H(_021E36EC,0)=1;W(_021E36EC,4)=0;W(_021E36EC,8)=0;PXI_Init();while(!PXI_IsCallbackReady(9,PXI_PROC_ARM7)){}*(int*)0x027fff90=0;PXI_SetFifoRecvCallback(9,MicCommonCallback);}

int MIC_StartAutoSamplingAsync(void*params,Callback cb,void*p){unsigned flags;OSIntrMode irq;if(W(params,4)&31)return 2;if(W(params,8)&31)return 2;if(!W(params,8))return 2;if((unsigned)W(params,12)<1024)return 2;switch(W(params,0)){case 0:flags=0;break;case 1:flags=1;break;case 2:flags=2;break;case 3:flags=3;break;case 4:flags=5;break;case 5:flags=7;break;default:return 2;}if(W(params,16))flags=(u8)(flags|16);else flags=(u8)flags;flags=(u8)flags;irq=OS_DisableInterrupts();if(W(_021E36EC,4)){OS_RestoreInterrupts(irq);return 1;}W(_021E36EC,4)=1;OS_RestoreInterrupts(irq);P(_021E36EC,8)=(void*)cb;P(_021E36EC,12)=p;P(_021E36EC,20)=P(params,20);P(_021E36EC,24)=P(params,24);return MicStartAutoSampling(W(params,4),W(params,8),W(params,12),flags)?0:3;}

int MIC_StartAutoSampling(void*p){int x=MIC_StartAutoSamplingAsync(p,MicGetResultCallback,0);W(_021E36EC,16)=x;if(!x)MicWaitBusy();return W(_021E36EC,16);}

int MIC_StopAutoSamplingAsync(Callback cb,void*p){OSIntrMode x=OS_DisableInterrupts();if(W(_021E36EC,4)){OS_RestoreInterrupts(x);return 1;}W(_021E36EC,4)=1;OS_RestoreInterrupts(x);P(_021E36EC,8)=(void*)cb;P(_021E36EC,12)=p;return MicStopAutoSampling()?0:3;}

int MIC_StopAutoSampling(void){int x=MIC_StopAutoSamplingAsync(MicGetResultCallback,0);W(_021E36EC,16)=x;if(!x)MicWaitBusy();return W(_021E36EC,16);}

void *MIC_GetLastSamplingAddress(void){return *(void**)0x027fff90;}
