#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
extern void VEC_CrossProduct(const VecFx32 *, const VecFx32 *, VecFx32 *);
void MTX_LookAt(const VecFx32 *, const VecFx32 *, const VecFx32 *, MtxFx43 *);

void MTX_LookAt(const VecFx32 *eye, const VecFx32 *up, const VecFx32 *target, MtxFx43 *m) {
    VecFx32 z, x, y;
    z.x = eye->x - target->x;
    z.y = eye->y - target->y;
    z.z = eye->z - target->z;
    VEC_Normalize(&z, &z);
    VEC_CrossProduct(up, &z, &x);
    VEC_Normalize(&x, &x);
    VEC_CrossProduct(&z, &x, &y);
    m->_00 = x.x;
    m->_01 = y.x;
    m->_02 = z.x;
    m->_10 = x.y;
    m->_11 = y.y;
    m->_12 = z.y;
    m->_20 = x.z;
    m->_21 = y.z;
    m->_22 = z.z;
    m->_30 = -VEC_DotProduct(eye, &x);
    m->_31 = -VEC_DotProduct(eye, &y);
    m->_32 = -VEC_DotProduct(eye, &z);
}
