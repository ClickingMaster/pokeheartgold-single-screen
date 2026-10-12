#include "global.h"

#include "constants/movements.h"

#include "map_object.h"
#include "map_object_behavior_internal.h"
#include "map_object_follow_internal.h"
#include "map_object_movement_commands_internal.h"
#include "map_object_movement_utilities_internal.h"

void FollowPartnerMovement_Init(LocalMapObject *object) {
    FollowPartnerMovementState *state = (FollowPartnerMovementState *)MapObject_InitMovementData(object, sizeof(FollowPartnerMovementState));
    FollowPartnerMovement_FindPartner(object, state);
    MapObject_SetAnimation(object, 0);
    MapObject_ClearSingleMovement(object);
    state->partnerKnown = FALSE;
}

void FollowPartnerMovement_Update(LocalMapObject *object) {
    FollowPartnerMovementState *state = (FollowPartnerMovementState *)MapObject_GetMovementData(object);

    if (FollowPartnerMovement_FindPartner(object, state) != FALSE) {
        while (gFollowPartnerStates[state->state](object, state) == TRUE) {
        }
    }
}

void FollowPartnerMovement_Delete(LocalMapObject *object) {
}

void FollowPartnerMovement_Resume(LocalMapObject *object) {
    ((FollowPartnerMovementState *)MapObject_GetMovementData(object))->partnerKnown = FALSE;
}

BOOL FollowPartnerState_Start(LocalMapObject *object, FollowPartnerMovementState *state) {
    MapObject_ClearSingleMovement(object);
    MapObject_ClearEndMovement(object);
    if (FollowPartnerMovement_HasPartnerMoved(object, state) == TRUE && FollowPartnerMovement_StartStep(object, state) == TRUE) {
        MapObject_SetSingleMovement(object);
        state->state++;
        return TRUE;
    }
    return FALSE;
}

BOOL FollowPartnerState_Moving(LocalMapObject *object, FollowPartnerMovementState *state) {
    if (!MapObject_RunForcedMovement(object)) {
        return FALSE;
    }
    MapObject_ClearSingleMovement(object);
    state->state = 0;
    return FALSE;
}

LocalMapObject *MapObject_FindTrainerPartner(LocalMapObject *object) {
    s32 index = 0;
    LocalMapObject *other;
    u32 type = MapObject_GetType(object);
    u32 mapId = MapObject_GetMapID(object);
    u32 trainerNum = sub_02064518(object);
    MapObjectManager *manager = MapObject_GetManager(object);

    switch (type) {
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
        if (MapObjectManager_GetNextObjectWithFlagFromIndex(manager, &other, &index, MAPOBJECTFLAG_ACTIVE) == TRUE) {
            do {
                if (object != other && mapId == MapObject_GetMapID(other) && trainerNum == sub_02064518(other)) {
                    return other;
                }
            } while (MapObjectManager_GetNextObjectWithFlagFromIndex(manager, &other, &index, MAPOBJECTFLAG_ACTIVE) == TRUE);
        }
        break;
    }
    return NULL;
}

BOOL FollowPartnerMovement_FindPartner(LocalMapObject *object, FollowPartnerMovementState *state) {
    u32 mapId;
    u32 trainerNum;
    MapObjectManager *manager;
    s32 index;
    LocalMapObject *other;

    manager = MapObject_GetManager(object);
    index = 0;
    mapId = MapObject_GetMapID(object);
    trainerNum = sub_02064518(object);

    if (MapObjectManager_GetNextObjectWithFlagFromIndex(manager, &other, &index, MAPOBJECTFLAG_ACTIVE) == TRUE) {
        do {
            if (object != other && mapId == MapObject_GetMapID(other) && trainerNum == sub_02064518(other)) {
                if (state->partnerKnown == FALSE) {
                    FollowPartnerMovement_SetPartner(object, state, other);
                }
                return TRUE;
            }
        } while (MapObjectManager_GetNextObjectWithFlagFromIndex(manager, &other, &index, MAPOBJECTFLAG_ACTIVE) == TRUE);
    }
    state->partnerKnown = FALSE;
    return FALSE;
}

void FollowPartnerMovement_SetPartner(LocalMapObject *object, FollowPartnerMovementState *state, LocalMapObject *partner) {
    state->partnerKnown = TRUE;
    state->partnerX = MapObject_GetXCoord(partner);
    state->partnerZ = MapObject_GetZCoord(partner);
    state->unk6 = 0xff;
    state->partner = partner;
}

BOOL FollowPartnerMovement_HasPartnerMoved(LocalMapObject *object, FollowPartnerMovementState *state) {
    LocalMapObject *partner = state->partner;
    int x = MapObject_GetXCoord(object);
    int z = MapObject_GetZCoord(object);
    int partnerPreviousX = MapObject_GetPreviousXCoord(partner);
    int partnerPreviousZ = MapObject_GetPreviousZCoord(partner);
    if ((x != partnerPreviousX || z != partnerPreviousZ) && (MapObject_CheckSingleMovement(partner) == TRUE || MapObject_GetFlagsBitsMask(partner, (MapObjectFlagBits)(MAPOBJECTFLAG_MOVEMENT_PAUSED | MAPOBJECTFLAG_UNK11 | MAPOBJECTFLAG_UNK12)) == 0)) {
        return TRUE;
    }
    return FALSE;
}

BOOL FollowPartnerMovement_StartStep(LocalMapObject *object, FollowPartnerMovementState *state) {
    int x = MapObject_GetXCoord(object);
    int z = MapObject_GetZCoord(object);
    int partnerX = MapObject_GetXCoord(state->partner);
    int partnerZ = MapObject_GetZCoord(state->partner);
    int partnerPreviousX = MapObject_GetPreviousXCoord(state->partner);
    int partnerPreviousZ = MapObject_GetPreviousZCoord(state->partner);
    int direction;
    if (x == partnerX && z == partnerZ) {
        return FALSE;
    }
    direction = GetDirectionTowards(x, z, partnerPreviousX, partnerPreviousZ);
    x += GetDeltaXByFacingDirection(direction);
    z += GetDeltaYByFacingDirection(direction);
    if (x != partnerX || z != partnerZ) {
        MapObject_ForceSetHeldMovement(object, GetMovementForDirection(direction, MOVEMENT_STEP_UP));
        return TRUE;
    }
    return FALSE;
}
