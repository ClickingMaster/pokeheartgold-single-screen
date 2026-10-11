#ifndef POKEHEARTGOLD_SHIP_DEPARTURE_CUTSCENE_H
#define POKEHEARTGOLD_SHIP_DEPARTURE_CUTSCENE_H

#include "field/model_attributes.h"

#include "overlay_manager.h"

// The 3D cutscene of the S.S. Aqua leaving port, played when the player boards at Olivine (port 0)
// or Vermilion (port 1).
typedef struct ShipDepartureCutsceneArgs {
    u8 port;
    u8 isNight;
    ModelAttributes *modelAttributes;
} ShipDepartureCutsceneArgs;

BOOL ShipDepartureCutscene_Init(OverlayManager *man, int *state);
BOOL ShipDepartureCutscene_Main(OverlayManager *man, int *state);
BOOL ShipDepartureCutscene_Exit(OverlayManager *man, int *state);

#endif // POKEHEARTGOLD_SHIP_DEPARTURE_CUTSCENE_H
