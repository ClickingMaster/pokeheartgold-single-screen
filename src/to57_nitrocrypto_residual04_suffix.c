#include "to55_nitrocrypto_residual01_private.h"

BOOL BN_set_word(BIGNUM *a, u32 w) {
    if (bn_expand(a, BN_BITS2) == NULL) {
        return 0;
    }
    a->neg = 0;
    a->top = 0;
    a->d[0] = w;
    if (a->d[0] != 0) {
        a->top = 1;
    }
    return 1;
}
