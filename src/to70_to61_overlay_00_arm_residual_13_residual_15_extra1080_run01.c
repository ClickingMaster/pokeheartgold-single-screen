#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
extern int sprintf(char *, const char *, ...);
#include <string.h>
extern const char ov00_0221A520[];
extern u8 __lower_mapC[];
extern int ov00_02213FD4(const char *, int);
extern void ov00_02212CD0(void *, const char *, const char *);

static inline u32 swap16(s32 value) {
    return ((value >> 8) & 255) | ((value << 8) & 0xff00);
}

static inline int lower(int c) {
    if (c < 0 || c >= 128) {
        return c;
    }
    return __lower_mapC[c];
}

void ov00_02213044(void *, const char *, int);
u32 ov00_0221321C(const char *, u32);

u32 ov00_0221321C(const char *p, u32 divisor) {
    u32 hash = 0;
    while (*p) {
        hash = hash * 0x9ccf9319 + lower(*p);
        p++;
    }
    return hash % divisor;
}
