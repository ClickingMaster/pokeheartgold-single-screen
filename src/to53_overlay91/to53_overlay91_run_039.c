#include "private_w19.h"

BOOL ov91_0225E990(u8 *p) {
    s32 v = *(s32 *)(p + 0x40);
    return v >= 0xbc000 && v <= 0xf0000;
}


BOOL ov91_0225E9AC(u8 *p) {
    return *(s32 *)(p + 0x40) <= 0x78000;
}


BOOL ov91_0225E9C0(u8 *p) {
    s32 v = *(s32 *)(p + 0x40);
    if (v >= 0xf0000) {
        if (*(s32 *)(p + 0x3c) == 1) {
            return TRUE;
        }
    } else if (*(s32 *)(p + 0x3c) == 0) {
        *(s32 *)(p + 0x3c) = 1;
    }
    return FALSE;
}


void ov91_0225E9E4(u8 *p, u8 *base) {
    p[0] = 3;
    *(u16 *)(p + 0x38) = 8;
    *(u16 *)(p + 0x3a) = *(u32 *)(base + 0x1c);
}
