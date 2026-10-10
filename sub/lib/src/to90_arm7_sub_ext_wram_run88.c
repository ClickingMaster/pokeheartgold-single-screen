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
} To90W35384Global;

#define G       ((To90W35384Global *)(wmspW + 0x1000))
#define H(p, o) (*(u16 *)((u8 *)(p) + (o)))
#define W(p, o) (*(u32 *)((u8 *)(p) + (o)))
extern void __VENEER_PM_SetLEDPattern(u32);
extern u16 *WMSP_WL_DevRestart(void *);
extern u16 *WMSP_WL_DevIdle(void *);
extern u16 *WMSP_WL_ParamGetEnableChannel(void *);
extern u16 *WMSP_WL_DevGetVersion(void *);
extern u16 *WMSP_WL_ParamGetMacAddress(void *);
extern u16 *WMSP_WL_ParamSetLifeTime(void *, u16, u16, u16);
extern u16 *WMSP_WL_ParamSetBeaconSendRecvInd(void *, u16);
extern u16 __VENEER_WMSP_GetAllowedChannel(u16);
extern void __VENEER_MIi_CpuCopy16(const void *, void *, u32);
extern void __VENEER_MI_CpuCopy8(const void *, void *, u32);
int WMSPi_CommonWlIdle(u16 *, u16 *);

int WMSPi_CommonWlIdle(u16 *code, u16 *error) {
    u32 buf[128];
    u8 *s = G->state;
    u16 *r;
    r = WMSP_WL_DevRestart(buf);
    if (r[2]) {
        *code = 0x304;
        *error = r[2];
        return 0;
    }
    r = WMSP_WL_DevIdle(buf);
    if (r[2]) {
        *code = 0x302;
        *error = r[2];
        return 0;
    }
    *(volatile u16 *)0x4808124 = 200;
    *(volatile u16 *)0x4808128 = 2000;
    *(volatile u16 *)0x4808150 = 0x202;
    r = WMSP_WL_ParamGetEnableChannel(buf);
    if (r[2]) {
        *code = 0x283;
        *error = r[2];
        return 0;
    }
    {
        u16 channels = r[3];
        H(s, 0x1f4) = channels;
        H(s, 0x1f6) = __VENEER_WMSP_GetAllowedChannel((u16)(channels >> 1));
    }
    WMSP_WL_ParamSetLifeTime(buf, 0xffff, 40, 5);
    W(s, 0x7b8) = 0x1ff621;
    W(s, 0x7bc) = 0;
    H(s, 0x1ec) = 2;
    H(s, 0x1ee) = 1;
    r = WMSP_WL_DevGetVersion(buf);
    if (r[2]) {
        *code = 0x306;
        *error = r[2];
        return 0;
    }
    __VENEER_MIi_CpuCopy16(r + 3, s + 0x20, 8);
    H(s, 0x28) = r[7];
    H(s, 0x2c) = r[8];
    H(s, 0x2e) = r[9];
    H(s, 0x2a) = r[10];
    r = WMSP_WL_ParamGetMacAddress(buf);
    if (r[2]) {
        *code = 0x281;
        *error = r[2];
        return 0;
    }
    __VENEER_MI_CpuCopy8(r + 3, s + 0xe0, 6);
    r = WMSP_WL_ParamSetBeaconSendRecvInd(buf, 1);
    if (r[2]) {
        *code = 0x215;
        *error = r[2];
        return 0;
    }
    return 1;
}
