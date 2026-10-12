#include "map_object_movement.h"

#include "global.h"

#include "field_system.h"
#include "map_object.h"
#include "map_object_movement_commands_internal.h"
#include "map_object_movement_utilities_internal.h"
#include "math_util.h"
#include "player_avatar.h"
#include "player_avatar_movement.h"

extern MovementStateFunc gPathMovementStateFuncs[];
extern MovementStateFunc gBackAndForthMovementStateFuncs[];
extern MovementStateFunc gAlternatingSpinMovementStateFuncs[];
extern MovementStateFunc gSpinMovementStateFuncs[];
extern const DirectionCycle gSpinCounterclockwiseOrder;
extern const DirectionCycle gSpinClockwiseOrder;
extern const int gAlternatingSpinCounterclockwiseOrder[5];
extern const int gAlternatingSpinClockwiseOrder[5];
extern const int gRandomLookDelays[];
extern int gWalkingSpinOrders[][4];
extern const int gPlayerAwareMovementTypes[];
extern const MovementListEntry gMovementDirectionTables[];

void RandomLookMovement_Init(LocalMapObject *object, u32 value) {
    u8 *state = MapObject_InitMovementData(object, 8);
    *(u16 *)(state + 2) = DirectionList_GetRandom(gRandomLookDelays, -1);
    *(u32 *)(state + 4) = value;
    MapObject_SetAnimation(object, 0);
    MapObject_ClearSingleMovement(object);
}

void RandomLookMovement_InitAnyDirection(LocalMapObject *object) {
    RandomLookMovement_Init(object, 0);
}

void RandomLookMovement_InitUpLeft(LocalMapObject *object) {
    RandomLookMovement_Init(object, 1);
}

void RandomLookMovement_InitUpRight(LocalMapObject *object) {
    RandomLookMovement_Init(object, 2);
}

void RandomLookMovement_InitDownLeft(LocalMapObject *object) {
    RandomLookMovement_Init(object, 3);
}

void RandomLookMovement_InitDownRight(LocalMapObject *object) {
    RandomLookMovement_Init(object, 4);
}

void RandomLookMovement_InitUpDownLeft(LocalMapObject *object) {
    RandomLookMovement_Init(object, 5);
}

void RandomLookMovement_InitUpDownRight(LocalMapObject *object) {
    RandomLookMovement_Init(object, 6);
}

void RandomLookMovement_InitUpLeftRight(LocalMapObject *object) {
    RandomLookMovement_Init(object, 7);
}

void RandomLookMovement_InitDownLeftRight(LocalMapObject *object) {
    RandomLookMovement_Init(object, 8);
}

void RandomLookMovement_InitUpDown(LocalMapObject *object) {
    RandomLookMovement_Init(object, 9);
}

void RandomLookMovement_InitLeftRight(LocalMapObject *object) {
    RandomLookMovement_Init(object, 10);
}

void RandomLookMovement_Update(LocalMapObject *object) {
    u8 *state = MapObject_GetMovementData(object);
    int direction = MapObject_GetAllowedDirectionToNearbyPlayer(object, *(u32 *)(state + 4), -1);
    if (direction != -1) {
        MapObject_SetFacingDirection(object, direction);
    } else if (*(u16 *)state == 0) {
        (*(s16 *)(state + 2))--;
        if (*(s16 *)(state + 2) <= 0) {
            *(u16 *)(state + 2) = DirectionList_GetRandom(gRandomLookDelays, -1);
            direction = GetRandomDirectionFromTable(*(u32 *)(state + 4), -1);
            MapObject_SetFacingDirection(object, direction);
        }
    }
    MapObject_SetPreviousCoordsToCurrent(object);
}

void RandomLookMovement_Delete(LocalMapObject *object) {
    (void)object;
}

void RandomWalkMovement_InitAnyDirection(LocalMapObject *object) {
    RandomWalkMovement_Init(object, 12, 11, 0);
}

void RandomWalkMovement_InitUpDown(LocalMapObject *object) {
    RandomWalkMovement_Init(object, 12, 12, 0);
}

void RandomWalkMovement_InitLeftRight(LocalMapObject *object) {
    RandomWalkMovement_Init(object, 12, 13, 0);
}

