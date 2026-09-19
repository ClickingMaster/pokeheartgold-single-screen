#include "private_w19.h"

void ov91_0225F3A0(W19Slots *p, u32 value, u32 slot) {
    p->value[slot] = value;
    p->mask |= 1 << slot;
    if (value != 0) {
        if (p->selected != 0xffff) {
            if (p->value[p->selected] >= value) {
                return;
            }
            p->selected = slot;
            return;
        }
        p->selected = slot;
    }
}


void ov91_0225F3D4(W19Slots *p) {
    p->mask = 0;
    p->selected = 0xffff;
}


u32 ov91_0225F3E4(W19Slots *p) {
    return p->selected;
}


BOOL ov91_0225F3E8(W19Slots *p, u32 n) {
    int i;
    int count = 0;
    for (i = 0; i < 4; i++) {
        if (p->mask & (1 << i)) count++;
    }
    return n <= count;
}
