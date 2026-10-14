#include "global.h"

#include "constants/movements.h"

#include "field/field_follow_mon.h"

#include "field_system.h"
#include "follow_mon.h"
#include "map_object.h"
#include "map_object_follow_internal.h"
#include "map_object_movement_commands_internal.h"
#include "map_object_movement_utilities_internal.h"
#include "metatile_behavior.h"
#include "overlay_01.h"
#include "player_avatar.h"
#include "field_terrain.h"

void FollowPlayerMovement_Init(LocalMapObject *object) {
    FollowPlayerMovementState *state = (FollowPlayerMovementState *)MapObject_InitMovementData(object, sizeof(FollowPlayerMovementState));
    FollowPlayerMovement_CheckPlayer(object, state);
    MapObject_SetAnimation(object, 0);
    MapObject_ClearSingleMovement(object);
    MapObject_ClearFlag18(object, FALSE);
}

void FollowPlayerMovement_UpdateFollowMon(LocalMapObject *object) {
    FollowPlayerMovementState *state = (FollowPlayerMovementState *)MapObject_GetMovementData(object);
    if (FollowPlayerMovement_CheckPlayer(object, state)) {
        MapObject_ClearFlag18(object, FALSE);
        while (gFollowMonFollowPlayerStates[state->state](object, state) == TRUE) {
        }
    }
}

void FollowPlayerMovement_Update(LocalMapObject *object) {
    FollowPlayerMovementState *state = (FollowPlayerMovementState *)MapObject_GetMovementData(object);
    if (FollowPlayerMovement_CheckPlayer(object, state)) {
        while (gFollowPlayerStates[state->state](object, state) == TRUE) {
        }
    }
}

void FollowPlayerMovement_UpdateMirror(LocalMapObject *object) {
    FollowPlayerMovementState *state = (FollowPlayerMovementState *)MapObject_GetMovementData(object);
    if (FollowPlayerMovement_CheckPlayer(object, state)) {
        while (gMirrorPlayerStates[state->state](object, state) == TRUE) {
        }
    }
}

const FollowPartnerStateFunc gFollowPartnerStates[2] = {
    FollowPartnerState_Start,
    FollowPartnerState_Moving,
};

const HiddenObjectStateFunc gHiddenObjectStates[2] = {
    HiddenObjectState_CreateCover,
    HiddenObjectState_Wait,
};

const VecFx32 gHiddenObjectResumeOffset = { 0, -32 * FX32_ONE, 0 };

const VecFx32 gHiddenObjectOffset = { 0, -32 * FX32_ONE, 0 };

const MovementList gStepMovements = { MOVEMENT_STEP_UP, MOVEMENT_STEP_DOWN, MOVEMENT_STEP_LEFT, MOVEMENT_STEP_RIGHT };

const FollowPlayerStateFunc gFollowPlayerStates[4] = {
    FollowPlayerState_Start,
    FollowPlayerState_Moving,
    FollowPlayerState_Hop,
    FollowPlayerState_Wait,
};

const FollowPlayerStateFunc gMirrorPlayerStates[4] = {
    FollowPlayerState_MirrorStart,
    FollowPlayerState_Moving,
    FollowPlayerState_Hop,
    FollowPlayerState_Wait,
};

const FollowPlayerStateFunc gFollowMonFollowPlayerStates[4] = {
    FollowPlayerState_FollowMonStart,
    FollowPlayerState_Moving,
    FollowPlayerState_Hop,
    FollowPlayerState_Wait,
};

const MovementList gRunMovements = { MOVEMENT_RUN_UP, MOVEMENT_RUN_DOWN, MOVEMENT_RUN_LEFT, MOVEMENT_RUN_RIGHT };

const MovementList gFastRunMovements = { 20, 21, MOVEMENT_RUN_FASTER_LEFT, MOVEMENT_RUN_FASTER_RIGHT };

void FollowPlayerMovement_Delete(LocalMapObject *object) {
}

u8 FollowPlayerMovement_GetHopFrame(LocalMapObject *object) {
    return ((FollowPlayerMovementState *)MapObject_GetMovementData(object))->hopFrame;
}

u32 FollowPlayerMovement_GetHopSpeed(LocalMapObject *object) {
    FollowPlayerMovementState *state = (FollowPlayerMovementState *)MapObject_GetMovementData(object);
    return state->hopSpeed;
}