void RandomWalkMovement_Init(LocalMapObject *object, u32 first, u32 second, u32 third) {
    u32 *state = (u32 *)MapObject_InitMovementData(object, 16);
    state[1] = third;
    state[2] = first;
    state[3] = second;
    MapObject_SetAnimation(object, 0);
    MapObject_ClearSingleMovement(object);
}

void RandomWalkMovement_Update(LocalMapObject *object) {
    LocalMovementState *state = (LocalMovementState *)MapObject_GetMovementData(object);
    int direction;
    switch (state->state) {
    case 0:
        MapObject_ClearSingleMovement(object);
        MapObject_ClearEndMovement(object);
        direction = GetMovementForDirection(MapObject_GetFacingDirection(object), 0);
        MapObject_ForceSetHeldMovement(object, direction);
        state->state++;
        break;
    case 1:
        if (!MapObject_RunForcedMovement(object)) {
            break;
        }
        state->delay = DirectionList_GetRandom(gRandomLookDelays, -1);
        state->state++;
    case 2:
        state->delay--;
        if (state->delay != 0) {
            break;
        }
        state->state++;
    case 3:
        direction = GetRandomDirectionFromTable(state->directionTable, -1);
        MapObject_SetOrQueueFacing(object, direction);
        if (state->restrictToRange == 1 && !MapObject_IsWithinMovementBounds(object, direction)) {
            state->state = 0;
            break;
        }
        if (MapObject_CheckCollisionInDirection(object, direction)) {
            state->state = 0;
            break;
        }
        MapObject_ForceSetHeldMovement(object, GetMovementForDirection(direction, state->heldMovement));
        MapObject_SetSingleMovement(object);
        state->state++;
    case 4:
        if (MapObject_RunForcedMovement(object)) {
            MapObject_ClearSingleMovement(object);
            state->state = 0;
        }
        break;
    }
}

void MapObject_GetMovementBounds(LocalMapObject *object, LocalMovementBounds *bounds) {
    s32 x = MapObject_GetInitialX(object);
    s32 z = MapObject_GetInitialZ(object);
    s32 xRange = MapObject_GetXRange(object);
    s32 zRange = MapObject_GetYRange(object);
    switch (MapObject_GetMovement(object)) {
    case 6:
        bounds->minX = x - xRange;
        bounds->maxX = x;
        bounds->minZ = z - zRange;
        bounds->maxZ = z;
        break;
    case 7:
        bounds->minX = x;
        bounds->maxX = x + xRange;
        bounds->minZ = z - zRange;
        bounds->maxZ = z;
        break;
    case 8:
        bounds->minX = x - xRange;
        bounds->maxX = x;
        bounds->minZ = z;
        bounds->maxZ = z + zRange;
        break;
    case 9:
        bounds->minX = x;
        bounds->maxX = x + xRange;
        bounds->minZ = z;
        bounds->maxZ = z + zRange;
        break;
    case 10:
        bounds->minX = x - xRange;
        bounds->maxX = x;
        bounds->minZ = z - zRange;
        bounds->maxZ = z + zRange;
        break;
    case 11:
        bounds->minX = x;
        bounds->maxX = x + xRange;
        bounds->minZ = z - zRange;
        bounds->maxZ = z + zRange;
        break;
    case 12:
        bounds->minX = x - xRange;
        bounds->maxX = x + xRange;
        bounds->minZ = z - zRange;
        bounds->maxZ = z;
        break;
    case 13:
        bounds->minX = x - xRange;
        bounds->maxX = x + xRange;
        bounds->minZ = z;
        bounds->maxZ = z + zRange;
        break;
    default:
        GF_AssertFail();
        break;
    }
}

BOOL MapObject_IsWithinMovementBounds(LocalMapObject *object, int direction) {
    LocalMovementBounds bounds;
    s32 x;
    s32 z;
    MapObject_GetMovementBounds(object, &bounds);
    x = MapObject_GetXCoord(object) + GetDeltaXByFacingDirection(direction);
    z = MapObject_GetZCoord(object) + GetDeltaYByFacingDirection(direction);
    if (bounds.minX > x || bounds.maxX < x) {
        return FALSE;
    }
    if (bounds.minZ > z || bounds.maxZ < z) {
        return FALSE;
    }
    return TRUE;
}

