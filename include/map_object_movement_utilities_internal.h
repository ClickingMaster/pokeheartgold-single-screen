#ifndef POKEHEARTGOLD_MAP_OBJECT_MOVEMENT_UTILITIES_INTERNAL_H
#define POKEHEARTGOLD_MAP_OBJECT_MOVEMENT_UTILITIES_INTERNAL_H

#include "map_object_movement_utilities.h"

typedef BOOL (*TileTestFunc)(u8 behavior);

extern const TileTestFunc gMetatileBlocksExitTests[4];
extern const TileTestFunc gMetatileBlocksEntryTests[4];
extern const s32 gDirectionDeltaX[4];
extern const s32 gDirectionDeltaY[4];
extern const s32 gOppositeDirections[4];

void MapObject_CallMovementAndBehaviorInit(LocalMapObject *object);
void MapObject_UpdateMovement(LocalMapObject *object);
u32 MapObject_CheckCollisionAt(LocalMapObject *object, int x, int y, int z, int direction);
u32 MapObject_CheckCollisionInDirection(LocalMapObject *object, int direction);
BOOL MapObject_IsTileOccupiedInLink(LocalMapObject *object, int x, int y, int z);
BOOL MapObject_IsTileOccupied(LocalMapObject *object, int x, int y, int z);
BOOL MapObject_IsOutsideMovementRange(LocalMapObject *object, int x, int y, int z);
BOOL MapObject_IsMoveBlockedByBehavior(LocalMapObject *object, int x, int z, int direction);
BOOL MapObject_CheckSandBehavior(LocalMapObject *object, int tile);
BOOL MapObject_CheckSnowFootprintBehavior(LocalMapObject *object, int tile);
BOOL MapObject_IsOnBridge(LocalMapObject *object, int tile);
void MapObject_MoveCoordsInDirection(LocalMapObject *object, int direction);
int MapObject_GetMetatileBehaviorInDirection(LocalMapObject *object, int direction);
u8 MapObject_GetTileAttrBits8To14InDirection(LocalMapObject *object, int direction);
void MapObject_OffsetPositionInDirection(LocalMapObject *object, int direction, int distance);
BOOL MapObject_UpdateMetatileBehaviors(LocalMapObject *object);
void OffsetPositionInDirection(int direction, VecFx32 *position, int distance);
void MapObject_UpdateHiddenMovement(LocalMapObject *object);
int GetDirectionTowards(int x, int z, int targetX, int targetZ);
BOOL FieldSystem_SnapToTerrainHeightEx(FieldSystem *fieldSystem, VecFx32 *position, BOOL allowType2);

#endif // POKEHEARTGOLD_MAP_OBJECT_MOVEMENT_UTILITIES_INTERNAL_H