void FollowPlayerMovement_DisableBallEffect(LocalMapObject *object) {
    FollowPlayerMovementState *state = (FollowPlayerMovementState *)MapObject_GetMovementData(object);
    state->noBallEffect = TRUE;
}

void FollowPlayerMovement_Reset(LocalMapObject *object) {
    FieldSystem *fieldSystem = MapObject_GetFieldSystem(object);
    FollowPlayerMovementState *state = (FollowPlayerMovementState *)MapObject_GetMovementData(object);

    MapObject_ClearSingleMovement(object);
    MapObject_ClearEndMovement(object);
    MapObject_ClearFlagsBits(object, MAPOBJECTFLAG_UNK5);
    MapObject_SetMovementCommand(object, MOVEMENT_NONE);
    MapObject_SetMovementStep(object, 0);
    state->state = 0;
    fieldSystem->followMon.queuedMovement = 0;
    fieldSystem->followMon.queuedStepState = 0;
    fieldSystem->followMon.queuedX = 0;
    fieldSystem->followMon.queuedZ = 0;
    MapObject_SetPositionFromXYZAndDirection(object, MapObject_GetXCoord(object), MapObject_GetYCoord(object), MapObject_GetZCoord(object), MapObject_GetFacingDirection(object));
}

static inline void FollowMon_SpawnStepEffects(LocalMapObject *object) {
    if (FollowMon_IsReleaseBallPending(object)) {
        FollowMonBall_Spawn(object, 0);
        FollowMon_SetReleaseBallPending(object, 0);
    } else {
        FollowMon_HideUntilStep(object, FALSE);
    }
    FollowMon_SpawnGrassEffects(object);
}

BOOL FollowPlayerState_FollowMonStart(LocalMapObject *object, FollowPlayerMovementState *state) {
    FieldSystem *fieldSystem = MapObject_GetFieldSystem(object);
    MapObject_ClearSingleMovement(object);
    MapObject_ClearEndMovement(object);

    if (fieldSystem->followMon.queuedStepState == 1) {
        fieldSystem->followMon.queuedStepState = 2;
        return FALSE;
    }
    if (fieldSystem->followMon.queuedStepState == 2) {
        FollowPlayerMovement_UpdatePlayerCoords(object, state);
        if (fieldSystem->followMon.queuedX == MapObject_GetXCoord(object) && fieldSystem->followMon.queuedZ == MapObject_GetZCoord(object)) {
            fieldSystem->followMon.queuedStepState = 0;
            state->state = 3;
            if (FollowMon_IsHiddenUntilStep(object) && !state->noBallEffect) {
                FollowMon_SpawnStepEffects(object);
            }
            if (IsMovement56To59(FollowMon_GetMovement(object))) {
                FollowMon_SetPendingHop(object, (u8)PlayerAvatar_GetFacingDirection(FieldSystem_GetPlayerAvatar(fieldSystem)));
            }
            return TRUE;
        }
        if (FollowPlayerMovement_StartFollowMonStep(object, state) == TRUE) {
            if (FollowMon_IsHiddenUntilStep(object)) {
                FollowMon_SpawnStepEffects(object);
            }
            MapObject_SetSingleMovement(object);
            fieldSystem->followMon.queuedStepState = 3;
            return TRUE;
        }
    } else if (fieldSystem->followMon.queuedStepState == 3) {
        fieldSystem->followMon.queuedStepState = 0;
    }
    return FALSE;
}

BOOL FollowPlayerState_Start(LocalMapObject *object, FollowPlayerMovementState *state) {
    MapObject_ClearSingleMovement(object);
    MapObject_ClearEndMovement(object);
    if (FollowPlayerMovement_HasPlayerMoved(object, state) == TRUE) {
        FollowPlayerMovement_UpdatePlayerCoords(object, state);
        if (FollowMon_IsHiddenUntilStep(object)) {
            FollowMon_SpawnStepEffects(object);
        }
        if (FollowPlayerMovement_StartStep(object) == TRUE) {
            MapObject_SetSingleMovement(object);
            state->state++;
            return TRUE;
        }
    }
    return FALSE;
}

BOOL FollowPlayerState_MirrorStart(LocalMapObject *object, FollowPlayerMovementState *state) {
    MapObject_ClearSingleMovement(object);
    MapObject_ClearEndMovement(object);
    if (FollowPlayerMovement_HasPlayerMoved(object, state) == TRUE) {
        FollowPlayerMovement_UpdatePlayerCoords(object, state);
        if (FollowPlayerMovement_StartMirrorStep(object) == TRUE) {
            MapObject_SetSingleMovement(object);
            state->state++;
            return TRUE;
        }
    }
    return FALSE;
}

