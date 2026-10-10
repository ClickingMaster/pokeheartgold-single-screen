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
extern void __VENEER_MI_CpuCopy8(const void *, void *, u32);
extern void *WMSP_WL_ParamSetMaxConnectableChild(void *, u16);
void WMSP_SetParentParam(u32 *r);

void WMSP_SetParentParam(u32 *r) {
    u16 buf[256];
    u8 *s = P(wmspW + 0x1000, 0x550);
    u16 *p, err;
    __VENEER_MI_CpuCopy8((void *)r[1], s + 0xe8, 64);
    u16 channel = H(s, 0x11a);
    int mask = 1 << channel;
    if (!(mask & H(s, 0x1f4))) {
        p = __VENEER_WMSP_GetBuffer4Callback2Wm9();
        p[0] = 7;
        p[1] = 6;
        __VENEER_WMSP_ReturnResult2Wm9(p);
    } else {
        err = ((u16 *)WMSP_WL_ParamSetMaxConnectableChild(buf, H(s, 0xf8)))[2];
        if (err) {
            p = __VENEER_WMSP_GetBuffer4Callback2Wm9();
            p[0] = 7;
            p[1] = 1;
            p[2] = 0x212;
            p[3] = err;
            __VENEER_WMSP_ReturnResult2Wm9(p);
        } else {
            p = __VENEER_WMSP_GetBuffer4Callback2Wm9();
            p[0] = 7;
            p[1] = 0;
            __VENEER_WMSP_ReturnResult2Wm9(p);
        }
    }
}
