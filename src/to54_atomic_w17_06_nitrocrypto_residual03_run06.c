#include "to54_atomic_w17_nitrocrypto_residual03_private.h"

BOOL BN_uadd(BIGNUM *r, const BIGNUM *a, const BIGNUM *b)
{
    int i;

    if (a->top < b->top) {
        const BIGNUM *tmp = a;
        a = b;
        b = tmp;
    }
    int max = a->top;
    int min = b->top;

    if (bn_wexpand(r, max + 1) == NULL) {
        return FALSE;
    }

    r->top = max;

    u32 *ap = a->d;
    u32 *bp = b->d;
    u32 *rp = r->d;
    u32 carry = bn_add_words(rp, ap, bp, min);

    rp += min;
    ap += min;
    bp += min;
    i = min;

    if (carry) {
        while (i < max) {
            i++;
            u32 t1 = *ap++;
            if ((*rp++ = (t1 + 1) & BN_MASK2) >= t1) {
                carry = 0;
                break;
            }
        }
        if (i >= max && carry) {
            *rp++ = 1;
            r->top++;
        }
    }

    if (rp != ap) {
        for (; i < max; i++) {
            *rp++ = *ap++;
        }
    }

    return TRUE;
}
