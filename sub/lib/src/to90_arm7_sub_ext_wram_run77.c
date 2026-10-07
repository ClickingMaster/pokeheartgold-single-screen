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
void WMSP_VAlarmSetMPData(void *arg);
// @fn WMSP_VAlarmSetMPData
void WMSP_VAlarmSetMPData(void *arg){void*s=STATE;H(s,0xce)=0;H(s,0x88)=1;H(s,0xc0)--;if(H(s,0xc0)==0){if(H(s,0xbe)==65535)H(s,0xbe)=4;WMSP_AddRssiToList((u8)H(s,0xbe));H(s,0xbc)=WMSP_GetAverageLinkLevel();H(s,0xbe)=65535;H(s,0xc0)=1;}if(H(s,0)==9){u32 irq=__VENEER_OS_DisableInterrupts();int skip,send;if(H(s,0x182)==0){H(s,0x62)=0;__VENEER_OS_RestoreInterrupts(irq);return;}skip=1;if(SH(s,0x62)>0&&SH(s,0x64)>0)skip=0;if(SH(s,0x62)<0)SH(s,0x62)=0;SH(s,0x62)+=SH(s,0x7c2);if(SH(s,0x62)>256)SH(s,0x62)=256;H(s,0x64)=H(s,0x7c4);if(skip&&SH(s,0x62)>0&&SH(s,0x64)>0)send=1;else send=0;__VENEER_OS_RestoreInterrupts(irq);if(send)WMSP_SendMaMP(65535);if(H(s,0x7c6)==1){H(s,0xa0)--;if(!H(s,0xa0)){H(s,0x9e)=1;H(s,0xa0)=60;}}}else if(H(s,0)==10){int send=0;u32 irq=__VENEER_OS_DisableInterrupts();if(U(s,0x734)!=1){H(s,0x60)=0;send=1;}__VENEER_OS_RestoreInterrupts(irq);if(send==1)WMSP_SendMaKeyData();}}
