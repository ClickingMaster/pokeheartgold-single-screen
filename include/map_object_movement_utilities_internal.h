#ifndef POKEHEARTGOLD_MAP_OBJECT_MOVEMENT_UTILITIES_INTERNAL_H
#define POKEHEARTGOLD_MAP_OBJECT_MOVEMENT_UTILITIES_INTERNAL_H

#include "map_object_movement_utilities.h"

// The ground effect bits of a map object's graphics info (ov01_021F9318).
typedef struct MapObjectEffectFlags {
    u16 unk0 : 4;
    u16 shadow : 3;
    u16 footprints : 4;
    u16 reflection : 2;
    u16 unk13 : 3;
} MapObjectEffectFlags;

typedef BOOL (*TileTestFunc)(u8 behavior);

extern const TileTestFunc gMetatileBlocksExitTests[4];
extern const TileTestFunc gMetatileBlocksEntryTests[4];
extern const s32 gDirectionDeltaX[4];
extern const s32 gDirectionDeltaY[4];
extern const s32 gOppositeDirections[4];

void sub_0205FD20(LocalMapObject *object);
void sub_0205FD30(LocalMapObject *object);
BOOL sub_0205FD98(LocalMapObject *object);
void sub_0205FE0C(LocalMapObject *object);
void sub_0205FE24(LocalMapObject *object);
void sub_0205FE48(LocalMapObject *object);
void sub_0205FE6C(LocalMapObject *object);
void sub_0205FEA4(LocalMapObject *object);
void sub_0205FEDC(LocalMapObject *object);
void sub_0205FF6C(LocalMapObject *object);
void sub_02060020(LocalMapObject *object);
void sub_0206008C(LocalMapObject *object);
void sub_02060114(LocalMapObject *object);
void sub_020601A4(LocalMapObject *object, u8 behavior, u8 prevBehavior, const MapObjectEffectFlags *flags);
void sub_020601BC(LocalMapObject *object, u8 behavior, u8 prevBehavior, const MapObjectEffectFlags *flags);
void sub_02060274(LocalMapObject *object, u8 behavior, u8 prevBehavior, const MapObjectEffectFlags *flags);
void MapObject_SpawnFootprints(LocalMapObject *object, u8 behavior, u8 prevBehavior, const MapObjectEffectFlags *flags);
void MapObject_UpdateShallowWaterRipple(LocalMapObject *object, u8 behavior, u8 prevBehavior, const MapObjectEffectFlags *flags);
void MapObject_ResetShallowWaterRipple(LocalMapObject *object, u8 behavior, u8 prevBehavior, const MapObjectEffectFlags *flags);
void sub_020603DC(LocalMapObject *object, u8 behavior, u8 prevBehavior, const MapObjectEffectFlags *flags);
void sub_020603F8(LocalMapObject *object, u8 behavior, u8 prevBehavior, const MapObjectEffectFlags *flags);
void sub_02060530(LocalMapObject *object, u8 behavior, u8 prevBehavior, const MapObjectEffectFlags *flags);
void MapObject_SpawnGroundDust(LocalMapObject *object, u8 behavior, u8 prevBehavior, const MapObjectEffectFlags *flags);
void sub_02060698(LocalMapObject *object, u8 behavior, u8 prevBehavior, const MapObjectEffectFlags *flags);
void sub_020606CC(LocalMapObject *object, u8 behavior, u8 prevBehavior, const MapObjectEffectFlags *flags);
void sub_02060700(LocalMapObject *object, u8 behavior, u8 prevBehavior, const MapObjectEffectFlags *flags);
void sub_02060704(LocalMapObject *object, u8 behavior, u8 prevBehavior, const MapObjectEffectFlags *flags);
void MapObject_SpawnPuddleSplashAtPrevious(LocalMapObject *object, u8 behavior, u8 prevBehavior, const MapObjectEffectFlags *flags);
void MapObject_SpawnPuddleSplash(LocalMapObject *object, u8 behavior, u8 prevBehavior, const MapObjectEffectFlags *flags);
void MapObject_SpawnMudSplashAtPrevious(LocalMapObject *object, u8 behavior, u8 prevBehavior, const MapObjectEffectFlags *flags);
void MapObject_SpawnMudSplash(LocalMapObject *object, u8 behavior, u8 prevBehavior, const MapObjectEffectFlags *flags);
void MapObject_CreateReflection(LocalMapObject *object, u8 behavior, u8 prevBehavior, const MapObjectEffectFlags *flags);
void MapObject_UpdateReflection(LocalMapObject *object, u8 behavior, u8 prevBehavior, const MapObjectEffectFlags *flags);
void sub_02060AB8(LocalMapObject *object, u8 behavior, u8 prevBehavior, const MapObjectEffectFlags *flags);
void sub_02060AF0(LocalMapObject *object, u8 behavior, u8 prevBehavior, const MapObjectEffectFlags *flags);
u32 MapObject_CheckCollision(LocalMapObject *object, VecFx32 *position, int x, int y, int z, int direction);
u32 MapObject_CheckCollisionAt(LocalMapObject *object, int x, int y, int z, int direction);
u32 MapObject_CheckCollisionInDirection(LocalMapObject *object, int direction);
BOOL sub_02060BFC(LocalMapObject *object, int x, int y, int z);
BOOL sub_02060CA8(LocalMapObject *object, int x, int y, int z);
BOOL MapObject_IsOutsideMovementRange(LocalMapObject *object, int x, int y, int z);
BOOL MapObject_IsMoveBlockedByBehavior(LocalMapObject *object, int x, int z, int direction);
BOOL sub_02060EA4(LocalMapObject *object, int tile);
BOOL sub_02060ED4(LocalMapObject *object, int tile);
BOOL sub_02060EEC(LocalMapObject *object, int tile);
void MapObject_MoveCoordsInDirection(LocalMapObject *object, int direction);
int MapObject_GetMetatileBehaviorInDirection(LocalMapObject *object, int direction);
u8 sub_02060FE0(LocalMapObject *object, int direction);
void MapObject_OffsetPositionInDirection(LocalMapObject *object, int direction, int distance);
BOOL MapObject_UpdateMetatileBehaviors(LocalMapObject *object);
void OffsetPositionInDirection(int direction, VecFx32 *position, int distance);
void sub_020611DC(LocalMapObject *object);
int GetDirectionTowards(int x, int z, int targetX, int targetZ);
BOOL sub_02061248(FieldSystem *fieldSystem, VecFx32 *position, BOOL allowType2);

#endif // POKEHEARTGOLD_MAP_OBJECT_MOVEMENT_UTILITIES_INTERNAL_H
