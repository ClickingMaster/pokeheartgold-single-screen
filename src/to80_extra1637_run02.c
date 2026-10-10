#pragma force_active on
#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#define I(p, o) (*(int *)((u8 *)(p) + (o)))
#define U(p, o) (*(u32 *)((u8 *)(p) + (o)))
#define H(p, o) (*(u16 *)((u8 *)(p) + (o)))
#define B(p, o) (*((u8 *)(p) + (o)))
#define V(p, o) (*(void **)((u8 *)(p) + (o)))
extern u8 *ov00_021F955C(void);
extern void ov00_02210CBC(void *);
extern void ov00_021F7DDC(int);
extern int ov00_021F7B30(void);
extern int ov00_021F4304(int, int, int, u16, void *, int);
int ov00_021F5920(int, int, int);

int ov00_021F5920(int id, int value, int port) {
    u8 *p, *q, *r;
    int data[2];
    int i, result;
    if (B(ov00_021F955C(), 23) && id == I(ov00_021F955C(), 32)) {
        return 0;
    }
    B(ov00_021F955C(), 23) = 1;
    I(ov00_021F955C(), 32) = id;
    I(ov00_021F955C(), 0x1bc) = 0;
    I(ov00_021F955C(), 0x1c8) = 0;
    ov00_02210CBC(V(ov00_021F955C(), 16));
    I(ov00_021F955C(), 0x204) = 0;
    p = ov00_021F955C();
    q = ov00_021F955C();
    I(p + (B(q, 20) + 1) * 4, 0xf4) = id;
    p = ov00_021F955C();
    q = ov00_021F955C();
    I(p + (B(q, 20) + 1) * 4, 36) = value;
    p = ov00_021F955C();
    q = ov00_021F955C();
    H(p + (B(q, 20) + 1) * 2, 0xa4) = port;
    I(ov00_021F955C(), 0x1b8) = value;
    H(ov00_021F955C(), 0x1b4) = port;
    p = ov00_021F955C();
    q = ov00_021F955C();
    result = ov00_021F7B30();
    B(p + (B(q, 20) + 1), 0x2d0) = result;
    data[0] = id;
    p = ov00_021F955C();
    q = ov00_021F955C();
    data[1] = B(p + (B(q, 20) + 1), 0x2d0);
    for (i = 1; i <= B(ov00_021F955C(), 20); i++) {
        p = ov00_021F955C();
        q = ov00_021F955C();
        r = ov00_021F955C();
        result = ov00_021F4304(7, ((int *)p)[i + 61], ((int *)q)[i + 9], ((u16 *)r)[i + 82], data, 2);
        if (result) {
            return result;
        }
    }
    ov00_021F7DDC(1);
    return 0;
}