BOOL FollowPlayerState_Moving(LocalMapObject *object, FollowPlayerMovementState *state) {
    if (MapObject_RunForcedMovement(object) == TRUE) {
        MapObject_ClearSingleMovement(object);
        state->state = 0;
    }
    return FALSE;
}

BOOL FollowPlayerState_Hop(LocalMapObject *object, FollowPlayerMovementState *state) {
    if (MapObject_RunForcedMovement(object) == TRUE) {
        state->hopCount++;
        if (state->hopCount >= 2) {
            MapObject_ClearSingleMovement(object);
            state->state = 0;
            state->hopFrame = 0;
            state->hopSpeed = 0;
            return FALSE;
        }
        MapObject_ForceSetHeldMovement(object, FollowMon_GetHopMovement(object));
    }
    state->hopFrame++;
    return FALSE;
}

BOOL FollowPlayerState_Wait(LocalMapObject *object, FollowPlayerMovementState *state) {
    PlayerAvatar *avatar = FieldSystem_GetPlayerAvatar(MapObject_GetFieldSystem(object));
    LocalMapObject *player = PlayerAvatar_GetMapObject(avatar);
    if (MapObject_TestFlagsBits(player, MAPOBJECTFLAG_UNK4) == TRUE && MapObject_TestFlagsBits(player, MAPOBJECTFLAG_UNK5) == TRUE) {
        state->state = 0;
    }
    if (PlayerAvatar_GetPlayerMoveState(avatar) == 3) {
        state->state = 0;
    }
    return FALSE;
}

BOOL FollowPlayerMovement_CheckPlayer(LocalMapObject *object, FollowPlayerMovementState *state) {
    if (MapObjectManager_GetFirstActiveObjectWithMovement1(MapObject_GetManager(object)) == NULL) {
        state->playerKnown = FALSE;
        return FALSE;
    }
    if (state->playerKnown == FALSE) {
        FollowPlayerMovement_RecordPlayer(object, state);
    }
    return TRUE;
}

void FollowPlayerMovement_RecordPlayer(LocalMapObject *object, FollowPlayerMovementState *state) {
    PlayerAvatar *avatar = FieldSystem_GetPlayerAvatar(MapObject_GetFieldSystem(object));
    state->playerKnown = TRUE;
    state->playerX = PlayerAvatar_GetXCoord(avatar);
    state->playerZ = PlayerAvatar_GetZCoord(avatar);
    state->unk8 = 0xff;
}

BOOL FollowPlayerMovement_HasPlayerMoved(LocalMapObject *object, FollowPlayerMovementState *state) {
    PlayerAvatar *avatar = FieldSystem_GetPlayerAvatar(MapObject_GetFieldSystem(object));
    if (avatar != NULL) {
        int x = PlayerAvatar_GetXCoord(avatar);
        int z = PlayerAvatar_GetZCoord(avatar);
        if (x != state->playerX || z != state->playerZ) {
            return TRUE;
        }
    }
    return FALSE;
}

void FollowPlayerMovement_UpdatePlayerCoords(LocalMapObject *object, FollowPlayerMovementState *state) {
    PlayerAvatar *avatar = FieldSystem_GetPlayerAvatar(MapObject_GetFieldSystem(object));
    state->playerX = PlayerAvatar_GetXCoord(avatar);
    state->playerZ = PlayerAvatar_GetZCoord(avatar);
}

// Movements 0x58-0x5B are the run movements under another speed; the follower uses the plain run movements.
int FollowMon_GetMovement(LocalMapObject *object) {
    FieldSystem *fieldSystem = MapObject_GetFieldSystem(object);
    FieldSystem_GetPlayerAvatar(fieldSystem);
    switch (fieldSystem->followMon.queuedMovement) {
    case 0x58:
        return MOVEMENT_RUN_UP;
    case 0x59:
        return MOVEMENT_RUN_DOWN;
    case 0x5a:
        return MOVEMENT_RUN_LEFT;
    case 0x5b:
        return MOVEMENT_RUN_RIGHT;
    default:
        return fieldSystem->followMon.queuedMovement;
    }
}

