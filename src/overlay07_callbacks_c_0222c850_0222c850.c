#include "overlay07_callbacks_w06_private.h"

void ov07_0222C850(W06Point *point, u32 *dest) {
    u32 value = ((u32)-point->y << 16) | (u16)-point->x;
    MIi_CpuClear32(value, dest, 0x300);
}
