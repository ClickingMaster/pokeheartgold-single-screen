#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#pragma force_active on

typedef struct {
    u8 pad[6];
    u16 id;
    u8 rest[72];
    u32 ip;
} Network;

extern Network _021D4620;
void send_ip_frag(u8 *, u32, u8 *, u32, u32, u32);
void send_ip(u8 *, u32, u8 *, u32, u32, u8);

static inline u16 Swap(u16 value) {
    return (value >> 8) | (value << 8);
}

void send_ip(u8 *data, u32 length, u8 *extra, u32 extraLength, u32 destination, u8 protocol) {
    u16 offset = 0;
    u8 *p;
    u32 chunk;
    data[-20] = 0x45;
    data[-19] = 0;
    _021D4620.id++;
    *(u16 *)(data - 16) = Swap(_021D4620.id);
    data[-12] = 128;
    data[-11] = protocol;
    *(u16 *)(data - 8) = Swap((u16)(_021D4620.ip >> 16));
    *(u16 *)(data - 6) = Swap((u16)_021D4620.ip);
    *(u16 *)(data - 4) = Swap((u16)(destination >> 16));
    *(u16 *)(data - 2) = Swap((u16)destination);
    if (length > 1480) {
        p = data;
        while (length > 1480) {
            send_ip_frag(data, 0, p, 1480, destination, offset | 0x2000);
            p += 1480;
            length -= 1480;
            offset += 185;
        }
        if (length) {
            if (extraLength) {
                send_ip_frag(data, 0, p, length, destination, offset | 0x2000);
            } else {
                send_ip_frag(data, 0, p, length, destination, offset);
            }
            offset += length >> 3;
            length = 0;
        }
    }
    while (length + extraLength > 1480) {
        chunk = 1480 - length;
        send_ip_frag(data, length, extra, chunk, destination, offset | 0x2000);
        length = 0;
        extra += chunk;
        extraLength -= chunk;
        offset += 185;
    }
    if (length + extraLength) {
        send_ip_frag(data, length, extra, extraLength, destination, offset);
    }
}
