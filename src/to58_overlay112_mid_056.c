#include "to58_overlay112_mid_private.h"

int ov112_021EAC34(Ov112State *p) {
    p->state = ov112_021FF6C4[p->state](p);
    if (p->state == 27) {
        p->state = 0;
        return 3;
    }
    return 2;
}
