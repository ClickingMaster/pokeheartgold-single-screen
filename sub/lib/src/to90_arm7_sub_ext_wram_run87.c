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
} To90W35383Global;

#define G       ((To90W35383Global *)(wmspW + 0x1000))
#define H(p, o) (*(u16 *)((u8 *)(p) + (o)))
#define W(p, o) (*(u32 *)((u8 *)(p) + (o)))
extern void __VENEER_PM_SetLEDPattern(u32);
extern int WMSPi_CommonWlIdle(u16 *, u16 *);
void WMSP_PowerOn(void *);

void WMSP_PowerOn(void *arg) {
    u16 code, err;
    u8 *s = G->state;
    u16 *out;
    if (H(s, 0) != 1) {
        out = __VENEER_WMSP_GetBuffer4Callback2Wm9();
        out[0] = 5;
        out[1] = 3;
        __VENEER_WMSP_ReturnResult2Wm9(out);
        return;
    }
    if (!WMSPi_CommonWlIdle(&code, &err)) {
        out = __VENEER_WMSP_GetBuffer4Callback2Wm9();
        out[0] = 5;
        out[1] = 1;
        out[2] = code;
        out[3] = err;
        __VENEER_WMSP_ReturnResult2Wm9(out);
        return;
    }
    H(s, 0) = 2;
    out = __VENEER_WMSP_GetBuffer4Callback2Wm9();
    out[0] = 5;
    out[1] = 0;
    __VENEER_WMSP_ReturnResult2Wm9(out);
}
