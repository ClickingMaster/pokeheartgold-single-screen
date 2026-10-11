#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#pragma force_active on

typedef struct {
    u8 pad[12];
    void *cipher;
} S;

extern void *(*gCpsAlloc)(int);
extern void (*gCpsFree)(void *);
extern int make_ciphertext(void *, u8 *);
extern u32 CPSi_TcpWrite2Raw(void *, int, int, int, S *);
int CPSi_SslWrite2(u8 *, u32, u8 *, int, S *);

int CPSi_SslWrite2(u8 *a, u32 na, u8 *b, int nb, S *s) {
    void *cipher = s->cipher;
    int total = 0;
    u8 *p;
    int left = na + nb;
    int first;
    do {
        int n = left > 2895 ? 2895 : left;
        p = gCpsAlloc(n + 25);
        int second;
        if (!p) {
            break;
        }
        first = na >= (u32)n ? n : na;
        second = n - first;
        MI_CpuCopy8(a, p + 5, first);
        a += first;
        na -= first;
        MI_CpuCopy8(b, p + 5 + first, second);
        p[0] = 23;
        p[1] = 3;
        p[2] = 0;
        p[3] = n >> 8;
        b += second;
        p[4] = n;
        first = make_ciphertext(cipher, p);
        if (CPSi_TcpWrite2Raw(p, first, 0, 0, s) < (u32)first) {
            n = 0;
        }
        gCpsFree(p);
        left -= n;
        total += n;
        if (!left || !n) {
            break;
        }
    } while (1);
    return total;
}
