#include "to54_atomic_w17_nitrocrypto_residual03_private.h"

void bn_mul_normal(u32 *r, u32 *a, int na, u32 *b, int nb)
{
    if (na < nb) {
        int itmp = na;
        na = nb;
        nb = itmp;
        u32 *ltmp = a;
        a = b;
        b = ltmp;
    }

    u32 *rr = &r[na];
    rr[0] = bn_mul_words(r, a, na, b[0]);

    while (TRUE) {
        if (--nb <= 0) {
            return;
        }
        rr[1] = bn_mul_add_words(&r[1], a, na, b[1]);
        if (--nb <= 0) {
            return;
        }
        rr[2] = bn_mul_add_words(&r[2], a, na, b[2]);
        if (--nb <= 0) {
            return;
        }
        rr[3] = bn_mul_add_words(&r[3], a, na, b[3]);
        if (--nb <= 0) {
            return;
        }
        rr[4] = bn_mul_add_words(&r[4], a, na, b[4]);
        rr += 4;
        r += 4;
        b += 4;
    }
}