int FollowPlayerMovement_GetPlayerMovement(LocalMapObject *object) {
    int movement = MapObject_GetMovementCommand(PlayerAvatar_GetMapObject(FieldSystem_GetPlayerAvatar(MapObject_GetFieldSystem(object))));
    switch (movement) {
    case 0x58:
        return MOVEMENT_RUN_UP;
    case 0x59:
        return MOVEMENT_RUN_DOWN;
    case 0x5a:
        return MOVEMENT_RUN_LEFT;
    case 0x5b:
        return MOVEMENT_RUN_RIGHT;
    default:
        return movement;
    }
}

#pragma force_active on

// Steps the following Pokemon onto the player's previous tile, at the player's speed. A pending hop movement of the
// Pokemon (FollowMon_GetPendingHop) is played instead, twice (FollowPlayerState_Hop).
BOOL FollowPlayerMovement_StartFollowMonStep(LocalMapObject *object, FollowPlayerMovementState *state) {
    u8 nextState;
    u32 x, z, previousX, previousZ, movement;
    int playerMovement, direction, fastMovement;
    PlayerAvatar *avatar = FieldSystem_GetPlayerAvatar(MapObject_GetFieldSystem(object));

    x = MapObject_GetXCoord(object);
    z = MapObject_GetZCoord(object);
    previousX = PlayerAvatar_GetPreviousXCoord(avatar);
    previousZ = PlayerAvatar_GetPreviousZCoord(avatar);
    if (x == previousX && z == previousZ) {
        goto none;
    }

    playerMovement = FollowMon_GetMovement(object);
    direction = GetDirectionTowards((int)x, (int)z, (int)previousX, (int)previousZ);
    movement = FollowMon_GetPendingHop(object);
    fastMovement = IsMovement56To59(playerMovement);
    nextState = 1;
    if (movement) {
        if (fastMovement) {
            movement = FollowMon_GetFasterMovement(movement, GetMovement8ForDirection(movement));
            state->hopSpeed = GetMovementSpeed(movement);
            FollowMon_SetHopMovement(object, (u8)movement);
            nextState = 2;
            state->hopCount = 0;
            state->hopFrame = 0;
            FollowMon_SetPendingHop(object, (u8)PlayerAvatar_GetFacingDirection(avatar));
        } else {
            if (!PlayerAvatar_CheckFlag6(avatar)) {
                return FALSE;
            }
            movement = FollowMon_GetFasterMovement(movement, (u32)playerMovement);
            state->hopSpeed = GetMovementSpeed(movement);
            FollowMon_SetHopMovement(object, (u8)movement);
            nextState = 2;
            state->hopCount = 0;
            state->hopFrame = 0;
            FollowMon_SetPendingHop(object, 0);
        }
    } else if (fastMovement) {
        movement = GetMovement8ForDirection(direction);
        FollowMon_SetPendingHop(object, (u8)PlayerAvatar_GetFacingDirection(avatar));
    } else {
        movement = GetMovementForDirection(direction, (u32)playerMovement);
    }
    MapObject_ForceSetHeldMovement(object, movement);
    state->state = nextState;
    return TRUE;
none:
    return FALSE;
}

#pragma force_active off

BOOL FollowPlayerMovement_StartStep(LocalMapObject *object) {
    PlayerAvatar *avatar = FieldSystem_GetPlayerAvatar(MapObject_GetFieldSystem(object));
    int x = MapObject_GetXCoord(object);
    int z = MapObject_GetZCoord(object);
    int previousX = PlayerAvatar_GetPreviousXCoord(avatar);
    int previousZ = PlayerAvatar_GetPreviousZCoord(avatar);

    if (x != previousX || z != previousZ) {
        int movement = FollowPlayerMovement_GetPlayerMovement(object);
        int direction = GetDirectionTowards(x, z, previousX, previousZ);
        if (movement == MOVEMENT_NONE) {
            GF_AssertFail();
            return FALSE;
        }
        MapObject_ForceSetHeldMovement(object, GetMovementForDirection(direction, movement));
        return TRUE;
    }
    return FALSE;
}

BOOL FollowPlayerMovement_StartMirrorStep(LocalMapObject *object) {
    PlayerAvatar *avatar = FieldSystem_GetPlayerAvatar(MapObject_GetFieldSystem(object));
    MapObject_GetXCoord(object);
    MapObject_GetZCoord(object);
    PlayerAvatar_GetPreviousXCoord(avatar);
    PlayerAvatar_GetPreviousZCoord(avatar);
    {
        int movement = FollowPlayerMovement_GetPlayerMovement(object);
        if (movement == MOVEMENT_NONE) {
            return FALSE;
        }
        MapObject_ForceSetHeldMovement(object, movement);
    }
    return TRUE;
}
