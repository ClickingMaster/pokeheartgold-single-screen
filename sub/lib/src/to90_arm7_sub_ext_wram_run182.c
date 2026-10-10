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

extern const struct W35498Reg {
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
void WCheckTxBuf(void);

void WCheckTxBuf(void) {
    u8 *p = ROOT, *state = p + 0x344, *desc = p + 0x42c;
    switch (H(p, 0x350)) {
    case 1:
        if (WCheckTxBufIdBeforeFrame(desc + 0x78)) {
            WaitMacStop();
            MakeBeaconFrame();
        }
        if (WCheckTxBufIdBeforeFrame(desc + 0x28)) {
            RestoreTxFrame(desc + 0x28);
        }
        break;
    case 2:
        WCheckTxBufIdBeforeFrame(desc + 0x64);
        if (WCheckTxBufIdBeforeFrame(desc + 0x28)) {
            if (H(desc, 0x28)) {
                WaitMacStop();
            }
            MakePsPollFrame(H(state, 0x6a));
            H(state, 0xb8)
            ++;
        }
        break;
    case 3:
        break;
    }
    if (WCheckTxBufIdBeforeFrame(desc + 0x14)) {
        RestoreTxFrame(desc + 0x14);
    }
    if (WCheckTxBufIdBeforeFrame(desc)) {
        RestoreTxFrame(desc);
    }
    if (!V(0x04808004)) {
        V(0x04808004) = 1;
    }
}
