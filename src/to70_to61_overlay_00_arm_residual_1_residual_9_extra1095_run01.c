#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

typedef struct {
    u8 p[0x24];
    int state;
    u8 q[6];
    u16 name[26];
    u8 r[6];
    void *token;
    int z;
    void *a, *b, *c, *d;
    u8 s[0xe0 - 0x80];
    u8 object[0x100];
    u8 other[4];
    u8 v[0x384 - 0x1e4];
    void *resource;
    u8 x;
    volatile u8 mode;
    u8 y[2];
    u8 cleanup;
} Work;

extern Work *ov00_0221A784;
extern int ov00_021EC230(void);
extern int ov00_021EC724(void);
extern u32 ov00_021ED704(const u16 *);
extern void ov00_021EF3C0(int, int);
extern void ov00_021EE800(int);
extern void ov00_021FE228(void *);
extern void ov00_021EEAD4(void);
extern void ov00_021F018C(void *, void *, void *, int, int, int, int, int);
int ov00_021EDD6C(const u16 *, int, void *, void *), ov00_021EDE74(int, void *, void *, int, int, int, int);
extern int ov00_021ECB40(void);
extern int ov00_021EE8B4(void);
extern int ov00_021FE3D8(void);
extern void ov00_021EE7FC(void);
extern void ov00_021FFD50(void *, int, int);
extern void ov00_021FFDD0(void *, int, void *, int);
extern void ov00_021EF274(void);
extern void ov00_021EF2AC(void);
extern void ov00_021F0028(void);
extern void ov00_021F1DE4(int);
extern void ov00_021F9A8C(void);
extern void ov00_02210D18(void *);
extern void ov00_021EEC94(void);
extern void ov00_021EECCC(void);
extern void ov00_021F0290(void);
extern void ov00_021F0338(void);
void ov00_021EDB1C(void);

void ov00_021EDB1C(void) {
    if (ov00_021ECB40()) {
        ov00_021EE7FC();
    }
    if (!ov00_0221A784 || !ov00_0221A784->state) {
        return;
    }
    if (ov00_021EC230()) {
        return;
    }
    switch (ov00_0221A784->state) {
    case 1:
        switch (ov00_021FE3D8()) {
        case 1:
            ov00_021FFD50((u8 *)ov00_0221A784 + 0x1c, *(int *)((u8 *)ov00_0221A784 + 0xa0), 0);
            if (ov00_021EE8B4()) {
                return;
            }
            ov00_021FFDD0((u8 *)ov00_0221A784 + 0x1c, 0, ov00_021EEC94, 0);
            if (ov00_021EE8B4()) {
                return;
            }
            ov00_021FFDD0((u8 *)ov00_0221A784 + 0x1c, 3, ov00_021EECCC, 0);
            if (ov00_021EE8B4()) {
                return;
            }
            ov00_021FFDD0((u8 *)ov00_0221A784 + 0x1c, 1, ov00_021F0290, 0);
            if (ov00_021EE8B4()) {
                return;
            }
            ov00_021FFDD0((u8 *)ov00_0221A784 + 0x1c, 2, ov00_021F0338, 0);
            if (ov00_021EE8B4()) {
                return;
            }
            ov00_021EE800(2);
            ov00_021EF274();
            break;
        case 2:
            ov00_021EF3C0(3, -20110);
            return;
        case 3:
            ov00_021EF3C0(4, -20101);
            return;
        }
        break;
    case 2:
        ov00_021EF2AC();
        break;
    case 3:
    case 4:
        ov00_021F0028();
        ov00_021F1DE4(0);
        break;
    case 5:
        ov00_021F1DE4(1);
        ov00_021F0028();
        break;
    case 6:
        ov00_021F9A8C();
        ov00_021F0028();
        if (ov00_0221A784->mode == 2 || ov00_0221A784->mode == 3) {
            ov00_021F1DE4(1);
        } else if (*(int *)ov00_0221A784) {
            ov00_021F1DE4(0);
        }
        break;
    }
    if (ov00_0221A784->cleanup == 1) {
        if (ov00_0221A784->resource) {
            ov00_02210D18(ov00_0221A784->resource);
            ov00_0221A784->resource = 0;
        }
        ov00_0221A784->cleanup = 0;
    }
}

int ov00_021EDD6C(const u16 *name, int unused, void *a, void *b) {
    int length;
    if (!name) {
        return 0;
    }
    if (ov00_021EC230() || ov00_0221A784->state) {
        return 0;
    }
    ov00_0221A784->a = a;
    ov00_0221A784->b = b;
    if (!name || !*name) {
        length = 0;
    } else {
        MI_CpuClear16(ov00_0221A784->name, 52);
        if (ov00_021ED704(name) > 25) {
            length = 25;
        } else {
            length = ov00_021ED704(name);
        }
        MI_CpuCopy16(name, ov00_0221A784->name, length * 2);
    }
    ov00_0221A784->name[length] = 0;
    if (ov00_021EC724() != 4) {
        ov00_021EF3C0(2, -60010);
        return 1;
    }
    ov00_021EE800(1);
    ov00_021FE228(ov00_0221A784->token);
    return 1;
}

int ov00_021EDE74(int unused, void *a, void *b, int c, int d, int e, int f) {
    if (ov00_021EC230() || ov00_0221A784->state < 3 || ov00_0221A784->state == 4) {
        return 0;
    }
    ov00_0221A784->c = a;
    ov00_0221A784->d = b;
    ov00_021EE800(4);
    ov00_021F018C(ov00_0221A784->object, ov00_0221A784->other, ov00_021EEAD4, 0, c, d, e, f);
    return 1;
}
