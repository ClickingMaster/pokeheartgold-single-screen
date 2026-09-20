#include "to54_atomic_w17_nitrocrypto_residual03_private.h"

void bn_sqr_words(u32 *r, u32 *a, int n)
{
    if (n <= 0) {
        return;
    }
    while (TRUE) {
        sqr64(r[0], r[1], a[0]);
        if (--n == 0) {
            break;
        }

        sqr64(r[2], r[3], a[1]);
        if (--n == 0) {
            break;
        }

        sqr64(r[4], r[5], a[2]);
        if (--n == 0) {
            break;
        }

        sqr64(r[6], r[7], a[3]);
        if (--n == 0) {
            break;
        }

        a += 4;
        r += 8;
    }
}
