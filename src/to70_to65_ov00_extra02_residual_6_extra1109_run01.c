#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

struct State {
    u8 p[0x10];
    int a;
    u8 q[3];
    u8 flag;
    u8 z[8];
    int b;
    u8 y[0xc0];
    void *handle;
    u8 w[12];
    int arg;
    u8 s[0x108];
    int current;
    u8 t[0xf0];
    int x, y2;
    u8 u[0x170];
    int one, two;
};
extern struct State *ov00_021F955C(void);
extern void ov00_021F3BB4(int, int, int, int);
extern void ov00_021F956C(int);
extern void ov00_021F04A0(int, void *, int);
extern void ov00_021F5B5C(int, int);
extern int ov00_021F82EC(int);
extern int ov00_021F8278(void);
extern int ov00_021F179C(int);
extern int ov00_021F5B30(void);
extern void *ov00_02213610(int, int, int, int, int, int, int, void *, int);
extern u8 ov00_021F85B8[];
extern u8 ov00_02217A60[];
void ov00_021F1BCC(int, int, int, int, int);

void ov00_021F1BCC(int arg, int b, int c, int d, int e) {
    struct State *s, *s1, *s2, *s3;
    ov00_021F3BB4(3, 0, b, c);
    ov00_021F955C()->one = d;
    ov00_021F955C()->two = e;
    ov00_021F955C()->flag = 1;
    s = ov00_021F955C();
    s->b = ov00_021F955C()->current;
    ov00_021F955C()->arg = arg;
    ov00_021F956C(4);
    if (ov00_021F955C()->handle == 0) {
        s = ov00_021F955C();
        s1 = ov00_021F955C();
        s2 = ov00_021F955C();
        s3 = ov00_021F955C();
        s->handle = ov00_02213610(s1->x, s2->x, s3->y2, 0, 20, 1, 0, ov00_021F85B8, 0);
    }
    if (ov00_021F955C()->handle == 0) {
        if (ov00_021F82EC(5)) {
            return;
        }
    }
    ov00_021F04A0(5, ov00_02217A60, 0);
    if (ov00_021F8278()) {
        return;
    }
    if (ov00_021F955C()->a == 0) {
        if (ov00_021F179C(ov00_021F955C()->current)) {
            return;
        }
    }
    ov00_021F5B5C(ov00_021F955C()->arg, 0);
    if (ov00_021F5B30()) {
        return;
    }
}
