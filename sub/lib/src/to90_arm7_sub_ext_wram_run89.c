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
} To90W35385Global;

#define G       ((To90W35385Global *)(wmspW + 0x1000))
#define H(p, o) (*(u16 *)((u8 *)(p) + (o)))
#define W(p, o) (*(u32 *)((u8 *)(p) + (o)))
extern void __VENEER_PM_SetLEDPattern(u32);
extern u16 *WMSP_WL_DevShutdown(void *);
void WMSP_PowerOff(void *);

void WMSP_PowerOff(void *arg) {
    u32 buf[128];
    u8 *s = G->state;
    u16 *out, *resp;
    if (H(s, 0) != 2) {
        out = __VENEER_WMSP_GetBuffer4Callback2Wm9();
        out[0] = 6;
        out[1] = 3;
        __VENEER_WMSP_ReturnResult2Wm9(out);
        return;
    }
    resp = WMSP_WL_DevShutdown(buf);
    if (resp[2]) {
        out = __VENEER_WMSP_GetBuffer4Callback2Wm9();
        out[0] = 6;
        out[1] = 1;
        out[2] = 0x301;
        out[3] = resp[2];
        __VENEER_WMSP_ReturnResult2Wm9(out);
        return;
    }
    H(s, 0) = 1;
    out = __VENEER_WMSP_GetBuffer4Callback2Wm9();
    out[0] = 6;
    out[1] = 0;
    __VENEER_WMSP_ReturnResult2Wm9(out);
}
