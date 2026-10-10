#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#pragma force_active on

typedef struct {
    u32 word;
    u16 attr;
} W40085Sprite;

typedef struct {
    void *base[4];
    W40085Sprite *first[10], *second[2];
    void *third[2];
    u8 p[24];
    u8 state, pad[2];
    s8 selected;
    u8 q[2];
    u8 available, enabled, other, held;
} W40085State;

extern W40085State *ov13_0224F4C0;
extern u8 ov13_02245314[];
extern u8 ov13_02245224[];
extern u8 ov13_02245220[];
extern int ov13_022416D4(int);
extern int ov13_02241694(int);
extern int ov13_02241714(int);
extern void ov13_0222AEB0(int);
extern void ov13_0223ABB8(int);
void ov13_0222A960(void);

void ov13_0222A960(void) {
    W40085State *s;
    if (ov13_022416D4(32)) {
        ov13_0222AEB0(0);
    }
    if (ov13_022416D4(64)) {
        ov13_0222AEB0(1);
    }
    if (ov13_022416D4(16)) {
        ov13_0222AEB0(2);
    }
    if (ov13_022416D4(128)) {
        ov13_0222AEB0(3);
    }
    if (ov13_02241694(1)) {
        s = ov13_0224F4C0;
        int selected = s->selected;
        if (selected < 10) {
            if (s->enabled) {
                s->state = ov13_02245314[selected];
            } else {
                ov13_0223ABB8(9);
            }
            return;
        } else if (selected - 10 < 2) {
            if ((selected - 10 == 0 && s->available == 0) || (selected - 10 == 1 && s->other == 0)) {
                ov13_0223ABB8(9);
                return;
            }
            s->state = ov13_02245224[selected - 10];
            return;
        } else {
            s->state = ov13_02245220[selected - 12];
        }
    }
    if (ov13_022416D4(2)) {
        s = ov13_0224F4C0;
        if (s->available == 0) {
            if (s->held) {
                return;
            }
            ov13_0223ABB8(9);
            ov13_0224F4C0->held = 1;
            return;
        }
        s->state = 16;
        return;
    }
    if (ov13_02241714(2)) {
        ov13_0224F4C0->held = 0;
    }
}
