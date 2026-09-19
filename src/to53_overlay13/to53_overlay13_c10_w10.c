#include "private_w10.h"

void ov13_02222968(void *dst, const void *src, u32 size) {
    MI_CpuCopy8(src, dst, size);
}
void ov13_02222978(void *dst, int value, u32 size) {
    MI_CpuFill8(dst, value, size);
}
