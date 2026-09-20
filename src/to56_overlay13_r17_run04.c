#include "to56_overlay13_r17_run04_private.h"

u8 *ov13_0222483C(u8 *dst, u16 type, const void *src, u32 length) {
    int result;
    int size;
    memset(dst, 0, 8);
    result = ov13_022247DC(dst + 8, type, src, length);
    result -= (int)dst;
    size = result;
    size -= 8;
    *(u16 *)dst = (u16)size;
    return (u8 *)result;
}
