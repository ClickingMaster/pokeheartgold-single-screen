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
void WMSP_SetGameInfo(void *req);

void WMSP_SetGameInfo(void *req) {
    u8 buf[512];
    u8 param[128];
    void *s = STATE;
    u8 flags;
    u16 *r, *c;
    U(s, 0xe8) = U(req, 4);
    H(s, 0xec) = U(req, 8);
    U(s, 0xf0) = U(req, 12);
    H(s, 0xf4) = U(req, 16);
    flags = U(req, 20);
    H(s, 0xf6) = (flags & 1) ? 1 : 0;
    H(s, 0xfa) = (flags & 2) ? 1 : 0;
    H(s, 0xfc) = (flags & 4) ? 1 : 0;
    H(s, 0xfe) = (flags & 8) ? 1 : 0;
    WMSP_CopyParentParam(param, (u8 *)s + 0xe8);
    r = WMSP_WL_ParamSetGameInfo(buf, (u16)(H(s, 0xec) + 16), param);
    c = __VENEER_WMSP_GetBuffer4Callback2Wm9();
    c[0] = 24;
    if (r[2] == 0) {
        c[1] = 0;
    } else {
        c[1] = 1;
    }
    if (r[2]) {
        c[2] = 0x245;
        c[3] = r[2];
    }
    __VENEER_WMSP_ReturnResult2Wm9(c);
}
