#include "to54_atomic_w17_nitrocrypto_residual03_private.h"

BOOL BN_rshift(BIGNUM *r, BIGNUM *a, int n)
{
    int i, j;

    int nw = n / BN_BITS2;
    int lb;
    int rb = n % BN_BITS2;
    lb = BN_BITS2 - rb;
    if (nw > a->top) {
        (void)BN_zero(r);
        return TRUE;
    }
    if (r != a) {
        if (bn_wexpand(r, a->top - nw + 2) == NULL) {
            return FALSE;
        }

        r->neg = a->neg;
    }
    u32 *f = &a->d[nw];
    u32 *t = r->d;
    j = a->top - nw;
    r->top = j;

    if (rb == 0) {
        for (i = j + 1; i > 0; i--) {
            *t++ = *f++;
        }
    } else {
        u32 l = *f++;
        for (i = 1; i < j; i++) {
            u32 tmp = (l >> rb) & BN_MASK2;
            l = *f++;
            *t++ = (tmp | (l << lb)) & BN_MASK2;
        }
        *t++ = (l >> rb) & BN_MASK2;
        *t = 0;
    }
    bn_fix_top(r);
    return TRUE;
}
