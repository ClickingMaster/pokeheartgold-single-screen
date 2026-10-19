#ifndef POKEHEARTGOLD_FIELD_MAP_LOAD_MANAGER_INTERNAL_H
#define POKEHEARTGOLD_FIELD_MAP_LOAD_MANAGER_INTERNAL_H

#include "field/bdhc.h"
#include "field/field_control.h"
#include "field/map_load_manager.h"

// The map slots of the map load manager, for the terrain and collision queries.

int TileIndex_GetQuadrant(int, int);
u8 MapLoadManager_GetMapSlotOfMatrixIndex(int, int, MapLoadManager *);
int MapLoadManager_GetMapMatrixIndex(MapLoadManager *, u8);
BOOL MapLoadManager_GetMapSlotAt(MapLoadManager *, int, int, u8 *);
Bdhc *MapLoadManager_GetBdhc(MapLoadManager *, u8);
SoundplateStruct *MapLoadManager_GetSoundplates(MapLoadManager *, u8);

#endif // POKEHEARTGOLD_FIELD_MAP_LOAD_MANAGER_INTERNAL_H
