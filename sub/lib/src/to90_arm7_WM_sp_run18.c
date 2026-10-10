#include <nitro/mi/memory.h>
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#pragma force_active on
extern u8 wmspW[];
extern u8 wmspIndicateThread[];
extern u8 wmspRequestThread[];
#define W       ((u8 *)(wmspW + 0x1000))
#define P       (*(u8 **)(W + 0x550))
#define H(p, o) (*(u16 *)((u8 *)(p) + (o)))
#define U(p, o) (*(u32 *)((u8 *)(p) + (o)))
extern u16 *__VENEER_WMSP_GetBuffer4Callback2Wm9(void);
extern void __VENEER_WMSP_ReturnResult2Wm9(void *);
extern void __VENEER_MI_CpuCopy8(const void *, void *, u32);
extern void __VENEER_MIi_CpuClear16(u16, u16 *, u32);

typedef struct W38854Params {
    u8 prefix[0x10];
    u16 mac[3], retry, channels, pad, mode, role, gameLength, beacon;
    u8 payload[80];
    u16 life, power, delay, active;
    u8 key[32];
    u16 preamble, enabled;
} W38854Params;

BOOL WMSP_SetAllParams(u32, u8 *);

extern void *WMSP_WL_ParamSetAll(void *buf);

BOOL WMSP_SetAllParams(u32 command, u8 *d) {
    u8 *p = P;
    u16 error;
    W38854Params *params = (W38854Params *)d;
    __VENEER_MI_CpuCopy8(p + 0xe0, d + 0x10, 6);
    H(d, 0x16) = 7;
    H(d, 0x18) = H(p, 0x1f4);
    H(d, 0x1e) = H(p, 0x1ec);
    H(d, 0x1c) = H(p, 0xe6);
    if (!U(p, 0x198)) {
        params->gameLength = 0;
        params->beacon = 0;
        __VENEER_MIi_CpuClear16(0, (u16 *)params->payload, 0x50);
        params->enabled = 0;
    } else {
        params->gameLength = H(p, 0x196);
        params->beacon = H(p, 0xc4);
        __VENEER_MI_CpuCopy8(p + 0x19c, params->payload, 0x50);
        params->enabled = 1;
    }
    H(d, 0x74) = 1;
    H(d, 0x76) = 1;
    H(d, 0x78) = H(p, 0xe6) == 1 ? 0 : 16;
    H(d, 0x7a) = 10;
    if (command == 0x26) {
        __VENEER_MIi_CpuClear16(0, (u16 *)(d + 0x7c), 32);
    } else {
        __VENEER_MIi_CpuClear16(0, (u16 *)(d + 0x7c), 8);
        __VENEER_MIi_CpuClear16(0xffff, (u16 *)(d + 0x84), 24);
    }
    H(d, 0x9c) = H(p, 0x1ee);
    error = ((u16 *)WMSP_WL_ParamSetAll(d))[2];
    if (!error) {
        return 1;
    }
    {
        u16 *r = __VENEER_WMSP_GetBuffer4Callback2Wm9();
        r[0] = command;
        r[1] = 1;
        r[2] = 0x200;
        r[3] = error;
        __VENEER_WMSP_ReturnResult2Wm9(r);
    }
    return 0;
}
