#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
void *WMSP_WlRequest(void *);
void __VENEER_MIi_CpuCopy16(const void *, void *, u32);
u16 *WMSP_WL_MlmeStart(void *, u16, void *, u16, u16, u16, u16, u16, u16, void *);
u16 *WMSP_WL_MlmeMeasureChannel(void *, u16, u16, u16, void *);

static inline void InitializeStartHeader(u16 *p, u16 n) {
    p[0] = 0;
    p[1] = 0;
    p[2] = 0;
    p[3] = 0;
    p[4] = 0;
    p[5] = 0;
    p[6] = 9;
    p[7] = (n + 1) / 2 + 23;
}

static inline void InitializeMeasureHeader(u16 *p) {
    p[0] = 0;
    p[1] = 0;
    p[2] = 0;
    p[3] = 0;
    p[4] = 0;
    p[5] = 0;
    p[6] = 10;
    p[7] = 12;
}

u16 *WMSP_WL_MlmeStart(void *buffer, u16 a, void *ssid, u16 channel, u16 beacon, u16 dtim, u16 rate, u16 flags, u16 length, void *data) {
    u16 *p = buffer, *reply;
    InitializeStartHeader(p, length);
    p[8] = a;
    __VENEER_MIi_CpuCopy16(ssid, p + 9, 32);
    p[25] = channel;
    p[26] = beacon;
    p[27] = dtim;
    p[28] = rate;
    p[29] = flags;
    p[30] = length;
    __VENEER_MIi_CpuCopy16(data, p + 31, length);
    reply = p + p[7] + 8;
    reply[0] = p[6];
    reply[1] = 1;
    WMSP_WlRequest(buffer);
    return reply;
}

u16 *WMSP_WL_MlmeMeasureChannel(void *buffer, u16 a, u16 b, u16 duration, void *channels) {
    u16 *p = buffer, *reply;
    InitializeMeasureHeader(p);
    p[8] = 0;
    p[9] = a;
    p[10] = b;
    p[11] = duration;
    __VENEER_MIi_CpuCopy16(channels, p + 12, 16);
    reply = p + p[7] + 8;
    reply[0] = p[6];
    reply[1] = 18;
    WMSP_WlRequest(buffer);
    return reply;
}
