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
int PMi_ReadRegister(unsigned id,u16*out){int result,x=PMi_ReadRegisterAsync(id,out,PMi_DummyCallback,&result);if(x)return x;PMi_WaitBusy();return result;}

int PMi_WriteRegisterAsync(unsigned id,unsigned value,Callback cb,void*p){if(!PMi_Lock())return 1;P(_021E370C,32)=(void*)cb;P(_021E370C,36)=p;PMi_SendPxiData((id&255)|0x6400|0x2000000);PMi_SendPxiData(0x01010000|(value&65535));return 0;}

int PMi_WriteRegister(unsigned id,unsigned v){int result,x=PMi_WriteRegisterAsync(id,v,PMi_DummyCallback,&result);if(x)return x;PMi_WaitBusy();return result;}

int PMi_SetLEDAsync(int mode,Callback cb,void*p){int cmd;switch(mode){case 1:cmd=1;break;case 3:cmd=2;break;case 2:cmd=3;break;default:cmd=0;break;}if(!cmd)return 65535;return PM_SendUtilityCommandAsync(cmd,cb,p);}

int PMi_SetLED(int mode){int result,x=PMi_SetLEDAsync(mode,PMi_DummyCallback,&result);if(x)return x;PMi_WaitBusy();return result;}

int PM_SetBackLightAsync(int screen,int on,Callback cb,void*p){int cmd=0;if(screen==0){if(on==1)cmd=6;if(on==0)cmd=7;}else if(screen==1){if(on==1)cmd=4;if(on==0)cmd=5;}else if(screen==2){if(on==1)cmd=8;if(on==0)cmd=9;}if(!cmd)return 65535;return PM_SendUtilityCommandAsync(cmd,cb,p);}

u32 PM_SetBackLight(PMLCDTarget screen,PMBackLightSwitch on){int result,x=PM_SetBackLightAsync(screen,on,PMi_DummyCallback,&result);if(x)return x;PMi_WaitBusy();return result;}

int PM_ForceToPowerOffAsync(Callback cb,void*p){int top,bottom;OS_SpinWait(0x996a00);if(PM_GetLCDPower()!=1){PM_GetBackLight((PMBackLightSwitch*)&top,(PMBackLightSwitch*)&bottom);if(top)PM_SetBackLight(PM_LCD_TOP,PM_BACKLIGHT_OFF);if(bottom)PM_SetBackLight(PM_LCD_BOTTOM,PM_BACKLIGHT_OFF);if(!PM_SetLCDPower(1)){do{OS_SpinWait(0x996a00);}while(!PM_SetLCDPower(1));}}return PM_SendUtilityCommandAsync(14,cb,p);}

u32 PM_ForceToPowerOff(void){int result,x=PM_ForceToPowerOffAsync(PMi_DummyCallback,&result);if(x)return x;PMi_WaitBusy();return result;}

u32 PM_SetAmp(PMAmpSwitch x){W(_021E370C,20)=x;return PMi_SetAmp(x);}

int PMi_SetAmp(unsigned x){if(PM_GetLCDPower()==0)return 0;return PMi_WriteRegister(2,(u16)x);}

u32 PM_SetAmpGain(PMAmpGain x){return PMi_WriteRegister(3,(u16)x);}

int PM_GetBackLight(PMBackLightSwitch*a,PMBackLightSwitch*b){u16 reg;int x=PMi_ReadRegister(0,&reg);if(x)return x;if(a)*a=(reg&8)?PM_BACKLIGHT_ON:PM_BACKLIGHT_OFF;if(b)*b=(reg&4)?PM_BACKLIGHT_ON:PM_BACKLIGHT_OFF;return x;}

void PMi_SendPxiData(unsigned x){while(PXI_SendWordByFifo(8,x,0)!=0){}}
