#include "to55_nitrocrypto_residual01_private.h"
BOOL BN_mod_mul_reciprocal(BIGNUM *r, BIGNUM *x, BIGNUM *y, BN_RECP_CTX *recp, BN_CTX *ctx)
{
    int ret = FALSE;

    BIGNUM *a = &ctx->bn[ctx->tos++];
    if (y != NULL) {
        if (x == y) {
            if (!BN_sqr(a, x, ctx)) {
                goto err;
            }
        } else {
            if (!BN_mul(a, x, y, ctx)) {
                goto err;
            }
        }
    } else {
        a = x; /* Just do the mod */
    }

    (void)BN_div_recp(NULL, r, a, recp, ctx);
    ret = TRUE;
err:
    ctx->tos--;
    return ret;
}

BOOL BN_div_recp(BIGNUM *dv, BIGNUM *rem, BIGNUM *m, BN_RECP_CTX *recp, BN_CTX *ctx)
{
    BOOL ret = FALSE;

    int tos = ctx->tos;
    BIGNUM *a = &ctx->bn[ctx->tos++];
    BIGNUM *b = &ctx->bn[ctx->tos++];
    BIGNUM *d, *r;
    if (dv != NULL) {
        d = dv;
    } else {
        d = &ctx->bn[ctx->tos++];
    }
    if (rem != NULL) {
        r = rem;
    } else {
        r = &ctx->bn[ctx->tos++];
    }

    if (BN_ucmp(m, &recp->N) < 0) {
        (void)BN_zero(d);
        (void)BN_copy(r, m);
        ctx->tos = tos;
        return TRUE;
    }

    /* We want the remainder
     * Given input of ABCDEF / ab
     * we need multiply ABCDEF by 3 digests of the reciprocal of ab
     *
     */
    int i = BN_num_bits(m);

    int j = recp->num_bits * 2;
    int ex;
    if (j > i) {
        i = j;
        ex = 0;
    } else {
        ex = (i - j) / 2;
    }

    j = i / 2;

    if (i != recp->shift) {
        recp->shift = BN_reciprocal(&recp->Nr, &recp->N, i, ctx);
    }

    if (!BN_rshift(a, m, j - ex)) {
        goto err;
    }
    if (!BN_mul(b, a, &recp->Nr, ctx)) {
        goto err;
    }
    if (!BN_rshift(d, b, j + ex)) {
        goto err;
    }
    d->neg = FALSE;
    if (!BN_mul(b, &recp->N, d, ctx)) {
        goto err;
    }
    if (!BN_usub(r, m, b)) {
        goto err;
    }
    r->neg = FALSE;

    j = 0;

    while (BN_ucmp(r, &recp->N) >= 0) {
        if (j++ > 2) {
            goto err;
        }
        if (!BN_usub(r, r, &recp->N)) {
            goto err;
        }
        if (!BN_add_word(d, 1)) {
            goto err;
        }
    }

    r->neg = BN_is_zero(r) ? 0 : m->neg;
    d->neg = m->neg ^ recp->N.neg;
    ret = TRUE;
err:
    ctx->tos = tos;
    return ret;
}

int BN_reciprocal(BIGNUM *r, BIGNUM *m, int len, BN_CTX *ctx)
{
    int ret = -1;
    BIGNUM t;

    BN_init(&t);

    (void)BN_zero(&t);
    if (!BN_set_bit(&t, len)) {
        goto err;
    }

    if (!BN_div(r, NULL, &t, m, ctx)) {
        goto err;
    }
    ret = len;
err:
    BN_free(&t);
    return ret;
}

u32 bn_div_words(u32 h, u32 l, u32 d)
{
    u32 ret = 0;
    u32 dh, dl, q;
    int count = 2;

    if (d == 0) {
        return BN_MASK2;
    }

    int i = BN_num_bits_word(d);
    if (i != BN_BITS2 && h > 1 << i) {
        return 0;
    }
    i = BN_BITS2 - i;
    if (h >= d) {
        h -= d;
    }

    if (i) {
        d <<= i;
        h = (h << i) | (l >> (BN_BITS2 - i));
        l <<= i;
    }
    dh = BN_MASK2l & (d >> BN_BITS4);
    dl = d & BN_MASK2l;
    while (TRUE) {
        if (h >> BN_BITS4 == dh) {
            q = BN_MASK2l;
        } else {
            q = h / dh;
        }

        u32 th = q * dh;
        u32 tl = q * dl;
        u32 t;
        while (TRUE) {
            t = h - th;
            if (t & BN_MASK2h || tl <= (t << BN_BITS4) + (BN_MASK2l & (l >> BN_BITS4))) {
                break;
            }
            th -= dh;
            tl -= dl;
            q--;
        }
        th = q * dh;
        t = q * dl >> BN_BITS4;
        tl = (q * dl & BN_MASK2l) << BN_BITS4;
        th += t;

        if (l < tl) {
            th++;
        }
        l -= tl;
        if (h < th) {
            h += d;
            q--;
        }
        h -= th;

        if (--count == 0) {
            break;
        }

        ret = (q & BN_MASK2l) << BN_BITS4;
        h = ((h << BN_BITS4) | (l >> BN_BITS4)) & BN_MASK2;
        l = (l & BN_MASK2l) << BN_BITS4;
    }
    ret |= q;
    return ret;
}

