#include "to54_atomic_w13_private.h"

static inline void SetPosition(Ov07CallbackPosition *work, const VecFx32 *offset) {
    work->position.x = offset->x + (*work->base)->position.x;
    work->position.y = offset->y + (*work->base)->position.y;
    work->position.z = offset->z + (*work->base)->position.z;
}

void ov07_022207D0(void *work, int a, int b) {
    void *ctx = sub_02015504();
    int c = ov07_0221C468(ctx);
    int d = ov07_0221C470(ctx);
    ov07_0222050C(ctx, work, c, d, a, b);
}
