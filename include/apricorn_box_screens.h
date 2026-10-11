#ifndef POKEHEARTGOLD_APRICORN_BOX_SCREENS_H
#define POKEHEARTGOLD_APRICORN_BOX_SCREENS_H

#include "overlay_manager.h"

// OVY_59, launched by the Apricorn Box app (apricorn_box_overlay.c, args ApricornBoxApp).

// The Apricorn Box screen: view the apricorns, put them into the Apriblender, give them to Kurt.
BOOL ApricornBoxScreen_Init(OverlayManager *man, int *state);
BOOL ApricornBoxScreen_Main(OverlayManager *man, int *state);
BOOL ApricornBoxScreen_Exit(OverlayManager *man, int *state);

// Give the Aprijuice to a party Pokemon and show the change of its Performance.
BOOL AprijuiceScreen_Init(OverlayManager *man, int *state);
BOOL AprijuiceScreen_Main(OverlayManager *man, int *state);
BOOL AprijuiceScreen_Exit(OverlayManager *man, int *state);

#endif // POKEHEARTGOLD_APRICORN_BOX_SCREENS_H
