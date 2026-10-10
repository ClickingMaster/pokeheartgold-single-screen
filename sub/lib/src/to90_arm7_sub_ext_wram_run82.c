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
} To90W35373Global;

#define G       ((To90W35373Global *)(wmspW + 0x1000))
#define H(p, o) (*(u16 *)((u8 *)(p) + (o)))
#define W(p, o) (*(u32 *)((u8 *)(p) + (o)))
extern void __VENEER_PM_SetLEDPattern(u32);
extern u32 __VENEER_OS_DisableInterrupts(void);
extern void __VENEER_OS_RestoreInterrupts(u32);
extern u32 __VENEER_MATH_CountPopulation(u32);
extern void __VENEER_OS_Sleep(u32);
extern void WMSP_RequestResumeMP(void);
extern u16 WmspGetTmptt(u32, u32, u32, u32);
extern void *WMSP_WL_MaMp(void *, u32, u32, u32, u16, u16, u16, u16, u16, u32);
void WMSP_ResumeMaMP(u32);

void WMSP_ResumeMaMP(u32 input) {
    u32 buf[128];
    u32 mask = input;
    u16 current;
    u32 time;
    u8 *s = G->state;
    u16 first, tmptt;
    __VENEER_OS_RestoreInterrupts(__VENEER_OS_DisableInterrupts());
    mask &= H(s, 0x182);
    first = H(s, 0x3a);
    current = *(volatile u16 *)0x48080f8;
    time = H(s, 0x6a);
    if ((s32)H(s, 0x72) < (s32)(((first + 12) * __VENEER_MATH_CountPopulation(mask) + 0x29) & ~31U)) {
        __VENEER_OS_Sleep(2);
        WMSP_RequestResumeMP();
        return;
    }
    if (*(s16 *)(s + 0x62) != 1 && *(s16 *)(s + 0x64) != 1) {
        time &= ~0x8000;
        tmptt = 0;
    } else {
        tmptt = WmspGetTmptt(H(s, 0x6e), H(s, 0x6c), mask, H(s, 0x40));
        time |= 0x8000;
    }
    WMSP_WL_MaMp(buf, 0x800c, 0, 0, (u16)mask, tmptt, current, 0, time, 0);
}