void FaceDirectionMovement_Init(LocalMapObject *object, u32 direction) {
    u32 *state = (u32 *)MapObject_InitMovementData(object, 8);
    state[0] = direction;
    MapObject_SetAnimation(object, 0);
    MapObject_ClearSingleMovement(object);
    MapObject_SetPreviousCoordsToCurrent(object);
}

void FaceDirectionMovement_Update(LocalMapObject *object) {
    u32 *state = (u32 *)MapObject_GetMovementData(object);
    switch (state[1]) {
    case 0:
        MapObject_SetFacingDirection(object, state[0]);
        state[1]++;
        break;
    case 1:
        break;
    }
}

void FaceDirectionMovement_InitUp(LocalMapObject *object) {
    FaceDirectionMovement_Init(object, 0);
}

void FaceDirectionMovement_InitDown(LocalMapObject *object) {
    FaceDirectionMovement_Init(object, 1);
}

void FaceDirectionMovement_InitLeft(LocalMapObject *object) {
    FaceDirectionMovement_Init(object, 2);
}

void FaceDirectionMovement_InitRight(LocalMapObject *object) {
    FaceDirectionMovement_Init(object, 3);
}

void SpinMovement_Init(LocalMapObject *object, u32 value) {
    u8 *state = MapObject_InitMovementData(object, 8);
    state[0] = value;
    MapObject_SetAnimation(object, 0);
    MapObject_ClearSingleMovement(object);
    MapObject_SetPreviousCoordsToCurrent(object);
}

void SpinMovement_InitCounterclockwise(LocalMapObject *object) {
    SpinMovement_Init(object, 2);
}

void SpinMovement_InitClockwise(LocalMapObject *object) {
    SpinMovement_Init(object, 3);
}

void SpinMovement_Update(LocalMapObject *object) {
    u8 *state = MapObject_GetMovementData(object);

    while (gSpinMovementStateFuncs[(s8)state[2]](object, state) == TRUE) {
    }
}

BOOL SpinMovement_FaceNearbyPlayer(LocalMapObject *object, u8 *state) {
    int direction = MapObject_GetAllowedDirectionToNearbyPlayer(object, 0x26, -1);
    if (direction == -1) {
        direction = MapObject_GetFacingDirection(object);
    }
    MapObject_ForceSetHeldMovement(object, GetMovementForDirection(direction, 0));
    state[2] = 1;
    return TRUE;
}

BOOL SpinMovement_WaitFace(LocalMapObject *object, u8 *state) {
    if (!MapObject_RunForcedMovement(object)) {
        return FALSE;
    }
    *(u32 *)(state + 4) = 0;
    state[2] = 2;
    return TRUE;
}

BOOL SpinMovement_Wait(LocalMapObject *object, u8 *state) {
    if (*(u32 *)(state + 4) != 0 && MapObject_GetAllowedDirectionToNearbyPlayer(object, 0x26, -1) != -1) {
        state[2] = 0;
        return TRUE;
    }
    (*(int *)(state + 4))++;
    if (*(int *)(state + 4) < 24) {
        return FALSE;
    }
    state[2] = 3;
    return TRUE;
}

BOOL SpinMovement_Turn(LocalMapObject *object, s8 *state) {
    DirectionCycle listA = gSpinCounterclockwiseOrder;
    DirectionCycle listB = gSpinClockwiseOrder;
    int i;
    DirectionCycle *list = &listA;
    int direction;

    if (state[0] != 2) {
        list = &listB;
    }
    direction = MapObject_GetFacingDirection(object);
    i = 0;
    if (list->value[0] != -1) {
        int *p = list->value;
        do {
            if (direction == *p) {
                break;
            }
            p++;
            i++;
        } while (*p != -1);
    }
    GF_ASSERT(list->value[i] != -1);
    i++;
    if (list->value[i] == -1) {
        i = 0;
    }
    MapObject_SetFacingDirection(object, list->value[i]);
    state[2] = 0;
    return TRUE;
}

void AlternatingSpinMovement_Init(LocalMapObject *object) {
    SpinMovement_Init(object, 3);
}

