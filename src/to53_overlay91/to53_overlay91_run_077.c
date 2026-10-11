#include "private_w20.h"

void ov91_022611D0(u8 *p) {
    ModelInstance_SetVisible(p, 0);
    ModelInstance_SetVisible(p + 0x78, 1);
    *(u32 *)(p + 0x1dc) = 0;
    *(u32 *)(p + 0x1e0) = 0x1000;
    p[0x1d8] = 1;
    p[0x1d9] = 0;
}

void ov91_02261200(u8 *p, s32 value) {
    *(s32 *)(p + 0x1d4) = value;
}