BOOL BN_mod_exp_recp(BIGNUM *r, const BIGNUM *a, const BIGNUM *p, const BIGNUM *m, BN_CTX *ctx)
{
    int i, j;
    BOOL ret = FALSE;
    int start = 1, ts = 0;
    BIGNUM val[16];
    BN_RECP_CTX recp;

    int bits = BN_num_bits(p);
    if (BN_is_zero(a)) {
        (void)BN_zero(r);
        return TRUE;
    }

    if (BN_is_zero(p)) {
        (void)BN_one(r);
        return TRUE;
    }

    if (BN_is_one(p)) {
        (void)BN_copy(r, a);
        return TRUE;
    }

    BN_RECP_CTX_init(&recp);
    if (BN_RECP_CTX_set(&recp, m, ctx) <= 0) {
        goto err;
    }

    BN_init(&val[0]);
    BIGNUM *aa = &ctx->bn[ctx->tos++];
    ts = 1;

    if (!BN_mod(&val[0], a, m, ctx)) {
        goto err; /* 1 */
    }
    if (!BN_mod_mul_reciprocal(aa, &val[0], &val[0], &recp, ctx)) {
        goto err; /* 2 */
    }

    int window;
    if (bits <= 17) { /* This is probably 3 or 0x10001, so just do singles */
        window = 1;
    } else if (bits >= 256) {
        window = 5; /* max size of window */
    } else if (bits >= 128) {
        window = 4;
    } else {
        window = 3;
    }

    j = 1 << (window - 1);
    for (i = 1; i < j; i++) {
        BN_init(&val[i]);
        if (!BN_mod_mul_reciprocal(&val[i], &val[i - 1], aa, &recp, ctx)) {
            goto err;
        }
    }
    ts = i;

    start = 1; /* This is used to avoid multiplication etc
                * when there is only the value '1' in the
                * buffer. */
    int wstart, wend;
    int wvalue = 0; /* The 'value' of the window */
    wstart = bits - 1; /* The top bit of the window */
    wend = 0; /* The bottom bit of the window */

    if (!BN_one(r)) {
        goto err;
    }

    while (TRUE) {
        if (!BN_is_bit_set(p, wstart)) {
            if (!start) {
                if (!BN_mod_mul_reciprocal(r, r, r, &recp, ctx)) {
                    goto err;
                }
            }
            if (wstart == 0) {
                break;
            }
            wstart--;
            continue;
        }
        /* We now have wstart on a 'set' bit, we now need to work out
         * how bit a window to do.  To do this we need to scan
         * forward until the last set bit before the end of the
         * window */
        j = wstart;
        wvalue = 1;
        wend = 0;
        for (i = 1; i < window; i++) {
            if (wstart - i < 0) {
                break;
            }
            if (BN_is_bit_set(p, wstart - i)) {
                wvalue <<= (i - wend);
                wvalue |= 1;
                wend = i;
            }
        }

        /* wend is the size of the current window */
        j = wend + 1;
        /* add the 'bytes above' */
        if (!start) {
            for (i = 0; i < j; i++) {
                if (!BN_mod_mul_reciprocal(r, r, r, &recp, ctx)) {
                    goto err;
                }
            }
        }

        /* wvalue will be an odd number < 2^window */
        if (!BN_mod_mul_reciprocal(r, r, &val[wvalue >> 1], &recp, ctx)) {
            goto err;
        }

        /* move the 'window' down further */
        wstart -= wend + 1;
        wvalue = 0;
        start = 0;
        if (wstart < 0) {
            break;
        }
    }
    ret = TRUE;
err:
    ctx->tos--;
    for (i = 0; i < ts; i++) {
        BN_clear_free(&val[i]);
    }
    BN_RECP_CTX_free(&recp);
    return ret;
}

