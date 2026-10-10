#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
void *WMSP_WlRequest(void *);
void __VENEER_MIi_CpuCopy16(const void *, void *, u32);
u16 *WMSP_WL_ParamSetGameInfo(void *, u16, void *);

static inline void InitializeHeader(u16 *p, u16 length) {
    p[0] = 0;
    p[1] = 0;
    p[2] = 0;
    p[3] = 0;
    p[4] = 0;
    p[5] = 0;
    p[6] = 581;
    p[7] = (length + 1) / 2 + 1;
}

u16 *WMSP_WL_ParamSetGameInfo(void *buffer, u16 length, void *source) {
    u16 *p = buffer, *reply;
    InitializeHeader(p, length);
    p[8] = length;
    __VENEER_MIi_CpuCopy16(source, p + 9, length);
    reply = p + p[7] + 8;
    reply[0] = p[6];
    reply[1] = 1;
    WMSP_WlRequest(buffer);
    return reply;
}
