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
extern u16 *WMSP_WL_DevShutdown(void *);
extern void __VENEER_PM_SetLEDPattern(int);
void WMSP_End(void *r);

void WMSP_End(void *r) {
    u8 buf[512];
    u8 *s = P(wmspW + 0x1000, 0x550);
    u16 *p, err;
    if (H(s, 0) != 2) {
        p = __VENEER_WMSP_GetBuffer4Callback2Wm9();
        p[0] = 2;
        p[1] = 3;
        __VENEER_WMSP_ReturnResult2Wm9(p);
    } else {
        err = WMSP_WL_DevShutdown(buf)[2];
        if (err) {
            p = __VENEER_WMSP_GetBuffer4Callback2Wm9();
            p[0] = 2;
            p[1] = 1;
            p[2] = 0x301;
            p[3] = err;
            __VENEER_WMSP_ReturnResult2Wm9(p);
        } else {
            H(s, 0) = 1;
            __VENEER_PM_SetLEDPattern(1);
            H(s, 0) = 0;
            p = __VENEER_WMSP_GetBuffer4Callback2Wm9();
            p[0] = 2;
            p[1] = 0;
            __VENEER_WMSP_ReturnResult2Wm9(p);
        }
    }
}
