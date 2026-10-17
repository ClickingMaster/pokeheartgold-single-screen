#ifndef POKEHEARTGOLD_SPAWN_WARP_H
#define POKEHEARTGOLD_SPAWN_WARP_H

#include "script.h"

u32 GetMomSpawnId(void);
void GetFlyWarpData(s32 spawnId, Location *dest);
void GetDeathWarpData(s32 spawnId, Location *dest);
void GetSpecialSpawnWarpData(s32 spawnId, Location *dest);
u32 MapHeader_GetSpawnIdForDeathWarp(u32 mapId);

#endif // POKEHEARTGOLD_SPAWN_WARP_H
