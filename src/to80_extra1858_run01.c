#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#pragma force_active on
extern int count_digits(u16 *, int);
extern void MI_CpuFill8(void *, u8, u32);
extern void CPSi_big_add_part(u16 *, u32, int, int);
void CPSi_big_sqr(u16 *, u16 *, int);
#pragma opt_lifetimes off

void CPSi_big_sqr(u16 *out, u16 *in, int n) {
    int digits = count_digits(in, n);
    int i, j;
    if (n > digits * 2) {
        MI_CpuFill8(out + digits * 2, 0, (n - digits * 2) * 2);
    }
    i = 0;
    if (digits > 0) {
        j = i;
        do {
            u32 v;
            if (j >= n) {
                break;
            }
            v = in[i] * in[i];
            out[j] = v;
            if (j == n - 1) {
                break;
            }
            out[j + 1] = v >> 16;
            i++;
            j += 2;
        } while (i < digits);
    }
    {
        int k;
        for (k = 0; k < digits; k++) {
            for (i = k + 1; i < digits && (j = k + i) < n; i++) {
                u32 v = (u32)in[i] * in[k];
                if (v <= 0x7fff8000) {
                    CPSi_big_add_part(out, v * 2, j, n);
                } else {
                    CPSi_big_add_part(out, v, j, n);
                    CPSi_big_add_part(out, v, j, n);
                }
            }
        }
    }
}
