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

void ov00_021F8D18(int id, void *out) {
    int index;
    switch (id) {
    case 8:
        ov00_02210DC8(out, B(ov00_021F955C(), 0x14));
        return;
    case 10:
        ov00_02210DC8(out, B(ov00_021F955C(), 0x16));
        return;
    case 50:
        ov00_02210DC8(out, W(ov00_021F955C(), 0x200));
        return;
    case 51:
        ov00_02210DC8(out, B(ov00_021F955C(), 0x15));
        return;
    case 52:
        ov00_02210DC8(out, W(ov00_021F955C(), 0x20));
        return;
    case 53:
        ov00_02210DC8(out, 3);
        return;
    case 54:
        ov00_02210DC8(out, 1);
        return;
    }
    index = id - 100;
    if (index < 0) {
        return;
    }
    if (index >= 154) {
        return;
    }
    index *= 12;
    if (!B(ov00_0221A948, index)) {
        return;
    }
    if (ov00_0221A949[index]) {
        ov00_02210DFC(out, *(void **)((u8 *)ov00_0221A950 + index));
    } else {
        ov00_02210DC8(out, *(int *)*(void **)((u8 *)ov00_0221A950 + index));
    }
}

void ov00_021F8E60(void) {
}

void ov00_021F8E64(void) {
}

void ov00_021F8E68(int state, void *p) {
    int i;
    Entry *e;
    switch (state) {
    case 0:
        break;
    case 1:
    case 2:
    default:
        return;
    }
    ov00_02210D9C(p, 8);
    ov00_02210D9C(p, 10);
    ov00_02210D9C(p, 50);
    ov00_02210D9C(p, 51);
    ov00_02210D9C(p, 52);
    ov00_02210D9C(p, 53);
    ov00_02210D9C(p, 54);
    e = ov00_0221A948;
    for (i = 0; i < 154; i++, e++) {
        if (e->id) {
            ov00_02210D9C(p, e->id);
        }
    }
}

int ov00_021F8F0C(void) {
    return 0;
}

int ov00_021F8F14(unsigned n) {
    return ov00_021F8380(n);
}

void ov00_021F8F20(u32 a, u16 b) {
    W(ov00_021F955C(), 0x1c) = a;
    H(ov00_021F955C(), 0x1a) = b;
}

void ov00_021F8F40(int profile) {
    if (W(ov00_021F955C(), 0x1a0) == 1) {
        ov00_021F956C(6);
    } else if (W(ov00_021F955C(), 0x1a0) != 6 && W(ov00_021F955C(), 0x1a0) != 11) {
        return;
    }
    if (profile == W(ov00_021F955C(), 0x180)) {
        B(ov00_021F955C(), 0x17c)
        ++;
    } else {
        B(ov00_021F955C(), 0x17c) = 0;
        W(ov00_021F955C(), 0x180) = profile;
    }
    T(ov00_021F955C(), 0x184) = 0;
    if (ov00_021F8468(ov00_021F3F5C(1, profile, 0))) {
        return;
    }
    B(ov00_021F955C(), 0x3cc) = 255;
}
