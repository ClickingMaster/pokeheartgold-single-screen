#include "map_object_movement.h"

#include "global.h"

#include "field_system.h"
#include "map_object.h"
#include "map_object_movement_utilities_internal.h"
#include "math_util.h"
#include "player_avatar.h"
#include "unk_0205CB48.h"

// Defined elsewhere without a header yet.
u32 GetMovementForDirection(s32 direction, u32 movement);
void MapObject_ForceSetHeldMovement(LocalMapObject *object, u32 movement);
BOOL MapObject_RunForcedMovement(LocalMapObject *object);

extern MovementStateFunc _020FD4EC[];
extern MovementStateFunc _020FD548[];
extern MovementStateFunc _020FD5A0[];
extern MovementStateFunc _020FD5D0[];
extern const DirectionCycle _020FD754;
extern const DirectionCycle _020FD768;
extern const int _020FD77C[5];
extern const int _020FD7A4[5];
extern const int _020FD7B8[];
extern int _020FD7E0[][4];
extern const int _020FD800[];
extern const MovementListEntry _020FD838[];

void sub_02061284(LocalMapObject *object, u32 value) {
    u8 *state = sub_0205F370(object, 8);
    *(u16 *)(state + 2) = DirectionList_GetRandom(_020FD7B8, -1);
    *(u32 *)(state + 4) = value;
    sub_0205F328(object, 0);
    MapObject_ClearSingleMovement(object);
}

void sub_020612B4(LocalMapObject *object) {
    sub_02061284(object, 0);
}

void sub_020612C0(LocalMapObject *object) {
    sub_02061284(object, 1);
}

void sub_020612CC(LocalMapObject *object) {
    sub_02061284(object, 2);
}

void sub_020612D8(LocalMapObject *object) {
    sub_02061284(object, 3);
}

void sub_020612E4(LocalMapObject *object) {
    sub_02061284(object, 4);
}

void sub_020612F0(LocalMapObject *object) {
    sub_02061284(object, 5);
}

void sub_020612FC(LocalMapObject *object) {
    sub_02061284(object, 6);
}

void sub_02061308(LocalMapObject *object) {
    sub_02061284(object, 7);
}

void sub_02061314(LocalMapObject *object) {
    sub_02061284(object, 8);
}

void sub_02061320(LocalMapObject *object) {
    sub_02061284(object, 9);
}

void sub_0206132C(LocalMapObject *object) {
    sub_02061284(object, 10);
}

void sub_02061338(LocalMapObject *object) {
    u8 *state = sub_0205F394(object);
    int direction = sub_02061F5C(object, *(u32 *)(state + 4), -1);
    if (direction != -1) {
        MapObject_SetFacingDirection(object, direction);
    } else if (*(u16 *)state == 0) {
        (*(s16 *)(state + 2))--;
        if (*(s16 *)(state + 2) <= 0) {
            *(u16 *)(state + 2) = DirectionList_GetRandom(_020FD7B8, -1);
            direction = GetRandomDirectionFromTable(*(u32 *)(state + 4), -1);
            MapObject_SetFacingDirection(object, direction);
        }
    }
    MapObject_SetPreviousCoordsToCurrent(object);
}

void sub_0206139C(LocalMapObject *object) {
    (void)object;
}

void sub_020613A0(LocalMapObject *object) {
    sub_020613D0(object, 12, 11, 0);
}

void sub_020613B0(LocalMapObject *object) {
    sub_020613D0(object, 12, 12, 0);
}

void sub_020613C0(LocalMapObject *object) {
    sub_020613D0(object, 12, 13, 0);
}

void sub_020613D0(LocalMapObject *object, u32 first, u32 second, u32 third) {
    u32 *state = (u32 *)sub_0205F370(object, 16);
    state[1] = third;
    state[2] = first;
    state[3] = second;
    sub_0205F328(object, 0);
    MapObject_ClearSingleMovement(object);
}

