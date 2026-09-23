#include "global.h"
void ov91_0225F388(u8 *data);
void ov91_0225F388(u8 *data) {
    u8 *p = data;
    u32 count = 24;
    do { *p++ = 0; } while (--count != 0);
    *(u32 *)(data + 0x14) = 0xFFFF;
}
