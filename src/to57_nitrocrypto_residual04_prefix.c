#include "to55_nitrocrypto_residual01_private.h"

int BN_bn2bin(const BIGNUM *a, u8 *to) {
    int n;
    int i;

    n = BN_num_bytes(a);
    i = n - 1;
    if (n <= 0) {
        return n;
    }
    do {
        *(to++) = (u8)(a->d[i / 4] >> (8 * (i % 4)));
    } while (i-- > 0);
    return n;
}

int BN_ucmp(const BIGNUM *a, const BIGNUM *b) {
    int i;
    u32 t1;
    u32 t2;
    u32 *ap;
    u32 *bp;

    i = a->top - b->top;
    if (i != 0) {
        return i;
    }
    ap = a->d;
    bp = b->d;
    for (i = a->top - 1; i >= 0; i--) {
        t1 = ap[i];
        t2 = bp[i];
        if (t1 != t2) {
            return t1 > t2 ? 1 : -1;
        }
    }
    return 0;
}

int BN_set_bit(BIGNUM *a, int n) {
    int i;
    int j;
    int k;

    i = n / BN_BITS2;
    j = n % BN_BITS2;
    if (a->top <= i) {
        if (bn_wexpand(a, i + 1) == NULL) {
            return 0;
        }
        for (k = a->top; k < i + 1; k++) {
            a->d[k] = 0;
        }
        a->top = i + 1;
    }
    a->d[i] |= (1U << j);
    return 1;
}
