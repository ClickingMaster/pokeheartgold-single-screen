#pragma force_active on
#include <nitro/types.h>
double ldexp(double, int);
double copysign(double, double);
int __fpclassifyd(double);
#define HI(d) (((s32 *)&(d))[1])
#define LO(d) (((s32 *)&(d))[0])
// @fn ldexp
double ldexp(double x, int n) {
    s32 k, hx, lx;
    if (__fpclassifyd(x) <= 2 || x == 0.0) {
        return x;
    }
    hx = HI(x);
    lx = LO(x);
    k = (hx & 0x7ff00000) >> 20;
    if (k == 0) {
        if ((lx | (hx & 0x7fffffff)) == 0) {
            return x;
        }
        x *= 18014398509481984.0;
        hx = HI(x);
        k = ((hx & 0x7ff00000) >> 20) - 54;
        if (n < -50000) {
            return 1.0e-300 * x;
        }
    }
    if (k == 0x7ff) {
        return x + x;
    }
    k = k + n;
    if (k > 0x7fe) {
        return 1.0e300 * copysign(1.0e300, x);
    }
    if (k > 0) {
        HI(x) = (hx & 0x800fffff) | (k << 20);
        return x;
    }
    if (k <= -54) {
        if (n > 50000) {
            return 1.0e300 * copysign(1.0e300, x);
        } else {
            return 1.0e-300 * copysign(1.0e-300, x);
        }
    }
    k += 54;
    HI(x) = (hx & 0x800fffff) | (k << 20);
    return x * 5.55111512312578270212e-17;
}
