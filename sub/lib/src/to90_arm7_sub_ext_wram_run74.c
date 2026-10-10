#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#define H(p, o)  (*(u16 *)((u8 *)(p) + (o)))
#define SH(p, o) (*(s16 *)((u8 *)(p) + (o)))
#define U(p, o)  (*(u32 *)((u8 *)(p) + (o)))
#define P(p, o)  (*(void **)((u8 *)(p) + (o)))
extern u8 wmspW[];
extern u8 wmspVAlarm[];
#define STATE P(wmspW + 0x1000, 0x550)
extern u16 *__VENEER_WMSP_GetBuffer4Callback2Wm9(void);
extern void __VENEER_WMSP_ReturnResult2Wm9(void *);
extern void __VENEER_MI_CpuCopy8(const void *, void *, u32);
extern void __VENEER_MI_CpuFill8(void *, u32, u32);
extern void __VENEER_MIi_CpuCopy16(const void *, void *, u32);
extern u64 __VENEER_OS_GetTick(void);
extern u32 __VENEER_OS_DisableInterrupts(void);
extern void __VENEER_OS_RestoreInterrupts(u32);
extern int __VENEER_OS_SendMessage(void *, void *, int);
extern void __VENEER_OS_CancelVAlarm(void *);
extern void __VENEER_OS_SetVAlarm(void *, s16, s16, void (*)(void *), void *);
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
extern u16 *WMSP_WL_ParamSetWepKeyId(void *, u16);
extern u16 *WMSP_WL_ParamSetBeaconSendRecvInd(void *, u16);
extern u16 *WMSP_WL_ParamSetLifeTime(void *, u16, u16, u16);
extern u16 *WMSP_WL_ParamSetGameInfo(void *, u16, void *);
extern void WMSP_CopyParentParam(void *, void *);
extern u16 *WMSP_WL_DevSetInitializeWirelessCounter(void *);
extern u16 *WMSP_WL_DevGetWirelessCounter(void *);
extern u16 *WMSP_WL_DevGetStationState(void *);
extern int WMSP_SetAllParams(int, void *);
extern u16 *WMSP_WL_DevClass1(void *);
extern u16 *WMSP_WL_DevIdle(void *);
extern u16 *WMSP_WL_MlmePowerManagement(void *, u16, u16, u16);
extern u16 *WMSP_WL_MlmeMeasureChannel(void *, u16, u16, u16, void *);
extern void WmspError_06005BB8(u16, u16);
extern void WMSP_AddRssiToList(u8);
extern u16 WMSP_GetAverageLinkLevel(void);
void WmspParentAdjustVSync(void *arg);

void WmspParentAdjustVSync(void *arg) {
    void *s = STATE;
    s32 line;
    u32 lo, hi, lo2;
    if (H(s, 0xdc) >= 60) {
        line = *(volatile u16 *)0x04000006;
        if (line >= 203 && line < 210) {
            *(volatile u16 *)0x04000006 = line;
            H(s, 0xdc) = 0;
        }
    } else {
        H(s, 0xdc)
        ++;
    }
    line = *(volatile u16 *)0x04000006;
    lo = *(volatile u16 *)0x048080f8;
    hi = *(volatile u16 *)0x048080fa;
    lo2 = *(volatile u16 *)0x048080f8;
    if (lo > lo2) {
        hi = *(volatile u16 *)0x048080fa;
    }
    *(volatile u16 *)0x0380fff0 = (((lo2 | (hi << 16)) * 2 - (u32)line * 127) >> 7);
    __VENEER_OS_SetVAlarm(wmspVAlarm, SH(s, 0x40), 263, WmspParentVAlarmMP, (void *)5);
}
