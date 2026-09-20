#include "to54_atomic_w13_private.h"

static inline void SetPosition(Ov07CallbackPosition *work, const VecFx32 *offset) {
    work->position.x = offset->x + (*work->base)->position.x;
    work->position.y = offset->y + (*work->base)->position.y;
    work->position.z = offset->z + (*work->base)->position.z;
}

void ov07_022202D8(int kind, int scale, VecFx32 *vec) {
    fx32 *z = &vec->z;
    switch (kind) {
    case 1:
        vec->x *= scale;
        break;
    case 2:
        vec->y *= scale;
        break;
    case 3:
        *z *= scale;
        break;
    case 4:
        vec->x *= scale;
        vec->y *= scale;
        break;
    case 5:
        vec->x *= scale;
        *z *= scale;
        break;
    case 6:
        vec->y *= scale;
        *z *= scale;
        break;
    case 7:
        vec->x *= scale;
        vec->y *= scale;
        *z *= scale;
        break;
    }
}
