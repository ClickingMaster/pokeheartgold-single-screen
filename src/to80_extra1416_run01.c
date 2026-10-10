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
    int id, status;
    char data[520];
} Status;

void *ov00_021EF3A4(void);
int DWC_GetGsProfileId(void *, Friend *);
int DWCi_Acc_IsValidFriendData(Friend *);
int ov00_02200540(void *, int, int *);
int ov00_02200400(void *, int, Status *);
int ov00_021ED4E8(char *, char *, char *, int);
u32 strtoul(const char *, char **, int);
int ov00_021F5B5C(int, int);
extern char ov00_02217A70[];
extern char ov00_02217A68[];
extern char ov00_02217A6C[];
int ov00_021F5D0C(int, int, int);

int ov00_021F5D0C(int mode, int same, int previous) {
    struct {
        char third[4];
        int buddy;
        char second[4], first[12];
        Status status;
    } buf;

    int profile, seen, advance, start, i, ret, a, b, d;
    Work *w, *x, *y;
    void *acc;
    if (same) {
        start = ov00_021F955C()->current;
    } else if (ov00_021F955C()->current >= ov00_021F955C()->friendCount - 1) {
        start = 0;
    } else {
        start = ov00_021F955C()->current + 1;
    }
    seen = 0;
    advance = same == 0 ? 1 : seen;
    for (;;) {
        if (advance || seen) {
            ov00_021F955C()->current++;
            if (ov00_021F955C()->current >= ov00_021F955C()->friendCount) {
                ov00_021F955C()->current = 0;
            }
        }
        if (seen && start == ov00_021F955C()->current) {
            ov00_021F955C()->delay = 3000;
            w = ov00_021F955C();
            *(u64 *)&w->time = OS_GetTick();
            ov00_021F955C()->active = 0;
            return 0;
        }
        seen = 1;
        acc = ov00_021EF3A4();
        w = ov00_021F955C();
        x = ov00_021F955C();
        y = ov00_021F955C();
        profile = DWC_GetGsProfileId(acc, &w->friends[x->friendIds[y->current]]);
        if (!profile || profile == -1) {
            continue;
        }
        w = ov00_021F955C();
        x = ov00_021F955C();
        y = ov00_021F955C();
        if (!DWCi_Acc_IsValidFriendData(&w->friends[x->friendIds[y->current]])) {
            continue;
        }
        for (i = 1; i <= ov00_021F955C()->players; i++) {
            if (profile == ov00_021F955C()->profiles[i]) {
                break;
            }
        }
        if (i <= ov00_021F955C()->players) {
            continue;
        }
        ret = ov00_02200540(ov00_021F955C()->ctx, profile, &buf.buddy);
        ret |= ov00_02200400(ov00_021F955C()->ctx, buf.buddy, &buf.status);
        if (ret || buf.status.status != 4) {
            continue;
        }
        a = ov00_021ED4E8(ov00_02217A70, buf.first, buf.status.data, 47);
        b = ov00_021ED4E8(ov00_02217A68, buf.second, buf.status.data, 47);
        d = ov00_021ED4E8(ov00_02217A6C, buf.third, buf.status.data, 47);
        if (a <= 0 || b <= 0 || d <= 0) {
            continue;
        }
        if (strtoul(buf.first, 0, 10) != 3) {
            continue;
        }
        a = strtoul(buf.second, 0, 10);
        if (a != ov00_021F955C()->limit) {
            continue;
        }
        if (profile == previous) {
            mode = 1;
        }
        return ov00_021F5B5C(profile, mode);
    }
}