void AlternatingSpinMovement_Update(LocalMapObject *object) {
    u8 *state = MapObject_GetMovementData(object);
    while (gAlternatingSpinMovementStateFuncs[(s8)state[2]](object, state) == TRUE) {}
}

void AlternatingSpinMovement_Face(LocalMapObject *object, u8 *state) {
    u32 facing = MapObject_GetFacingDirection(object);
    MapObject_ForceSetHeldMovement(object, GetMovementForDirection(facing, 0));
    state[2] = 1;
}

BOOL AlternatingSpinMovement_WaitFace(LocalMapObject *object, u8 *state) {
    if (!MapObject_RunForcedMovement(object)) {
        return FALSE;
    }
    *(u32 *)(state + 4) = 0;
    state[2] = 2;
    return TRUE;
}

BOOL AlternatingSpinMovement_Wait(LocalMapObject *object, u8 *state) {
    (void)object;
    (*(int *)(state + 4))++;
    if (*(int *)(state + 4) < 24) {
        return FALSE;
    }
    state[2] = 3;
    return TRUE;
}

BOOL AlternatingSpinMovement_Turn(LocalMapObject *object, DirectionCycleState *state) {
    typedef struct DirectionOrder {
        int values[5];
    } DirectionOrder;

    DirectionOrder orderA = *(const DirectionOrder *)gAlternatingSpinCounterclockwiseOrder;
    DirectionOrder orderB = *(const DirectionOrder *)gAlternatingSpinClockwiseOrder;
    int i;
    int direction;
    int *order;
    int *alternate;

    order = orderA.values;
    alternate = orderB.values;
    if (state->state != 2) {
        order = alternate;
    }

    direction = MapObject_GetFacingDirection(object);
    i = 0;
    if (order[i] != -1) {
        do {
            if (direction == order[i]) {
                break;
            }
            i++;
        } while (order[i] != -1);
    }
    GF_ASSERT(order[i] != -1);
    i++;
    if (order[i] == -1) {
        i = 0;
    }
    MapObject_SetFacingDirection(object, order[i]);

    direction = MapObject_GetFacingDirection(object);
    if (direction == MapObject_GetInitialFacingDirection(object)) {
        state->state = GetOppositeDirection(state->state);
    }
    state->flag = 0;
    return TRUE;
}

void BackAndForthMovement_Init(LocalMapObject *object) {
    u8 *state = MapObject_InitMovementData(object, 8);
    if (MapObject_SpinsWhileWalking(object) == TRUE) {
        WalkingSpin_Init(object, state + 4);
    }
}

void BackAndForthMovement_Update(LocalMapObject *object) {
    u8 *state = MapObject_GetMovementData(object);
    while (gBackAndForthMovementStateFuncs[*(s16 *)state](object, state) == TRUE) {}
}

void BackAndForthMovement_FaceStart(LocalMapObject *object, u8 *state) {
    u32 direction = MapObject_GetInitialFacingDirection(object);
    if (*(s16 *)(state + 2) == 1) {
        direction = GetOppositeDirection(direction);
    }
    MapObject_SetNextFacingDirection(object, direction);
    if (!MapObject_SpinsWhileWalking(object)) {
        MapObject_SetFacingDirection(object, direction);
    }
    *(u16 *)state = 1;
}

int BackAndForthMovement_Step(void *mapObject, BackAndForthMovementState *state) {
    int direction;
    int result;
    int kind;

    if (state->turn != 0) {
        int initialX = MapObject_GetInitialX(mapObject);
        int initialZ = MapObject_GetInitialZ(mapObject);
        int x = MapObject_GetXCoord(mapObject);
        int z = MapObject_GetZCoord(mapObject);

        if (initialX == x && initialZ == z) {
            direction = GetOppositeDirection(MapObject_GetNextFacingDirection(mapObject));
            MapObject_SetNextFacingDirection(mapObject, direction);
            if (MapObject_SpinsWhileWalking(mapObject) == 0) {
                MapObject_SetFacingDirection(mapObject, direction);
            }
            state->turn = 0;
        }
    }
    direction = MapObject_GetNextFacingDirection(mapObject);
    result = MapObject_CheckCollisionInDirection(mapObject, direction);
    if (result & 1) {
        state->turn = 1;
        direction = GetOppositeDirection(direction);
        result = MapObject_CheckCollisionInDirection(mapObject, direction);
    }
    kind = 0xc;
    if (result != 0) {
        kind = 0x20;
    }
    result = GetMovementForDirection(direction, kind);
    MapObject_ForceSetHeldMovement(mapObject, result);
    if (MapObject_SpinsWhileWalking(mapObject) == 1) {
        WalkingSpin_TurnBeforeStep(mapObject, state->data);
    }
    MapObject_SetSingleMovement(mapObject);
    state->state = 2;
    return 1;
}

