#include "private_w10.h"

void ov13_02222B20(void *arg) {
    u8 *p = arg;
    *(u32 *)(p + 0) = 0;
    *(u16 *)(p + 4) = 0;
    *(u16 *)(p + 6) = 0;
}
void ov13_02222B2C(u32 value, void *arg) {
    u8 *p = arg;
    *(u32 *)(p + 0) = value;
    *(u16 *)(p + 4) = 1;
}
