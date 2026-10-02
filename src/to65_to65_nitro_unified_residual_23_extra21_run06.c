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
void PMi_DummyCallback(int x,void*p){W(p,0)=x;}

void PMi_CallCallbackAndUnlock(int x){Callback cb=(Callback)P(_021E370C,32);void*p=P(_021E370C,36);if(W(_021E370C,28))W(_021E370C,28)=0;if(cb){P(_021E370C,32)=0;cb(x,p);}}
