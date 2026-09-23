#include "global.h"
void ov13_02226C7C(void *, int, u32);
void ov13_02226C7C(void *dst, int value, u32 count) {
    u32 i = 0;
    s8 *p = dst;
    for (; i < count; ) {
        i++;
        *p++ = (s8)value;
    }
}
