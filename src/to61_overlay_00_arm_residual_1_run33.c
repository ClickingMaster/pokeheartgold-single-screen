#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#pragma force_active on
#pragma warn_unusedvar off
#pragma warn_uninitializedvar off
#define B(p, o) (((u8 *)(p))[o])
#define H(p, o) (*(u16 *)((u8 *)(p) + (o)))
#define W(p, o) (*(u32 *)((u8 *)(p) + (o)))
#define T(p, o) (*(u64 *)((u8 *)(p) + (o)))

typedef struct {
    u8 id, type, pad[2];
    void *owned;
    void *value;
} Entry;

extern Entry ov00_0221A948[];

typedef struct {
    u8 pad[0xf4];
    u32 profiles[32];
} Profiles;

typedef struct {
    u8 active, one, attempt, pad;
    u32 time, bits, other;
    u64 start, last;
} Timer;

extern struct {
    Timer *timer;
    u8 *context;
    u32 extra[5];
} ov00_0221A928;

typedef struct {
    u8 *send, *recv;
    int size, sent, received, total, pending;
    u8 sending, receiving;
    u16 pad;
    u16 p20, p22;
    u64 tick;
    u32 timeout;
} Channel;

typedef struct {
    Channel channel[32];
    void (*sent)(int, int);
    void (*recv)(int, const void *, int);
    void (*timeout)(int);
    void (*other)(int, int);
    u16 max, pad;
} Net;

extern Net *ov00_0221B0A0;
extern char ov00_02217B70[];
extern char ov00_02217B7C[];
extern char ov00_02217AB4[];
extern char ov00_02217AC0[];
extern char ov00_02217ACC[];
extern char ov00_02217AAC[];
extern char ov00_02217B88[];
extern void ov00_021EC2EC(int, void *, int);
extern void ov00_021EC24C(int, int);
extern void ov00_021F2EE4(int, int);
extern void ov00_021EF3C0(int, int);
extern void ov00_021F0238(int, int);
extern int ov00_02212DB4(void *, const char *, int);
extern int ov00_021F03BC(int);
extern int ov00_021EE568(void);
extern int ov00_021EC230(void);
extern int ov00_021EE530(int);
extern int ov00_021EE7B4(int);
extern int ov00_021FA090(int);
extern int ov00_021EE490(void);
extern int ov00_021EE64C(int);
extern int ov00_021EE6B0(int);
extern void ov00_0220D194(int, const void *, int, int);
extern void ov00_021F9E28(int, const void *, int, int);
extern void ov00_021F9EA8(int, const void *, int);
extern void ov00_021F9F70(int, const void *, int);
extern void ov00_021FA054(int, const void *, int);
extern int memcmp(const void *, const void *, u32);
extern char *strncpy(char *, const char *, u32);
extern int ov00_021F9630(int, int, const void *, int);
u8 ov00_021F7A98(void);
void ov00_021F7AD8(void);
u8 ov00_021F7B30(void);
int ov00_021F7BA0(int profile);
int ov00_021F7C10(void *p);
u8 ov00_021F7CEC(u32 profile, int first);
u32 ov00_021F7D50(int flag);
void ov00_021F7DB4(void);
void ov00_021F7DDC(int flag);
int ov00_021F8278(unsigned n);
int ov00_021F82EC(unsigned n);
int ov00_021F8380(unsigned n);
int ov00_021F8468(unsigned n);
int ov00_021F8514(unsigned n);
void ov00_021F89CC(void);
void ov00_021F8E60(void);
void ov00_021F8E64(void);
int ov00_021F8F0C(void);
int ov00_021F8F14(unsigned n);
void ov00_021F8F20(u32 a, u16 b);
void ov00_021F9100(void);
int ov00_021F94DC(int flag);
void ov00_021F954C(u8 *p);
u8 *ov00_021F955C(void);
void ov00_021F956C(int state);
int ov00_021F9580(int aid);
int ov00_021F9590(int aid, int flag);
int DWC_SendReliable(int aid, const void *p, int n);
int DWC_SendUnreliable(int aid, const void *p, int n);
int ov00_021F9850(int aid, u8 *p, int n);
int ov00_021F989C(void (*cb)(int, int));
int ov00_021F98BC(void (*cb)(int, const void *, int));
int ov00_021F98DC(void (*cb)(int));
void ov00_021F98FC(void *p, int flag, u32 n);
unsigned ov00_021F992C(const void *p);
int ov00_021F9968(int n);
int ov00_021F9988(int aid, u32 timeout);
void ov00_021F99D4(Net *p);
void ov00_021F9A10(int aid, const void *p, int n, int flag);
void ov00_021F9A4C(int id, int n);
void ov00_021F9C20(int aid);
void ov00_021F9CA8(void);
Channel *ov00_021F9CBC(int aid);
int ov00_021F9CD4(int aid);
unsigned ov00_021F9CF0(int aid);
void ov00_021F9D0C(int aid, const void *p, int n, int flag);
void ov00_021F9D34(int id, const void *p, int n);

extern void ov00_02210D9C(void *, int);
extern int ov00_021F3F5C(int, int, int);
extern int ov00_021EE4AC(u8 **);
void ov00_021F8E68(int state, void *p);
void ov00_021F8F40(int profile);
int ov00_021F9630(int flag, int aid, const void *p, int n);
u32 ov00_021F9750(u32 bits, const void *p, int n);
void ov00_021F9A8C(void);
extern u8 ov00_0221A949[];
extern void *ov00_0221A950[];
extern void ov00_02210DC8(void *, int);
extern void ov00_02210DFC(void *, void *);
void ov00_021F8D18(int, void *);

