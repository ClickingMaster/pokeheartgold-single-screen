#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#define H(p, o) (*(u16 *)((u8 *)(p) + (o)))
#define W(p, o) (*(u32 *)((u8 *)(p) + (o)))
#define P(p, o) (*(void **)((u8 *)(p) + (o)))
extern u8 wmspW[];
extern u16 *__VENEER_WMSP_GetBuffer4Callback2Wm9(void);
extern void __VENEER_WMSP_ReturnResult2Wm9(void *);
extern u32 __VENEER_OS_DisableInterrupts(void);
extern void __VENEER_OS_RestoreInterrupts(u32);
extern void __VENEER_MI_CpuCopy8(const void *, void *, u32);
extern void __VENEER_MI_CpuFill8(void *, int, u32);
extern void __VENEER_MIi_CpuClear16(int, void *, u32);

void WmspError_06002FC0(u16 a, u16 b);

void WmspError_06002FC0(u16 a, u16 b) {
    u16 *p = __VENEER_WMSP_GetBuffer4Callback2Wm9();
    p[0] = 1;
    p[1] = 1;
    p[2] = a;
    p[3] = b;
    __VENEER_WMSP_ReturnResult2Wm9(p);
}
