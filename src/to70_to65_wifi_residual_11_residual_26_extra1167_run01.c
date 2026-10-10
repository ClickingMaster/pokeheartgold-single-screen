#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
u32 parse_time(const u8 *p, int type);
#include <string.h>

typedef struct {
    u32 a;
    int length;
    void *modulus;
    int explen;
    void *exponent;
} Key;

extern void *(*_021D4634)(int);
extern void (*_021D4660)(void *);
extern void CPSi_big_from_char(u16 *, void *, int, int);
extern void CPSi_big_power(u16 *, u16 *, u16 *, int, u16 *);
extern void exit_computebound(int);
extern void CPSi_char_from_big(u8 *, u16 *, int, int);
extern int enter_computebound(void);
int validate_signature(u8 *, Key *);

u32 parse_time(const u8 *p, int type) {
    u32 year = p[0] * 10 + p[1] - 528;
    p += 2;
    if (type == 23) {
        if (year < 50) {
            year += 2000;
        } else {
            year += 1900;
        }
    } else {
        u32 low = p[0] * 10 + p[1] - 528;
        p += 2;
        year = year * 100 + low;
    }
    return (year << 16) + ((p[0] * 10 + p[1] - 528) << 8) + (p[2] * 10 + p[3] - 528);
}
