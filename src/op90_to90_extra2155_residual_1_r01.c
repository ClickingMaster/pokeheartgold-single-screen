#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
#include <nitro/std/string.h>
// clang-format on

typedef struct STDiWriter {
    u32 remaining;
    char *cursor;
    char *base;
} STDiWriter;

void string_put_char(STDiWriter *w, char c);
void string_fill_char(STDiWriter *w, char c, int n);
void string_put_string(STDiWriter *w, const char *s, int n);

// @fn STD_TVSNPrintf
int STD_TVSNPrintf(char *dest, u32 len, const char *format, va_list va) {
    char prefix[2];
    char digits[24];
    STDiWriter w;
    const char *s;

    s = format;
    w.remaining = len;
    w.cursor = w.base = dest;

    while (*s != 0) {
        if ((u32)(((u8)*s ^ 0x20) - 0xa1) < 0x3c) {
            string_put_char(&w, *s);
            if (*++s != 0) {
                string_put_char(&w, *s++);
            }
        } else if (*s != '%') {
            string_put_char(&w, *s++);
        } else {
            u64 value;
            int count;
            const char *start;
            int radix;
            int prefixlen;
            int flags;
            int width;
            int left;
            int precision;
            int hexoffset;
            int c;

            flags = 0;
            width = 0;
            precision = -1;
            radix = 10;
            hexoffset = 'a' - 10;
            start = s;

            for (;;) {
                switch (c = *++s) {
                case '+':
                    if (s[-1] != ' ') {
                        break;
                    }
                    flags |= 2;
                    continue;
                case ' ':
                    flags |= 1;
                    continue;
                case '-':
                    flags |= 8;
                    continue;
                case '0':
                    flags |= 0x10;
                    continue;
                }
                break;
            }
            if (c == '*') {
                va += 4;
                width = *(int *)(va - 4);
                s++;
                if (width < 0) {
                    width = -width;
                    flags |= 8;
                }
            } else {
                while (*s >= '0' && *s <= '9') {
                    width = width * 10 + *s++ - '0';
                }
            }
            if (*s == '.') {
                c = *++s;
                precision = 0;
                if (c == '*') {
                    va += 4;
                    precision = *(int *)(va - 4);
                    s++;
                    if (precision < 0) {
                        precision = -1;
                    }
                } else {
                    while (*s >= '0' && *s <= '9') {
                        precision = precision * 10 + *s++ - '0';
                    }
                }
            }
            switch (*s) {
            case 'h':
                if (*++s != 'h') {
                    flags |= 0x40;
                } else {
                    s++;
                    flags |= 0x100;
                }
                break;
            case 'l':
                if (*++s != 'l') {
                    flags |= 0x20;
                } else {
                    s++;
                    flags |= 0x80;
                }
                break;
            }
            switch (*s) {
            case 'd':
            case 'i':
                goto put_integer;
            case 'o':
                flags |= 0x1000;
                radix = 8;
                goto put_integer;
            case 'u':
                flags |= 0x1000;
                goto put_integer;
            case 'X':
                hexoffset = 'A' - 10;
                goto put_hexadecimal;
            case 'p':
                flags |= 4;
                precision = 8;
                goto put_hexadecimal;
            case 'x':
                goto put_hexadecimal;
            case 'c':
                if (precision >= 0) {
                    goto put_invalid;
                }
                va += 4;
                c = *(int *)(va - 4);
                if (flags & 8) {
                    string_put_char(&w, (char)c);
                    string_fill_char(&w, ' ', width - 1);
                } else {
                    string_fill_char(&w, (char)((flags & 0x10) ? '0' : ' '), width - 1);
                    string_put_char(&w, (char)c);
                }
                s++;
                continue;
            case 's': {
                int n = 0;
                const char *str;
                va += 4;
                str = *(const char **)(va - 4);
                if (precision < 0) {
                    while (str[n] != 0) {
                        n++;
                    }
                } else {
                    while (n < precision && str[n] != 0) {
                        n++;
                    }
                }
                width -= n;
                if (flags & 8) {
                    string_put_string(&w, str, n);
                    string_fill_char(&w, ' ', width);
                } else {
                    string_fill_char(&w, (char)((flags & 0x10) ? '0' : ' '), width);
                    string_put_string(&w, str, n);
                }
                s++;
                continue;
            }
            case 'n': {
                int n = w.cursor - w.base;
                if (!(flags & 0x100)) {
                    if (flags & 0x40) {
                        va += 4;
                        **(s16 **)(va - 4) = n;
                    } else if (!(flags & 0x80)) {
                        va += 4;
                        **(int **)(va - 4) = n;
                    } else {
                        va += 4;
                        **(s64 **)(va - 4) = n;
                    }
                }
                s++;
                continue;
            }
            case '%':
                if (start + 1 == s) {
                    string_put_char(&w, *s++);
                    continue;
                }
                goto put_invalid;
            default:
            put_invalid:
                string_put_string(&w, start, s - start);
                continue;
            }
        put_hexadecimal:
            flags |= 0x1000;
            radix = 16;
        put_integer:
            prefixlen = 0;
            if (flags & 8) {
                flags &= ~0x10;
            }
            if (precision >= 0) {
                flags &= ~0x10;
            } else {
                precision = 1;
            }
            if (flags & 0x1000) {
                if (flags & 0x100) {
                    va += 4;
                    value = *(u8 *)(va - 4);
                } else if (flags & 0x40) {
                    va += 4;
                    value = *(u16 *)(va - 4);
                } else if (flags & 0x80) {
                    va += 8;
                    value = *(u64 *)(va - 8);
                } else {
                    va += 4;
                    value = *(u32 *)(va - 4);
                }
                flags &= ~3;
                if (flags & 4) {
                    if (radix == 16) {
                        if (value != 0) {
                            prefix[0] = hexoffset + 'X' - 'A' + 10;
                            prefix[1] = '0';
                            prefixlen = 2;
                        }
                    } else if (radix == 8) {
                        prefix[0] = '0';
                        prefixlen = 1;
                    }
                }
            } else {
                if (flags & 0x100) {
                    va += 4;
                    value = *(s8 *)(va - 4);
                } else if (flags & 0x40) {
                    va += 4;
                    value = *(s16 *)(va - 4);
                } else if (flags & 0x80) {
                    va += 8;
                    value = *(s64 *)(va - 8);
                } else {
                    va += 4;
                    value = *(s32 *)(va - 4);
                }
                if ((value >> 32) & 0x80000000) {
                    value = ~value + 1;
                    prefix[0] = '-';
                    prefixlen = 1;
                } else if (value != 0 || precision != 0) {
                    if (flags & 2) {
                        prefix[0] = '+';
                        prefixlen = 1;
                    } else if (flags & 1) {
                        prefix[0] = ' ';
                        prefixlen = 1;
                    }
                }
            }
            count = 0;
            switch (radix) {
            case 8:
                while (value != 0) {
                    int d = (int)(value & 7);
                    value >>= 3;
                    digits[count++] = d + '0';
                }
                break;
            case 10:
                if ((value >> 32) == 0) {
                    u32 v = (u32)value;
                    while (v != 0) {
                        u32 q = v / 10;
                        u32 r = v - q * 10;
                        v = q;
                        digits[count++] = r + '0';
                    }
                } else {
                    while (value != 0) {
                        u64 q = value / 10;
                        digits[count++] = (int)(value - q * 10) + '0';
                        value = q;
                    }
                }
                break;
            case 16:
                while (value != 0) {
                    int d = (int)(value & 15);
                    value >>= 4;
                    digits[count++] = d < 10 ? d + '0' : d + hexoffset;
                }
                break;
            }
            if (prefixlen > 0 && prefix[0] == '0') {
                digits[count++] = '0';
                prefixlen = 0;
            }
            precision -= count;
            if (flags & 0x10) {
                if (precision < width - count - prefixlen) {
                    precision = width - count - prefixlen;
                }
            }
            if (precision > 0) {
                width -= precision;
            }
            width -= prefixlen + count;
            left = flags & 8;
            if (!left) {
                string_fill_char(&w, ' ', width);
            }
            while (prefixlen > 0) {
                string_put_char(&w, prefix[--prefixlen]);
            }
            string_fill_char(&w, '0', precision);
            while (count > 0) {
                string_put_char(&w, digits[--count]);
            }
            if (left) {
                string_fill_char(&w, ' ', width);
            }
            s++;
        }
    }
    if (w.remaining != 0) {
        *w.cursor = 0;
    } else if (len != 0) {
        w.base[len - 1] = 0;
    }
    return w.cursor - w.base;
}
