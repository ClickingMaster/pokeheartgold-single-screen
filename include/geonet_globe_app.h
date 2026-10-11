#ifndef POKEHEARTGOLD_GEONET_GLOBE_APP_H
#define POKEHEARTGOLD_GEONET_GLOBE_APP_H

#include "overlay_manager.h"

BOOL GeonetGlobe_Init(OverlayManager *man, int *state);
BOOL GeonetGlobe_Main(OverlayManager *man, int *state);
BOOL GeonetGlobe_Exit(OverlayManager *man, int *state);

#endif // POKEHEARTGOLD_GEONET_GLOBE_APP_H
