#pragma force_active on
#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#include <string.h>
extern void srand(unsigned int);

typedef struct {
    u8 state;
    u8 p[0xff];
    int a, b, c, d;
    u8 p2[0x94];

    struct {
        int a, b;
    } entries[6];

    u8 p3[0x244];
    int x;
    u8 p4[4];
    int y;
    u8 p5[0x3c];
    int z;
    u8 p6[8];
    int arg1, arg2;
} Work;

extern void *ov00_021EC3A8(u32);
extern void ov00_021EC3D8(void *);
extern int ov00_02207B34(Work **);
extern int ov00_02200A98(Work **);
extern void ov00_0220A3BC(Work **, void *);
extern void ov00_02200A3C(Work **);
extern void ov00_021FF3CC(void);
extern u32 ov00_021FF368(void);
extern u8 ov00_0221800C[];
int ov00_022008F4(Work **, int, int);

int ov00_022008F4(Work **out, int a, int b) {
    Work *w;
    int i, result;
    *out = 0;
    w = ov00_021EC3A8(0x490);
    if (w == 0) {
        return 1;
    }
    memset(w, 0, 0x490);
    w->state = 0;
    w->x = 0;
    w->a = 1;
    w->b = 0;
    w->c = 0;
    w->d = 0;
    w->arg1 = a;
    w->arg2 = b;
    if (ov00_02207B34(&w) == 0) {
        ov00_021EC3D8(w);
        w = 0;
        return 1;
    }
    w->y = 0;
    for (i = 0; i < 6; i++) {
        w->entries[i].a = 0;
        w->entries[i].b = 0;
    }
    w->z = 0;
    ov00_0220A3BC(&w, ov00_0221800C);
    result = ov00_02200A98(&w);
    if (result) {
        ov00_02200A3C(&w);
        return result;
    }
    ov00_021FF3CC();
    srand(ov00_021FF368());
    *out = w;
    return 0;
}
