#ifndef POKEHEARTGOLD_GTS_H
#define POKEHEARTGOLD_GTS_H

#include "overlay_manager.h"

// The GTS (Global Trade System) app, launched by Gts_LaunchApp with a GtsArgs.
BOOL Gts_Init(OverlayManager *man, int *state);
BOOL Gts_Main(OverlayManager *man, int *state);
BOOL Gts_Exit(OverlayManager *man, int *state);

#endif // POKEHEARTGOLD_GTS_H
