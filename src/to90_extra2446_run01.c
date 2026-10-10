#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#pragma force_active on

typedef union {
    u32 word;

    struct {
        unsigned int a : 8, b : 8, c : 9, d : 7;
    } bits;
} Obj;

typedef struct {
    void *window;
    Obj *obj;
    u8 data[12];
    u8 index;
} Work;

extern Work *ov13_0224F544;
extern const u16 ov13_02245480[];
extern const u8 ov13_022454C0[];
extern void ov13_02240D80(void *, int);
extern void ov13_02240CC0(void *, int, int, int, int, int, int, void *);
extern void ov13_02240E2C(void *);

static inline void draw(void *p, u16 x, volatile u16 *r, u16 *pair) {
    ov13_02240CC0(p, x, r[1], r[2], r[3], 2, 0x480, pair);
}

static inline int clamp(int i) {
    if (i > 3) {
        i = 3;
    }
    return i;
}

void ov13_0223237C(void);
void ov13_02232440(void);

void ov13_0223237C(void) {
    Work *initial = ov13_0224F544;
    u16 rect[4];
    u16 pair[2];
    int i;
    rect[0] = ov13_02245480[7];
    rect[1] = ov13_02245480[8];
    rect[2] = 11;
    rect[3] = 16;
    ov13_02240D80(initial->window, 0);
    pair[1] = 0;
    for (i = 0; i < 12; i++) {
        u16 x = ov13_022454C0[i];
        pair[0] = ov13_0224F544->data[i];
        rect[0] = x;
        draw(ov13_0224F544->window, x, rect, pair);
    }
    ov13_02240E2C(ov13_0224F544->window);
}
