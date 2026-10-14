#ifndef POKEHEARTGOLD_FIELD_MAP_LOAD_MANAGER_H
#define POKEHEARTGOLD_FIELD_MAP_LOAD_MANAGER_H

#include "field/map_prop_animation.h"

#include "field_system.h"

#define MAP_PROP_MAX 32

void MapLoadManager_Tick(MapLoadManager *mapLoadManager);
MapLoadManager *MapLoadManager_New(MapMatrix *mapMatrix, void *unk34, void *unkC0, MapPropAnimationManager *mapPropAnimationManager, void *unkCC, int unk64, SaveData *saveData);
void MapLoadManager_InitialLoad(MapLoadManager *mapLoadManager, int x, int z);
void MapLoadManager_RenderLoadedMaps(MapLoadManager *mapLoadManager, ModelAttributes *modelAttributes);
void MapLoadManager_End(MapLoadManager *mapLoadManager);
void MapLoadManager_FreeNARCAndLoadedMapBuffers(MapLoadManager *mapLoadManager);
BOOL MapLoadManager_HasEnded(MapLoadManager *mapLoadManager);
void MapLoadManager_TrackTarget(VecFx32 *vec, MapLoadManager *mapLoadManager);
void MapLoadManager_ForgetTrackedTarget(MapLoadManager *mapLoadManager);
void MapLoadManager_GetMapPropManager(int mapSlot, MapLoadManager *mapLoadManager, MapPropManager **mapPropManager);
u8 MapLoadManager_GetCurrentMapSlot(MapLoadManager *mapLoadManager);
u16 *MapLoadManager_GetPermissions(MapLoadManager *mapLoadManager, u8 mapSlot);
u8 *MapLoadManager_GetPermissionBytes(MapLoadManager *mapLoadManager, u8 mapSlot);

#endif // POKEHEARTGOLD_FIELD_MAP_LOAD_MANAGER_H
