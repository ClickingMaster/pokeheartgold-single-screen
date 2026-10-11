#include "to55_old_ov96_d9_private.h"

int ov96_0220DAA0(Ov96Object *object, s16 targetX, s16 targetY) {
    VecFx32 distances;
    fx32 min;
    int best;
    int i;

    best = 12;
    distances = ov96_0221CEF4;
    min = 1 << 20;
    for (i = 0; i < 3; i++) {
        if (PokeathlonCourseObject_IsInTouchRange(object->sprite, object->position.x, object->position.y, targetX, targetY)) {
            VecFx32 delta = { 0, 0, 0 };
            fx32 difference;
            difference = (targetX << FX32_SHIFT) - object->position.x;
            if (difference < 0) {
                difference = -difference;
            }
            delta.x = difference;
            difference = (targetY << FX32_SHIFT) - object->position.y;
            if (difference < 0) {
                difference = -difference;
            }
            delta.y = difference;
            ((fx32 *)&distances)[i] = VEC_Mag(&delta);
        }
        object++;
    }
    for (i = 0; i < 3; i++) {
        if (((fx32 *)&distances)[i] < min) {
            min = ((fx32 *)&distances)[i];
            best = i;
        }
    }
    return (s8)best;
}