int ov00_021F94DC(int flag) {
    if (flag) {
        return 1;
    }
    if (B(ov00_021F955C(), 0x15) != 3) {
        B(ov00_021F955C(), 0x17d)
        ++;
    }
    if (B(ov00_021F955C(), 0x15) == 3 || B(ov00_021F955C(), 0x17d) >= 5) {
        ov00_021F2EE4(6, -86420);
        return 0;
    }
    return 1;
}

void ov00_021F954C(u8 *p) {
    ov00_0221A928.context = p;
}

u8 *ov00_021F955C(void) {
    return ov00_0221A928.context;
}

void ov00_021F956C(int state) {
    W(ov00_0221A928.context, 0x1a0) = state;
}

int ov00_021F9580(int aid) {
    return ov00_021F9590(aid, 1);
}

int ov00_021F9590(int aid, int flag) {
    int n;
    if (ov00_021EC230() || (flag == 1 && !ov00_021EE530(aid)) || !ov00_021EE7B4(aid)) {
        return 0;
    }
    if (ov00_021F9CD4(aid) == 1) {
        return 0;
    }
    n = ov00_021FA090(aid);
    return n >= ov00_021F9968(flag);
}

int DWC_SendReliable(int aid, const void *p, int n) {
    return ov00_021F9630(1, aid, p, n);
}

int ov00_021F9630(int flag, int aid, const void *p, int n) {
    Channel *c = ov00_021F9CBC(aid);
    u8 header[8];
    int amount;
    if (!ov00_021F9590(aid, flag)) {
        return 0;
    }
    c->sending = 1;
    c->send = (u8 *)p;
    c->sent = 0;
    c->total = n;
    ov00_021F98FC(header, flag, n);
    ov00_021F9D0C(aid, header, 8, 1);
    amount = ov00_0221B0A0->max;
    if (n <= amount) {
        amount = n;
    }
    if (amount > ov00_021FA090(aid)) {
        return 1;
    }
    ov00_021F9D0C(aid, p, amount, 1);
    c->sent += amount;
    {
        int total = c->total;
        if (c->sent == total) {
            c->sending = 0;
            c->send = 0;
            c->sent = 0;
            c->total = 0;
            if (ov00_0221B0A0->sent && flag == 1) {
                ov00_0221B0A0->sent(total, aid);
            }
        }
    }
    return 1;
}

u32 ov00_021F9750(u32 bits, const void *p, int n) {
    u8 i;
    for (i = 0; i < 32; i++) {
        u32 bit = i ? 1 << i : 1;
        if (bits & bit) {
            if (i == ov00_021EE490()) {
                continue;
            }
            if (!DWC_SendReliable(i, p, n)) {
                bits &= ~bit;
            }
        }
    }
    return bits;
}

int DWC_SendUnreliable(int aid, const void *p, int n) {
    if (ov00_021EC230()) {
        return 0;
    }
    if (!ov00_021EE530(aid)) {
        return 0;
    }
    if (n > ov00_0221B0A0->max) {
        return 0;
    }
    ov00_021F9D0C(aid, p, n, 0);
    if (ov00_0221B0A0->sent) {
        ov00_0221B0A0->sent(n, aid);
    }
    return 1;
}

int ov00_021F9850(int aid, u8 *p, int n) {
    Channel *c = ov00_021F9CBC(aid);
    if (ov00_021F9CF0(aid) == 2) {
        return 0;
    }
    c->recv = p;
    c->size = n;
    c->receiving = 1;
    c->received = 0;
    c->pending = 0;
    return 1;
}

int ov00_021F989C(void (*cb)(int, int)) {
    if (!ov00_0221B0A0) {
        return 0;
    }
    ov00_0221B0A0->sent = cb;
    return 1;
}

int ov00_021F98BC(void (*cb)(int, const void *, int)) {
    if (!ov00_0221B0A0) {
        return 0;
    }
    ov00_0221B0A0->recv = cb;
    return 1;
}

int ov00_021F98DC(void (*cb)(int)) {
    if (!ov00_0221B0A0) {
        return 0;
    }
    ov00_0221B0A0->timeout = cb;
    return 1;
}

void ov00_021F98FC(void *p, int flag, u32 n) {
    strncpy((char *)p + 6, ov00_02217B88, 2);
    H(p, 4) = flag;
    W(p, 0) = n;
}

unsigned ov00_021F992C(const void *p) {
    u8 buf[8];
    MI_CpuCopy8(p, buf, 8);
    return memcmp(buf + 6, ov00_02217B88, 2) == 0 ? H(buf, 4) : 0;
}

int ov00_021F9968(int n) {
    switch (n) {
    case 2:
    case 3:
    case 4:
        return 12;
    default:
        return 8;
    }
}

int ov00_021F9988(int aid, u32 timeout) {
    if (!ov00_0221B0A0) {
        return 0;
    }
    ov00_0221B0A0->channel[aid].timeout = timeout;
    ov00_0221B0A0->channel[aid].tick = OS_GetTick();
    return 1;
}

void ov00_021F99D4(Net *p) {
    ov00_0221B0A0 = p;
    MI_CpuFill8(p, 0, 1556);
    ov00_0221B0A0->max = 1465;
}

void ov00_021F9A10(int aid, const void *p, int n, int flag) {
    if (!ov00_0221B0A0) {
        return;
    }
    if (!p || !n) {
        return;
    }
    if (flag) {
        ov00_021F9D34(aid, p, n);
    } else {
        ov00_021F9E28(aid, p, n, flag);
    }
}

void ov00_021F9A4C(int id, int n) {
    if (!ov00_0221B0A0->other) {
        return;
    }
    id = ov00_021EE6B0(id);
    ov00_0221B0A0->other(n, id);
}
