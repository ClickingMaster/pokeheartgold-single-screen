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

typedef BOOL (*MovementStateFunc)(LocalMapObject *object, u8 *state);

void RandomLookMovement_Init(LocalMapObject *object, u32 value);
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
void RandomWalkMovement_Init(LocalMapObject *object, u32 first, u32 second, u32 third);
void RandomWalkMovement_Update(LocalMapObject *object);
void MapObject_GetMovementBounds(LocalMapObject *object, LocalMovementBounds *bounds);
BOOL MapObject_IsWithinMovementBounds(LocalMapObject *object, int direction);
void FaceDirectionMovement_Init(LocalMapObject *object, u32 direction);
void FaceDirectionMovement_Update(LocalMapObject *object);
void FaceDirectionMovement_InitUp(LocalMapObject *object);
void FaceDirectionMovement_InitDown(LocalMapObject *object);
void FaceDirectionMovement_InitLeft(LocalMapObject *object);
void FaceDirectionMovement_InitRight(LocalMapObject *object);
void SpinMovement_Init(LocalMapObject *object, u32 value);
void SpinMovement_InitCounterclockwise(LocalMapObject *object);
void SpinMovement_InitClockwise(LocalMapObject *object);
void SpinMovement_Update(LocalMapObject *object);
BOOL SpinMovement_FaceNearbyPlayer(LocalMapObject *object, u8 *state);
BOOL SpinMovement_WaitFace(LocalMapObject *object, u8 *state);
BOOL SpinMovement_Wait(LocalMapObject *object, u8 *state);
BOOL SpinMovement_Turn(LocalMapObject *object, s8 *state);
void AlternatingSpinMovement_Init(LocalMapObject *object);
void AlternatingSpinMovement_Update(LocalMapObject *object);
void AlternatingSpinMovement_Face(LocalMapObject *object, u8 *state);
BOOL AlternatingSpinMovement_WaitFace(LocalMapObject *object, u8 *state);
BOOL AlternatingSpinMovement_Wait(LocalMapObject *object, u8 *state);
BOOL AlternatingSpinMovement_Turn(LocalMapObject *object, DirectionCycleState *state);
void BackAndForthMovement_Init(LocalMapObject *object);
void BackAndForthMovement_Update(LocalMapObject *object);
void BackAndForthMovement_FaceStart(LocalMapObject *object, u8 *state);
int BackAndForthMovement_Step(void *mapObject, BackAndForthMovementState *state);
BOOL BackAndForthMovement_WaitStep(LocalMapObject *object, u8 *state);
void PathMovement_Init(LocalMapObject *object, u32 first, u32 second, u32 value);
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
BOOL PathMovement_Step(LocalMapObject *object, MovementPathState *state);
BOOL PathMovement_WaitStep(LocalMapObject *object, u8 *state);
void PathMovement_InitTable30(LocalMapObject *object);
void PathMovement_InitTable31(LocalMapObject *object);
void PathMovement_InitTable32(LocalMapObject *object);
void PathMovement_InitTable33(LocalMapObject *object);
void PathMovement_InitTable34(LocalMapObject *object);
void PathMovement_InitTable35(LocalMapObject *object);
void PathMovement_InitTable36(LocalMapObject *object);
void PathMovement_InitTable37(LocalMapObject *object);
int DirectionList_Count(const int *list, int value);
int DirectionList_GetRandom(const int *list, int value);
int GetRandomDirectionFromTable(int tableId, int value);
const int *GetDirectionTable(int id);
int MapObject_GetDirectionToNearbyPlayer(LocalMapObject *object);
int MapObject_GetAllowedDirectionToNearbyPlayer(LocalMapObject *object, int tableId, int value);
BOOL MapObject_SpinsWhileWalking(LocalMapObject *object);
void WalkingSpin_Init(LocalMapObject *object, u8 *state);
void WalkingSpin_TurnBeforeStep(LocalMapObject *object, s8 *state);
void WalkingSpin_EndStep(LocalMapObject *object, u8 *state);

#endif // POKEHEARTGOLD_MAP_OBJECT_MOVEMENT_H
