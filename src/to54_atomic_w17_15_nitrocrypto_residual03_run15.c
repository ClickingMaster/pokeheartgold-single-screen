#include "to54_atomic_w17_nitrocrypto_residual03_private.h"

u32 bn_mul_words(u32 *rp, u32 *ap, int num, u32 w)
{
    u32 carry = 0;

    if (num <= 0) {
        return 0;
    }

    u32 bl = LBITS(w);
    u32 bh = HBITS(w);

    while (TRUE) {
        mul(rp[0], ap[0], bl, bh, carry);
        if (--num == 0) {
            break;
        }
        mul(rp[1], ap[1], bl, bh, carry);
        if (--num == 0) {
            break;
        }
        mul(rp[2], ap[2], bl, bh, carry);
        if (--num == 0) {
            break;
        }
        mul(rp[3], ap[3], bl, bh, carry);
        if (--num == 0) {
            break;
        }
        ap += 4;
        rp += 4;
    }
    return carry;
}
