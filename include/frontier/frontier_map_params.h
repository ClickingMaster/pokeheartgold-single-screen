#ifndef POKEHEARTGOLD_FRONTIER_FRONTIER_MAP_PARAMS_H
#define POKEHEARTGOLD_FRONTIER_FRONTIER_MAP_PARAMS_H

#include "global.h"

u32 FrontierMap_GetParam(u32 mapId, u32 field);
void FrontierMap_InitEffect(void *arg, void *effect, u32 mapId);
void FrontierMap_ExitEffect(void *arg, void *effect, u32 mapId);

#endif // POKEHEARTGOLD_FRONTIER_FRONTIER_MAP_PARAMS_H
