#pragma force_active on
#include <nitro/types.h>
double frexp(double, int *);
#define HI(d) (((s32 *)&(d))[1])
#define LO(d) (((s32 *)&(d))[0])
// @fn frexp
double frexp(double x, int *eptr) {
    s32 hx = HI(x);
    s32 ix = hx & 0x7fffffff;
    s32 lx = LO(x);
    *eptr = 0;
    if (ix >= 0x7ff00000 || (ix | lx) == 0) {
        return x;
    }
    if (ix < 0x00100000) {
        x *= 18014398509481984.0;
        hx = HI(x);
        ix = hx & 0x7fffffff;
        *eptr = -54;
    }
    *eptr += (ix >> 20) - 1022;
    hx = (hx & 0x800fffff) | 0x3fe00000;
    HI(x) = hx;
    return x;
}
