#ifndef POKEHEARTGOLD_MAP_OBJECT_MOVEMENT_UTILITIES_INTERNAL_H
#define POKEHEARTGOLD_MAP_OBJECT_MOVEMENT_UTILITIES_INTERNAL_H

#include "map_object_movement_utilities.h"

// The ground effect bits of a map object's graphics info (MapObject_GetEffectFlags).
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

void MapObject_CallMovementAndBehaviorInit(LocalMapObject *object);
void MapObject_UpdateMovement(LocalMapObject *object);
BOOL MapObject_CanUpdateMovement(LocalMapObject *object);
void MapObject_UpdateHeightIfFlagged(LocalMapObject *object);
void MapObject_RestartIfBehaviorsLoaded(LocalMapObject *object);
void MapObject_UpdateEffectsBeforeMovement(LocalMapObject *object);
void MapObject_UpdateEffectsAfterMovement(LocalMapObject *object);
void MapObject_UpdateEffectsAtStepEnd(LocalMapObject *object);
void MapObject_SpawnStartEffects(LocalMapObject *object);
void MapObject_SpawnStepStartEffects(LocalMapObject *object);
void MapObject_SpawnJumpStartEffects(LocalMapObject *object);
void MapObject_SpawnStepEndEffects(LocalMapObject *object);
void MapObject_SpawnJumpLandEffects(LocalMapObject *object);
void MapObject_ResetTileDrawOffset(LocalMapObject *object, u8 behavior, u8 prevBehavior, const MapObjectEffectFlags *flags);
void MapObject_SpawnTallGrassStanding(LocalMapObject *object, u8 behavior, u8 prevBehavior, const MapObjectEffectFlags *flags);
void MapObject_SpawnTallGrassOnStep(LocalMapObject *object, u8 behavior, u8 prevBehavior, const MapObjectEffectFlags *flags);
void MapObject_SpawnFootprints(LocalMapObject *object, u8 behavior, u8 prevBehavior, const MapObjectEffectFlags *flags);
void MapObject_UpdateShallowWaterRipple(LocalMapObject *object, u8 behavior, u8 prevBehavior, const MapObjectEffectFlags *flags);
void MapObject_ResetShallowWaterRipple(LocalMapObject *object, u8 behavior, u8 prevBehavior, const MapObjectEffectFlags *flags);
void MapObject_CreateShadowWithSavedBehavior(LocalMapObject *object, u8 behavior, u8 prevBehavior, const MapObjectEffectFlags *flags);
void MapObject_CreateShadow(LocalMapObject *object, u8 behavior, u8 prevBehavior, const MapObjectEffectFlags *flags);
void MapObject_UpdateShadowVisibility(LocalMapObject *object, u8 behavior, u8 prevBehavior, const MapObjectEffectFlags *flags);
void MapObject_SpawnGroundDust(LocalMapObject *object, u8 behavior, u8 prevBehavior, const MapObjectEffectFlags *flags);
void MapObject_SpawnVeryTallGrassStanding(LocalMapObject *object, u8 behavior, u8 prevBehavior, const MapObjectEffectFlags *flags);
void MapObject_SpawnVeryTallGrassOnStep(LocalMapObject *object, u8 behavior, u8 prevBehavior, const MapObjectEffectFlags *flags);
void MapObject_TileEffectDummy(LocalMapObject *object, u8 behavior, u8 prevBehavior, const MapObjectEffectFlags *flags);
void MapObject_TileEffectDummy2(LocalMapObject *object, u8 behavior, u8 prevBehavior, const MapObjectEffectFlags *flags);
void MapObject_SpawnPuddleSplashAtPrevious(LocalMapObject *object, u8 behavior, u8 prevBehavior, const MapObjectEffectFlags *flags);
void MapObject_SpawnPuddleSplash(LocalMapObject *object, u8 behavior, u8 prevBehavior, const MapObjectEffectFlags *flags);
void MapObject_SpawnMudSplashAtPrevious(LocalMapObject *object, u8 behavior, u8 prevBehavior, const MapObjectEffectFlags *flags);
void MapObject_SpawnMudSplash(LocalMapObject *object, u8 behavior, u8 prevBehavior, const MapObjectEffectFlags *flags);
void MapObject_CreateReflection(LocalMapObject *object, u8 behavior, u8 prevBehavior, const MapObjectEffectFlags *flags);
void MapObject_UpdateReflection(LocalMapObject *object, u8 behavior, u8 prevBehavior, const MapObjectEffectFlags *flags);
void MapObject_UpdateOnBridgeFlag(LocalMapObject *object, u8 behavior, u8 prevBehavior, const MapObjectEffectFlags *flags);
void MapObject_TileEffectDummy3(LocalMapObject *object, u8 behavior, u8 prevBehavior, const MapObjectEffectFlags *flags);
u32 MapObject_CheckCollision(LocalMapObject *object, VecFx32 *position, int x, int y, int z, int direction);
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
