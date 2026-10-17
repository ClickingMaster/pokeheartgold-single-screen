#include "global.h"

#include "pokeathlon/pokeathlon_course_internal.h"

void PokeathlonCourseMonObjects_SetPalette(PokeathlonCourseMonObjects *objects, u8 index, u32 vram, void *palette);

// Loads a palette into the slot of mon object `index`.
void PokeathlonCourseMonObjects_SetPalette(PokeathlonCourseMonObjects *objects, u8 index, u32 vram, void *palette) {
    PokeathlonCourseMonGfx_SetPalette(objects->gfxManager, index, (int)vram, palette);
}