BOOL BackAndForthMovement_WaitStep(LocalMapObject *object, u8 *state) {
    if (MapObject_RunForcedMovement(object) == TRUE) {
        MapObject_ClearSingleMovement(object);
        if (MapObject_SpinsWhileWalking(object) == TRUE) {
            WalkingSpin_EndStep(object, state + 4);
        }
        *(u16 *)state = 0;
    }
    return FALSE;
}

void PathMovement_Init(LocalMapObject *object, u32 first, u32 second, u32 value) {
    u8 *state = MapObject_InitMovementData(object, 12);
    state[2] = first;
    state[3] = second;
    *(u32 *)(state + 4) = value;
    if (MapObject_SpinsWhileWalking(object) == TRUE) {
        WalkingSpin_Init(object, state + 8);
    }
}

void PathMovement_InitTable14(LocalMapObject *object) {
    PathMovement_Init(object, 2, 0, 14);
}

void PathMovement_InitTable15(LocalMapObject *object) {
    PathMovement_Init(object, 2, 0, 15);
}

void PathMovement_InitTable16(LocalMapObject *object) {
    PathMovement_Init(object, 2, 1, 16);
}

void PathMovement_InitTable17(LocalMapObject *object) {
    PathMovement_Init(object, 2, 1, 17);
}

void PathMovement_InitTable18(LocalMapObject *object) {
    PathMovement_Init(object, 2, 0, 18);
}

void PathMovement_InitTable19(LocalMapObject *object) {
    PathMovement_Init(object, 2, 0, 19);
}

void PathMovement_InitTable20(LocalMapObject *object) {
    PathMovement_Init(object, 2, 1, 20);
}

void PathMovement_InitTable21(LocalMapObject *object) {
    PathMovement_Init(object, 2, 1, 21);
}

void PathMovement_InitTable22(LocalMapObject *object) {
    PathMovement_Init(object, 2, 1, 22);
}

void PathMovement_InitTable23(LocalMapObject *object) {
    PathMovement_Init(object, 2, 1, 23);
}

void PathMovement_InitTable24(LocalMapObject *object) {
    PathMovement_Init(object, 2, 0, 24);
}

void PathMovement_InitTable25(LocalMapObject *object) {
    PathMovement_Init(object, 2, 0, 25);
}

void PathMovement_InitTable26(LocalMapObject *object) {
    PathMovement_Init(object, 2, 1, 26);
}

void PathMovement_InitTable27(LocalMapObject *object) {
    PathMovement_Init(object, 2, 1, 27);
}

void PathMovement_InitTable28(LocalMapObject *object) {
    PathMovement_Init(object, 2, 0, 28);
}

void PathMovement_InitTable29(LocalMapObject *object) {
    PathMovement_Init(object, 2, 0, 29);
}

void PathMovement_Update(LocalMapObject *object) {
    u8 *state = MapObject_GetMovementData(object);
    while (gPathMovementStateFuncs[state[0]](object, state) == TRUE) {}
}

