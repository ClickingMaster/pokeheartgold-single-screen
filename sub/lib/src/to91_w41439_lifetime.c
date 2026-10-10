#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
void *WMSP_WlRequest(void *);
u16 *WMSP_WL_ParamSetLifeTime(void *, u16, u16, u16);

static inline void InitializeHeader(u16 *packet) {
    packet[0] = 0;
    packet[1] = 0;
    packet[2] = 0;
    packet[3] = 0;
    packet[4] = 0;
    packet[5] = 0;
    packet[6] = 529;
    packet[7] = 3;
}

u16 *WMSP_WL_ParamSetLifeTime(void *buffer, u16 first, u16 second, u16 third) {
    u16 *packet = buffer, *reply;
    InitializeHeader(packet);
    packet[8] = first;
    packet[9] = second;
    packet[10] = third;
    reply = packet + packet[7] + 8;
    reply[0] = packet[6];
    reply[1] = 1;
    WMSP_WlRequest(buffer);
    return reply;
}
