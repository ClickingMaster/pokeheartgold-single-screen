#ifndef POKEHEARTGOLD_SPAWN_WARP_H
#define POKEHEARTGOLD_SPAWN_WARP_H

#include "script.h"

u16 GetMomSpawnId(void);
void GetDeathWarpData(u16 spawnId, Location *dest);
void GetSpecialSpawnWarpData(u16 spawnId, Location *dest);
u32 MapHeader_GetSpawnIdForDeathWarp(u32 mapId);

#endif // POKEHEARTGOLD_SPAWN_WARP_H
