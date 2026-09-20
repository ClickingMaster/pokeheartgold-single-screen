#include "to54_atomic_w17_nitrocrypto_residual03_private.h"

extern const u8 gNitroCryptoExpP16[];
extern const u8 gNitroCryptoExpP2[];
extern const u8 gNitroCryptoExpP4[];
extern const u8 *gNitroCryptoShiftVal[];

BOOL BN_sqr(BIGNUM *r, BIGNUM *a, BN_CTX *ctx)
{
    int max, al;
    BIGNUM *tmp = &ctx->bn[ctx->tos];
    BIGNUM *rr = a != r ? r : &ctx->bn[ctx->tos + 1];

    al = a->top;
    if (al <= 0) {
        r->top = 0;
        return TRUE;
    }

    max = al + al;
    if (bn_wexpand(rr, max) == NULL) {
        return FALSE;
    }
    rr->top = max;
    rr->neg = FALSE;
    if (al == 4) {
        u32 t[8];
        bn_sqr_normal(rr->d, a->d, 4, t);
    } else if (al == 8) {
        u32 t[16];
        bn_sqr_normal(rr->d, a->d, 8, t);
    } else {
        if (bn_wexpand(tmp, max) == NULL) {
            return FALSE;
        }
        bn_sqr_normal(rr->d, a->d, al, tmp->d);
    }

    if (max > 0 && rr->d[max - 1] == 0) {
        rr->top--;
    }
    if (rr != r) {
        (void)BN_copy(r, rr);
    }
    return TRUE;
}

void bn_sqr_normal(u32 *r, u32 *a, int n, u32 *tmp)
{
    int i, j;
    int max = n * 2;
    u32 *ap = a;
    u32 *rp = r;
    rp[0] = rp[max - 1] = 0;
    rp++;
    j = n;

    if (--j > 0) {
        ap++;
        rp[j] = bn_mul_words(rp, ap, j, *a);
        rp += 2;
    }

    for (i = n - 2; i > 0; i--) {
        u32 *prevAP = ap;
        j--;
        ap++;
        rp[j] = bn_mul_add_words(rp, ap, j, *prevAP);
        rp += 2;
    }

    (void)bn_add_words(r, r, r, max);
    (void)bn_sqr_words(tmp, a, n);
    (void)bn_add_words(r, r, tmp, max);
}

void bn_from_montgomery_words(u32 *param0, u32 *param1, u32 *param2, int param3, u32 param4)
{
    u32 v0 = 0;
    u32 *v1 = param1 + param3;
    int v2 = param4;

    for (int i = 0; i < param3; i++) {
        u32 v3 = bn_mul_add_words(param1, param2, param3, *param1 * v2);
        v3 += v0;
        *v1 += v3;
        if (v3 < v0) {
            v0 = 1;
        } else {
            v0 = 0;
        }
        if (*v1 < v3) {
            v0 += 1;
        }
        param1 += 1;
        v1 += 1;
    }

    int v4 = param3 - 1;
    if (v0 == 0) {
        if (param1[v4] == param2[v4] && v4 > 0) {
            do {
                if (param1[v4] != param2[v4]) {
                    break;
                }
                v4--;
            } while (v4 > 0);
        }
        if (param1[v4] >= param2[v4]) {
            v0 = 1;
        } else {
            v0 = 0;
        }
    }

    if (v0 != 0) {
        (void)bn_sub_words(param0, param1, param2, param3);
        return;
    }
    for (int i = 0; i < param3; i++) {
        param0[i] = param1[i];
    }
}

u32 BN_mod_inverse_word(u32 word)
{
    u32 v0;
    u32 v1;
    u32 v2 = -word % word;
    v0 = word;
    u32 v3 = 0;
    v1 = 1;
    int v4 = -1;
    while (v2 != 0) {
        u32 v5 = v0 % v2;
        u32 v6 = v0 / v2;
        v6 *= v1;
        v6 += v3;
        v3 = v1;
        v1 = v6;
        v0 = v2;
        v2 = v5;
        v4 = -v4;
    }
    if (v4 < 0) {
        v3 = word - v3;
    }
    u32 ret;
    if (v0 != 1) {
        ret = 0;
    } else {
        ret = v3 % word;
    }
    return ret;
}

int BN_gen_exp_bits(const BIGNUM *param0, u8 **param1, int unused, BN_CTX *param3)
{
    int v0;
    int v1;
    int v2;
    u8 *v3 = NULL;
    BIGNUM *v4 = &param3->bn[param3->tos];
    int v5 = param0->top << 5;
    if (param0->top == 0) {
        return 0;
    }

    if (param0->top == 1) {
        if (BN_is_word(param0, 0x10001)) {
            v3 = (u8 *)gNitroCryptoExpP16;
        } else if (BN_is_word2(param0, 0x11)) {
            v3 = (u8 *)gNitroCryptoExpP4;
        } else if (BN_is_word2(param0, 3)) {
            v3 = (u8 *)gNitroCryptoExpP2;
        }
        v0 = 1;
        v1 = 1;
        v2 = 32;
    } else if (v5 >= 256) {
        v0 = 5;
        v1 = 16;
        v2 = 7;
    } else if (v5 >= 128) {
        v0 = 4;
        v1 = 8;
        v2 = 8;
    } else {
        v0 = 3;
        v1 = 4;
        v2 = 11;
    }

    int v6;
    int v7 = ((param0->top * v2 * 2) + 7) / 4;
    if (v3 == NULL) {
        if (bn_wexpand(v4, v7) == NULL) {
            return 0;
        }
        v3 = (u8 *)v4->d;
        v6 = BN_gen_exp_string(v3 + 4, param0, v0) + 2;
        v3[0] = v6 >> 8;
        v3[1] = v6;
        v3[2] = v0;
        v3[3] = v1;
    } else {
        v6 = 8;
    }
    *param1 = v3;
    return v6 + 2;
}

