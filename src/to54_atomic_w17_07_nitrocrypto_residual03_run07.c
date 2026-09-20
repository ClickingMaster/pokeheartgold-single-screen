#include "to54_atomic_w17_nitrocrypto_residual03_private.h"

BOOL BN_add(BIGNUM *r, const BIGNUM *a, const BIGNUM *b)
{
    /*  a +  b	a+b
     *  a + -b	a-b
     * -a +  b	b-a
     * -a + -b	-(a+b)
     */
    if (a->neg ^ b->neg) {
        /* only one is negative */
        if (a->neg) {
            const BIGNUM *tmp = a;
            a = b;
            b = tmp;
        }

        /* we are now a - b */

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

    if (a->neg) { /* both are neg */
        r->neg = TRUE;
    } else {
        r->neg = FALSE;
    }

    if (!BN_uadd(r, a, b)) {
        return FALSE;
    }
    return TRUE;
}
