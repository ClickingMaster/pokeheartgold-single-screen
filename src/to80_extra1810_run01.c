#pragma force_active on
#include "global.h"
#pragma force_active on
extern void *PokeathlonCourseMonGfx_GetSprite(void*);extern int Sprite_GetDrawFlag(void*);
int ov96_021EB120(void**w);
int ov96_021EB120(void**w){if(Sprite_GetDrawFlag(PokeathlonCourseMonGfx_GetSprite(*w)))return 1;return 0;}
