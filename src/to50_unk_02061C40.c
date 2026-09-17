#include "to50_unk_02061C40_internal.h"

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
    list = sub_02061E6C(state->tableId);
    direction = list[state->index];
    MapObject_SetNextFacingDirection(object, direction);
    if (!sub_02062050(object)) {
        MapObject_SetFacingDirection(object, direction);
    }
    collision = sub_02060BB8(object, direction);
    if (collision & 1) {
        state->index++;
        direction = list[state->index];
        MapObject_SetNextFacingDirection(object, direction);
        if (!sub_02062050(object)) {
            MapObject_SetFacingDirection(object, direction);
        }
        collision = sub_02060BB8(object, direction);
    }
    movementKind = 12;
    if (collision != 0) {
        movementKind = 32;
    }
    MapObject_ForceSetHeldMovement(object, sub_0206234C(direction, movementKind));
    if (sub_02062050(object) == TRUE) {
        sub_0206207C(object, state->saved);
    }
    MapObject_SetSingleMovement(object);
    state->active = TRUE;
    return TRUE;
}
