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
extern void WMSP_WlRequest(void *);
u16 *WMSPi_WL_NoArg(u16 *p, u16 cmd, u16 size);
#pragma opt_propagation off

u16 *WMSPi_WL_NoArg(u16 *p, u16 cmd, u16 size) {
    u32 i = 0;
    p[0] = 0;
    p[1] = 0;
    p[2] = 0;
    p[3] = 0;
    p[4] = 0;
    p[5] = 0;
    p[6] = cmd;
    p[7] = 0;
    u16 *out = p + i;
    out[8] = p[6];
    out[9] = size;
    WMSP_WlRequest(p);
    return out + 8;
}
