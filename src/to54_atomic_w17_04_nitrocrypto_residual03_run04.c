#include "to54_atomic_w17_nitrocrypto_residual03_private.h"

BOOL BN_sub(BIGNUM *r, const BIGNUM *a, const BIGNUM *b)
{
    BOOL add = FALSE, neg = FALSE;

    /*  a -  b	a-b
     *  a - -b	a+b
     * -a -  b	-(a+b)
     * -a - -b	b-a
     */
    if (a->neg) {
        if (b->neg) {
            const BIGNUM *tmp = a;
            a = b;
            b = tmp;
        } else {
            add = TRUE;
            neg = TRUE;
        }
    } else {
        if (b->neg) {
            add = TRUE;
            neg = FALSE;
        }
    }

    if (add) {
        if (!BN_uadd(r, a, b)) {
            return FALSE;
        }
        r->neg = neg;
        return TRUE;
    }

    /* We are actually doing a - b :-) */

    int max = a->top > b->top ? a->top : b->top;
    if (bn_wexpand(r, max) == NULL) {
        return FALSE;
    }

    if (BN_ucmp(a, b) < 0) {
        if (!BN_usub(r, b, a)) {
            return FALSE;
        }
        r->neg = TRUE;
    } else {
        if (!BN_usub(r, a, b)) {
            return FALSE;
        }
        r->neg = FALSE;
    }

    return TRUE;
}
