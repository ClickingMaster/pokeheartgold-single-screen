#pragma force_active on
#include <nitro/types.h>
typedef struct {
    u8 justification;
    u8 sign;
    u8 precisionSet;
    u8 alternate;
    u8 argument;
    u8 conversion;
    u8 pad[2];
    int width;
    int precision;
} Format;
char *long2str(long num, char *buf, Format f);
// @fn long2str
char *long2str(long num, char *buf, Format f) {
    unsigned long base;
    char *p = buf;
    int d;
    int neg = 0;
    int prec = f.precision;
    int sign = f.sign;
    long digits = 0;
    unsigned long u = num;
    *--p = 0;
    if (u == 0 && prec == 0 && (!f.alternate || f.conversion != 'o')) {
        return p;
    }
    switch (f.conversion) {
    case 'd':
    case 'i':
        base = 10;
        if (num < 0) {
            if (u != 0x80000000) {
                u = -u;
            }
            neg = 1;
        }
        break;
    case 'o':
        base = 8;
        sign = 0;
        break;
    case 'u':
        base = 10;
        sign = 0;
        break;
    case 'x':
    case 'X':
        base = 16;
        sign = 0;
        break;
    }
    do {
        d = u % base;
        u /= base;
        if (d < 10) {
            d += '0';
        } else if (f.conversion == 'x') {
            d += 'a' - 10;
        } else {
            d += 'A' - 10;
        }
        *--p = d;
        digits++;
    } while (u != 0);
    if (base == 8 && f.alternate && *p != '0') {
        *--p = '0';
        digits++;
    }
    if (f.justification == 2) {
        prec = f.width;
        if (neg || sign != 0) {
            prec--;
        }
        if (base == 16 && f.alternate) {
            prec -= 2;
        }
    }
    if (buf - p + prec > 0x1fd) {
        return NULL;
    }
    while (digits < prec) {
        *--p = '0';
        digits++;
    }
    if (base == 16 && f.alternate) {
        *--p = f.conversion;
        *--p = '0';
    }
    if (neg) {
        *--p = '-';
    } else if (sign == 1) {
        *--p = '+';
    } else if (sign == 2) {
        *--p = ' ';
    }
    return p;
}
