#pragma force_active on
#include <nitro/types.h>
char *strcpy(char *, const char *);
// @fn strcpy
char *strcpy(char *dst, const char *src) {
    u8 *d = (u8 *)dst;
    const u8 *s = (const u8 *)src;
    u32 align = (u32)s & 3;
    u32 x;
    u32 y;
    u8 c;
    if (((u32)d & 3) == align) {
        if (align) {
            c = *s;
            *d = c;
            if (!c) {
                return dst;
            }
            align = 3 - align;
            if (align) {
                do {
                    c = *++s;
                    *++d = c;
                    if (!c) {
                        return dst;
                    }
                } while (--align);
            }
            d++;
            s++;
        }
        x = *(const u32 *)s;
        if (!(((x + 0xfefefeff) & ~x) & 0x80808080)) {
            d -= 4;
            do {
                *(u32 *)(d += 4) = x;
                x = *(const u32 *)(s += 4);
                y = x + 0xfefefeff;
                y &= ~x;
            } while (!(y & 0x80808080));
            d += 4;
        }
    }
    c = *s;
    *d = c;
    if (!c) {
        return dst;
    }
    do {
        c = *++s;
        *++d = c;
    } while (c);
    return dst;
}
