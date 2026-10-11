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

typedef struct {
    int x[12];
} Args;

extern char ov45_022565C0[];
extern char ov45_022565D0[];
extern char ov45_022560D8[];
extern char ov45_02256848[];
extern char ov45_02256860[];
extern void __msl_assertion_failed(char *, char *, char *, int);
extern void ov45_02248558(void *, char *);
extern void ov45_02249A0C(void *, void *, void *);
extern void *ov45_022496AC(void *, int, void *);
void ov45_0224C9D8(u8 *, u8 *);
#pragma opt_lifetimes off

void ov45_0224C9D8(u8 *p, u8 *q) {
    Args args;

    struct {
        int one, zero;
        char *name;
    } x1;

    struct {
        char *name;
        int a, b, c;
    } x2;

    struct {
        int one;
        char *name;
        int a, b, c;
    } x3;

    u8 *res;
    char *name;
    if (!p) {
        __msl_assertion_failed(ov45_022565C0, ov45_022565D0, ov45_022560D8, 0x99f);
    }
    if (I(q, 36) != 3) {
        __msl_assertion_failed(ov45_02256848, ov45_022565D0, ov45_022560D8, 0x9a5);
    }
    if (I(q, 36) != 3) {
        return;
    }
    name = V(V(q, 32), 4);
    if (strcmp(name, ov45_02256860) == 0) {
        name = 0;
    }
    memset(&args, 0, sizeof(args));
    args.x[0] = 1;
    args.x[1] = (int)name;
    args.x[3] = 15;
    args.x[4] = (int)name;
    args.x[6] = 3;
    args.x[7] = (int)name;
    args.x[9] = 3;
    res = ov45_022496AC(p, 4, &args);
    if (!res) {
        return;
    }
    if (I(res, 0) == 1) {
        x1.one = 1;
        x1.zero = 0;
        x1.name = name;
        if (I(res, 16) == 0) {
            ov45_02248558(p, name);
        }
        ov45_02249A0C(p, res, &x1);
    } else if (I(res, 0) == 15) {
        u8 *r = V(res, 28);
        x2.name = name;
        x2.a = I(r, 4);
        x2.b = I(r, 8);
        x2.c = I(r, 12);
        ov45_02249A0C(p, res, &x2);
    } else if (I(res, 0) == 3) {
        u8 *r = V(res, 28);
        x3.one = 1;
        x3.name = name;
        x3.a = I(r, 4);
        x3.b = I(r, 8);
        x3.c = I(r, 12);
        ov45_02249A0C(p, res, &x3);
    }
}
