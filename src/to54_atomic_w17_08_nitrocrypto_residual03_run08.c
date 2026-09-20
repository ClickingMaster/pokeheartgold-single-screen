#include "to54_atomic_w17_nitrocrypto_residual03_private.h"

BOOL BN_sub_word(BIGNUM *a, u32 w)
{
    if (w == 0) {
        return TRUE;
    }

    if (a->neg) {
        a->neg = FALSE;
        int result = BN_add_word(a, w);
        a->neg = TRUE;
        return result;
    }

    if (a->top <= 1) {
        if (a->top == 0) {
            if (bn_wexpand(a, 1) == NULL) {
                return FALSE;
            }

            a->d[0] = w;
            a->neg = TRUE;
            a->top = 1;
            return TRUE;
        } else {
            u32 d = a->d[0];
            if (d == w) {
                a->top = 0;
            } else if (d > w) {
                a->d[0] -= w;
            } else {
                a->neg = TRUE;
                a->d[0] = w - d;
            }
            return TRUE;
        }
    }
    int i = 0;
    while (TRUE) {
        if (a->d[i] >= w) {
            a->d[i] -= w;
            break;
        } else {
            a->d[i] = (a->d[i] - w) & BN_MASK2;
            i++;
            w = 1;
        }
    }
    if (a->d[i] == 0 && i == a->top - 1) {
        a->top--;
    }
    return TRUE;
}
