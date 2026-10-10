#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#define ROOT    (*(u8 **)0x0380fff4)
#define H(p, o) (*(u16 *)((u8 *)(p) + (o)))
#define W(p, o) (*(u32 *)((u8 *)(p) + (o)))
#define V(o)    (*(volatile u16 *)(o))
extern void __VENEER_MIi_CpuCopy16(const void *, void *, u32);
extern void MIi_CpuCopy16(const void *, void *, u32);
extern u32 __VENEER_OS_DisableIrqMask(u32);
extern u32 __VENEER_OS_EnableIrqMask(u32);
extern void WaitMacStop(void);
extern void CopyTxFrmToMacBuf(void *, void *);
extern void RestoreTxFrame(void *);
extern void MakeBeaconFrame(void);
extern void MakePsPollFrame(u16);
extern void AddTask(u32, u32);
extern int WCheckTxBufIdBeforeFrame(void *);
extern void *AllocateHeapBuf(void *, u32);
extern void SendMessageToWmDirect(void *, void *);
extern void WStop(void);
extern void WShutdown(void);
extern void IssueMlmeConfirm(void *);
extern void ReleaseIntr(void);
extern void ReleaseHeapBuf(void *, void *);
extern void __VENEER_OS_ExitThread(void);
extern u32 DeleteTask(u32);
extern const u16 crc16_table[];
extern const u16 test_pattern[];

extern const struct W35500Reg {
    u16 offset, mask;
} test_reg[];

extern void WlIntrStartTx(void);
extern void WlIntrStartRx(void);
extern void WlIntrPreTbtt(void);
extern void WlIntrTbtt(void);
extern void WlIntrActEnd(void);
extern void WlIntrRfWakeup(void);
extern void WlIntrTxErr(void);
extern void WlIntrRxCntup(void);
extern void WlIntrRxEnd(void);
extern void WlIntrCntOvf(void);
extern void WlIntrTxEnd(void);
extern void WlIntrMpEnd(int);
void SendFatalErrMsgTask(void);

void SendFatalErrMsgTask(void) {
    u8 *p = ROOT, *state = p + 0x344, *msg;
    u32 mask;
    if (!H(state, 0xb0)) {
        return;
    }
    msg = AllocateHeapBuf(p + 0x188, 18);
    if (!msg) {
        return;
    }
    H(msg, 12) = 390;
    H(msg, 14) = 1;
    mask = __VENEER_OS_DisableIrqMask(0x1000000);
    H(msg, 16) = H(state, 0xb0);
    H(state, 0xb0) = 0;
    __VENEER_OS_EnableIrqMask(mask);
    SendMessageToWmDirect(ROOT + 0x188, msg);
}
