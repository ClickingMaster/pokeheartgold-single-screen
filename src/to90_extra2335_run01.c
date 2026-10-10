#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#pragma force_active on

typedef struct {
    u8 p[99];
    s8 state, old;
} Work;

extern Work *ov13_0224F4C0;
extern const s8 ov13_022452DA[][4];
void ov13_0222AE24(void), ov13_0223ABB8(int), ov13_0222AEB0(int);

void ov13_0222AEB0(int action) {
    s8 old = ov13_0224F4C0->state;
    ov13_0224F4C0->state = ov13_022452DA[old][action];
    if (ov13_0224F4C0->state == 13 && (action == 1 || action == 3)) {
        ov13_0224F4C0->old = old;
    } else if (ov13_0224F4C0->state == -1) {
        if (ov13_0224F4C0->old == 1 || ov13_0224F4C0->old == 10) {
            ov13_0224F4C0->state = 10;
        } else {
            ov13_0224F4C0->state = 11;
        }
    } else if (ov13_0224F4C0->state == -2) {
        if (ov13_0224F4C0->old == 1 || ov13_0224F4C0->old == 10) {
            ov13_0224F4C0->state = 1;
        } else {
            ov13_0224F4C0->state = 2;
        }
    }
    ov13_0222AE24();
    ov13_0223ABB8(8);
}
