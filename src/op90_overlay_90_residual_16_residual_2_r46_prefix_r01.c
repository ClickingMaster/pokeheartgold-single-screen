#pragma force_active on
#include "global.h"
extern u8 ov90_0225C284[][4];
int ov90_0225A050(u32 n, u8 a, u8 b);
// @fn ov90_0225A050
int ov90_0225A050(u32 n, u8 a, u8 b) {
    int ib, ia;
    u32 i;
    int r;
    for (i = 0; i < n; i++) {
        if (a == ov90_0225C284[n - 1][i]) {
            ia = i;
        }
        if (b == ov90_0225C284[n - 1][i]) {
            ib = i;
        }
    }
    r = ia - ib;
    if (r < 0) {
        r += n;
    }
    return r;
}
