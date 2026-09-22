#include "to57_r3_02_private.h"

void ov13_02222874(u8 *dst, s32 size, u8 *tmp) {
    s32 half = (s32)(size + ((u32)size >> 31)) >> 1;
    ov13_02222968(tmp, dst + half, half);
    ov13_02222968(tmp + half, dst, half);
    ov13_02222968(dst, tmp, size);
}
