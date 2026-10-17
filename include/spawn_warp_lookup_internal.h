#ifndef POKEHEARTGOLD_SPAWN_WARP_LOOKUP_INTERNAL_H
#define POKEHEARTGOLD_SPAWN_WARP_LOOKUP_INTERNAL_H

#include "field_system.h"

u32 GetMomSpawnId(void);
void GetFlyWarpData(s32 spawnId, Location *warp);
void GetDeathWarpData(s32 spawnId, Location *warp);
void GetSpecialSpawnWarpData(s32 spawnId, Location *warp);
u32 MapHeader_GetSpawnIdForDeathWarp(u32 mapId);
u32 GetSpawnIdForFlyMap(u32 mapId);
void FlypointFlagAction(FieldSystem *fieldSystem, u32 mapId);

#endif // POKEHEARTGOLD_SPAWN_WARP_LOOKUP_INTERNAL_H
