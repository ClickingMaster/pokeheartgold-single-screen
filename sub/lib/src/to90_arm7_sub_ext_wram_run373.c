#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#define H(p, o) (*(u16 *)((u8 *)(p) + (o)))
#define U(p, o) (*(u32 *)((u8 *)(p) + (o)))
#define P(p, o) (*(void **)((u8 *)(p) + (o)))
extern u8 wmspW[];
#define STATE P(wmspW + 0x1000, 0x550)
extern u16 *__VENEER_WMSP_GetBuffer4Callback2Wm9(void);
extern void __VENEER_WMSP_ReturnResult2Wm9(void *);
extern void __VENEER_MI_CpuCopy8(const void *, void *, u32);
extern void __VENEER_MIi_CpuCopy16(const void *, void *, u32);
extern void __VENEER_MI_CpuFill8(void *, u32, u32);
extern void __VENEER_MIi_CpuClear16(u32, void *, u32);
extern u16 *WMSP_WL_DevGetStationState(void *);
extern u16 *WMSP_WL_DevClass1(void *);
extern u16 *WMSP_WL_MlmePowerManagement(void *, u16, u16, u16);
extern u16 *WMSP_WL_MlmeScan(void *, u32, void *, u16, void *, u16, void *, u16);
extern int WMSP_SetAllParams(u16, void *);
extern void WmspError_06003ED4(u16, u16, int);
extern u32 WMSP_GetRssi8_06003988(u32);
extern u32 WMSP_GetLinkLevel(u32);
extern void WMSP_AddRssiToRandomPool(u32);
extern u16 *WMSP_WL_ParamSetPreambleType(void *, u16);
extern u16 *WMSP_WL_ParamSetSsidMask(void *, void *);
void WMSP_StartScan(void *req);

// WM_StartScan validates the channel range 1..14 before sending command 10.
void WMSP_StartScan(void *req) {
    u32 buffer[128];
    u8 ssid[32];
    u8 channels[16];
    u16 bssid[3];
    u16 *buf = (u16 *)buffer;
    u16 channel, duration;
    void *s = STATE;
    u16 *r, *c;
    u32 rssi;
    if (H(s, 0) != 2 && H(s, 0) != 3 && H(s, 0) != 5) {
        c = __VENEER_WMSP_GetBuffer4Callback2Wm9();
        c[0] = 10;
        c[1] = 3;
        c[4] = 4;
        __VENEER_WMSP_ReturnResult2Wm9(c);
        return;
    }
    P(s, 0x184) = P(req, 4);
    channel = H(req, 2);
    H(s, 0x190) = channel;
    duration = H(req, 8);
    __VENEER_MI_CpuCopy8((u8 *)req + 10, bssid, 6);
    if (bssid[0] != 65535 && (bssid[0] & 1)) {
        bssid[0] &= ~1;
    }
    if (channel == 0) {
        c = __VENEER_WMSP_GetBuffer4Callback2Wm9();
        c[0] = 10;
        c[1] = 6;
        c[4] = 4;
        __VENEER_WMSP_ReturnResult2Wm9(c);
        return;
    }
    if (!((1u << channel) & H(s, 0x1f4))) {
        c = __VENEER_WMSP_GetBuffer4Callback2Wm9();
        c[0] = 10;
        c[1] = 6;
        c[4] = 4;
        __VENEER_WMSP_ReturnResult2Wm9(c);
        return;
    }
    H(s, 0xe6) = 2;
    r = WMSP_WL_DevGetStationState(buf);
    if (r[2]) {
        WmspError_06003ED4(0x308, r[2], 0);
        return;
    }
    if (r[3] == 16) {
        if (!WMSP_SetAllParams(10, buf)) {
            return;
        }
        r = WMSP_WL_DevClass1(buf);
        if (r[2]) {
            WmspError_06003ED4(0x303, r[2], 0);
            return;
        }
        H(s, 0) = 3;
        r = WMSP_WL_MlmePowerManagement(buf, 1, 0, 1);
        if (r[2]) {
            WmspError_06003ED4(1, r[2], 0);
            return;
        }
        H(s, 0xc6) = 1;
    }
    H(s, 0) = 5;
    __VENEER_MIi_CpuClear16(65535, ssid, 32);
    channels[0] = channel;
    __VENEER_MI_CpuFill8(channels + 1, 0, 15);
    r = WMSP_WL_MlmeScan(buf, 0x11e, bssid, 0, ssid, 1, channels, duration);
    if (r[2]) {
        WmspError_06003ED4(2, r[2], 0);
        return;
    }
    c = __VENEER_WMSP_GetBuffer4Callback2Wm9();
    if (r[4] == 0) {
        c[0] = 10;
        c[1] = 0;
        c[4] = 4;
        c[8] = channel;
        c[9] = 0;
    } else {
        __VENEER_MIi_CpuClear16(0, (u8 *)P(s, 0x184) + 64, 128);
        __VENEER_MI_CpuCopy8(r + 5, P(s, 0x184), r[5] * 2);
        c[0] = 10;
        c[1] = 0;
        c[4] = 5;
        c[8] = H(r, 0x40);
        rssi = WMSP_GetRssi8_06003988(H(r, 12) & 255);
        c[9] = WMSP_GetLinkLevel(rssi);
        WMSP_AddRssiToRandomPool(rssi);
        c[10] = H(r, 0x14);
        __VENEER_MI_CpuCopy8((u8 *)r + 14, (u8 *)c + 10, 6);
        __VENEER_MIi_CpuCopy16((u8 *)r + 22, (u8 *)c + 22, 32);
        c[27] = H(r, 0x46);
        if (c[27] > 128) {
            c[0] = 10;
            c[1] = 0;
            c[4] = 4;
            c[8] = channel;
            c[9] = 0;
        } else {
            __VENEER_MIi_CpuClear16(0, (u8 *)c + 56, 128);
            __VENEER_MIi_CpuCopy16((u8 *)r + 74, (u8 *)c + 56, (c[27] + 1) & ~1u);
        }
    }
    __VENEER_WMSP_ReturnResult2Wm9(c);
}
