#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
typedef struct ScanFormat {
    u8 suppress;
    u8 widthGiven;
    u8 size;
    u8 conversion;
    int width;
    u8 set[32];
} ScanFormat;
extern const u16 __msl_digit[];
#pragma dollar_identifiers on
extern const ScanFormat parse_format$f;
extern float __float_nan;
extern unsigned long __strtoul(int base, int width, int (*read)(void *, int, int), void *arg, int *count, int *negative, int *overflow);
extern unsigned long long __strtoull(int base, int width, int (*read)(void *, int, int), void *arg, int *count, int *negative, int *overflow);
extern double __strtold(int width, int (*read)(void *, int, int), void *arg, int *count, int *overflow);
extern int mbtowc(u16 *dst, const char *src, u32 n);
int __sformatter(int (*read)(void *, int, int), void *arg, const char *format, char *args, int secure);
static inline int IS_SPACE(int c) {
    return (c < 0 || c >= 128) ? 0 : (__msl_digit[c] & 0x100);
}
#define IS_DIGIT(c) (((c) < 0 || (c) >= 128) ? 0 : (__msl_digit[(c)] & 8))
// @fn parse_format
static const char *parse_format(const char *s, ScanFormat *out) {
    const char *p;
    int c;
    int more;
    int invert;
    int last;
    int i;
    u8 *q;
    ScanFormat f = parse_format$f;

    c = s[1];
    p = s + 1;
    if (c == '%') {
        f.conversion = c;
        *out = f;
        return p + 1;
    }
    if (c == '*') {
        f.suppress = 1;
        c = *++p;
    }
    if (IS_DIGIT(c)) {
        f.width = 0;
        do {
            f.width = f.width * 10 + (c - '0');
            c = *++p;
        } while (IS_DIGIT(c));
        if (f.width == 0) {
            f.conversion = 0xFF;
            *out = f;
            return p + 1;
        }
        f.widthGiven = 1;
    }
    more = 1;
    switch (c) {
    case 'h':
        f.size = 2;
        if (p[1] == 'h') {
            f.size = 1;
            c = *++p;
        }
        break;
    case 'l':
        f.size = 3;
        if (p[1] == 'l') {
            f.size = 7;
            c = *++p;
        }
        break;
    case 'L':
        f.size = 9;
        break;
    case 'j':
        f.size = 4;
        break;
    case 'z':
        f.size = 5;
        break;
    case 't':
        f.size = 6;
        break;
    default:
        more = 0;
        break;
    }
    if (more) {
        c = *++p;
    }
    f.conversion = c;
    switch (c) {
    case 'd':
    case 'i':
    case 'o':
    case 'u':
    case 'x':
    case 'X':
        if (f.size == 9) {
            f.conversion = 0xFF;
        }
        break;
    case 'a':
    case 'A':
    case 'e':
    case 'E':
    case 'f':
    case 'F':
    case 'g':
    case 'G':
        if (f.size == 1 || f.size == 2 || f.size == 4 || f.size == 5 || f.size == 6 || f.size == 7) {
            f.conversion = 0xFF;
        } else if (f.size == 3) {
            f.size = 8;
        }
        break;
    case 'p':
        f.size = 3;
        f.conversion = 'x';
        break;
    case 'c':
        if (f.size == 3) {
            f.size = 10;
        } else if (f.size != 0) {
            f.conversion = 0xFF;
        }
        break;
    case 's':
        if (f.size == 3) {
            f.size = 10;
        } else if (f.size != 0) {
            f.conversion = 0xFF;
        }
        for (q = f.set, i = 32; i > 0; i--) {
            *q++ = 0xFF;
        }
        f.set[1] = 0xC1;
        f.set[4] = 0xFE;
        break;
    case '[':
        if (f.size == 3) {
            f.size = 10;
        } else if (f.size != 0) {
            f.conversion = 0xFF;
        }
        c = *++p;
        invert = 0;
        if (c == '^') {
            c = *++p;
            invert = 1;
        }
        if (c == ']') {
            f.set[']' >> 3] |= 1 << (']' & 7);
            c = *++p;
        }
        while (c != 0 && c != ']') {
            f.set[(u8)c >> 3] |= 1 << (c & 7);
            if (p[1] == '-' && (last = p[2]) != 0 && last != ']') {
                for (c++; c <= last; c++) {
                    f.set[(u8)c >> 3] |= 1 << (c & 7);
                }
                c = *(p += 3);
            } else {
                c = *++p;
            }
        }
        if (c == 0) {
            f.conversion = 0xFF;
        } else if (invert) {
            {
                int j;
                u8 *r;
                for (j = 32, r = f.set; j > 0; j--) {
                    *r = ~*r;
                    r++;
                }
            }
        }
        break;
    case 'n':
        break;
    default:
        f.conversion = 0xFF;
        break;
    }
    *out = f;
    return p + 1;
}
// @fn __sformatter
int __sformatter(int (*read)(void *, int, int), void *arg, const char *format, char *args, int secure) {
    char c;
    int chars;
    int items;
    int conversions;
    int base;
    int count;
    int negative;
    int overflow;
    int ch;
    char cc;
    int ok;
    u32 limit;
    char *ptr;
    long sval;
    unsigned long uval;
    long long sval64 = 0;
    unsigned long long uval64;
    int failed;
    char *start;
    double fval;
    ScanFormat fmt;

    failed = 0;
    chars = 0;
    items = 0;
    conversions = 0;
    while ((c = *format) != 0) {
        if (IS_SPACE(c)) {
            do {
                c = *++format;
            } while (IS_SPACE(c));
            if (!failed) {
                while (IS_SPACE(cc = read(arg, 0, 0))) {
                    chars++;
                }
                read(arg, cc, 1);
            }
            continue;
        }
        if (c != '%' && !failed) {
            if ((cc = read(arg, 0, 0)) != (u8)c) {
                read(arg, cc, 1);
                if (!secure) {
                    goto exit;
                }
                failed = 1;
                format++;
                continue;
            }
            chars++;
            format++;
            continue;
        }
        format = parse_format(format, &fmt);
        if (!fmt.suppress && fmt.conversion != '%') {
            ptr = *(char **)((args += 4) - 4);
        } else {
            ptr = NULL;
        }
        if (fmt.conversion != 'n' && !failed && read(arg, 0, 2)) {
            if (!secure) {
                goto exit;
            }
            failed = 1;
        }
        switch (fmt.conversion) {
        case 'd':
            base = 10;
            goto signed_int;
        case 'i':
            base = 0;
        signed_int:
            if (failed) {
                sval = 0;
                sval64 = 0;
            } else {
                if (fmt.size == 7 || fmt.size == 4) {
                    uval64 = __strtoull(base, fmt.width, read, arg, &count, &negative, &overflow);
                } else {
                    uval = __strtoul(base, fmt.width, read, arg, &count, &negative, &overflow);
                }
                if (count == 0) {
                    if (!secure) {
                        goto exit;
                    }
                    failed = 1;
                    sval = 0;
                    sval64 = 0;
                } else {
                    chars += count;
                    if (fmt.size == 7 || fmt.size == 4) {
                        sval64 = negative ? -uval64 : uval64;
                    } else {
                        sval = negative ? -uval : uval;
                    }
                }
            }
            if (ptr) {
                switch (fmt.size) {
                case 0:
                    *(int *)ptr = sval;
                    break;
                case 1:
                    *(s8 *)ptr = sval;
                    break;
                case 2:
                    *(s16 *)ptr = sval;
                    break;
                case 3:
                    *(long *)ptr = sval;
                    break;
                case 4:
                    *(long long *)ptr = sval64;
                    break;
                case 5:
                    *(u32 *)ptr = sval;
                    break;
                case 6:
                    *(int *)ptr = sval;
                    break;
                case 7:
                    *(long long *)ptr = sval64;
                    break;
                }
                if (!failed) {
                    items++;
                }
            }
            conversions++;
            break;
        case 'o':
            base = 8;
            goto unsigned_int;
        case 'u':
            base = 10;
            goto unsigned_int;
        case 'x':
        case 'X':
            base = 16;
        unsigned_int:
            if (failed) {
                uval = 0;
                uval64 = 0;
            } else {
                if (fmt.size == 7 || fmt.size == 4) {
                    uval64 = __strtoull(base, fmt.width, read, arg, &count, &negative, &overflow);
                } else {
                    uval = __strtoul(base, fmt.width, read, arg, &count, &negative, &overflow);
                }
                if (count == 0) {
                    if (!secure) {
                        goto exit;
                    }
                    failed = 1;
                    uval = 0;
                    uval64 = 0;
                } else {
                    chars += count;
                    if (negative) {
                        if (fmt.size == 7) {
                            uval64 = -uval64;
                        } else {
                            uval = -uval;
                        }
                    }
                }
            }
            if (ptr) {
                switch (fmt.size) {
                case 0:
                    *(u32 *)ptr = uval;
                    break;
                case 1:
                    *(u8 *)ptr = uval;
                    break;
                case 2:
                    *(u16 *)ptr = uval;
                    break;
                case 3:
                    *(unsigned long *)ptr = uval;
                    break;
                case 4:
                    *(unsigned long long *)ptr = uval64;
                    break;
                case 5:
                    *(u32 *)ptr = uval;
                    break;
                case 6:
                    *(u32 *)ptr = uval;
                    break;
                case 7:
                    *(unsigned long long *)ptr = uval64;
                    break;
                }
                if (!failed) {
                    items++;
                }
            }
            conversions++;
            break;
        case 'a':
        case 'A':
        case 'e':
        case 'E':
        case 'f':
        case 'F':
        case 'g':
        case 'G':
            if (failed) {
                fval = __float_nan;
            } else {
                fval = __strtold(fmt.width, read, arg, &count, &overflow);
                if (count == 0) {
                    if (!secure) {
                        goto exit;
                    }
                    failed = 1;
                    fval = __float_nan;
                } else {
                    chars += count;
                }
            }
            if (ptr) {
                switch (fmt.size) {
                case 0:
                    *(float *)ptr = fval;
                    break;
                case 8:
                    *(double *)ptr = fval;
                    break;
                case 9:
                    *(double *)ptr = fval;
                    break;
                }
                if (!failed) {
                    items++;
                }
            }
            conversions++;
            break;
        case 'c':
            if (!fmt.widthGiven) {
                fmt.width = 1;
            }
            if (ptr) {
                if (secure) {
                    ok = 1;
                    limit = *(u32 *)((args += 4) - 4);
                }
                count = 0;
                if (failed) {
                    if (limit) {
                        *ptr = 0;
                    }
                    continue;
                }
                start = ptr;
                while (fmt.width-- && (!secure || (ok = (limit > count))) && (ch = read(arg, 0, 0)) != -1) {
                    cc = ch;
                    if (fmt.size == 10) {
                        mbtowc((u16 *)ptr, &cc, 1);
                        ptr++;
                    } else {
                        *ptr++ = cc;
                    }
                    count++;
                }
                cc = ch;
                if (!count || (secure && !ok)) {
                    if (!secure) {
                        goto exit;
                    }
                    failed = 1;
                    if (limit) {
                        *start = 0;
                    }
                    continue;
                }
                chars += count;
                items++;
            } else {
                count = 0;
                while (fmt.width-- && (ch = read(arg, 0, 0)) != -1) {
                    cc = ch;
                    count++;
                }
                cc = ch;
                if (!count) {
                    goto exit;
                }
            }
            conversions++;
            break;
        case '%':
            if (failed) {
                continue;
            }
            while (IS_SPACE(cc = read(arg, 0, 0))) {
                chars++;
            }
            if (cc != '%') {
                read(arg, cc, 1);
                if (!secure) {
                    goto exit;
                }
                failed = 1;
                continue;
            }
            chars++;
            continue;
        case 's':
            if (!failed) {
                cc = read(arg, 0, 0);
                while (IS_SPACE(cc)) {
                    chars++;
                    cc = read(arg, 0, 0);
                }
                read(arg, cc, 1);
            }
        case '[':
            if (ptr) {
                if (secure) {
                    ok = 1;
                    limit = *(u32 *)((args += 4) - 4) - 1;
                }
                count = 0;
                if (failed) {
                    if (limit) {
                        *ptr = 0;
                    }
                    continue;
                }
                start = ptr;
                while (fmt.width-- && (!secure || (ok = (limit >= count))) && (ch = read(arg, 0, 0)) != -1) {
                    cc = ch;
                    if (!(fmt.set[(u8)cc >> 3] & (1 << (cc & 7)))) {
                        break;
                    }
                    if (fmt.size == 10) {
                        mbtowc((u16 *)ptr, &cc, 1);
                        ptr += 2;
                    } else {
                        *ptr++ = cc;
                    }
                    count++;
                }
                cc = ch;
                if (!count || (secure && !ok)) {
                    read(arg, cc, 1);
                    if (!secure) {
                        goto exit;
                    }
                    failed = 1;
                    if (limit) {
                        *start = 0;
                    }
                    continue;
                }
                chars += count;
                if (fmt.size == 10) {
                    *(u16 *)ptr = 0;
                } else {
                    *ptr = 0;
                }
                items++;
            } else {
                count = 0;
                while (fmt.width-- && (ch = read(arg, 0, 0)) != -1) {
                    cc = ch;
                    if (!(fmt.set[(u8)cc >> 3] & (1 << (cc & 7)))) {
                        break;
                    }
                    count++;
                }
                cc = ch;
                if (!count) {
                    read(arg, cc, 1);
                    continue;
                }
                chars += count;
            }
            if (fmt.width >= 0) {
                read(arg, cc, 1);
            }
            conversions++;
            break;
        case 'n':
            if (ptr) {
                switch (fmt.size) {
                case 0:
                    *(int *)ptr = chars;
                    break;
                case 2:
                    *(s16 *)ptr = chars;
                    break;
                case 3:
                    *(long *)ptr = chars;
                    break;
                case 1:
                    *(s8 *)ptr = chars;
                    break;
                case 7:
                    *(long long *)ptr = chars;
                    break;
                }
            }
            continue;
        case 0xFF:
        default:
            goto exit;
        }
    }
exit:
    if (read(arg, 0, 2) && conversions == 0) {
        return -1;
    }
    return items;
}
