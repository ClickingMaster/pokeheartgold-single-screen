#include "private_w20.h"

void ov91_0226137C(u8 *p, u32 value) {
    *(u32 *)(p + 0x198) = value;
}

void ov91_02261384(u8 *p, u16 value) {
    u16 angle[4];
    Camera_GetAngle(angle, *(void **)(p + 0x190));
    angle[0] = value;
    Camera_SetAnglePos(angle, *(void **)(p + 0x190));
}

void ov91_022613AC(u8 *p, u8 value) {
    p[0] = 0;
    p[1] = 0;
    p[2] = 0;
    p[3] = 0;
    p[4] = 0;
    p[5] = 0;
    p[6] = 0;
    p[7] = 0;
    *(u16 *)(p + 4) = 2;
    p[6] = value;
}
