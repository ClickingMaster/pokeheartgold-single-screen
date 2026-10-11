#include "overlay96_course_entity_r118_private.h"

void ov96_02218510(void *arg, int mode) {
    void *value = PokeathlonCourseMonObject_GetGfx(arg);
    int offset = PokeathlonCourseMonGfx_GetTopRow(value);
    VecFx32 vec = { 0 };
    int width = PokeathlonCourseMonGfx_GetInfo(value)[2] != 0 ? 0x40 : 0x20;

    if (mode != 0) {
        vec.x = (width / 2) << FX32_SHIFT;
        vec.y = (offset + (width - offset) / 2) << FX32_SHIFT;
    } else {
        vec.x = (width / 2) << FX32_SHIFT;
        vec.y = width << FX32_SHIFT;
    }
    PokeathlonCourseMonObject_SetAffineMatrix(arg, &vec);
}

void ov96_02218578(Ov96CourseEntityR118 *object, int state) {
    object->unk5E = state == 4 ? 0x3C : 0;
    if (state == 7) {
        object->unk59 = 0x10;
    }
    object->state = state;
}
