#pragma force_active on
#include "global.h"
#pragma force_active on
extern void *ov96_021E8BAC(void*);extern int Sprite_GetDrawFlag(void*);
int ov96_021EB120(void**w);
int ov96_021EB120(void**w){if(Sprite_GetDrawFlag(ov96_021E8BAC(*w)))return 1;return 0;}
