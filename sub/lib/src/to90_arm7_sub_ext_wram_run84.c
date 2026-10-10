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
} To90W35379Global;

#define G       ((To90W35379Global *)(wmspW + 0x1000))
#define H(p, o) (*(u16 *)((u8 *)(p) + (o)))
#define W(p, o) (*(u32 *)((u8 *)(p) + (o)))
extern void __VENEER_PM_SetLEDPattern(u32);
extern void __VENEER_MI_CpuCopy8(const void *, void *, u32);
extern u16 *WMSP_WL_MlmeDeAuthenticate(void *, void *, u16);
void WMSP_AutoDeAuth(void *);

void WMSP_AutoDeAuth(void *arg) {
    u8 addr[8];
    u32 buf[128];
    int i;
    u16 *resp, *out;
    __VENEER_MI_CpuCopy8((u8 *)arg + 4, addr, 6);
    for (i = 0; i < 2; i++) {
        resp = WMSP_WL_MlmeDeAuthenticate(buf, addr, 19);
        if (resp[2] == 0 || (resp[2] != 7 && resp[2] != 12)) {
            break;
        }
    }
    out = __VENEER_WMSP_GetBuffer4Callback2Wm9();
    out[0] = 0x22;
    if (!resp[2]) {
        out[1] = 0;
    } else {
        out[1] = 1;
        out[2] = 5;
        out[3] = resp[2];
    }
    __VENEER_WMSP_ReturnResult2Wm9(out);
}
