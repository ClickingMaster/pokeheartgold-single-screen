#include "to54_atomic_w17_nitrocrypto_residual03_private.h"

BOOL BN_mul(BIGNUM *r, BIGNUM *a, BIGNUM *b, BN_CTX *ctx)
{
    int top;

    int al = a->top;
    int bl = b->top;

    if (al == 0 || bl == 0) {
        (void)(void)BN_zero(r);
        return TRUE;
    }

    top = al + bl;
    BOOL neg = a->neg ^ b->neg;

    BIGNUM *rr;
    if (r == a || r == b) {
        rr = &ctx->bn[ctx->tos + 1];
    } else {
        rr = r;
    }

    /* asymmetric and >= 4 */
    if (bn_wexpand(rr, top) == NULL) {
        return FALSE;
    }

    rr->top = top;
    bn_mul_normal(rr->d, a->d, al, b->d, bl);
    r->neg = neg;

    bn_fix_top(rr);
    if (r != rr) {
        (void)BN_copy(r, rr);
    }
    return TRUE;
}
