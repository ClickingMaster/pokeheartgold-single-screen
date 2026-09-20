#include "to54_atomic_w17_nitrocrypto_residual03_private.h"

BOOL BN_div(BIGNUM *dv, BIGNUM *rm, const BIGNUM *num, const BIGNUM *divisor, BN_CTX *ctx)
{
    int norm_shift, i, j, loop;
    BIGNUM *tmp, wnum, *snum, *sdiv, *res;
    u32 *resp, *wnump;
    u32 d0, d1;

    if (BN_is_zero(divisor)) {
        return FALSE;
    }

    if (BN_ucmp(num, divisor) < 0) {
        if (rm != NULL) {
            if (BN_copy(rm, num) == NULL) {
                return FALSE;
            }
        }
        if (dv != NULL) {
            (void)BN_zero(dv);
        }
        return TRUE;
    }

    tmp = &ctx->bn[ctx->tos];
    tmp->neg = FALSE;
    snum = &ctx->bn[ctx->tos + 1];
    sdiv = &ctx->bn[ctx->tos + 2];
    if (dv == NULL) {
        res = &ctx->bn[ctx->tos + 3];
    } else {
        res = dv;
    }

    /* First we normalise the numbers */
    norm_shift = BN_BITS2 - BN_num_bits(divisor) % BN_BITS2;
    if (!BN_lshift(sdiv, divisor, norm_shift)) {
        return FALSE;
    }
    sdiv->neg = FALSE;
    norm_shift += BN_BITS2;
    if (!BN_lshift(snum, num, norm_shift)) {
        return FALSE;
    }
    snum->neg = FALSE;
    int div_n = sdiv->top;
    int num_n = snum->top;
    loop = num_n - div_n;

    /* Lets setup a 'window' into snum
     * This is the part that corresponds to the current
     * 'area' being divided */
    BN_init(&wnum);
    wnum.d = &snum->d[loop];
    wnum.top = div_n;
    wnum.max = snum->max + 1; /* a bit of a lie */

    /* Get the top 2 words of sdiv */
    d0 = sdiv->d[div_n - 1];
    d1 = div_n == 1 ? 0 : sdiv->d[div_n - 2];

    /* pointer to the 'top' of snum */
    wnump = &snum->d[num_n - 1];

    /* Setup to 'res' */

    if (bn_wexpand(res, loop + 1) == NULL) {
        goto err;
    }

    res->neg = num->neg ^ divisor->neg;
    res->top = loop;
    resp = &res->d[loop - 1];

    /* space for temp */
    if (bn_wexpand(tmp, div_n + 1) == NULL) {
        goto err;
    }

    if (BN_ucmp(&wnum, sdiv) >= 0) {
        if (!BN_usub(&wnum, &wnum, sdiv)) {
            goto err;
        }
        *resp = 1;
        res->d[res->top - 1] = 1;
    } else {
        res->top--;
    }
    resp--;

    for (i = 0; i < loop - 1; i++) {
        u32 q;

        wnum.d--;
        wnum.top++;
        u32 n0 = wnump[0];
        u32 n1 = wnump[-1];
        if (n0 == d0) {
            q = BN_MASK2;
        } else { /* n0 < d0 */
            q = bn_div_words(n0, n1, d0);
        }

        u32 t1h = n0;
        u32 t1l = n1;
        while (TRUE) {
            u32 t2l = LBITS(d1);
            u32 t2h = HBITS(d1);
            u32 t3l, t3h;
            u32 ql = LBITS(q);
            u32 qh = HBITS(q);
            mul64(t2l, t2h, ql, qh); /* t2=(BN_ULLONG)d1*q; */

            u32 t3t = LBITS(d0);
            t3h = HBITS(d0);
            mul64(t3t, t3h, ql, qh); /* t3=t1-(BN_ULLONG)q*d0; */
            t3l = (t1l - t3t) & BN_MASK2;
            if (t3l > t1l) {
                t3h++;
            }
            t3h = (t1h - t3h) & BN_MASK2;

            if (t3h) {
                break;
            }
            if (t2h < t3l) {
                break;
            }
            if (t2h == t3l && t2l <= wnump[-2]) {
                break;
            }

            q--;
        }

        tmp->d[div_n] = bn_mul_words(tmp->d, sdiv->d, div_n, q);
        for (j = div_n + 1; j > 0; j--) {
            if (tmp->d[j - 1]) {
                break;
            }
        }
        tmp->top = j;

        j = wnum.top;
        (void)BN_sub(&wnum, &wnum, tmp);

        snum->top = snum->top + wnum.top - j;

        if (wnum.neg) {
            q--;
            j = wnum.top;
            (void)BN_add(&wnum, &wnum, sdiv);
            snum->top += wnum.top - j;
        }
        *resp-- = q;
        wnump--;
    }
    bn_fix_top(snum);
    if (rm != NULL) {
        BOOL neg = num->neg;
        if (!BN_rshift(rm, snum, norm_shift)) {
            return FALSE;
        }
        rm->neg = neg;
    }
    return TRUE;
err:
    return FALSE;
}
