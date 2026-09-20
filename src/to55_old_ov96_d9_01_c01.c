#include "to55_old_ov96_d9_private.h"

void ov96_0220D910(Ov96Object *object) {
    u8 group;
    int rem;
    int quotient;
    s32 x;
    s32 y;
    VecFx32 zero;

    group = (u8)((object->flags << 12) >> 28);
    rem = group % 3;
    quotient = group / 3;
    ov96_021EB0A4(object->sprite, ov96_0221CFBC[quotient][rem][0], ov96_0221CFBE[quotient][rem][0], &x, &y);
    object->position.x = x << FX32_SHIFT;
    object->position.y = y << FX32_SHIFT;
    object->previous.x = x << FX32_SHIFT;
    object->previous.y = y << FX32_SHIFT;
    zero = (VecFx32) { 0, 0, 0 };
    object->velocity = zero;
    object->flags = ((object->flags & 0xFF0FFFFF) | 0x00200000) & 0xFBFFFFFF;
}