BOOL BN_mod_exp_mont(BIGNUM *rr, BIGNUM *a, const BIGNUM *p, const BIGNUM *m, BN_CTX *ctx, BN_MONT_CTX *in_mont)
{
    int ret = FALSE;
    int tosBackup;
    int v0;
    BIGNUM *v1;
    BN_MONT_CTX *mont = NULL;
    u8 *expBits = NULL;
    UnusedFunc *unusedFunc = NULL;
    int v2 = 0;

    if (!(m->d[0] & 1)) {
        return FALSE;
    }

    tosBackup = ctx->tos;
    if (BN_is_zero(a)) {
        (void)BN_zero(rr);
        return TRUE;
    }

    if (BN_is_zero(p)) {
        (void)BN_one(rr);
        return TRUE;
    }

    if (BN_is_one(p)) {
        (void)BN_copy(rr, a);
        return TRUE;
    }

    if (in_mont != NULL) {
        mont = in_mont;
    } else {
        if ((mont = BN_MONT_CTX_new()) == NULL) {
            goto err;
        }
        if (!BN_MONT_CTX_set_word(mont, m, ctx)) {
            goto err;
        }
    }

    unusedFunc = ctx->unusedFunc;
    if (!BN_gen_exp_bits(p, &expBits, 0, ctx)) {
        goto err;
    }

    ctx->tos++;
    v0 = expBits[3];
    int v3 = (expBits[2] + 63) / expBits[2];
    expBits += 4;

    int nTop = mont->N.top;
    if (a->top == nTop && a->d[nTop - 1] < m->d[nTop - 1]) {
        v1 = a;
    } else if (a->top < nTop) {
        v1 = &ctx->bn[ctx->tos++];
        v1->top = a->top;
        bn_zexpand(v1, nTop);
        for (int i = 0; i < a->top; i++) {
            v1->d[i] = a->d[i];
        }
    } else {
        v1 = &ctx->bn[ctx->tos++];
        if (BN_mod(v1, a, m, ctx) == 0) {
            goto err;
        }
        bn_zexpand(v1, nTop);
    }

    BIGNUM *v4;
    BIGNUM *v5;
    BIGNUM *v6;
    int v7 = v0 * nTop + (p->top * v3 * 2 + 7) / 4;
    BIGNUM *v8 = &ctx->bn[ctx->tos++];
    v4 = &ctx->bn[ctx->tos++];
    v5 = &ctx->bn[ctx->tos++];
    v6 = &ctx->bn[ctx->tos++];
    if (bn_wexpand(rr, nTop) == NULL) {
        goto err;
    }
    if (bn_wexpand(v4, nTop * 4) == NULL) {
        goto err;
    }
    if (bn_wexpand(v5, nTop * 2) == NULL) {
        goto err;
    }
    if (bn_wexpand(v8, v7) == NULL) {
        goto err;
    }
    if (bn_wexpand(v6, nTop * 2) == NULL) {
        goto err;
    }

    int v9;
    u32 *v10 = v4->d;
    u32 *v11 = v5->d;
    u32 v12 = mont->n0;
    u32 *v13 = v6->d;
    u32 *v14 = mont->N.d;
    u32 *v15[0x10];
    v15[0] = v8->d;
    bn_mul_normal(v13, v1->d, nTop, mont->RR.d, nTop);
    bn_from_montgomery_words(v15[0], v13, v14, nTop, v12);

    if (v0 > 1) {
        bn_sqr_normal(v13, v15[0], nTop, v10);
        bn_from_montgomery_words(v10, v13, v14, nTop, v12);
        for (int i = 1; i < v0; i++) {
            v15[i] = &v15[i - 1][nTop];
            bn_mul_normal(v13, v15[i - 1], nTop, v10, nTop);
            bn_from_montgomery_words(v15[i], v13, v14, nTop, v12);
        }
    }

    int v16 = *expBits++;
    v9 = *expBits++;
    if (v9 == 0xFF && v16 == 0) {
        v16 = *expBits++;
        while (expBits[0] == 0xFF && v16 == 0) {
            v9 += 0x100;
            expBits++;
            v16 = *expBits++;
        }
        v9 += *expBits++ + 1;
    }

    MI_CpuCopy8(v15[v16 >> 1], v11, nTop * sizeof(u32));

    while (v9 != 0) {
        if (!((unusedFunc == NULL || (*unusedFunc)(unusedFunc, -1, v2++) == 0) && !(ctx->flags & 0x4000))) {
            goto err;
        }

        for (int i = 0; i < v9; i++) {
            bn_sqr_normal(v13, v11, nTop, v10);
            bn_from_montgomery_words(v11, v13, v14, nTop, v12);
        }

        v16 = *expBits++;
        v9 = *expBits++;
        if (v9 == 0xFF && v16 == 0) {
            v16 = *expBits++;
            while (expBits[0] == 0xFF && v16 == 0) {
                v9 += 0x100;
                expBits++;
                v16 = *expBits++;
            }
            v9 += *expBits++ + 1;
        }

        if (v16 != 0 || v9 != 0) {
            if (v9 != 0 || v16 != 1) {
                bn_mul_normal(v13, v11, nTop, v15[v16 >> 1], nTop);
                bn_from_montgomery_words(v11, v13, v14, nTop, v12);
            } else {
                bn_mul_normal(v13, v11, nTop, v1->d, nTop);
                bn_from_montgomery_words(rr->d, v13, v14, nTop, v12);
                goto here;
            }
        } else {
            break;
        }
    }

    for (int i = nTop; i < nTop * 2; i++) {
        v11[i] = 0;
    }
    bn_from_montgomery_words(rr->d, v11, v14, nTop, v12);

here:
    if (!(ctx->flags & 0x4000)) {
        rr->top = nTop;
        bn_fix_top(rr);
        ret = TRUE;
    }
err:
    if (unusedFunc) {
        if ((*unusedFunc)(unusedFunc, -1, -1) != 0) {
            ret = FALSE;
        }
    }

    if (in_mont == NULL && mont != NULL) {
        BN_MONT_CTX_free(mont);
    }

    ctx->tos = tosBackup;
    return ret;
}
