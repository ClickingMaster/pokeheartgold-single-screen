#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

typedef struct {
    u8 p[8];
    u8 state;
    u8 q[0x2c - 9];
    u16 remaining, window;
    u8 z[4];
    u32 position;
} Socket;

extern u16 gCpsState[];
extern void send_tcp(void *, u32, Socket *, int, int);
u32 tcp_write_do(void *, u32, Socket *, int);

u32 tcp_write_do(void *data, u32 len, Socket *s, int flag) {
    u32 count;
    u32 limit;
    u32 previous;
    u32 budget;
    u32 current;
    u32 available;
    if (flag) {
        limit = 1;
    } else {
        limit = s->remaining;
    }
    previous = s->position;
    budget = previous * 2 + 4;
    while (len && s->state == 4) {
        current = s->position;
        count = s->window;
        if (count >= limit) {
            count = limit;
        }
        if (gCpsState[1] < count) {
            count = gCpsState[1];
        }
        if (!flag) {
            count &= ~1u;
        }
        if (len < count) {
            count = len;
        }
        available = budget + (current - previous);
        previous = current;
        budget = available - 1;
        if (!available) {
            count = 0;
        }
        if (count == 0) {
            break;
        }
        limit -= count;
        send_tcp(data, count, s, 24, 0);
        OS_YieldThread();
        data = (u8 *)data + count;
        len -= count;
    }
    return count;
}
