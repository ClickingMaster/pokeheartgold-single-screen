#include "to55_overlay91_run_040_private.h"

void ov91_0225E9F4(Ov91R40Work *work, void *outer) {
    VecFx32 a;
    VecFx32 b;
    int result;

    ov91_0225E728(work, &a);
    result = ov91_0225DE8C((u8 *)outer + 0x19cc, &work->position, &a, &b);
    ov91_0225EA54(work, &a);
    if (result == 3 || result == 1) {
        ov91_0225EA54(work, &b);
    }
    work->counter++;
    work->timer--;
    if (work->timer <= 0) {
        ov91_0225E3F4(outer, work, 1, work->value3A);
    }
}

void ov91_0225EA54(Ov91R40Work *work, const VecFx32 *delta) {
    VecFx32 horizontal;

    VEC_Add(&work->position, delta, &work->position);
    horizontal.y = 0;
    horizontal.x = work->position.x;
    horizontal.z = work->position.z;
    work->magnitude = VEC_Mag(&horizontal);
}

void ov91_0225EA7C(Ov91R40Work *work) {
    if (gSystem.touchHeld == 0) {
        ov91_0225EA98(work);
    } else {
        ov91_0225EC7C(work);
    }
}
