#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#pragma force_active on
int count_digits(const u16 *, int);
void CPSi_big_sub(u16 *, const u16 *, const u16 *, int);

void CPSi_big_sub(u16 *out, const u16 *a, const u16 *b, int n) {
    int limit = count_digits(a, n), j = count_digits(b, n), i, carry;
    if (limit < j) {
        limit = j;
    }
    if (limit != n) {
        limit++;
    }
    carry = 0;
    i = 0;
    while (i < limit || (i < n && carry != 0)) {
        int v = a[i] - b[i] + carry;
        out[i] = v;
        carry = v >> 16;
        i++;
    }
    if (out != a && out != b) {
        MI_CpuFill8(out + i, 0, (n - i) * 2);
    }
}
