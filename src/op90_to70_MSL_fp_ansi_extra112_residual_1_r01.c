#pragma force_active on
typedef unsigned char u8;
typedef unsigned long u32;
typedef short s16;
typedef struct {
    char sign;
    char pad;
    s16 exp;
    u8 len;
    u8 digits[32];
} decimal;
void __dorounddecup(decimal *d, int digits);
void __timesdec(decimal *res, const decimal *x, const decimal *y);
// @fn __timesdec
void __timesdec(decimal *res, const decimal *x, const decimal *y) {
    u32 carry = 0;
    int i = x->len + y->len - 1;
    u8 *ip;
    u8 *ep;
    u8 buf[64];
    ip = buf + i + 1;
    ep = ip;
    res->sign = 0;
    for (; i > 0; i--) {
        long n;
        int j;
        int xl;
        const u8 *xp;
        const u8 *yp;
        long k;
        k = y->len - 1;
        j = i - k - 1;
        if (j < 0) {
            k = i - 1;
            j = 0;
        }
        xl = x->len;
        n = k + 1;
        xp = x->digits;
        yp = y->digits + k;
        xp += j;
        if (n > xl - j) {
            n = xl - j;
        }
        for (; n > 0; n--) {
            carry += *xp * *yp;
            xp++;
            yp--;
        }
        *--ip = carry % 10;
        carry /= 10;
    }
    res->exp = x->exp + y->exp;
    if (carry != 0) {
        --ip;
        *ip = carry;
        res->exp++;
    }
    for (i = 0; i < 32 && ip < ep; i++) {
        res->digits[i] = *ip;
        ++ip;
    }
    res->len = i;
    if (ip >= ep || *ip < 5) {
        return;
    }
    if (*ip == 5) {
        u8 *p = ip + 1;
        for (; p < ep; p++) {
            if (*p != 0) {
                goto round;
            }
        }
        if (!(ip[-1] & 1)) {
            return;
        }
    }
round:
    __dorounddecup(res, res->len);
}
