#include "to54_atomic_w17_nitrocrypto_residual03_private.h"

u32 bn_mul_add_words(u32 *rp, u32 *ap, int num, u32 w)
{
    u32 c = 0;

    if (num <= 0) {
        return 0;
    }

    u32 bl = LBITS(w);
    u32 bh = HBITS(w);

    while (TRUE) {
        mul_add(rp[0], ap[0], bl, bh, c);
        if (--num == 0) {
            break;
        }
        mul_add(rp[1], ap[1], bl, bh, c);
        if (--num == 0) {
            break;
        }
        mul_add(rp[2], ap[2], bl, bh, c);
        if (--num == 0) {
            break;
        }
        mul_add(rp[3], ap[3], bl, bh, c);
        if (--num == 0) {
            break;
        }
        ap += 4;
        rp += 4;
    }
    return c;
}
