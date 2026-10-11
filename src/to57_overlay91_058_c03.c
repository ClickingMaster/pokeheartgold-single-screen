#include "to57_overlay91_058_private.h"

void ov91_022601F4(u8 *p, u8 *b) {
    u8 *q;
    u32 v;
    int i;
    q = p;
    v = *(u16 *)(b + 2);
    for (i = 0; i < 3; i++, q += 0x78) {
        ModelInstance_SetRotation(q, (u16)v, 1);
    }
}
