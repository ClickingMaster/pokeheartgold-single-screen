#include "overlay96_course_ui_r101_private.h"

void ov96_0220EAA4(void *work) {
    PokeathlonCourseMonObjects_Free(PTR(work, 0x20));
    PokeathlonCourseSprites_FreeResources(PTR(work, 0x1c));
    SpriteSystem_FreeResourcesAndManager(PTR(work, 0xc), PTR(work, 0x10));
    SpriteSystem_Free(PTR(work, 0xc));
}
