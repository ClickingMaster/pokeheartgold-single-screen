#include "to56_overlay13_r17_run03_private.h"

u8 *ov13_02224694(u8 **cursor, u8 *end, u32 *type, u32 *length) {
    u8 *p = *cursor;
    u32 padded;
    if (p >= end) {
        return NULL;
    }
    *type = Swap16(*(u16 *)p);
    *length = Swap16(*(u16 *)(p + 2));
    p += 4;
    padded = (*length + 11) & ~7;
    padded -= 4;
    *cursor = p + padded;
    return p;
}

u8 *ov13_022246E4(u8 *data, u32 *type, u32 *length) {
    u8 *cursor = data + 8;
    return ov13_02224694(&cursor, cursor + Swap16(*(u16 *)data), type, length);
}
