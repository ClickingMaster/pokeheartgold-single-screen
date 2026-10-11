#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

typedef struct {
    u32 a;

    union {
        u16 word;

        struct {
            u16 tile : 10;
            u16 palette : 2;
            u16 rest : 4;
        } bits;
    } attr;
} Object;

typedef struct {
    u8 p[0x10];
    Object *a[5], *b[5];
    u8 p2[0xa];
    u16 c[3], d[4];
    u8 p3;
    u8 count;
} Work;

typedef struct {
    int a;
    Work *w;
} Global;

extern Global ov13_0224F538;
extern u8 ov13_0224544C[];
extern u8 ov13_02245450[];
extern Object *WifiSetupOamTemplate_AllocOam(int, int);
extern void WifiSetupOamTemplate_Copy(int, u8, Object *);
void ov13_0222FD10(void);

void ov13_0222FD10(void) {
    int n = ov13_0224F538.w->count;
    if (n > 5) {
        n = 5;
    }
    int i = 0;
    for (; i < n; i++) {
        ov13_0224F538.w->a[i] = WifiSetupOamTemplate_AllocOam(0, 46);
        ov13_0224F538.w->b[i] = WifiSetupOamTemplate_AllocOam(0, 24);
    }
    for (i = 0; (u32)i < 3; i++) {
        WifiSetupOamTemplate_Copy(0, ov13_0224544C[i], ov13_0224F538.w->a[0]);
        ov13_0224F538.w->c[i] = ov13_0224F538.w->a[0]->attr.word & 1023;
    }
    for (i = 0; (u32)i < 4; i++) {
        WifiSetupOamTemplate_Copy(0, ov13_02245450[i], ov13_0224F538.w->b[0]);
        ov13_0224F538.w->d[i] = ov13_0224F538.w->b[0]->attr.word & 1023;
    }
    for (i = 0; i < n; i++) {
        ov13_0224F538.w->a[i]->attr.bits.palette = 3;
        ov13_0224F538.w->b[i]->attr.bits.palette = 3;
    }
}
