#ifndef TO52_SOL_R6_W05_PRIVATE_H
#define TO52_SOL_R6_W05_PRIVATE_H

#include "global.h"

void *Sprite_GetPaletteProxy(void *sprite);
void GfGfxLoader_GXLoadPal(int narc, int member, int location, int offset, int size, int heap);
void TintPalette_GrayScale(void *palette, int size);
void GF_AssertFail(void);
void *PokeathlonCourse_GetHeapAllocPtr4(void *course);
int PokeathlonCourse_GetField1ED(void *course);
void PokeathlonCourse_IncrementField1ED(void *course);
void PokeathlonCourse_SetStateTransitionType(void *course, int type);
void ov96_021EEFAC(void *course);
void sub_0203A994(int value);
void GfGfx_EngineATogglePlanes(int plane, int enable);
void GfGfx_EngineBTogglePlanes(int plane, int enable);
void BeginNormalPaletteFade(int, int, int, int, int, int, int);
int IsPaletteFadeFinished(void);
void ov96_021EE8CC(void *object, int value);
int ov96_021EE830(void *object);
void ov96_021EE994(void *object);

#endif
