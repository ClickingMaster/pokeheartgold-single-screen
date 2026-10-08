#pragma force_active on
typedef unsigned long u32;
typedef struct StringReader {
    const char *str;
    int offset;
} StringReader;
extern int __StringRead(StringReader *, int, int);
extern u32 __strtoul(int, u32, int (*)(StringReader *, int, int), StringReader *, int *, int *, int *);
extern int errno;
u32 strtoul(const char *, char **, int);
// @fn strtoul
u32 strtoul(const char *str, char **end, int base) {
    int n;
    int negative;
    int overflow;
    StringReader reader;
    u32 result;
    reader.str = str;
    reader.offset = 0;
    result = __strtoul(base, 0x7fffffff, __StringRead, &reader, &n, &negative, &overflow);
    if (end != 0) {
        *end = (char *)str + n;
    }
    if (overflow) {
        result = 0xffffffff;
        errno = 34;
    } else if (negative != 0) {
        result = -result;
    }
    return result;
}