void sub_020613F8(LocalMapObject *object) {
    LocalMovementState *state = (LocalMovementState *)sub_0205F394(object);
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
        state->delay = DirectionList_GetRandom(_020FD7B8, -1);
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

void sub_02061648(LocalMapObject *object, u32 direction) {
    u32 *state = (u32 *)sub_0205F370(object, 8);
    state[0] = direction;
    sub_0205F328(object, 0);
    MapObject_ClearSingleMovement(object);
    MapObject_SetPreviousCoordsToCurrent(object);
}

void sub_0206166C(LocalMapObject *object) {
    u32 *state = (u32 *)sub_0205F394(object);
    switch (state[1]) {
    case 0:
        MapObject_SetFacingDirection(object, state[0]);
        state[1]++;
        break;
    case 1:
        break;
    }
}

void sub_02061690(LocalMapObject *object) {
    sub_02061648(object, 0);
}

void sub_0206169C(LocalMapObject *object) {
    sub_02061648(object, 1);
}

void sub_020616A8(LocalMapObject *object) {
    sub_02061648(object, 2);
}

void sub_020616B4(LocalMapObject *object) {
    sub_02061648(object, 3);
}

void sub_020616C0(LocalMapObject *object, u32 value) {
    u8 *state = sub_0205F370(object, 8);
    state[0] = value;
    sub_0205F328(object, 0);
    MapObject_ClearSingleMovement(object);
    MapObject_SetPreviousCoordsToCurrent(object);
}

void sub_020616E4(LocalMapObject *object) {
    sub_020616C0(object, 2);
}

void sub_020616F0(LocalMapObject *object) {
    sub_020616C0(object, 3);
}

void sub_020616FC(LocalMapObject *object) {
    u8 *state = sub_0205F394(object);

    while (_020FD5D0[(s8)state[2]](object, state) == TRUE) {
    }
}

BOOL sub_02061720(LocalMapObject *object, u8 *state) {
    int direction = sub_02061F5C(object, 0x26, -1);
    if (direction == -1) {
        direction = MapObject_GetFacingDirection(object);
    }
    MapObject_ForceSetHeldMovement(object, GetMovementForDirection(direction, 0));
    state[2] = 1;
    return TRUE;
}

BOOL sub_02061754(LocalMapObject *object, u8 *state) {
    if (!MapObject_RunForcedMovement(object)) {
        return FALSE;
    }
    *(u32 *)(state + 4) = 0;
    state[2] = 2;
    return TRUE;
}

BOOL sub_02061770(LocalMapObject *object, u8 *state) {
    if (*(u32 *)(state + 4) != 0 && sub_02061F5C(object, 0x26, -1) != -1) {
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

BOOL sub_020617AC(LocalMapObject *object, s8 *state) {
    DirectionCycle listA = _020FD754;
    DirectionCycle listB = _020FD768;
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

void sub_02061844(LocalMapObject *object) {
    sub_020616C0(object, 3);
}

void sub_02061850(LocalMapObject *object) {
    u8 *state = sub_0205F394(object);
    while (_020FD5A0[(s8)state[2]](object, state) == TRUE) {}
}

void sub_02061874(LocalMapObject *object, u8 *state) {
    u32 facing = MapObject_GetFacingDirection(object);
    MapObject_ForceSetHeldMovement(object, GetMovementForDirection(facing, 0));
    state[2] = 1;
}

BOOL sub_02061894(LocalMapObject *object, u8 *state) {
    if (!MapObject_RunForcedMovement(object)) {
        return FALSE;
    }
    *(u32 *)(state + 4) = 0;
    state[2] = 2;
    return TRUE;
}

BOOL sub_020618B0(LocalMapObject *object, u8 *state) {
    (void)object;
    (*(int *)(state + 4))++;
    if (*(int *)(state + 4) < 24) {
        return FALSE;
    }
    state[2] = 3;
    return TRUE;
}

BOOL sub_020618C8(LocalMapObject *object, DirectionCycleState *state) {
    typedef struct DirectionOrder {
        int values[5];
    } DirectionOrder;

    DirectionOrder orderA = *(const DirectionOrder *)_020FD77C;
    DirectionOrder orderB = *(const DirectionOrder *)_020FD7A4;
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

void sub_0206197C(LocalMapObject *object) {
    u8 *state = sub_0205F370(object, 8);
    if (sub_02062050(object) == TRUE) {
        sub_02062064(object, state + 4);
    }
}

void sub_0206199C(LocalMapObject *object) {
    u8 *state = sub_0205F394(object);
    while (_020FD548[*(s16 *)state](object, state) == TRUE) {}
}

void sub_020619C0(LocalMapObject *object, u8 *state) {
    u32 direction = MapObject_GetInitialFacingDirection(object);
    if (*(s16 *)(state + 2) == 1) {
        direction = GetOppositeDirection(direction);
    }
    MapObject_SetNextFacingDirection(object, direction);
    if (!sub_02062050(object)) {
        MapObject_SetFacingDirection(object, direction);
    }
    *(u16 *)state = 1;
}

int sub_020619FC(void *mapObject, Unk020619FCState *state) {
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
            if (sub_02062050(mapObject) == 0) {
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
    if (sub_02062050(mapObject) == 1) {
        sub_0206207C(mapObject, state->data);
    }
    MapObject_SetSingleMovement(mapObject);
    state->state = 2;
    return 1;
}

BOOL sub_02061ABC(LocalMapObject *object, u8 *state) {
    if (MapObject_RunForcedMovement(object) == TRUE) {
        MapObject_ClearSingleMovement(object);
        if (sub_02062050(object) == TRUE) {
            sub_020620F8(object, state + 4);
        }
        *(u16 *)state = 0;
    }
    return FALSE;
}

void sub_02061AEC(LocalMapObject *object, u32 first, u32 second, u32 value) {
    u8 *state = sub_0205F370(object, 12);
    state[2] = first;
    state[3] = second;
    *(u32 *)(state + 4) = value;
    if (sub_02062050(object) == TRUE) {
        sub_02062064(object, state + 8);
    }
}

void sub_02061B1C(LocalMapObject *object) {
    sub_02061AEC(object, 2, 0, 14);
}

void sub_02061B2C(LocalMapObject *object) {
    sub_02061AEC(object, 2, 0, 15);
}

void sub_02061B3C(LocalMapObject *object) {
    sub_02061AEC(object, 2, 1, 16);
}

void sub_02061B4C(LocalMapObject *object) {
    sub_02061AEC(object, 2, 1, 17);
}

void sub_02061B5C(LocalMapObject *object) {
    sub_02061AEC(object, 2, 0, 18);
}

void sub_02061B6C(LocalMapObject *object) {
    sub_02061AEC(object, 2, 0, 19);
}

void sub_02061B7C(LocalMapObject *object) {
    sub_02061AEC(object, 2, 1, 20);
}

void sub_02061B8C(LocalMapObject *object) {
    sub_02061AEC(object, 2, 1, 21);
}

void sub_02061B9C(LocalMapObject *object) {
    sub_02061AEC(object, 2, 1, 22);
}

void sub_02061BAC(LocalMapObject *object) {
    sub_02061AEC(object, 2, 1, 23);
}

void sub_02061BBC(LocalMapObject *object) {
    sub_02061AEC(object, 2, 0, 24);
}

void sub_02061BCC(LocalMapObject *object) {
    sub_02061AEC(object, 2, 0, 25);
}

void sub_02061BDC(LocalMapObject *object) {
    sub_02061AEC(object, 2, 1, 26);
}

void sub_02061BEC(LocalMapObject *object) {
    sub_02061AEC(object, 2, 1, 27);
}

void sub_02061BFC(LocalMapObject *object) {
    sub_02061AEC(object, 2, 0, 28);
}

void sub_02061C0C(LocalMapObject *object) {
    sub_02061AEC(object, 2, 0, 29);
}

void sub_02061C1C(LocalMapObject *object) {
    u8 *state = sub_0205F394(object);
    while (_020FD4EC[state[0]](object, state) == TRUE) {}
}

BOOL sub_02061C40(LocalMapObject *object, MovementPathState *state) {
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
    if (!sub_02062050(object)) {
        MapObject_SetFacingDirection(object, direction);
    }
    collision = MapObject_CheckCollisionInDirection(object, direction);
    if (collision & 1) {
        state->index++;
        direction = list[state->index];
        MapObject_SetNextFacingDirection(object, direction);
        if (!sub_02062050(object)) {
            MapObject_SetFacingDirection(object, direction);
        }
        collision = MapObject_CheckCollisionInDirection(object, direction);
    }
    movementKind = 12;
    if (collision != 0) {
        movementKind = 32;
    }
    MapObject_ForceSetHeldMovement(object, GetMovementForDirection(direction, movementKind));
    if (sub_02062050(object) == TRUE) {
        sub_0206207C(object, state->saved);
    }
    MapObject_SetSingleMovement(object);
    state->active = TRUE;
    return TRUE;
}

BOOL sub_02061D50(LocalMapObject *object, u8 *state) {
    if (MapObject_RunForcedMovement(object) == TRUE) {
        MapObject_ClearSingleMovement(object);
        if (sub_02062050(object) == TRUE) {
            sub_020620F8(object, state + 8);
        }
        state[0] = 0;
    }
    return FALSE;
}

void sub_02061D80(LocalMapObject *object) {
    sub_02061AEC(object, 2, 1, 30);
}

void sub_02061D90(LocalMapObject *object) {
    sub_02061AEC(object, 2, 1, 31);
}

void sub_02061DA0(LocalMapObject *object) {
    sub_02061AEC(object, 2, 0, 32);
}

void sub_02061DB0(LocalMapObject *object) {
    sub_02061AEC(object, 2, 0, 33);
}

void sub_02061DC0(LocalMapObject *object) {
    sub_02061AEC(object, 2, 1, 34);
}

void sub_02061DD0(LocalMapObject *object) {
    sub_02061AEC(object, 2, 1, 35);
}

void sub_02061DE0(LocalMapObject *object) {
    sub_02061AEC(object, 2, 0, 36);
}

void sub_02061DF0(LocalMapObject *object) {
    sub_02061AEC(object, 2, 0, 37);
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
    const MovementListEntry *entry = _020FD838;
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
    if (!sub_0205DE98(avatar)) {
        return -1;
    }
    i = 0;
    movement = MapObject_GetMovement(object);
    do {
        entry = _020FD800[i++];
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

int sub_02061F5C(LocalMapObject *object, int tableId, int value) {
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

BOOL sub_02062050(LocalMapObject *object) {
    u32 type = MapObject_GetType(object) - 7;
    if (type <= 1) {
        return TRUE;
    }
    return FALSE;
}

void sub_02062064(LocalMapObject *object, u8 *state) {
    if (MapObject_GetType(object) == 7) {
        state[1] = 0;
    } else {
        state[1] = 1;
    }
}

#pragma force_active on

void sub_0206207C(LocalMapObject *object, s8 *state) {
    int facing = MapObject_GetFacingDirection(object);
    int i = 0;
    while (i < 4 && facing != _020FD7E0[state[1]][i]) {
        i++;
    }
    GF_ASSERT(i < 4);
    state[0] = facing;
    i = (i + 1) % 4;
    facing = _020FD7E0[state[1]][i];
    if (MapObject_GetFlagsBitsMask(object, MAPOBJECTFLAG_UNK7)) {
        state[2] = 1;
    } else {
        state[2] = 0;
    }
    MapObject_SetFacingDirection(object, facing);
    MapObject_SetFlagsBits(object, MAPOBJECTFLAG_UNK7);
}

#pragma force_active off

void sub_020620F8(LocalMapObject *object, u8 *state) {
    if (state[2] == 0) {
        MapObject_ClearFlagsBits(object, (MapObjectFlagBits)0x80);
    }
}
