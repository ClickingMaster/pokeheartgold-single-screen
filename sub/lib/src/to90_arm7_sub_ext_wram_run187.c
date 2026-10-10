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

extern const struct W35505Reg {
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
void WlIntr(void);

void WlIntr(void) {
    u32 pending;
    for (;;) {
        pending = V(0x04808010) & V(0x04808012);
        if (!pending) {
            break;
        }
        if (pending & 128) {
            WlIntrStartTx();
        }
        if (pending & 64) {
            WlIntrStartRx();
        }
        if (pending & 0x8000) {
            WlIntrPreTbtt();
        }
        if (pending & 0x4000) {
            WlIntrTbtt();
        }
        if (pending & 0x2000) {
            WlIntrActEnd();
        }
        if (pending & 0x800) {
            WlIntrRfWakeup();
        }
        if (pending & 8) {
            WlIntrTxErr();
        }
        if (pending & 4) {
            WlIntrRxCntup();
        }
        if (pending & 1) {
            WlIntrRxEnd();
        }
        if (pending & 0x30) {
            WlIntrCntOvf();
        }
        if (pending & 2) {
            WlIntrTxEnd();
        }
        if (pending & 0x1000) {
            WlIntrMpEnd(1);
        }
    }
    *(volatile u32 *)0x0380fff8 |= 0x1000000;
}
