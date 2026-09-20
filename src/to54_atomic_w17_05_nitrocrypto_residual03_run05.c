#include "to54_atomic_w17_nitrocrypto_residual03_private.h"

BOOL BN_usub(BIGNUM *r, const BIGNUM *a, const BIGNUM *b)
{
    u32 t1, t2;

    if (a->top < b->top) /* hmm... should not be happening */
    {
        return FALSE;
    }

    int max = a->top;
    int min = b->top;
    if (bn_wexpand(r, max) == NULL) {
        return FALSE;
    }

    u32 *ap = a->d;
    u32 *bp = b->d;
    u32 *rp = r->d;

    BOOL carry = FALSE;
    int i;
    for (i = 0; i < min; i++) {
        t1 = *ap++;
        t2 = *bp++;
        if (carry) {
            carry = t1 <= t2;
            t1 = (t1 - t2 - 1) & BN_MASK2;
        } else {
            carry = t1 < t2;
            t1 = (t1 - t2) & BN_MASK2;
        }
        *rp++ = t1 & BN_MASK2;
    }

    if (carry) /* subtracted */
    {
        while (i < max) {
            i++;
            t1 = *(ap++);
            t2 = (t1 - 1) & BN_MASK2;
            *rp++ = t2;
            if (t1 > t2) {
                break;
            }
        }
    }

    if (rp != ap) {
        while (TRUE) {
            if (i++ >= max) {
                break;
            }
            rp[0] = ap[0];
            if (i++ >= max) {
                break;
            }
            rp[1] = ap[1];
            if (i++ >= max) {
                break;
            }
            rp[2] = ap[2];
            if (i++ >= max) {
                break;
            }
            rp[3] = ap[3];
            rp += 4;
            ap += 4;
        }
    }

    r->top = max;
    bn_fix_top(r);
    return TRUE;
}
