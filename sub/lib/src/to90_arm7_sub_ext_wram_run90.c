#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
extern u16 *__VENEER_WMSP_GetBuffer4Callback2Wm9(void);
extern void __VENEER_WMSP_ReturnResult2Wm9(void *);
extern u32 WMSP_SetMPParameterCore(void *, void *);
extern void __VENEER_MI_CpuCopy8(const void *, void *, u32);
void WMSP_SetMPParameter(u32 *arg);

void WMSP_SetMPParameter(u32 *arg) {
    u32 old[7];
    u32 err = WMSP_SetMPParameterCore(arg + 1, old);
    u16 *out = __VENEER_WMSP_GetBuffer4Callback2Wm9();
    out[0] = 0x23;
    out[1] = err;
    *(u32 *)(out + 2) = arg[1];
    __VENEER_MI_CpuCopy8(old, out + 4, 0x1c);
    __VENEER_WMSP_ReturnResult2Wm9(out);
}
