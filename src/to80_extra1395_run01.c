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
    u8 p0[7];
    u8 mode, limit;
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
    u8 enabled, wanted, retries, pad;
    u32 timeout, mask, all;
    Tick initial, last;
} State;

typedef struct {
    State *state;
    u8 pad[20];
    int busy;
} Global;

extern Global ov00_0221A928;
u32 ov00_021F7D50(int);
void ov00_021F64C0(int);
int ov00_021F5D0C(int, int, int);
void ov00_021F7DDC(int);
void ov00_021F7128(void);
void ov00_021F6EC4(int);
void ov00_021F4304(int, int, u32, u16, int, int);
int ov00_021F5B30(void);
void ov00_021F60AC(int);
void ov00_021F7E4C(void);

void ov00_021F7E4C(void) {
    State *s;
    int retries, i;
    u32 mask;
    u64 elapsed;
    if (!ov00_0221A928.state || !ov00_0221A928.state->enabled) {
        return;
    }
    if (ov00_021F955C()->mode == 2) {
        return;
    }
    if (ov00_021F955C()->mode == 3) {
        return;
    }
    if (ov00_021F955C()->state == 19) {
        mask = ov00_021F7D50(0);
        s = ov00_0221A928.state;
        if (s->mask == mask) {
            if (s->all == mask) {
                ov00_021F955C()->limit = ov00_021F955C()->players;
                ov00_021F955C()->initial = ov00_021F955C()->players - 1;
                ov00_021F64C0(0);
                return;
            }
            *(u64 *)&s->last = OS_GetTick();
            s->mask = 0;
            if (ov00_021F955C()->mode == 0) {
                ov00_021F956C(3);
                *(int *)((u8 *)ov00_021F955C() + 0xe8) = 2;
                {
                    Work *w = ov00_021F955C();
                    *(u64 *)((u8 *)w + 0xec) = OS_GetTick();
                }
                return;
            }
            ov00_021F956C(4);
            ov00_021F5D0C(1, 0, 0);
            return;
        }
        retries = s->retries;
        elapsed = ((OS_GetTick() - *(u64 *)&s->last) << 6) / 0x82ea;
        if (elapsed < (u64)(retries * 6000)) {
            return;
        }
        if ((u32)retries > 5) {
            ov00_021F7DDC(1);
            ov00_021F7128();
            ov00_021F6EC4(1);
            return;
        }
        for (i = 1; i <= ov00_021F955C()->players; i++) {
            if (!(ov00_0221A928.state->mask & (1 << ov00_021F955C()->slots[i]))) {
                Work *a = ov00_021F955C(), *b = ov00_021F955C(), *c = ov00_021F955C();
                ov00_021F4304(17, a->profiles[i], b->addresses[i], c->ports[i], 0, 0);
                if (ov00_021F5B30()) {
                    return;
                }
            }
        }
        ov00_0221A928.state->retries++;
        return;
    }
    if (ov00_021F955C()->state != 4 && ov00_021F955C()->state != 3) {
        return;
    }
    s = ov00_0221A928.state;
    if (ov00_021F955C()->players < s->wanted - 1) {
        return;
    }
    if (s->retries == 0) {
        s = ov00_0221A928.state;
        elapsed = ((OS_GetTick() - *(u64 *)&s->initial) << 6) / 0x82ea;
        if (elapsed >= s->timeout) {
            goto send;
        }
    }
    if (!s->retries) {
        return;
    }
    s = ov00_0221A928.state;
    elapsed = ((OS_GetTick() - *(u64 *)&s->last) << 6) / 0x82ea;
    if (elapsed < (s->timeout >> 2)) {
        return;
    }
send:
    if (ov00_021F955C()->target) {
        ov00_021F60AC(ov00_021F955C()->target);
        if (ov00_021F5B30()) {
            return;
        }
    }
    ov00_021F956C(19);
    for (i = 1; i <= ov00_021F955C()->players; i++) {
        Work *a = ov00_021F955C(), *b = ov00_021F955C(), *c = ov00_021F955C();
        ov00_021F4304(17, a->profiles[i], b->addresses[i], c->ports[i], 0, 0);
        if (ov00_021F5B30()) {
            return;
        }
    }
    s = ov00_0221A928.state;
    *(u64 *)&s->last = OS_GetTick();
    s->retries = 1;
}
