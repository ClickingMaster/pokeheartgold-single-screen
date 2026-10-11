#ifndef POKEHEARTGOLD_HATCH_EGG_APP_H
#define POKEHEARTGOLD_HATCH_EGG_APP_H

#include "overlay_manager.h"

// The egg hatching scene (overlay 95), run by the hatch egg task (hatch_egg_task.c): the egg shakes and
// cracks (or, for a Manaphy egg, glows), the Pokemon appears and the player is asked whether to give
// it a nickname.
BOOL HatchEggApp_Init(OverlayManager *man, int *state);
BOOL HatchEggApp_Main(OverlayManager *man, int *state);
BOOL HatchEggApp_Exit(OverlayManager *man, int *state);

#endif // POKEHEARTGOLD_HATCH_EGG_APP_H
