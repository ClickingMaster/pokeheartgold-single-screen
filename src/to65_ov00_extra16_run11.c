#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#pragma force_active on
#pragma warn_uninitializedvar off
#define B(p, o) (*(u8 *)((p) + (o)))
#define H(p, o) (*(u16 *)((p) + (o)))
#define W(p, o) (*(u32 *)((p) + (o)))
typedef void (*CB)(int, int, int, int, int, u32);
extern u8 *ov00_021F955C(void);
extern u32 *ov00_021EE73C(u32, int);
extern u8 ov00_02217A60[];
extern u8 ov00_02217AAC[];
extern int ov00_021EE31C();
extern int ov00_021F0404();
extern int ov00_021F04A0();
extern int ov00_021F2EE4();
extern int ov00_021F3334();
extern int ov00_021F381C();
extern int ov00_021F3C5C();
extern int ov00_021F3CF8();
extern int ov00_021F4304();
extern int ov00_021F5D0C();
extern int ov00_021F7BA0();
extern int ov00_021F7CEC();
extern int ov00_021F8278();
extern int ov00_021F82EC();
extern int ov00_021F956C();
extern int ov00_021F9630();
extern int ov00_0220D218();
extern int ov00_0220D238();
extern int ov00_0221019C();
extern int ov00_02212DB4();
extern int ov00_02212E78();
extern int ov00_02212E80();
extern int ov00_022139B8();
void ov00_021F5AE0(u32 x, u32 *p);
int ov00_021F5B30(u32 x);
int ov00_021F5FD4(int x);
int ov00_021F60AC(int x);
int ov00_021F631C(void);
int ov00_021F63DC(void);
int ov00_021F6B20(u32 *p, u32 n);
void ov00_021F6D30(void);
int ov00_021F6DF0(void);
void ov00_021F6FC4(void);
int ov00_021F6FF8(void);
int ov00_021F70B8(u32 x);
void ov00_021F7128(void);
int ov00_021F75F4(int x, int y);
void ov00_021F6BA8(void);
int ov00_021F6EC4(int x);
int ov00_021F7158(u32 mask);
void ov00_021F7294(int x, int y);
int ov00_021F5B5C(u32 x, int y);

int ov00_021F6B20(u32 *p, u32 n) {
    u32 i;
    if (B(ov00_021F955C(), 0x1a6) && W(ov00_021F955C(), 0x1a0) == 4) {
        return 1;
    }
    for (i = 0; i < n; i++) {
        if (!ov00_021F7BA0(p[i])) {
            return 0;
        }
        if (B(ov00_021F955C(), 0x1a6) && W(ov00_021F955C(), 0x1a0) == 1) {
            return 1;
        }
    }
    return 1;
}

void ov00_021F6BA8(void) {
    W(ov00_021F955C(), 0x20c) = 0;
    if (W(ov00_021F955C(), 0x1a0) == 2) {
        ov00_021F6D30();
        return;
    }
    B(ov00_021F955C(), 0x1a9) = 1;
    if (B(ov00_021F955C(), 21) == 3) {
        B(ov00_021F955C(), 0x1a9) = 1;
        if (B(ov00_021F955C(), 13)) {
            ov00_0220D238(**(u32 **)(ov00_021F955C() + 4));
        } else if (ov00_021F5B30(ov00_021F60AC(W(ov00_021F955C(), 0xf4)))) {
            return;
        }
        ov00_021F6D30();
        return;
    }
    if (W(ov00_021F955C(), 0x1a0) == 4 || W(ov00_021F955C(), 0x1a0) == 5 || W(ov00_021F955C(), 0x1a0) == 6 || W(ov00_021F955C(), 0x1a0) == 7 || W(ov00_021F955C(), 0x1a0) == 11 || W(ov00_021F955C(), 0x1a0) == 12) {
        if (W(ov00_021F955C(), 0x204) && ov00_021F5B30(ov00_021F60AC(W(ov00_021F955C(), 0xf4)))) {
            return;
        }
        if (ov00_021F5B30(ov00_021F6DF0())) {
            return;
        }
    }
    if (W(ov00_021F955C(), 0x19c)) {
        ov00_0221019C(W(ov00_021F955C(), 0x19c));
        W(ov00_021F955C(), 0x19c) = 0;
    }
    if (B(ov00_021F955C(), 13)) {
        B(ov00_021F955C(), 0x1a9) = 1;
        ov00_0220D238(**(u32 **)(ov00_021F955C() + 4));
    }
    ov00_021F6D30();
}

void ov00_021F6D30(void) {
    int a, b, r;
    u8 *p;
    ov00_021F04A0(1, ov00_02217A60, 0);
    if (ov00_021F8278()) {
        return;
    }
    ov00_021F3C5C();
    if (W(ov00_021F955C(), 0x20c)) {
        a = 1;
    } else {
        a = B(ov00_021F955C(), 0x15) == 2;
    }
    b = W(ov00_021F955C(), 0x20c) == 0;
    p = ov00_021F955C();
    r = ov00_021F0404(W(ov00_021F955C(), 0x20c));
    {
        u8 *q = ov00_021F955C();
        (*(CB *)(p + 0x464))(0, 1, b, a, r, W(q, 0x468));
    }
    B(ov00_021F955C(), 0x1a9) = 0;
}

int ov00_021F6DF0(void) {
    int r = 0, z = 0;
    if (B(ov00_021F955C(), 23) && W(ov00_021F955C(), 32)) {
        u8 *p = ov00_021F955C();
        if (W(p, 32) != W(ov00_021F955C(), 0x200)) {
            u8 *a = ov00_021F955C(), *b = ov00_021F955C(), *c = ov00_021F955C(), *d = ov00_021F955C();
            r = ov00_021F4304(12, W(a, 32), W(b + (B(c, 13) + 1) * 4, 0x24), H(d + (B(ov00_021F955C(), 13) + 1) * 2, 0xa4), &z, 1);
            B(ov00_021F955C(), 23) = 0;
            W(ov00_021F955C(), 32) = 0;
        }
    }
    return r;
}
