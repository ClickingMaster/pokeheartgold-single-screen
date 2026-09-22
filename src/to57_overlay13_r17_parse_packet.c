#include "global.h"

#define Swap16(value) ((u16)((u8)((value) >> 8) | (((value) << 8) & 0xff00)))

u8 *ov13_02224624(u8 *data, u32 *type, u32 *length);

u8 *ov13_02224624(u8 *data, u32 *type, u32 *length) {
    u32 sum = 0;
    u8 *end;
    u8 *p;

    *type = Swap16(*(u16 *)data);
    end = data + 6 + (*length = Swap16(*(u16 *)(data + 2)));
    for (p = data; p < end; p++) {
        sum += *p;
    }
    if ((u16)sum != Swap16(*(u16 *)end)) {
        return NULL;
    }
    return data + 6;
}
