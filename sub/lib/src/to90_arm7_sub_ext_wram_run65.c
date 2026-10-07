#pragma force_active on
#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
#define H(p,o) (*(u16 *)((u8 *)(p)+(o)))
#define SH(p,o) (*(s16 *)((u8 *)(p)+(o)))
#define U(p,o) (*(u32 *)((u8 *)(p)+(o)))
#define P(p,o) (*(void **)((u8 *)(p)+(o)))
extern u8 wmspW[];
extern u8 wmspVAlarm[];
#define STATE P(wmspW+0x1000,0x550)
extern u16 *__VENEER_WMSP_GetBuffer4Callback2Wm9(void);
extern void __VENEER_WMSP_ReturnResult2Wm9(void *);
extern void __VENEER_MI_CpuCopy8(const void *,void *,u32);
extern void __VENEER_MI_CpuFill8(void *,u32,u32);
extern void __VENEER_MIi_CpuCopy16(const void *,void *,u32);
extern u64 __VENEER_OS_GetTick(void);
extern u32 __VENEER_OS_DisableInterrupts(void);
extern void __VENEER_OS_RestoreInterrupts(u32);
extern int __VENEER_OS_SendMessage(void *,void *,int);
extern void __VENEER_OS_CancelVAlarm(void *);
extern void __VENEER_OS_SetVAlarm(void *,s16,s16,void (*)(void *),void *);
extern void OS_CreateVAlarm(void *);
extern void OS_CancelVAlarm(void *);
extern void WmspParentAdjustVSync(void *);
extern void WmspChildAdjustVSync1(void *);
extern void WmspChildAdjustVSync2(void *);
extern void WmspChildVAlarmMP(void *);
extern void WmspParentVAlarmMP(void *);
extern void WmspFromVAlarmToWmspThread(void);
extern void *WMSP_GetInternalRequestBuf(void);
extern void WMSP_SendMaMP(u16);
extern void WMSP_ResumeMaMP(u16);
extern void WMSP_SendMaKeyData(void);
extern u16 *WMSP_WL_ParamSetWepKeyId(void *,u16);
extern u16 *WMSP_WL_ParamSetBeaconSendRecvInd(void *,u16);
extern u16 *WMSP_WL_ParamSetLifeTime(void *,u16,u16,u16);
extern u16 *WMSP_WL_ParamSetGameInfo(void *,u16,void *);
extern void WMSP_CopyParentParam(void *,void *);
extern u16 *WMSP_WL_DevSetInitializeWirelessCounter(void *);
extern u16 *WMSP_WL_DevGetWirelessCounter(void *);
extern u16 *WMSP_WL_DevGetStationState(void *);
extern int WMSP_SetAllParams(int,void *);
extern u16 *WMSP_WL_DevClass1(void *);
extern u16 *WMSP_WL_DevIdle(void *);
extern u16 *WMSP_WL_MlmePowerManagement(void *,u16,u16,u16);
extern u16 *WMSP_WL_MlmeMeasureChannel(void *,u16,u16,u16,void *);
extern void WmspError_06005BB8(u16,u16);
extern void WMSP_AddRssiToList(u8);
extern u16 WMSP_GetAverageLinkLevel(void);
void WMSP_SetLifeTime(void *req);
#pragma opt_lifetimes off
// @fn WMSP_SetLifeTime
void WMSP_SetLifeTime(void *req){u8 b[512];u16 a=U(req,4),d=U(req,8),e=U(req,12),life=U(req,16);void*s=STATE;u16 err=WMSP_WL_ParamSetLifeTime(b,a,d,e)[2];u16*c;if(err){c=__VENEER_WMSP_GetBuffer4Callback2Wm9();c[0]=29;c[1]=1;c[2]=0x211;c[3]=err;__VENEER_WMSP_ReturnResult2Wm9(c);}else{u64 ticks;int i;if(life!=65535){if(!life)ticks=1;else ticks=((u64)(s32)(life*100)*0x82ea)>>6;*(u64*)((u8*)s+0x7b8)=ticks;}else *(u64*)((u8*)s+0x7b8)=0;ticks=__VENEER_OS_GetTick()|1;for(i=0;i<16;i++)*(u64*)((u8*)s+0x738+i*8)=ticks;c=__VENEER_WMSP_GetBuffer4Callback2Wm9();c[0]=29;c[1]=0;__VENEER_WMSP_ReturnResult2Wm9(c);}}
