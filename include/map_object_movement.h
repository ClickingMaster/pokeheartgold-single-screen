#ifndef POKEHEARTGOLD_MAP_OBJECT_MOVEMENT_H
#define POKEHEARTGOLD_MAP_OBJECT_MOVEMENT_H

#include "map_object.h"

typedef struct LocalMovementState {
    s16 state;
    s16 delay;
    u32 restrictToRange;
    u32 heldMovement;
    u32 directionTable;
} LocalMovementState;

typedef struct LocalMovementBounds {
    s32 minX;
    s32 minZ;
    s32 maxX;
    s32 maxZ;
} LocalMovementBounds;

typedef struct DirectionCycle {
    int value[5];
} DirectionCycle;

typedef struct DirectionCycleState {
    s8 state;
    u8 unk_01;
    u8 flag;
} DirectionCycleState;

typedef struct BackAndForthMovementState {
    s16 state;
    s16 turn;
    u8 data[1];
} BackAndForthMovementState;

typedef struct MovementPathState {
    u8 active;
    u8 index;
    u8 limit;
    u8 axis;
    u32 tableId;
    u8 saved[4];
} MovementPathState;

typedef struct MovementListEntry {
    int id;
    const int *list;
} MovementListEntry;

typedef BOOL (*MovementStateFunc)(LocalMapObject *object, void *data);

void RandomLookMovement_InitAnyDirection(LocalMapObject *object);
void RandomLookMovement_InitUpLeft(LocalMapObject *object);
void RandomLookMovement_InitUpRight(LocalMapObject *object);
void RandomLookMovement_InitDownLeft(LocalMapObject *object);
void RandomLookMovement_InitDownRight(LocalMapObject *object);
void RandomLookMovement_InitUpDownLeft(LocalMapObject *object);
void RandomLookMovement_InitUpDownRight(LocalMapObject *object);
void RandomLookMovement_InitUpLeftRight(LocalMapObject *object);
void RandomLookMovement_InitDownLeftRight(LocalMapObject *object);
void RandomLookMovement_InitUpDown(LocalMapObject *object);
void RandomLookMovement_InitLeftRight(LocalMapObject *object);
void RandomLookMovement_Update(LocalMapObject *object);
void RandomLookMovement_Delete(LocalMapObject *object);
void RandomWalkMovement_InitAnyDirection(LocalMapObject *object);
void RandomWalkMovement_InitUpDown(LocalMapObject *object);
void RandomWalkMovement_InitLeftRight(LocalMapObject *object);
void RandomWalkMovement_Update(LocalMapObject *object);
void FaceDirectionMovement_Update(LocalMapObject *object);
void FaceDirectionMovement_InitUp(LocalMapObject *object);
void FaceDirectionMovement_InitDown(LocalMapObject *object);
void FaceDirectionMovement_InitLeft(LocalMapObject *object);
void FaceDirectionMovement_InitRight(LocalMapObject *object);
void SpinMovement_InitCounterclockwise(LocalMapObject *object);
void SpinMovement_InitClockwise(LocalMapObject *object);
void SpinMovement_Update(LocalMapObject *object);
void AlternatingSpinMovement_Init(LocalMapObject *object);
void AlternatingSpinMovement_Update(LocalMapObject *object);
void BackAndForthMovement_Init(LocalMapObject *object);
void BackAndForthMovement_Update(LocalMapObject *object);
void PathMovement_InitTable14(LocalMapObject *object);
void PathMovement_InitTable15(LocalMapObject *object);
void PathMovement_InitTable16(LocalMapObject *object);
void PathMovement_InitTable17(LocalMapObject *object);
void PathMovement_InitTable18(LocalMapObject *object);
void PathMovement_InitTable19(LocalMapObject *object);
void PathMovement_InitTable20(LocalMapObject *object);
void PathMovement_InitTable21(LocalMapObject *object);
void PathMovement_InitTable22(LocalMapObject *object);
void PathMovement_InitTable23(LocalMapObject *object);
void PathMovement_InitTable24(LocalMapObject *object);
void PathMovement_InitTable25(LocalMapObject *object);
void PathMovement_InitTable26(LocalMapObject *object);
void PathMovement_InitTable27(LocalMapObject *object);
void PathMovement_InitTable28(LocalMapObject *object);
void PathMovement_InitTable29(LocalMapObject *object);
void PathMovement_Update(LocalMapObject *object);
void PathMovement_InitTable30(LocalMapObject *object);
void PathMovement_InitTable31(LocalMapObject *object);
void PathMovement_InitTable32(LocalMapObject *object);
void PathMovement_InitTable33(LocalMapObject *object);
void PathMovement_InitTable34(LocalMapObject *object);
void PathMovement_InitTable35(LocalMapObject *object);
void PathMovement_InitTable36(LocalMapObject *object);
void PathMovement_InitTable37(LocalMapObject *object);

#endif // POKEHEARTGOLD_MAP_OBJECT_MOVEMENT_H
