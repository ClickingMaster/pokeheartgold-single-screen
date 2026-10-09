#pragma force_active on
#include "global.h"
#include <nitro/code32.h>

typedef struct {
    u8 pad0[4];
    u16 pingSeq;
    u16 ipId;
    u8 pad8[0x48];
    u32 ip;
} CpsState;

typedef struct {
    u8 pad0[10];
    u16 seq;
    u8 padC[0x1c - 0xc];
    u32 remote;
    u8 pad20[0x4c - 0x20];
    u8 *packet;
} Socket;

// Judgement call: declared const so the seq accesses stay out of MWCC's worst-case alias set
// (native schedules them freely around the packet stores); the counter is written through a cast.
extern const CpsState _021D4620;

u32 calc_checksum_do(u8 *, u32, u32);
u16 invert_checksum(u16);
void send_ip(u8 *, u32, u8 *, u32, u32, u8);
void send_ping(u8 *, u32, Socket *);

// @fn send_ping
void send_ping(u8 *data, u32 length, Socket *s) {
    u32 sum;
    u8 *buf;
    int c;
    u16 seq;
    buf = s->packet;
    *(u16 *)(buf + 0x22) = 8;
    *(u16 *)(buf + 0x26) = (u32)OS_GetCurrentThread();
    *(u16 *)(buf + 0x24) = 0;
    seq = _021D4620.pingSeq;
    s->seq = seq;
    ((CpsState *)&_021D4620)->pingSeq = seq + 1;
    *(u16 *)(buf + 0x28) = seq;
    sum = calc_checksum_do(buf + 0x22, 8, 0);
    sum = calc_checksum_do(data, length, sum);
    c = invert_checksum((u16)sum);
    *(u16 *)(buf + 0x24) = (c >> 8) | (c << 8);
    send_ip(buf + 0x22, 8, data, length, s->remote, 1);
}
