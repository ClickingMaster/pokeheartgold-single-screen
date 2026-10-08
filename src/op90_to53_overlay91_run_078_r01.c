#pragma force_active on
#include "global.h"
void ov91_02261208(VecFx32 *p, VecFx32 *d, VecFx32 *n, fx32 dist, VecFx32 *out, fx32 *outT);
// @fn ov91_02261208
void ov91_02261208(VecFx32 *p, VecFx32 *d, VecFx32 *n, fx32 dist, VecFx32 *out, fx32 *outT) {
    fx32 t;
    t = FX_Div(dist + (-FX_Mul(n->x, p->x) - FX_Mul(n->y, p->y) - FX_Mul(n->z, p->z)),
               FX_Mul(n->z, d->z) + (FX_Mul(n->x, d->x) + FX_Mul(n->y, d->y)));
    out->x = p->x + FX_Mul(d->x, t);
    out->y = p->y + FX_Mul(d->y, t);
    out->x = p->z + FX_Mul(d->z, t);
    *outT = t;
}
