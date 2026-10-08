#pragma force_active on
#include "global.h"
#include <nitro/code32.h>
int ov38_022219A4(char *buf, u32 val, int n);
// @fn ov38_022219A4
int ov38_022219A4(char *buf, u32 val, int n) {
    int len = 0;
    int i;
    if (n > 8) return -1;
    for (i = 1; i <= n; i++) {
        u32 d = (val >> ((n - i) * 4)) & 0xF;
        if (d < 10) buf[len++] = d + '0'; else buf[len++] = d + 'a' - 10;
    }
    return len;
}
