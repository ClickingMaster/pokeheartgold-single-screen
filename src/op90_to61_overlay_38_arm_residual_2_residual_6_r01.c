#pragma force_active on
#include "global.h"
#include <nitro/code32.h>
extern u64 ov38_02224EF4[];
int ov38_022218CC(char *buf, u64 val, int width);
// @fn ov38_022218CC
int ov38_022218CC(char *buf, u64 val, int width) {
    int i;
    int len = 0;
    BOOL started;
    if (val >= 100000000000000000ULL) {
        return len - 1;
    }
    i = 0;
    started = FALSE;
    for (; i < 16; i++) {
        u64 p = ov38_02224EF4[i];
        if (val >= p) {
            u32 q = val / p;
            val -= q * p;
            buf[len] = q + '0';
            started = TRUE;
            len++;
        } else if (started || 17 - i <= width) {
            buf[len] = '0';
            len++;
        }
    }
    buf[len] = val + '0';
    return len + 1;
}
