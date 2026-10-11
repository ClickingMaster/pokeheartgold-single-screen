#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#pragma force_active on

typedef struct {
    u8 p[0x30];
    u32 *a;
    u8 p2[0x2c];
    u32 *b;
    u8 p3[0x2c];
    u32 *c;
    u8 p4[0x2c];
    u32 *d;
    u8 p5[0x59];
    u8 mode;
} State;

extern State *ov13_0224F4BC;
extern void ov13_022290F0(int, int, int);
extern void WifiSetupTask_SetFunc(void *, void (*)(void *));
void ov13_02228444(void *), ov13_022284D8(void *), ov13_0222856C(void *), ov13_02228600(void *), ov13_02228694(void *);
#pragma force_active on
#pragma opt_propagation off

void ov13_02228600(void *w) {
    u32 word = *ov13_0224F4BC->d;
    u32 flag = word & 0x1ff0000;
    int size;

    volatile struct {
        int size;
        u32 flag;
    } info;

    word &= 255;
    size = word - 12;
    word = flag >> 16;
    info.flag = word;
    info.size = size;
    if (size > 147) {
        ov13_022290F0(ov13_0224F4BC->mode, 3, size);
        return;
    }
    ov13_022290F0(ov13_0224F4BC->mode, 3, 147);
    ov13_022290F0(ov13_0224F4BC->mode, 4, 192);
    WifiSetupTask_SetFunc(w, ov13_02228694);
}
