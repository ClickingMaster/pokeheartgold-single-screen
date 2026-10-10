#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
void *WMSP_WlRequest(void *);

typedef union {
    u16 words[256];

    struct {
        u16 headerAndArgs[10];
        u32 encodedData;
    } key;

    struct {
        u16 headerAndArgs[16];
        u32 encodedData;
    } mp;
} Packet09;

u16 *WMSP_WL_MaKeyData(void *, u16, u16, void *);
void *WMSP_WL_MaMp(void *, u32, u32, u32, u16, u16, u16, u16, u16, u32);

static inline void InitializeKeyHeader(u16 *p) {
    p[0] = 0;
    p[1] = 0;
    p[2] = 0;
    p[3] = 0;
    p[4] = 0;
    p[5] = 0;
    p[6] = 257;
    p[7] = 4;
}

static inline void InitializeMpHeader(u16 *p) {
    p[0] = 0;
    p[1] = 0;
    p[2] = 0;
    p[3] = 0;
    p[4] = 0;
    p[5] = 0;
    p[6] = 258;
    p[7] = 10;
}

u16 *WMSP_WL_MaKeyData(void *buffer, u16 a, u16 b, void *data) {
    Packet09 *packet = buffer;
    u16 *p = packet->words, *reply;
    InitializeKeyHeader(p);
    p[8] = a;
    p[9] = b;
    packet->key.encodedData = (u32)data;
    reply = p + p[7] + 8;
    reply[0] = p[6];
    reply[1] = 1;
    WMSP_WlRequest(buffer);
    return reply;
}

void *WMSP_WL_MaMp(void *buffer, u32 a, u32 b, u32 c, u16 d, u16 e, u16 f, u16 g, u16 h, u32 encodedData) {
    Packet09 *packet = buffer;
    u16 *p = packet->words, *reply;
    InitializeMpHeader(p);
    p[8] = a;
    p[9] = b;
    p[10] = c;
    p[11] = d;
    p[12] = e;
    p[13] = f;
    p[14] = g;
    p[15] = h;
    packet->mp.encodedData = encodedData;
    reply = p + p[7] + 8;
    reply[0] = p[6];
    reply[1] = 1;
    WMSP_WlRequest(buffer);
    return reply;
}
