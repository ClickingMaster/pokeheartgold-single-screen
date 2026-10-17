#ifndef POKEHEARTGOLD_WIFI_PLAZA_VISITOR_PROFILES_H
#define POKEHEARTGOLD_WIFI_PLAZA_VISITOR_PROFILES_H

#include "overlay_manager.h"

BOOL VisitorProfiles_Init(OverlayManager *man, int *state);
BOOL VisitorProfiles_Main(OverlayManager *man, int *state);
BOOL VisitorProfiles_Exit(OverlayManager *man, int *state);

#endif // POKEHEARTGOLD_WIFI_PLAZA_VISITOR_PROFILES_H