BOOL PathMovement_Step(LocalMapObject *object, MovementPathState *state) {
    const int *list;
    int direction;
    u32 collision;
    int movementKind;

    if (state->index == state->limit) {
        if (state->axis == 0) {
            if (MapObject_GetInitialX(object) == MapObject_GetXCoord(object)) {
                state->index++;
            }
        } else {
            if (MapObject_GetInitialZ(object) == MapObject_GetZCoord(object)) {
                state->index++;
            }
        }
    }
    if (state->index == 3) {
        int initialX = MapObject_GetInitialX(object);
        int initialZ = MapObject_GetInitialZ(object);
        int x = MapObject_GetXCoord(object);
        int z = MapObject_GetZCoord(object);
        if (initialX == x && initialZ == z) {
            state->index = 0;
        }
    }
    list = GetDirectionTable(state->tableId);
    direction = list[state->index];
    MapObject_SetNextFacingDirection(object, direction);
    if (!MapObject_SpinsWhileWalking(object)) {
        MapObject_SetFacingDirection(object, direction);
    }
    collision = MapObject_CheckCollisionInDirection(object, direction);
    if (collision & 1) {
        state->index++;
        direction = list[state->index];
        MapObject_SetNextFacingDirection(object, direction);
        if (!MapObject_SpinsWhileWalking(object)) {
            MapObject_SetFacingDirection(object, direction);
        }
        collision = MapObject_CheckCollisionInDirection(object, direction);
    }
    movementKind = 12;
    if (collision != 0) {
        movementKind = 32;
    }
    MapObject_ForceSetHeldMovement(object, GetMovementForDirection(direction, movementKind));
    if (MapObject_SpinsWhileWalking(object) == TRUE) {
        WalkingSpin_TurnBeforeStep(object, state->saved);
    }
    MapObject_SetSingleMovement(object);
    state->active = TRUE;
    return TRUE;
}

BOOL PathMovement_WaitStep(LocalMapObject *object, u8 *state) {
    if (MapObject_RunForcedMovement(object) == TRUE) {
        MapObject_ClearSingleMovement(object);
        if (MapObject_SpinsWhileWalking(object) == TRUE) {
            WalkingSpin_EndStep(object, state + 8);
        }
        state[0] = 0;
    }
    return FALSE;
}

void PathMovement_InitTable30(LocalMapObject *object) {
    PathMovement_Init(object, 2, 1, 30);
}

void PathMovement_InitTable31(LocalMapObject *object) {
    PathMovement_Init(object, 2, 1, 31);
}

void PathMovement_InitTable32(LocalMapObject *object) {
    PathMovement_Init(object, 2, 0, 32);
}

void PathMovement_InitTable33(LocalMapObject *object) {
    PathMovement_Init(object, 2, 0, 33);
}

void PathMovement_InitTable34(LocalMapObject *object) {
    PathMovement_Init(object, 2, 1, 34);
}

void PathMovement_InitTable35(LocalMapObject *object) {
    PathMovement_Init(object, 2, 1, 35);
}

void PathMovement_InitTable36(LocalMapObject *object) {
    PathMovement_Init(object, 2, 0, 36);
}

void PathMovement_InitTable37(LocalMapObject *object) {
    PathMovement_Init(object, 2, 0, 37);
}

int DirectionList_Count(const int *list, int value) {
    int i = 0;
    while (list[i] != value) {
        i++;
    }
    GF_ASSERT(i != 0);
    return i;
}

int DirectionList_GetRandom(const int *list, int value) {
    int random = LCRandom();
    int count = DirectionList_Count(list, value);
    return list[random % count];
}

int GetRandomDirectionFromTable(int tableId, int value) {
    const int *list = GetDirectionTable(tableId);
    int random = LCRandom();
    int count = DirectionList_Count(list, value);
    return list[random % count];
}

const int *GetDirectionTable(int id) {
    const MovementListEntry *entry = gMovementDirectionTables;
    int current = 0;

    while (current != 0x27) {
        if (current == id) {
            return entry->list;
        }
        entry++;
        current = entry->id;
    }
    GF_ASSERT(FALSE);
    return NULL;
}

#pragma force_active on

