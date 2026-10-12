#ifndef POKEHEARTGOLD_MAP_OBJECT_MOVEMENT_UTILITIES_H
#define POKEHEARTGOLD_MAP_OBJECT_MOVEMENT_UTILITIES_H

#include "map_object.h"
#include "task.h"

BOOL MapObject_UpdateHeight(LocalMapObject *object);
u32 GetOppositeDirection(u32 direction);
BOOL sub_02060E54(LocalMapObject *object, int tile);
BOOL MapObject_CheckSnowBehavior(LocalMapObject *object, int tile);
int GetDeltaXByFacingDirection(int direction);
int GetDeltaYByFacingDirection(int direction);
void SetPositionToTileCenter(int x, int y, VecFx32 *a2);
BOOL FieldSystem_SnapToTerrainHeight(FieldSystem *fieldSystem, VecFx32 *position);
void MapObject_SetPreviousCoordsToCurrent(LocalMapObject *object);

#endif // POKEHEARTGOLD_MAP_OBJECT_MOVEMENT_UTILITIES_H
