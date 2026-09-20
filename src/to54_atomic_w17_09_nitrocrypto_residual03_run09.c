#include "to54_atomic_w17_nitrocrypto_residual03_private.h"

BOOL BN_add_word(BIGNUM *a, u32 w)
{
    if (w == 0) {
        return TRUE;
    }

    if (a->neg) {
        if (a->top > 1) {
            a->neg = FALSE;
            int result = BN_sub_word(a, w);
            a->neg = TRUE;
            return result;
        } else {
            u32 d = a->d[0];
            if (d > w) {
                a->d[0] -= w;
            } else if (d < w) {
                a->neg = FALSE;
                a->d[0] = w - d;
            } else {
                a->neg = FALSE;
                a->top = 0;
            }
            return TRUE;
        }
    }

    if (bn_wexpand(a, a->top + 1) == NULL) {
        return FALSE;
    }

    int i = 0;
    a->d[a->top] = 0;
    while (TRUE) {
        u32 l = (a->d[i] + w) & BN_MASK2;
        a->d[i] = l;
        if (w > l) {
            w = 1;
        } else {
            break;
        }
        i++;
    }

    if (i >= a->top) {
        a->top++;
    }

    return TRUE;
}
