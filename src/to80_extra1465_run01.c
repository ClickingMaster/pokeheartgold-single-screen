#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#pragma force_active on

typedef struct {
    u8 x, y, s[256];
} RC4;

void CPSi_rc4_init(RC4 *, const u8 *, int);
void CPSi_rc4_crypt(RC4 *, u8 *, int);

void CPSi_rc4_crypt(RC4 *w, u8 *buf, int len) {
    int i, x = w->x, y = w->y;
    u8 *s = w->s;
    for (i = 0; i < len; i++) {
        int a, b;
        x = (x + 1) & 255;
        a = s[x];
        y = (y + a) & 255;
        b = s[y];
        s[x] = b;
        s[y] = a;
        buf[i] ^= s[(a + b) & 255];
    }
    w->x = x;
    w->y = y;
}
