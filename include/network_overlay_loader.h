#ifndef POKEHEARTGOLD_NETWORK_OVERLAY_LOADER_H
#define POKEHEARTGOLD_NETWORK_OVERLAY_LOADER_H

#include "heap.h"
#include "overlay_manager.h"

BOOL LoadDwcOverlay(void);
void UnloadDwcOverlay(void);
BOOL LoadOVY13(void);
void UnloadOVY13(void);
void RunNintendoWFCSetupAndReset(void *unused, enum HeapID heapID);
BOOL LoadOVY38(void);
void UnloadOVY38(void);
BOOL NintendoWFCSetupApp_Init(OverlayManager *man, int *state);

#endif // POKEHEARTGOLD_NETWORK_OVERLAY_LOADER_H
