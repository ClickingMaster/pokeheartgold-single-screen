#include "to56_r2_w16_private.h"

int ov07_0222FFD0(void *arg) {
    Ov07W16Work *p = arg;
    int i;
    int result;

    if (p->count < 15) {
        p->timer++;
        if (p->timer > 6) {
            p->timer = 0;
            ov07_0222FEB0(p->sprites[p->count], &p->records[p->count], p->argA, p->argB);
            p->count++;
        }
    }
    for (i = 0; i < p->count; i++) {
        result = ov07_0222FF04(p->sprites[i], &p->records[i]);
    }
    if (p->count >= 15 && result == 1) {
        return 1;
    }
    return 0;
}
