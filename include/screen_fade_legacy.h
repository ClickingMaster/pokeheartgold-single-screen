#ifndef POKEHEARTGOLD_SCREEN_FADE_LEGACY_H
#define POKEHEARTGOLD_SCREEN_FADE_LEGACY_H

#include "heap.h"

void BeginNormalPaletteFade(int pattern, int typeTop, int typeBottom, u16 color, int duration, int framesPer, enum HeapID heapID);
void StopPaletteFade(void);
void sub_0200FBDC(int a0);
void SetMasterBrightnessByColor(int screen, int color);
void HandleFadeUpdateFrame(void);
BOOL IsPaletteFadeFinished(void);
void SetMasterBrightnessNeutral(u32 screen);

#endif // POKEHEARTGOLD_SCREEN_FADE_LEGACY_H
