#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
extern u16 *__VENEER_WMSP_GetBuffer4Callback2Wm9(void);
extern void __VENEER_WMSP_ReturnResult2Wm9(void *);
extern u8 wmspW[];

typedef struct {
    u8 pad[0x54c];
    u8 *iface;
    u8 *state;
} To90W35381Global;

#define G       ((To90W35381Global *)(wmspW + 0x1000))
#define H(p, o) (*(u16 *)((u8 *)(p) + (o)))
#define W(p, o) (*(u32 *)((u8 *)(p) + (o)))
extern void __VENEER_PM_SetLEDPattern(u32);
extern u32 __VENEER_OS_DisableInterrupts(void);
extern void __VENEER_OS_RestoreInterrupts(u32);
extern void WMSP_CancelVAlarm(void);
extern void WMSP_SetThreadPriorityLow(void);
extern void WMSP_ResetSizeVars(void);
extern void WMSP_InitAlarm(void);
extern void WMSP_InitVAlarm(void);
extern void WMSP_CleanSendQueue(u32);
extern void __VENEER_MI_CpuFill8(void *, u32, u32);
extern void __VENEER_MIi_CpuClear16(u32, void *, u32);
extern void __VENEER_OS_InitMutex(void *);
void WMSPi_CommonInit(u32);

void WMSPi_CommonInit(u32 flags) {
    u8 *iface;
    int clean = 0, i;
    u32 irq;
    u8 *s = G->state;
    iface = G->iface;
    irq = __VENEER_OS_DisableInterrupts();
    if (W(s, 12) == 1) {
        W(s, 12) = 0;
        clean = 1;
        WMSP_CancelVAlarm();
        WMSP_SetThreadPriorityLow();
    }
    H(s, 0x182) = 0;
    H(s, 0x86) = 0;
    W(s, 0x14) = 0;
    W(s, 0x10) = 0;
    W(s, 0x1c) = 0;
    H(s, 0xce) = 0;
    H(s, 0xc2) = 0;
    H(s, 0x58) = 1;
    H(s, 0x5a) = 1;
    H(s, 0x5c) = 6;
    H(s, 0x98) = 0;
    H(s, 0x92) = 0;
    H(s, 0x94) = 0;
    H(s, 0x9a) = 0;
    H(s, 0x9c) = 0;
    H(s, 0x7c0) = H(s, 0x58);
    H(s, 0x7c2) = H(s, 0x5a);
    H(s, 0x7c4) = H(s, 0x5c);
    H(s, 0x7ca) = H(s, 0x98);
    H(s, 0x7c6) = H(s, 0x92);
    H(s, 0x7c8) = H(s, 0x94);
    H(s, 0x7cc) = H(s, 0x9a);
    W(s, 0x198) = 0;
    H(s, 0x196) = 0;
    __VENEER_MI_CpuFill8(s + 0x19c, 0, 0x50);
    WMSP_ResetSizeVars();
    H(s, 0x40) = 0x104;
    H(s, 0x42) = 0xf0;
    H(s, 0x44) = 1000;
    H(s, 0x46) = 0;
    W(s, 0x48) = 0x20b;
    W(s, 0x4c) = 0;
    W(s, 0x50) = 0;
    W(s, 0x54) = 0;
    H(s, 0xc6) = 0;
    H(s, 0x1ee) = 1;
    W(s, 0xc8) = flags;
    __VENEER_OS_RestoreInterrupts(irq);
    if (clean) {
        WMSP_CleanSendQueue(0xffff);
    }
    for (i = 0; i < 32; i++) {
        u8 *entry = iface + i * 16;
        W(entry, 0xd0) = 0x8000;
    }
    __VENEER_MIi_CpuClear16(1, s + 0x1f8, 0x100);
    WMSP_InitAlarm();
    __VENEER_OS_InitMutex(s + 0x71c);
    WMSP_InitVAlarm();
    if (!(flags & 2)) {
        __VENEER_PM_SetLEDPattern(15);
    }
    H(s, 0) = 1;
}
