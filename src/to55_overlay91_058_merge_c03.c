#include "to55_overlay91_merge_private.h"

void ov91_02260218(u8 *p, u8 *b) {
    int i;
    for (i = 0; i < 3; i++, p += 0x78) {
        u32 v = *(u32 *)(b + 0xd4);
        ModelInstance_SetScale(p, v, v, v);
    }
}

void ov91_0226023C(u8 *p) {
    int i;
    for (i = 0; i < 3; i++, p += 0x78) {
        ModelInstance_Draw(p);
    }
}
