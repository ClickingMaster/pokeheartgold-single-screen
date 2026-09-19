#include "overlay_96_residual_97_residual_3_prefix_exact_private.h"
void ov96_0220CD84(R97PrefixItem *p, int mode) {
    if (mode != 1) {
        if (mode == 2) {
            p->w6.raw &= 0xFFFFF800;
        }
    } else {
        int value = p->v5.b.count - 10;
        if (value > 100) {
            value = 100;
        } else if (value < 0) {
            value = 0;
        }
        p->v5.b.count = value;
        p->v5.b.state++;
        if (p->v5.b.state >= 3) {
            p->v5.b.state = 0;
        }
    }
    p->v5.b.mid = mode;
    p->v5.b.field8 = 0;
}
