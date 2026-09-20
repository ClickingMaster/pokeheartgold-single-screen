#include "to54_atomic_w17_nitrocrypto_residual03_private.h"

u32 bn_add_words(u32 *r, u32 *a, u32 *b, int n)
{
    if (n <= 0) {
        return 0;
    }

    u32 c = 0;
    u32 l, t;
    while (TRUE) {
        t = a[0];
        t = (t + c) & BN_MASK2;
        c = t < c;
        l = (t + b[0]) & BN_MASK2;
        c += l < t;
        r[0] = l;
        if (--n <= 0) {
            break;
        }

        t = a[1];
        t = (t + c) & BN_MASK2;
        c = t < c;
        l = (t + b[1]) & BN_MASK2;
        c += l < t;
        r[1] = l;
        if (--n <= 0) {
            break;
        }

        t = a[2];
        t = (t + c) & BN_MASK2;
        c = t < c;
        l = (t + b[2]) & BN_MASK2;
        c += l < t;
        r[2] = l;
        if (--n <= 0) {
            break;
        }

        t = a[3];
        t = (t + c) & BN_MASK2;
        c = t < c;
        l = (t + b[3]) & BN_MASK2;
        c += l < t;
        r[3] = l;
        if (--n <= 0) {
            break;
        }

        a += 4;
        b += 4;
        r += 4;
    }

    return c;
}
