#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
extern void OS_YieldThread__(void);

typedef struct {
    u8 p[8];
    u8 state;
    u8 q[0x28 - 9];
    u32 sent;
    u16 window, pad;
    u32 ack, pending;
} Socket;

typedef struct {
    u8 p[0x48];
    int (*alive)(void);
} Network;

extern Network gCpsState;
extern void tcp_write_do2(const u8 *, u32, const u8 *, u32, Socket *, int);
u32 CPSi_TcpWrite2Raw(const u8 *, u32, const u8 *, u32, Socket *);

u32 CPSi_TcpWrite2Raw(const u8 *first, u32 len, const u8 *second, u32 len2, Socket *s) {
    u32 total = 0, start, old, wait, done;
    int forced = 0;
    s->pending = 0;
    start = (u32)(OS_GetTick() >> 16);
    while (gCpsState.alive() && len && s->state == 4 && (s32)((u32)(OS_GetTick() >> 16) - start) < 159) {
        old = s->sent;
        tcp_write_do2(first, len, second, len2, s, forced);
        wait = (u32)(OS_GetTick() >> 16);
        do {
            OS_YieldThread__();
        } while (gCpsState.alive() && s->state == 4 && s->sent != s->ack && (s32)((u32)(OS_GetTick() >> 16) - wait) < 15 && (!forced || s->window == 0));
        done = s->ack - old;
        if (done > s->sent - old) {
            done = 0;
        }
        total += done;
        if (done) {
            start = (u32)(OS_GetTick() >> 16);
        }
        s->sent = s->ack;
        if (s->state == 4 && s->window == 0 && done == 0) {
            if (!forced) {
                wait = (u32)(OS_GetTick() >> 16);
                while (gCpsState.alive() && (s32)((u32)(OS_GetTick() >> 16) - wait) < 15) {
                    OS_YieldThread__();
                    if (s->window) {
                        break;
                    }
                }
                if (s->window == 0) {
                    forced = 1;
                }
            }
        } else {
            forced = 0;
        }
        if (done < len) {
            first += done;
            len -= done;
        } else {
            u32 n = done - len;
            first = second + n;
            second = 0;
            len = len2 - n;
            len2 = 0;
        }
    }
    return total;
}
