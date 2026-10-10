#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

typedef struct {
    u32 bits;
} PackedSprite;

typedef struct {
    u8 pad[0x10];
    PackedSprite *s0;
    u8 pad2[8];
    PackedSprite *s1;
    u8 pad3[8];
    PackedSprite *s2;
    u8 pad4[8];
    PackedSprite *s3;
} Work;

extern Work *ov13_0224F4C0;
extern void ov13_0222AAFC(int, int), ov13_022425C8(void *, void (*)(void *));
void ov13_0222A0DC(void *), ov13_0222A160(void *), ov13_0222A1E4(void *), ov13_0222A268(void *), ov13_0222A2EC(void *);

static inline void GetPos(PackedSprite *s, volatile int *x, volatile int *y) {
    u32 v = s->bits;
    *x = (v & 0x1ff0000) >> 16;
    *y = v & 255;
}

#pragma force_active on
#pragma opt_propagation off

void ov13_0222A268(void *p) {
    volatile struct {
        int y, x;
    } pos;

    u32 raw = ov13_0224F4C0->s3->bits;
    int x;
    int y;
    u32 masked = raw & 0x1ff0000;
    y = raw & 255;
    y -= 12;
    x = masked >> 16;
    pos.x = x;
    pos.y = y;
    if (y > 145) {
        ov13_0222AAFC(3, y);
        return;
    }
    ov13_0222AAFC(3, 145);
    ov13_0222AAFC(4, 192);
    ov13_022425C8(p, ov13_0222A2EC);
}
