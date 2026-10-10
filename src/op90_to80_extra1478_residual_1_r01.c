#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
int NNS_G3dScrPosToWorldLine(int px, int py, VecFx32 *near, VecFx32 *far);
fx64 FX_GetDivResultFx64c(void);
fx32 FX_Div(fx32, fx32);
void FX_InvAsync(fx32);
extern void NNS_G3dGlbGetViewPort(int *, int *, int *, int *);
extern const MtxFx44 *NNS_G3dGlbGetInvVP(void);

int NNS_G3dScrPosToWorldLine(int px, int py, VecFx32 *near, VecFx32 *far) {
    const MtxFx44 *m;
    int nx, ny, height;
    int x1, y1, x2, y2;
    int result;
    fx32 vw;
    int fw;
    int fx;
    int fy;
    int vx;
    int vy;
    int vz;
    int fz;
    NNS_G3dGlbGetViewPort(&x1, &y1, &x2, &y2);
    height = y2 - y1;
    nx = FX_Div((px - x1) << 12, (x2 - x1) << 12);
    ny = FX_Div((py + y1 - 191) << 12, (-height) << 12);
    if (nx < 0 || ny < 0 || nx > 4096 || ny > 4096) {
        result = -1;
    } else {
        result = 0;
    }
    fx64 inv;
    nx = (nx - 2048) * 2;
    ny = (ny - 2048) * 2;
    m = NNS_G3dGlbGetInvVP();
    vw = m->_33 + (int)(((fx64)nx * m->_03 + (fx64)ny * m->_13) >> 12);
    FX_InvAsync(vw - m->_23);
    vx = (int)(((fx64)nx * m->_00 + (fx64)ny * m->_10) >> 12) + m->_30;
    vy = m->_31 + (int)(((fx64)nx * m->_01 + (fx64)ny * m->_11) >> 12);
    vz = m->_32 + (int)(((fx64)nx * m->_02 + (fx64)ny * m->_12) >> 12);
    if (far) {
        fx = vx + m->_20;
        fy = vy + m->_21;
        fz = vz + m->_22;
        fw = vw + m->_23;
    }
    vx -= m->_20;
    vy -= m->_21;
    vz -= m->_22;
    inv = FX_GetDivResultFx64c();
    if (far != 0) {
        FX_InvAsync(fw);
    }
    near->x = (int)((inv * vx + 0x80000000LL) >> 32);
    near->y = (int)((inv * vy + 0x80000000LL) >> 32);
    near->z = (int)((inv * vz + 0x80000000LL) >> 32);
    if (far) {
        inv = FX_GetDivResultFx64c();
        far->x = (int)((inv * fx + 0x80000000LL) >> 32);
        far->y = (int)((inv * fy + 0x80000000LL) >> 32);
        far->z = (int)((inv * fz + 0x80000000LL) >> 32);
    }
    return result;
}
