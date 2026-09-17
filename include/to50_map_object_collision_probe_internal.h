#ifndef POKEHEARTGOLD_TO50_MAP_OBJECT_COLLISION_PROBE_INTERNAL_H
#define POKEHEARTGOLD_TO50_MAP_OBJECT_COLLISION_PROBE_INTERNAL_H

#include "map_object_behavior_internal.h"

int GetDeltaXByFacingDirection(int direction);
int GetDeltaYByFacingDirection(int direction);
int sub_02060B90(LocalMapObject *object, int x, int y, int z, int direction);
int sub_02064468(LocalMapObject *object, int direction, int distance);

#endif
