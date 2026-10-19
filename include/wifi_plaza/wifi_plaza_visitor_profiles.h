#ifndef POKEHEARTGOLD_WIFI_PLAZA_VISITOR_PROFILES_H
#define POKEHEARTGOLD_WIFI_PLAZA_VISITOR_PROFILES_H

#include "overlay_manager.h"
#include "save.h"

// Copied around as one word; only its low byte, the current hour (0-23), is read.
typedef union VisitorProfilesClock {
    u32 raw;
    u8 hour;
} VisitorProfilesClock;

// Arguments built by the lobby overlay.
typedef struct VisitorProfilesArgs {
    void *visitors; // lobby visitor table (OVY_45), read through WiFiPlazaPlaces_GetCountry and friends
    SaveData *saveData;
    VisitorProfilesClock clock;
    void *lobby;
} VisitorProfilesArgs;

BOOL VisitorProfiles_Init(OverlayManager *man, int *state);
BOOL VisitorProfiles_Main(OverlayManager *man, int *state);
BOOL VisitorProfiles_Exit(OverlayManager *man, int *state);

#endif // POKEHEARTGOLD_WIFI_PLAZA_VISITOR_PROFILES_H
