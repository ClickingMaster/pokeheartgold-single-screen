#include "global.h"
void WifiSetup_FillBytes(void *, int, u32);
void WifiSetup_FillBytes(void *dst, int value, u32 count) {
    u32 i = 0;
    s8 *p = dst;
    for (; i < count; ) {
        i++;
        *p++ = (s8)value;
    }
}