int BN_gen_exp_string(u8 *param0, const BIGNUM *param1, int param2)
{
    u8 *v0;
    int v1;
    u32 v2;
    u32 v3;
    u32 *v4;
    u32 v5;
    int v6;

    if (param2 > 6) {
        param2 = 6;
    }
    const u8 *v7 = gNitroCryptoShiftVal[param2];
    v1 = (1 << param2) - 1;
    u32 v8 = 0;
    v0 = param0 + ((param2 + (param1->top << 5) - 1) / param2 * 2 + 2);
    *v0-- = 0;
    *v0-- = 0;
    int v9 = param1->top;
    v4 = param1->d + 1;
    v3 = param1->d[0];
    u32 v10 = param1->d[0];
    v2 = 0;
    if (v9 > 1) {
        v2 = *v4++;
    }
    v5 = 0;

    while (TRUE) {
        v6 = v10 & v1;
        if (v7[v6] != 0) {
            v5 += v7[v6];
            v8 += v7[v6];
            if (v8 >= 32) {
                if (v9 > 1) {
                    v9 -= 1;
                    v3 = v2;
                    if (v9 <= 1) {
                        v2 = 0;
                    } else {
                        v2 = *v4++;
                    }
                    v8 -= 32;
                } else {
                    goto here;
                }
            }
            if (v8 == 0) {
                v10 = v3;
            } else {
                v10 = (v3 >> v8) | (v2 << (32 - v8));
            }
            continue;
        }
here:
        if (v6 != 0) {
            *v0-- = v5;
            *v0-- = v6;
            if (v5 >= 256) {
                for (; v5 >= 256; v5 -= 256) {
                    *v0-- = 0xFF;
                    *v0-- = 0;
                }
            }
            v5 = param2;
            v8 += param2;
            if (v8 >= 32) {
                if (v9 > 1) {
                    v9 -= 1;
                    v3 = v2;
                    v8 -= 32;
                    if (v9 <= 1) {
                        v2 = 0;
                    } else {
                        v2 = *v4++;
                    }
                } else {
                    break;
                }
            }
            if (v8 == 0) {
                v10 = v3;
            } else {
                v10 = (v3 >> v8) | (v2 << (32 - v8));
            }
        } else {
            break;
        }
    }
    v0++;
    int ret = 2;
    while (v0[0] || v0[1]) {
        param0[0] = v0[0];
        param0[1] = v0[1];
        param0 += 2;
        v0 += 2;
        ret += 2;
    }
    param0[0] = 0;
    param0[1] = 0;
    return ret;
}

BOOL BN_MONT_CTX_set_word(BN_MONT_CTX *mont, const BIGNUM *modulus, BN_CTX *ctx)
{
    BIGNUM Ri;

    if (modulus->top == 0) {
        return FALSE;
    }
    BIGNUM *R = &mont->RR;
    if (!BN_copy(&mont->N, modulus)) {
        return FALSE;
    }
    BN_init(&Ri);
    mont->use_word = 1;
    mont->ri = (BN_num_bits(modulus) + (BN_BITS2 - 1)) / BN_BITS2;
    if (!BN_zero(R)) {
        return FALSE;
    }
    if (!BN_set_bit(R, BN_BITS2)) {
        goto err;
    }

    u32 word = modulus->d[0];
    if (BN_set_word(&Ri, BN_mod_inverse_word(word)) == 0) {
        goto err;
    }
    if (!BN_lshift(&Ri, &Ri, BN_BITS2)) {
        goto err;
    }
    if (Ri.top != 0 && (Ri.top != 1 || Ri.d[0])) {
        (void)BN_sub_word(&Ri, 1);
    } else {
        if (!BN_set_word(&Ri, BN_MASK2)) {
            goto err;
        }
    }
    mont->n0 = bn_div_words(Ri.top >= 2 ? Ri.d[1] : 0, Ri.top >= 1 ? Ri.d[0] : 0, word);
    (void)BN_zero(&mont->RR);
    if (!BN_set_bit(&mont->RR, mont->ri * 64)) {
        goto err;
    }
    (void)BN_mod(&mont->RR, &mont->RR, &mont->N, ctx);
    (void)bn_zexpand(&mont->RR, mont->ri);
err:
    BN_free(&Ri);
    return TRUE;
}
