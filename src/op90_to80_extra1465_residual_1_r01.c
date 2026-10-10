#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

typedef struct {
    u8 x, y, s[256];
} RC4;

void CPSi_rc4_init(RC4 *, const u8 *, int);

void CPSi_rc4_init(RC4 *w, const u8 *key, int len) {
    int a;
    u8 *s;
    int i;
    int j;
    int b;
    int k;
    s = w->s;
    w->x = 0;
    w->y = 0;
    for (i = 0; i < 256; i++) {
        s[i] = i;
    }
    k = 0;
    j = 0;
    for (i = 0; i < 256; i++) {
        a = s[i];
        b = key[k];
        k = (k + 1) & 255;
        j = (j + (a + b)) & 255;
        b = s[j];
        if (k >= len) {
            k = 0;
        }
        s[i] = b;
        s[j] = a;
    }
}
