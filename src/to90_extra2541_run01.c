#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#pragma force_active on

typedef struct {
    u8 family, pad;
    u16 port;
    u32 address;
} Addr;

typedef struct {
    u8 flag, retries;
    u8 pad[6];
    int active;
} Request;

typedef struct SocSockAddr {
    u8 len, family;
    u8 port[2], addr[4];
} SocSockAddr;

typedef struct {
    int socket, other, handle, pad, state, p2, p3, p4, attempts, limit;
    u32 deadline, addr;
    u16 port;
    u8 p5[6];
    void (*callback)(int, int, Addr *, Request *);
    Request *arg;
} Work;

extern u8 ov00_0221B678[];
extern void ov00_0220FAD8(void *);
extern void ov00_0221075C(const u8 *, int, void *);
extern void ov00_0221019C(int);
extern void ov00_0220FC7C(void *);
extern void ov00_0220FEC4(void *);
extern int ov00_021FF694(int);
extern int ov00_021FF940(int, void *, int, int, SocSockAddr *, int *);
extern u32 ov00_021FF368(void);

void ov00_022101D4(void *);

void ov00_022101D4(void *arg) {
    Work *w = arg;
    SocSockAddr remote;
    int size = 8;
    Addr packet;
    if (w->state == 4) {
        ov00_0220FAD8(w);
        return;
    }
    if (w->socket != size - 9) {
        for (;;) {
            if (ov00_021FF694(w->socket) == 0) {
                break;
            }
            int got = ov00_021FF940(w->socket, ov00_0221B678, 512, 0, &remote, &size);
            if (got == -1) {
                break;
            }
            ov00_0221075C(ov00_0221B678, got, &remote);
            if (w->state == 4 || w->socket == -1) {
                break;
            }
        }
    }
    if ((w->state == 0 || w->state == 2) && ov00_021FF368() > w->deadline) {
        if (w->attempts > w->limit) {
            w->callback(2, -1, 0, w->arg);
            ov00_0221019C(w->handle);
        } else {
            w->attempts++;
            if (w->state == 0) {
                ov00_0220FC7C(w);
            } else {
                ov00_0220FEC4(w);
            }
        }
    }
    if (w->state == 3 && ov00_021FF368() > w->deadline) {
        if (w->other == -1) {
            packet.pad = 2;
            packet.port = (u16)((u8)(w->port >> 8) | ((w->port << 8) & 0xff00));
            packet.address = w->addr;
            w->callback(0, w->socket, &packet, w->arg);
            w->socket = -1;
        }
        ov00_0221019C(w->handle);
    }
    if (w->state == 1 && ov00_021FF368() > w->deadline) {
        w->callback(1, -1, 0, w->arg);
        ov00_0221019C(w->handle);
    }
}
