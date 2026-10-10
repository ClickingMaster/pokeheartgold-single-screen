#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

typedef struct {
    u32 low, high;
} Tick;

typedef struct {
    u32 a, b, c;
} Friend;

typedef struct {
    void *ctx;
    void **peer;
    void *callback;
    u8 flag, players;
    u8 p0[6];
    u8 connections, mode, limit;
    u8 p1[3];
    u16 port;
    u32 addr;
    u8 p2[4];
    u32 addresses[32];
    u16 ports[40];
    int profiles[32];
    Tick pending;
    u8 response;
    u8 p3[3];
    u32 attempts;
    Tick retry, wait;
    u8 p4[12];
    int state;
    u8 initial, current;
    u8 p5[14];
    u16 remotePort;
    u16 unused;
    u32 remoteAddr;
    int active;
    u8 p6[8];
    int delay;
    Tick time;
    u8 p7[0x2c];
    u32 identity;
    int target;
    u8 p8[8];
    u32 addr2[32];
    u16 port2[32];
    u8 slots[44];
    Friend *friends;
    u32 unused2;
    u8 friendIds[64];
    int friendCount;
} Work;

Work *ov00_021F955C(void);
void ov00_021F956C(int);

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

extern char ov00_02217A64[];
u32 ov00_0220F724(u32, u16, int);
int ov00_0220D018(void *, int, u32, char *, int, int, void *, int);
void ov00_021F8514(void);
int ov00_021F6100(int);
int ov00_021F84C4(int);
int ov00_021F94DC(int);
int ov00_021F631C(int);
void ov00_021F4248(Request *);
int ov00_021F8468(void);
void ov00_021F9104(int, int, Addr *, Request *);

void ov00_021F9104(int result, int unused, Addr *addr, Request *req) {
    int index, type, ret;
    Work *w, *a, *b;
    u32 remote;
    u16 port;
    char buffer[12];
    if (ov00_021F955C()->state != 6 && ov00_021F955C()->state != 11) {
        return;
    }
    if (!req) {
        return;
    }
    if (!result) {
        req->active = 0;
        ov00_021F955C()->connections++;
        index = ov00_021F955C()->connections;
        if (req->flag) {
            ov00_021F955C()->addr2[index] = addr->address;
            port = addr->port;
            ov00_021F955C()->port2[index] = ((port >> 8) & 255) | ((port << 8) & 0xff00);
            ov00_021F955C()->response = 0;
            ov00_021F955C()->attempts = 0;
            w = ov00_021F955C();
            *(u64 *)&w->retry = 0;
            if (ov00_021F955C()->state == 11) {
                ov00_021F956C(12);
            } else {
                ov00_021F956C(7);
            }
            ov00_021F955C()->flag = 0;
            OS_SNPrintf(buffer, 12, ov00_02217A64, ov00_021F955C()->identity);
            w = ov00_021F955C();
            a = ov00_021F955C();
            b = ov00_021F955C();
            remote = ov00_0220F724(a->addr2[index], b->port2[index], 0);
            a = ov00_021F955C();
            ret = ov00_0220D018(*w->peer, 0, remote, buffer, -1, 5000, a->callback, 0);
            if (ret == 1) {
                ov00_021F8514();
                return;
            }
            if (!ret) {
                return;
            }
            if (ov00_021F6100(ov00_021F955C()->profiles[index])) {
                return;
            }
            return;
        }
        if (addr) {
            index--;
            ov00_021F955C()->addr2[index] = addr->address;
            port = addr->port;
            ov00_021F955C()->port2[index] = ((port >> 8) & 255) | ((port << 8) & 0xff00);
        }
        w = ov00_021F955C();
        *(u64 *)&w->wait = OS_GetTick();
        ov00_021F956C(7);
        return;
    }
    if (!req->active) {
        return;
    }
    type = ov00_021F84C4(result);
    if (type != 2 && type != 1) {
        return;
    }
    if (!req->flag) {
        if (type == 1 || (type == 2 && req->retries >= 1)) {
            req->active = 0;
            if (!ov00_021F94DC(0)) {
                return;
            }
            w = ov00_021F955C();
            if (ov00_021F631C(w->profiles[ov00_021F955C()->players])) {
                return;
            }
            return;
        }
        req->retries++;
        ov00_021F4248(req);
        if (ov00_021F8468()) {
            return;
        }
        return;
    }
    w = ov00_021F955C();
    *(u64 *)&w->retry = OS_GetTick();
    switch (type) {
    case 1:
        break;
    case 2:
        if (ov00_021F955C()->response < 1) {
            return;
        }
        break;
    default:
        return;
    }
    req->active = 0;
    if (ov00_021F955C()->mode == 3 || ov00_021F955C()->mode == 2) {
        if (!ov00_021F94DC(1)) {
            return;
        }
    } else if (!ov00_021F94DC(0)) {
        return;
    }
    ov00_021F955C()->response = 0;
    ov00_021F955C()->attempts = 0;
    w = ov00_021F955C();
    *(u64 *)&w->retry = 0;
    w = ov00_021F955C();
    if (ov00_021F6100(w->profiles[ov00_021F955C()->players + 1])) {
        return;
    }
}
