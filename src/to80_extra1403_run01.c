#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#pragma force_active on
extern int *ov13_0224F4B0;
extern void ov13_02227D18(int);
extern int ov13_022401B0(int, int, int);
extern void ov13_02240008(int, int, int);
extern void ov13_0223FF18(int, int, int, int);
void ov13_02227C28(int, int, int, int);

void ov13_02227C28(int a, int b, int c, int d) {
    int j = 6, i;
    ov13_02227D18(a);
    for (i = 0; i < 4; i++, j++) {
        ov13_0224F4B0[i] = ov13_022401B0(0, j, 1);
        ov13_02240008(ov13_0224F4B0[i], -1, 1);
    }
    ov13_0223FF18(ov13_0224F4B0[0], -1, a, c);
    ov13_0223FF18(ov13_0224F4B0[1], -1, b, c);
    ov13_0223FF18(ov13_0224F4B0[2], -1, a, d);
    ov13_0223FF18(ov13_0224F4B0[3], -1, b, d);
}
