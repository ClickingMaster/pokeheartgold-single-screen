#include "global.h"

#include "frontier/frontier_map.h"

// Scripted movements of frontier map objects, run every frame for the objects that have one.

typedef BOOL (*FrontierMapMovementFunc)(void *frontier, FrontierMapObject *object);

FrontierMap *Frontier_GetMap(void *frontier);
FrontierMapObject *Frontier_GetMapObjects(void *frontier);
void ov80_0222EFD0(UnkStruct_ov44_02232031 *command, UnkStruct_ov42_02228110 *object, u16 id, s32 direction);

extern const FrontierMapMovementFunc ov80_0223DB24[];

void FrontierMap_SetObjectMovement(void *frontier, u32 id, u32 type, const s16 *params, s32 count) {
    FrontierMapObject *object = FrontierMap_FindObject(frontier, id);
    s32 i;

    MI_CpuFill8(&object->movement, 0, sizeof(FrontierMapObjectMovement));
    object->movement.type = type;
    for (i = 0; i < count; i++) {
        object->movement.data[i] = *params++;
    }
}

void FrontierMap_RunObjectMovement(void *frontier, FrontierMapObject *object) {
    FrontierMapMovementFunc func = ov80_0223DB24[object->movement.type];

    if (func != NULL && func(frontier, object) == TRUE) {
        MI_CpuFill8(&object->movement, 0, sizeof(FrontierMapObjectMovement));
    }
}

void FrontierMap_RunObjectMovements(void *frontier) {
    FrontierMapObject *object = Frontier_GetMapObjects(frontier);
    s32 i;

    for (i = 0; i < FRONTIER_MAP_OBJECT_MAX; i++, object++) {
        if (object->object != NULL && object->unk_38 == NULL) {
            FrontierMap_RunObjectMovement(frontier, object);
        }
    }
}

// Movement 1: turn to data[3], then data[4], then back to the direction in data[0] (data[1] picks the side,
// data[2] is the wait timer).
BOOL FrontierMapMovement_LookAround(void *frontier, FrontierMapObject *object) {
    FrontierMapObjectMovement *movement = &object->movement;
    FrontierMap *map = Frontier_GetMap(frontier);
    UnkStruct_ov44_02232031 command;
    int nextState;

    if (movement->data[2] > 0) {
        movement->data[2]--;
        return FALSE;
    }
    switch (movement->state) {
    case 0:
        switch (movement->data[0]) {
        case 0:
        case 1:
            if (movement->data[1] == 0) {
                movement->data[3] = 2;
                movement->data[4] = 3;
            } else {
                movement->data[3] = 3;
                movement->data[4] = 2;
            }
            break;
        case 2:
        case 3:
            if (movement->data[1] == 0) {
                movement->data[3] = 0;
                movement->data[4] = 1;
            } else {
                movement->data[3] = 1;
                movement->data[4] = 0;
            }
            break;
        default:
            GF_AssertFail();
            return TRUE;
        }
        movement->data[5] = movement->data[0];
        movement->state++;
        // fallthrough
    case 1:
    case 2:
    case 3:
        ov80_0222EFD0(&command, object->object, object->template.id, movement->data[movement->state + 2]);
        ov42_022299C0(map->unk30, &command);
        if (movement->data[0] == movement->data[movement->state + 2]) {
            movement->data[2] = 45;
            nextState = 1;
        } else {
            movement->data[2] = 30;
            nextState = movement->state + 1;
        }
        break;
    default:
        return TRUE;
    }
    movement->state = nextState;
    return FALSE;
}
