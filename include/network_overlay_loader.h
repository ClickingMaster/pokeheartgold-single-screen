#ifndef POKEHEARTGOLD_NETWORK_OVERLAY_LOADER_H
#define POKEHEARTGOLD_NETWORK_OVERLAY_LOADER_H

#include "heap.h"
#include "overlay_manager.h"

extern const OverlayManagerTemplate gApp_MainMenu_SelectOption_NintendoWFCSetup;

BOOL LoadDwcOverlay(void);
void UnloadDwcOverlay(void);
BOOL LoadOVY13(void);
BOOL LoadOVY38(void);
void UnloadOVY38(void);

#endif // POKEHEARTGOLD_NETWORK_OVERLAY_LOADER_H
