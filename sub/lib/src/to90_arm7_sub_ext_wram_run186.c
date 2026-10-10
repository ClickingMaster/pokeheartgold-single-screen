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

extern const struct W35504Reg {
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
void DiagMacMemory(void);

void DiagMacMemory(void) {
    volatile u16 *p = (volatile u16 *)0x04804000;
    u32 errors = 0, i;
    u16 pattern = 0xffff;
    for (i = 0; i < 0x2000; i += 2) {
        *p++ = pattern--;
    }
    p = (volatile u16 *)0x04804000;
    pattern = 0xffff;
    for (i = 0; i < 0x2000; i += 2, p++, pattern--) {
        if (*p != pattern && errors++ > 32) {
            goto done;
        }
    }
    p = (volatile u16 *)0x04804000;
    pattern = 0x5a5a;
    for (i = 0; i < 0x2000; i += 2) {
        *p++ = pattern;
        pattern = ~pattern;
    }
    p = (volatile u16 *)0x04804000;
    pattern = 0x5a5a;
    for (i = 0; i < 0x2000; i += 2, p++, pattern = (u16)~pattern) {
        if (*p != pattern && errors++ > 32) {
            goto done;
        }
    }
    p = (volatile u16 *)0x04804000;
    pattern = 0xa5a5;
    for (i = 0; i < 0x2000; i += 2) {
        *p++ = pattern;
        pattern = ~pattern;
    }
    p = (volatile u16 *)0x04804000;
    pattern = 0xa5a5;
    for (i = 0; i < 0x2000; i += 2, p++, pattern = (u16)~pattern) {
        if (*p != pattern && errors++ > 32) {
            goto done;
        }
    }
done:
    if (errors) {
        H(ROOT, 0x33e) |= 2;
    }
}
