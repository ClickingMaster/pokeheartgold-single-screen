#include "to54_atomic_w17_nitrocrypto_residual03_private.h"

BOOL BN_lshift(BIGNUM *r, const BIGNUM *a, int n)
{
    if (bn_wexpand(r, a->top + (n / BN_BITS2) + 1) == NULL) {
        return 0;
    }

    r->neg = a->neg;
    int nw = n / BN_BITS2;
    int lb = n % BN_BITS2;
    int rb = BN_BITS2 - lb;
    u32 *f = a->d;
    u32 *t = r->d;
    t[a->top + nw] = 0;
    if (lb == 0) {
        for (int i = a->top - 1; i >= 0; i--) {
            t[nw + i] = f[i];
        }
    } else {
        for (int i = a->top - 1; i >= 0; i--) {
            u32 l = f[i];
            t[nw + i + 1] |= (l >> rb) & BN_MASK2;
            t[nw + i] = (l << lb) & BN_MASK2;
        }
    }
    MI_CpuFill8(t, 0, nw * sizeof(t[0]));
    r->top = a->top + nw + 1;
    bn_fix_top(r);
    return TRUE;
}