int MapObject_GetDirectionToNearbyPlayer(LocalMapObject *object) {
    PlayerAvatar *avatar;
    int type = MapObject_GetType(object);
    int i, movement, entry, x, z, distance, objectX, objectZ, maxZ, minX, minZ, maxX;
    u32 y;
    if (type != 1 && type != 2) {
        return -1;
    }
    avatar = FieldSystem_GetPlayerAvatar(MapObject_GetFieldSystem(object));
    if (!PlayerAvatar_IsRunning(avatar)) {
        return -1;
    }
    i = 0;
    movement = MapObject_GetMovement(object);
    do {
        entry = gPlayerAwareMovementTypes[i++];
    } while (entry != movement && entry != 255);
    if (movement != entry) {
        return -1;
    }
    y = MapObject_GetPositionVectorYCoordUInt(PlayerAvatar_GetMapObject(avatar));
    if (y != MapObject_GetPositionVectorYCoordUInt(object)) {
        return -1;
    }
    x = PlayerAvatar_GetXCoord(avatar);
    z = PlayerAvatar_GetZCoord(avatar);
    distance = MapObject_GetParam(object, 0);
    objectX = MapObject_GetXCoord(object);
    objectZ = MapObject_GetZCoord(object);
    minX = objectX - distance;
    maxX = objectX + distance;
    minZ = objectZ - distance;
    maxZ = objectZ + distance;
    if (minZ > z) {
        goto fail;
    }
    if (maxZ < z) {
        goto fail;
    }
    if (minX > x) {
        goto fail;
    }
    if (maxX < x) {
        goto fail;
    }
    return GetDirectionTowards(objectX, objectZ, x, z);
fail:
    return -1;
}

#pragma force_active off

int MapObject_GetAllowedDirectionToNearbyPlayer(LocalMapObject *object, int tableId, int value) {
    const int *list;
    int count;
    int i;
    int xDirection;
    int zDirection;
    int objectX;
    int objectZ;
    PlayerAvatar *avatar;
    int playerX;
    int playerZ;
    int direction;

    list = GetDirectionTable(tableId);
    count = DirectionList_Count(list, value);
    if (count == 1) {
        return -1;
    }
    direction = MapObject_GetDirectionToNearbyPlayer(object);
    if (direction == -1) {
        goto done;
    }
    i = 0;
    do {
        if (list[i] == direction) {
            goto done;
        }
        i++;
    } while (i < count);
    xDirection = -1;
    zDirection = -1;
    objectX = MapObject_GetXCoord(object);
    objectZ = MapObject_GetZCoord(object);
    avatar = FieldSystem_GetPlayerAvatar(MapObject_GetFieldSystem(object));
    playerX = PlayerAvatar_GetXCoord(avatar);
    playerZ = PlayerAvatar_GetZCoord(avatar);
    if (objectX > playerX) {
        xDirection = 2;
    } else if (objectX < playerX) {
        xDirection = 3;
    }
    if (objectZ > playerZ) {
        zDirection = 0;
    } else if (objectZ < playerZ) {
        zDirection = 1;
    }
    i = 0;
    if (xDirection == -1) {
        do {
            if (list[i] == zDirection) {
                return zDirection;
            }
            i++;
        } while (i < count);
    } else if (zDirection == -1) {
        do {
            if (list[i] == xDirection) {
                return xDirection;
            }
            i++;
        } while (i < count);
    } else {
        do {
            if (list[i] == xDirection) {
                return xDirection;
            }
            if (list[i] == zDirection) {
                return zDirection;
            }
            i++;
        } while (i < count);
    }
    direction = -1;
done:
    return direction;
}

BOOL MapObject_SpinsWhileWalking(LocalMapObject *object) {
    u32 type = MapObject_GetType(object) - 7;
    if (type <= 1) {
        return TRUE;
    }
    return FALSE;
}

void WalkingSpin_Init(LocalMapObject *object, u8 *state) {
    if (MapObject_GetType(object) == 7) {
        state[1] = 0;
    } else {
        state[1] = 1;
    }
}

#pragma force_active on

void WalkingSpin_TurnBeforeStep(LocalMapObject *object, s8 *state) {
    int facing = MapObject_GetFacingDirection(object);
    int i = 0;
    while (i < 4 && facing != gWalkingSpinOrders[state[1]][i]) {
        i++;
    }
    GF_ASSERT(i < 4);
    state[0] = facing;
    i = (i + 1) % 4;
    facing = gWalkingSpinOrders[state[1]][i];
    if (MapObject_GetFlagsBitsMask(object, MAPOBJECTFLAG_UNK7)) {
        state[2] = 1;
    } else {
        state[2] = 0;
    }
    MapObject_SetFacingDirection(object, facing);
    MapObject_SetFlagsBits(object, MAPOBJECTFLAG_UNK7);
}

#pragma force_active off

void WalkingSpin_EndStep(LocalMapObject *object, u8 *state) {
    if (state[2] == 0) {
        MapObject_ClearFlagsBits(object, (MapObjectFlagBits)0x80);
    }
}
