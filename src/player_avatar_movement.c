#include "player_avatar_movement.h"

#include "global.h"

#include "constants/global_fieldmap.h"
#include "constants/movements.h"
#include "constants/player_avatar.h"
#include "constants/sndseq.h"

#include "field/field_follow_mon.h"

#include "follow_mon.h"
#include "game_stats.h"
#include "gymmick_init.h"
#include "map_object.h"
#include "map_object_movement_commands.h"
#include "map_object_movement_utilities.h"
#include "metatile_behavior.h"
#include "player_avatar_actions.h"
#include "player_avatar.h"
#include "sound_playback.h"
#include "field_terrain.h"
#include "field_misc_events.h"

// Player avatar movement control (ARM9 static 0x0205CB48-0x0205E0A0): turns the d-pad input into the player's map
// object movement each frame, on foot and on the bike, with collision checks, forced movement (ice, Cycling Road
// slopes) and step sounds. The tables are in asm/player_avatar_movement_data.s.

typedef struct ForcedMovementEntry {
    BOOL (*matches)(u8 behavior);
    int kind;
} ForcedMovementEntry;

typedef struct FootstepSound {
    u16 sequence;
    u16 fieldBank; // 1: played from the field sound bank
} FootstepSound;

typedef BOOL (*ForcedMovementFunc)(PlayerAvatar *avatar, int value);

extern ForcedMovementFunc gPlayerForcedMovementFuncs[];
extern const ForcedMovementEntry gPlayerForcedMovementBehaviors[];
extern const FootstepSound gPlayerFootstepSounds[];

// Declared with the types their call sites were matched against.
int GetMovementDirection(u32 movement);
int MapObject_GetTileAttrBits8To14InDirection(LocalMapObject *object, int direction);
int GetMovementForDirection(int direction, int movement);
BOOL MapObject_IsOnBridge(LocalMapObject *object, int tile);
BOOL MapObject_IsOutsideMovementRange(LocalMapObject *object, int x, int y, int z);
BOOL MapObject_IsMoveBlockedByBehavior(LocalMapObject *object, int x, int z, int direction);
void OffsetPositionInDirection(int direction, VecFx32 *position, int distance);
void FuchsiaGymmick_CheckCollision(FieldSystem *fieldSystem, int x, int z);
void Sound_ApplyFootstepVariation(u16 sequence);
int MapObject_GetMetatileBehaviorInDirection(LocalMapObject *object, int direction);
void Field_PlayerAvatar_TryEnterPoketchPose(PlayerAvatar *avatar);

// PlayerAvatar_IsBikeBlockedInDirection passes the behavior as a full word (matched against int parameters).
#define MetatileBehavior_IsVeryTallGrass_Int(behavior) ((BOOL(*)(u32))MetatileBehavior_IsVeryTallGrass)(behavior)
#define MetatileBehavior_IsMud_Int(behavior)           ((BOOL(*)(u32))MetatileBehavior_IsMud)(behavior)

int PlayerAvatar_CheckReadyToMove(PlayerAvatar *avatar, int direction, int keys, int heldKeys);
int PlayerAvatar_IsReadyToMove(PlayerAvatar *avatar, int direction);
void PlayerAvatar_StoreHeldDirections(PlayerAvatar *avatar, int direction, int keys, int heldKeys);
void PlayerAvatar_ClearFlag1IfFlag6Moving(PlayerAvatar *avatar);
void PlayerAvatar_PlayTerrainSounds(PlayerAvatar *avatar);
void PlayerAvatar_PlayFootstepSound(LocalMapObject *object, PlayerAvatar *avatar);
BOOL PlayerAvatar_TryForcedMovement(PlayerAvatar *avatar, int value);
int PlayerAvatar_GetForcedMovementKind(PlayerAvatar *avatar, int value);
BOOL PlayerAvatar_RunForcedMovement(PlayerAvatar *avatar, int kind, int value);
BOOL ForcedMovement_None(PlayerAvatar *avatar, int value);
BOOL ForcedMovement_Ice(PlayerAvatar *avatar, int value);
BOOL ForcedMovement_CyclingRoadSlope(PlayerAvatar *avatar, int value);
void PlayerAvatar_EndForcedMovement(PlayerAvatar *avatar);
int PlayerAvatar_GetSlopeInDirection(PlayerAvatar *avatar, int direction);
BOOL PlayerAvatar_AdjustSpeedForSlope(PlayerAvatar *avatar, int slope);
void PlayerAvatar_SlideInDirection(PlayerAvatar *avatar, int direction);
void PlayerAvatar_MoveByState(PlayerAvatar *avatar, MapLoadManager *mapLoadManager, int direction, u16 keys, u16 heldKeys);
void PlayerAvatar_MoveOnFoot(PlayerAvatar *avatar, LocalMapObject *mapObject, MapLoadManager *mapLoadManager, int direction, u16 keys, u16 heldKeys);
int PlayerAvatar_UpdateFootStepKind(PlayerAvatar *avatar, int direction);
int FootStepKindToMoveState(int stepKind);
int PlayerAvatar_AdjustMovementDummy(LocalMapObject *mapObject, u8 behavior, int movement);
int PlayerAvatar_GetFootStepKind(PlayerAvatar *avatar, int direction);
void PlayerAvatar_FootStand(PlayerAvatar *avatar, LocalMapObject *mapObject, int direction, u16 keys, u16 heldKeys);
void PlayerAvatar_FootStep(PlayerAvatar *avatar, LocalMapObject *mapObject, int direction, u16 keys, u16 heldKeys);
void PlayerAvatar_FootTurn(PlayerAvatar *avatar, LocalMapObject *mapObject, int direction, u16 keys, u16 heldKeys);
void PlayerAvatar_MoveOnBike(PlayerAvatar *avatar, LocalMapObject *mapObject, MapLoadManager *mapLoadManager, int direction, u16 keys, u16 heldKeys);
int PlayerAvatar_GetBikeMovement(PlayerAvatar *avatar, int direction);
BOOL PlayerAvatar_AccelerateBike(PlayerAvatar *avatar);
BOOL PlayerAvatar_DecelerateBike(PlayerAvatar *avatar);
void PlayerAvatar_DispatchBikeStep(PlayerAvatar *avatar, LocalMapObject *mapObject, MapLoadManager *mapLoadManager, int direction, u16 keys, u16 heldKeys);
int PlayerAvatar_UpdateBikeStepKind(PlayerAvatar *avatar, int direction);
int BikeStepKindToMoveState(int stepKind);
int PlayerAvatar_GetBikeStepKind(PlayerAvatar *avatar, int direction);
void PlayerAvatar_BikeStop(PlayerAvatar *avatar, LocalMapObject *mapObject, int direction, u16 keys, u16 heldKeys);
void PlayerAvatar_BikeRide(PlayerAvatar *avatar, LocalMapObject *mapObject, int direction, u16 keys, u16 heldKeys);
void PlayerAvatar_BikeTurn(PlayerAvatar *avatar, LocalMapObject *mapObject, int direction, u16 keys, u16 heldKeys);
void PlayerAvatar_BikeCoast(PlayerAvatar *avatar, LocalMapObject *mapObject, int direction, u16 keys, u16 heldKeys);
void PlayerAvatar_SetObjectMovement(PlayerAvatar *avatar, LocalMapObject *mapObject, int movement);
u32 PlayerAvatar_CheckCollision(PlayerAvatar *avatar, LocalMapObject *mapObject, int direction);
u32 MapObject_GetCollisionInDirection(PlayerAvatar *avatar, LocalMapObject *mapObject, int direction);
BOOL MapObject_IsLedgeInDirection(PlayerAvatar *avatar, LocalMapObject *mapObject, int direction);
BOOL MapObject_IsWarpInDirection(PlayerAvatar *avatar, LocalMapObject *mapObject, int direction);
BOOL MapObject_IsWaterInDirection(PlayerAvatar *avatar, LocalMapObject *mapObject, int direction);
BOOL PlayerAvatar_IsBikeBlockedInDirection(PlayerAvatar *avatar, LocalMapObject *mapObject, int direction);
int GetHorizontalDirectionFromKeys(int keys);
int GetVerticalDirectionFromKeys(int keys);
int PlayerAvatar_ChooseInputDirection(PlayerAvatar *avatar, int keys, int heldKeys);
BOOL PlayerAvatar_IsReadyOrBumpingWall(PlayerAvatar *avatar);
BOOL IsWallBumpMovement(u32 movement);
BOOL IsRunButtonHeld(PlayerAvatar *avatar, int heldKeys);
int PlayerAvatar_GetMoveStateForDirection(PlayerAvatar *avatar, int direction);
u32 PlayerAvatar_GetMovementFromInput(PlayerAvatar *avatar, int keys, int heldKeys, u32 speed, int allowRun, int collision);
BOOL PlayerAvatar_IsReadyForMovement(PlayerAvatar *avatar);
void PlayerAvatar_SetMovement(PlayerAvatar *avatar, int movement);
void PlayerAvatar_GetMetatileBehaviorInDirection(PlayerAvatar *avatar, int direction);
void PlayerAvatar_GetCoordsInDirection(PlayerAvatar *avatar, int direction, int *x, int *z);
void PlayerAvatar_CountStep(PlayerAvatar *avatar);
BOOL PlayerAvatar_IsOnCyclingRoadSlope(PlayerAvatar *avatar, int behavior, int value);

void PlayerAvatar_MoveControl(PlayerAvatar *avatar, MapLoadManager *mapLoadManager, int direction, u16 keys, u16 heldKeys, int allowTurn) {
    register MapLoadManager *manager = mapLoadManager;
    if (direction == -1) {
        direction = PlayerAvatar_ChooseInputDirection(avatar, keys, heldKeys);
    }
    if (PlayerAvatar_IsReadyToMove(avatar, direction)) {
        PlayerAvatar_StoreHeldDirections(avatar, direction, keys, heldKeys);
        Field_PlayerAvatar_ApplyTransitionFlags(avatar);
        if (PlayerAvatar_TryForcedMovement(avatar, direction) == TRUE) {
            Field_PlayerAvatar_TryLeavePoketchPose(avatar);
            return;
        }
        if (PlayerAvatar_GetState(avatar) == PLAYER_STATE_WALKING) {
            if (PlayerAvatar_UpdateFootStepKind(avatar, direction)) {
                Field_PlayerAvatar_TryLeavePoketchPose(avatar);
            } else if (allowTurn == TRUE) {
                Field_PlayerAvatar_TryEnterPoketchPose(avatar);
            }
        }
        PlayerAvatar_MoveByState(avatar, manager, direction, keys, heldKeys);
        PlayerAvatar_ClearFlag1IfFlag6Moving(avatar);
        PlayerAvatar_PlayTerrainSounds(avatar);
    }
}

int PlayerAvatar_CheckReadyToMove(PlayerAvatar *avatar, int direction, int keys, int heldKeys) {
    return PlayerAvatar_IsReadyToMove(avatar, direction);
}

int PlayerAvatar_IsReadyToMove(PlayerAvatar *avatar, int direction) {
    LocalMapObject *mapObject = PlayerAvatar_GetMapObject(avatar);

    if (MapObject_AreBitsSetForMovementScriptInit(mapObject) == TRUE) {
        return TRUE;
    }
    if (direction == -1) {
        return FALSE;
    }
    if (IsWallBumpMovement(MapObject_GetMovementCommand(mapObject)) == TRUE) {
        int collision = PlayerAvatar_CheckCollision(avatar, mapObject, direction);
        if (collision == 0) {
            return TRUE;
        }
        if (collision == 0x20 && PlayerAvatar_GetState(avatar) == PLAYER_STATE_SURFING) {
            return TRUE;
        }
        return FALSE;
    }
    return FALSE;
}

void PlayerAvatar_StoreHeldDirections(PlayerAvatar *avatar, int direction, int keys, int heldKeys) {
    int horizontal = GetHorizontalDirectionFromKeys(heldKeys);
    int vertical = GetVerticalDirectionFromKeys(heldKeys);
    PlayerAvatar_SetLastInputs(avatar, horizontal, vertical);
    PlayerAvatar_ClearFlag6(avatar);
}

void PlayerAvatar_ClearFlag1IfFlag6Moving(PlayerAvatar *avatar) {
    if (PlayerAvatar_CheckFlag6(avatar) == TRUE && PlayerAvatar_GetMoveState(avatar) == AVATAR_MOVE_STATE_MOVING) {
        PlayerAvatar_SetFlag1(avatar, FALSE);
    }
}

void PlayerAvatar_PlayTerrainSounds(PlayerAvatar *avatar) {
    LocalMapObject *object;
    u8 behavior;
    u8 previous;
    int direction;
    u32 movement;

    if (PlayerAvatar_GetMoveState(avatar) != AVATAR_MOVE_STATE_MOVING) {
        return;
    }
    object = PlayerAvatar_GetMapObject(avatar);
    behavior = MapObject_GetCurrentMetatileBehavior(object);
    direction = GetMovementDirection(MapObject_GetMovementCommand(object));
    if (direction == -1) {
        previous = behavior;
    } else {
        previous = MapObject_GetMetatileBehaviorInDirection(object, direction);
    }

    if (MetatileBehavior_IsPuddle(behavior) == TRUE) {
        PlaySE(SEQ_SE_DP_FOOT3_0);
    }
    if (MetatileBehavior_IsShallowWater(behavior) == TRUE) {
        PlaySE(SEQ_SE_DP_FOOT3_1);
    }
    MetatileBehavior_IsSand(behavior);
    if (MetatileBehavior_IsMud(behavior) == TRUE) {
        PlaySE(SEQ_SE_DP_MARSH_WALK);
    }

    movement = MapObject_GetMovementCommand(object);
    PlayerAvatar_GetState(avatar);
    if (IsWallBumpMovement(movement) == FALSE) {
        if (MetatileBehavior_IsVeryTallGrass(behavior) == TRUE || MetatileBehavior_IsVeryTallGrass(previous) == TRUE) {
            PlaySE(SEQ_SE_DP_KUSA);
        }
        if (MetatileBehavior_IsTallGrass(behavior) == TRUE || MetatileBehavior_IsTallGrass(previous) == TRUE) {
            PlaySE(SEQ_SE_GS_KUSA2);
        }
    }
    PlayerAvatar_PlayFootstepSound(object, avatar);
}

void PlayerAvatar_PlayFootstepSound(LocalMapObject *object, PlayerAvatar *avatar) {
    int direction = GetMovementDirection(MapObject_GetMovementCommand(object));
    u8 behavior = MapObject_GetCurrentMetatileBehavior(object);
    u8 current = MapObject_GetCurrentMetatileBehavior(object);
    u16 sequence;
    int state;

    if (MetatileBehavior_IsVeryTallGrass(current) != FALSE || MetatileBehavior_IsPuddle(current) == TRUE || MetatileBehavior_IsShallowWater(current) == TRUE || MetatileBehavior_IsTallGrass(current) != FALSE) {
        return;
    }
    if (direction != -1) {
        behavior = MapObject_GetTileAttrBits8To14InDirection(object, direction);
    }
    if (gPlayerFootstepSounds[behavior].sequence == SEQ_SE_GS_EDAPAKI) {
        PlayerAvatar_ResetFootstepToggle(avatar);
    } else if (PlayerAvatar_GetPlayerMoveState(avatar) == PLAYER_MOVE_STATE_NONE) {
        PlayerAvatar_ResetFootstepToggle(avatar);
    } else {
        PlayerAvatar_ToggleFootstep(avatar);
    }
    if (PlayerAvatar_GetFootstepToggle(avatar) != 0) {
        return;
    }
    state = PlayerAvatar_GetState(avatar);
    switch (state) {
    case PLAYER_STATE_CYCLING:
    case PLAYER_STATE_SURFING:
        return;
    }
    if (behavior < 16) {
        if (behavior == 0 && PlayerAvatar_IsRunning(avatar) == TRUE) {
            behavior = 1;
        }
        if (gPlayerFootstepSounds[behavior].fieldBank == 1) {
            sequence = gPlayerFootstepSounds[behavior].sequence;
            PlaySEWithFieldBank(sequence);
        } else {
            sequence = gPlayerFootstepSounds[behavior].sequence;
            PlaySE(sequence);
        }
        Sound_ApplyFootstepVariation(sequence);
        return;
    }
    if (behavior >= 16) {
        GF_AssertFail();
    }
}

void PlayerAvatar_UpdateMovement(PlayerAvatar *avatar) {
    int moveState = PlayerAvatar_GetMoveState(avatar);
    int playerMoveState = PlayerAvatar_GetPlayerMoveState(avatar);
    LocalMapObject *object = PlayerAvatar_GetMapObject(avatar);

    PlayerAvatar_SetPlayerMoveState(avatar, PLAYER_MOVE_STATE_NONE);
    switch (PlayerAvatar_GetForcedMovementKind(avatar, -1)) {
    case 0:
    case 2:
        break;
    default:
        PlayerAvatar_SetPlayerMoveState(avatar, PLAYER_MOVE_STATE_MOVING);
        return;
    }

    if (!MapObject_AreBitsSetForMovementScriptInit(object)) {
        switch (moveState) {
        case AVATAR_MOVE_STATE_NONE:
            return;
        case AVATAR_MOVE_STATE_MOVING:
            if (IsWallBumpMovement(MapObject_GetMovementCommand(object)) == TRUE) {
                return;
            }
            if (playerMoveState == PLAYER_MOVE_STATE_NONE || playerMoveState == PLAYER_MOVE_STATE_END) {
                PlayerAvatar_SetPlayerMoveState(avatar, PLAYER_MOVE_STATE_START);
            } else {
                PlayerAvatar_SetPlayerMoveState(avatar, PLAYER_MOVE_STATE_MOVING);
            }
            return;
        case AVATAR_MOVE_STATE_TURNING:
            PlayerAvatar_SetPlayerMoveState(avatar, PLAYER_MOVE_STATE_MOVING);
            return;
        default:
            return;
        }
    }

    if (MapObject_IsMovementPaused(object) != TRUE) {
        return;
    }
    switch (moveState) {
    case AVATAR_MOVE_STATE_NONE:
        return;
    case AVATAR_MOVE_STATE_MOVING:
        if (playerMoveState == PLAYER_MOVE_STATE_NONE) {
            return;
        }
        if (playerMoveState == PLAYER_MOVE_STATE_END) {
            PlayerAvatar_SetPlayerMoveState(avatar, PLAYER_MOVE_STATE_NONE);
        } else {
            PlayerAvatar_SetPlayerMoveState(avatar, PLAYER_MOVE_STATE_END);
        }
        return;
    case AVATAR_MOVE_STATE_TURNING:
        if (playerMoveState == PLAYER_MOVE_STATE_NONE) {
            return;
        }
        if (playerMoveState == PLAYER_MOVE_STATE_END) {
            PlayerAvatar_SetPlayerMoveState(avatar, PLAYER_MOVE_STATE_NONE);
        } else {
            PlayerAvatar_SetPlayerMoveState(avatar, PLAYER_MOVE_STATE_END);
        }
        return;
    }
}

void PlayerAvatar_ResetMovement(PlayerAvatar *avatar) {
    PlayerAvatar_SetMoveState(avatar, AVATAR_MOVE_STATE_NONE);
    PlayerAvatar_SetPlayerMoveState(avatar, PLAYER_MOVE_STATE_NONE);
    PlayerAvatar_ClearSpeedAndFlag2(avatar);
}

BOOL PlayerAvatar_IsBetweenSteps(PlayerAvatar *avatar) {
    int moveState = PlayerAvatar_GetMoveState(avatar);
    int playerMoveState = PlayerAvatar_GetPlayerMoveState(avatar);
    LocalMapObject *mapObject;

    if (moveState == AVATAR_MOVE_STATE_NONE) {
        return TRUE;
    }
    if (moveState == AVATAR_MOVE_STATE_TURNING) {
        return TRUE;
    }
    if (moveState == AVATAR_MOVE_STATE_MOVING) {
        if (playerMoveState == PLAYER_MOVE_STATE_NONE || playerMoveState == PLAYER_MOVE_STATE_END) {
            return TRUE;
        }
        mapObject = PlayerAvatar_GetMapObject(avatar);
        if (MapObject_AreBitsSetForMovementScriptInit(mapObject) == TRUE) {
            return TRUE;
        }
        if (IsWallBumpMovement(MapObject_GetMovementCommand(mapObject)) == TRUE) {
            return TRUE;
        }
    }
    return FALSE;
}

void PlayerAvatar_StopAndFace(PlayerAvatar *avatar, int direction) {
    LocalMapObject *mapObject;
    PlayerAvatar_SetMoveState(avatar, AVATAR_MOVE_STATE_NONE);
    PlayerAvatar_SetPlayerMoveState(avatar, PLAYER_MOVE_STATE_NONE);
    mapObject = PlayerAvatar_GetMapObject(avatar);
    MapObject_SetFacingDirection(mapObject, direction);
    MapObject_SetAnimation(mapObject, 0);
    MapObject_SetPreviousCoordsToCurrent(mapObject);
    MapObject_ClearHeldMovement(mapObject);
    MapObject_SetHeldMovement(mapObject, GetMovementForDirection(direction, MOVEMENT_FACE_UP));
}

BOOL PlayerAvatar_TryForcedMovement(PlayerAvatar *avatar, int value) {
    int kind = PlayerAvatar_GetForcedMovementKind(avatar, value);
    return PlayerAvatar_RunForcedMovement(avatar, kind, value);
}

int PlayerAvatar_GetForcedMovementKind(PlayerAvatar *avatar, int value) {
    int i = 0;
    int behavior = MapObject_GetCurrentMetatileBehavior(PlayerAvatar_GetMapObject(avatar));
    if (PlayerAvatar_IsOnCyclingRoadSlope(avatar, behavior, value) == TRUE) {
        return 2;
    }
    if (PlayerAvatar_CheckFlag1(avatar) == TRUE) {
        return 0;
    }
    do {
        if (gPlayerForcedMovementBehaviors[i].matches((u8)behavior) == TRUE) {
            return gPlayerForcedMovementBehaviors[i].kind;
        }
        i++;
    } while (gPlayerForcedMovementBehaviors[i].matches != NULL);
    return 0;
}

BOOL PlayerAvatar_RunForcedMovement(PlayerAvatar *avatar, int kind, int value) {
    if (gPlayerForcedMovementFuncs[kind](avatar, value) == TRUE) {
        return TRUE;
    }
    return FALSE;
}

BOOL ForcedMovement_None(PlayerAvatar *avatar, int value) {
    PlayerAvatar_EndForcedMovement(avatar);
    return FALSE;
}

BOOL ForcedMovement_Ice(PlayerAvatar *avatar, int value) {
    LocalMapObject *mapObject = PlayerAvatar_GetMapObject(avatar);
    int direction = MapObject_GetNextFacingDirection(mapObject);
    int slope = PlayerAvatar_GetSlopeInDirection(avatar, direction);
    u32 collision = PlayerAvatar_CheckCollision(avatar, mapObject, direction);

    PlayerAvatar_SetForcedMovement(avatar, TRUE);
    if (collision != 0) {
        FieldSystem *fieldSystem = MapObject_GetFieldSystem(mapObject);
        PlayerAvatar_SetFlag1(avatar, TRUE);
        PlayerAvatar_SetMoveState(avatar, AVATAR_MOVE_STATE_NONE);
        if (FieldSystem_TryPushIceBlock(fieldSystem) == FALSE) {
            PlayerAvatar_EndForcedMovement(avatar);
            return FALSE;
        }
        return TRUE;
    }
    if (PlayerAvatar_AdjustSpeedForSlope(avatar, slope) == FALSE) {
        PlayerAvatar_EndForcedMovement(avatar);
        direction = GetOppositeDirection(direction);
        if (PlayerAvatar_CheckCollision(avatar, mapObject, direction) != 0) {
            return FALSE;
        }
        MapObject_SetFlagsBits(mapObject, (MapObjectFlagBits)(MAPOBJECTFLAG_UNK7 | MAPOBJECTFLAG_UNK8));
        PlayerAvatar_SetObjectMovement(avatar, mapObject, GetMovementForDirection(direction, 8));
        PlayerAvatar_SetFlag1(avatar, TRUE);
        PlayerAvatar_SetForcedMovement(avatar, TRUE);
        PlayerAvatar_SetLastInputs(avatar, -1, -1);
    } else {
        MapObject_SetFlagsBits(mapObject, (MapObjectFlagBits)(MAPOBJECTFLAG_UNK7 | MAPOBJECTFLAG_UNK8));
        PlayerAvatar_SlideInDirection(avatar, direction);
    }
    PlayerAvatar_SetMoveState(avatar, AVATAR_MOVE_STATE_MOVING);
    return TRUE;
}

BOOL ForcedMovement_CyclingRoadSlope(PlayerAvatar *avatar, int value) {
    LocalMapObject *mapObject = PlayerAvatar_GetMapObject(avatar);
    if (PlayerAvatar_CheckCollision(avatar, mapObject, DIR_SOUTH) == 0) {
        PlayerAvatar_SetObjectMovement(avatar, mapObject, GetMovementForDirection(DIR_SOUTH, 0x14));
        PlayerAvatar_SetMoveState(avatar, AVATAR_MOVE_STATE_MOVING);
        PlayerAvatar_SetSpeed(avatar, 3);
        return TRUE;
    }
    PlayerAvatar_SetObjectMovement(avatar, mapObject, GetMovementForDirection(DIR_SOUTH, 1));
    PlayerAvatar_ClearSpeedAndFlag2(avatar);
    PlayerAvatar_SetMoveState(avatar, AVATAR_MOVE_STATE_NONE);
    PlayerAvatar_SetFlag2(avatar, FALSE);
    return TRUE;
}

void PlayerAvatar_EndForcedMovement(PlayerAvatar *avatar) {
    if (PlayerAvatar_CheckForcedMovement(avatar) == TRUE) {
        MapObject_ClearFlagsBits(PlayerAvatar_GetMapObject(avatar), (MapObjectFlagBits)(MAPOBJECTFLAG_UNK7 | MAPOBJECTFLAG_UNK8));
        if (!PlayerAvatar_CheckFlag7(avatar)) {
            PlayerAvatar_ClearSpeedAndFlag2(avatar);
        }
        PlayerAvatar_SetForcedMovement(avatar, FALSE);
        PlayerAvatar_SetFlag7(avatar, FALSE);
        PlayerAvatar_SetFlag5(avatar, FALSE);
    }
}

// 0: level ground ahead, 1: uphill, 2: downhill.
int PlayerAvatar_GetSlopeInDirection(PlayerAvatar *avatar, int direction) {
    LocalMapObject *mapObject = PlayerAvatar_GetMapObject(avatar);
    FieldSystem *fieldSystem = MapObject_GetFieldSystem(mapObject);
    VecFx32 original;
    VecFx32 position;
    MapObject_CopyPositionVector(mapObject, &original);
    position = original;
    OffsetPositionInDirection(direction, &position, FX32_ONE * 4);
    if (!FieldSystem_SnapToTerrainHeight(fieldSystem, &position) || original.y == position.y) {
        return 0;
    }
    if (original.y > position.y) {
        return 2;
    }
    return 1;
}

BOOL PlayerAvatar_AdjustSpeedForSlope(PlayerAvatar *avatar, int slope) {
    int speed = PlayerAvatar_GetSpeed(avatar);
    if (slope == 1) {
        speed--;
        if (speed < 0) {
            return FALSE;
        }
    } else if (slope == 2) {
        speed++;
        if (speed > 3) {
            speed = 3;
        }
    }
    PlayerAvatar_SetSpeed(avatar, speed);
    return TRUE;
}

void PlayerAvatar_SlideInDirection(PlayerAvatar *avatar, int direction) {
    LocalMapObject *mapObject = PlayerAvatar_GetMapObject(avatar);
    int movement = MOVEMENT_RUN_UP;
    int command;
    FieldSystem *fieldSystem;
    switch (PlayerAvatar_GetSpeed(avatar)) {
    case 1:
        movement = 0x50;
        break;
    case 2:
        movement = 0x14;
        break;
    case 3:
        movement = 0x14;
        break;
    }
    command = GetMovementForDirection(direction, movement);
    PlayerAvatar_SetObjectMovement(avatar, mapObject, command);
    fieldSystem = MapObject_GetFieldSystem(mapObject);
    if (FollowMon_IsActive(fieldSystem)) {
        int x = MapObject_GetPreviousXCoord(mapObject);
        int z = MapObject_GetPreviousZCoord(mapObject);
        FollowMon_QueuePlayerStep(command, x, z, &fieldSystem->followMon);
    }
}

void PlayerAvatar_MoveByState(PlayerAvatar *avatar, MapLoadManager *mapLoadManager, int direction, u16 keys, u16 heldKeys) {
    int state = PlayerAvatar_GetState(avatar);
    LocalMapObject *mapObject = PlayerAvatar_GetMapObject(avatar);

    switch (state) {
    case PLAYER_STATE_WALKING:
    case PLAYER_STATE_SURFING:
    case PLAYER_STATE_ROCKET:
        PlayerAvatar_MoveOnFoot(avatar, mapObject, mapLoadManager, direction, keys, heldKeys);
        break;
    case PLAYER_STATE_CYCLING:
        PlayerAvatar_MoveOnBike(avatar, mapObject, mapLoadManager, direction, keys, heldKeys);
        break;
    default:
        GF_AssertFail();
        break;
    }
}

void PlayerAvatar_MoveOnFoot(PlayerAvatar *avatar, LocalMapObject *mapObject, MapLoadManager *mapLoadManager, int direction, u16 keys, u16 heldKeys) {
    switch (PlayerAvatar_UpdateFootStepKind(avatar, direction)) {
    case 0:
        PlayerAvatar_FootStand(avatar, mapObject, direction, keys, heldKeys);
        break;
    case 1:
        PlayerAvatar_FootStep(avatar, mapObject, direction, keys, heldKeys);
        break;
    case 2:
        PlayerAvatar_FootTurn(avatar, mapObject, direction, keys, heldKeys);
        break;
    }
}

int PlayerAvatar_UpdateFootStepKind(PlayerAvatar *avatar, int direction) {
    int stepKind = PlayerAvatar_GetFootStepKind(avatar, direction);
    PlayerAvatar_SetMoveState(avatar, FootStepKindToMoveState(stepKind));
    return stepKind;
}

int FootStepKindToMoveState(int stepKind) {
    switch (stepKind) {
    case 0:
        return AVATAR_MOVE_STATE_NONE;
    case 1:
        return AVATAR_MOVE_STATE_MOVING;
    case 2:
        return AVATAR_MOVE_STATE_TURNING;
    default:
        GF_AssertFail();
        return AVATAR_MOVE_STATE_NONE;
    }
}

int PlayerAvatar_AdjustMovementDummy(LocalMapObject *mapObject, u8 behavior, int movement) {
    return movement;
}

// 0: stand, 1: step, 2: turn.
int PlayerAvatar_GetFootStepKind(PlayerAvatar *avatar, int direction) {
    int facing;
    if (direction == -1) {
        PlayerAvatar_SetMoveState(avatar, AVATAR_MOVE_STATE_NONE);
        return 0;
    }
    facing = PlayerAvatar_GetFacingDirection(avatar);
    if (facing != direction) {
        if (PlayerAvatar_GetMoveState(avatar) != AVATAR_MOVE_STATE_MOVING) {
            PlayerAvatar_SetMoveState(avatar, AVATAR_MOVE_STATE_TURNING);
            return 2;
        }
    }
    PlayerAvatar_SetMoveState(avatar, AVATAR_MOVE_STATE_MOVING);
    return 1;
}

void PlayerAvatar_FootStand(PlayerAvatar *avatar, LocalMapObject *mapObject, int direction, u16 keys, u16 heldKeys) {
    int movement = GetMovementForDirection(MapObject_GetFacingDirection(mapObject), MOVEMENT_FACE_UP);
    PlayerAvatar_SetObjectMovement(avatar, mapObject, movement);
}

void PlayerAvatar_FootStep(PlayerAvatar *avatar, LocalMapObject *mapObject, int direction, u16 keys, u16 heldKeys) {
    int blocked = FALSE;
    int movement;
    int collision = PlayerAvatar_CheckCollision(avatar, mapObject, direction);

    if (PlayerAvatar_GetState(avatar) != PLAYER_STATE_SURFING) {
        if (collision & 4) {
            movement = 0x38;
        } else if (collision != 0) {
            movement = 0x1C;
            if (!(collision & 8)) {
                FieldSystem *fieldSystem = MapObject_GetFieldSystem(mapObject);
                if (FieldSystem_IsSaveGymmickTypeEqualTo(fieldSystem, 7)) {
                    int x = MapObject_GetXCoord(mapObject);
                    int dx = GetDeltaXByFacingDirection(direction);
                    int z = MapObject_GetZCoord(mapObject);
                    int dz = GetDeltaYByFacingDirection(direction);
                    FuchsiaGymmick_CheckCollision(fieldSystem, x + dx, z + dz);
                } else {
                    PlaySE(SEQ_SE_DP_WALL_HIT);
                }
            }
            MapObject_SetNextFacingDirection(mapObject, direction);
            blocked = TRUE;
        } else {
            int kind = MOVEMENT_STEP_UP;
            if (PlayerSaveData_CheckRunningShoes(PlayerAvatar_GetPlayerSaveData(avatar)) == TRUE && IsRunButtonHeld(avatar, heldKeys) == TRUE) {
                kind = 0x58;
            }
            movement = PlayerAvatar_AdjustMovementDummy(mapObject, (u8)MapObject_GetCurrentMetatileBehavior(mapObject), kind);
            PlayerAvatar_CountStep(avatar);
            PlayerAvatar_SetFlag6(avatar);
        }
    } else if (collision == 0 || collision == 0x20) {
        movement = PlayerAvatar_AdjustMovementDummy(mapObject, (u8)MapObject_GetCurrentMetatileBehavior(mapObject), MOVEMENT_RUN_UP);
        PlayerAvatar_CountStep(avatar);
        PlayerAvatar_SetFlag6(avatar);
    } else {
        movement = 0x1C;
        if (!(collision & 8)) {
            PlaySE(SEQ_SE_DP_WALL_HIT);
        }
        MapObject_SetNextFacingDirection(mapObject, direction);
    }

    movement = GetMovementForDirection(direction, movement);
    PlayerAvatar_SetObjectMovement(avatar, mapObject, movement);
    if (!blocked) {
        FieldSystem *fieldSystem = MapObject_GetFieldSystem(mapObject);
        int x = MapObject_GetPreviousXCoord(mapObject);
        int z = MapObject_GetPreviousZCoord(mapObject);
        FollowMon_QueuePlayerStep(movement, x, z, &fieldSystem->followMon);
    }
}

void PlayerAvatar_FootTurn(PlayerAvatar *avatar, LocalMapObject *mapObject, int direction, u16 keys, u16 heldKeys) {
    PlayerAvatar_SetObjectMovement(avatar, mapObject, GetMovementForDirection(direction, 0x28));
    MapObject_SetNextFacingDirection(mapObject, direction);
    PlayerAvatar_ResetFootstepToggle(avatar);
    PlayerAvatar_ToggleFootstep(avatar);
}

void PlayerAvatar_MoveOnBike(PlayerAvatar *avatar, LocalMapObject *mapObject, MapLoadManager *mapLoadManager, int direction, u16 keys, u16 heldKeys) {
    PlayerAvatar_DispatchBikeStep(avatar, mapObject, mapLoadManager, direction, keys, heldKeys);
}

int PlayerAvatar_GetBikeMovement(PlayerAvatar *avatar, int direction) {
    int movement = 0x4C;
    switch (PlayerAvatar_GetSpeed(avatar)) {
    case 1:
        movement = MOVEMENT_RUN_UP;
        break;
    case 2:
        movement = 0x50;
        break;
    case 3:
        movement = 0x14;
        break;
    }
    return GetMovementForDirection(direction, movement);
}

BOOL PlayerAvatar_AccelerateBike(PlayerAvatar *avatar) {
    int speed = PlayerAvatar_AddSpeedWithCeiling(avatar, 1, 3);
    if (!PlayerAvatar_CheckFlag2(avatar) && speed >= 2) {
        PlayerAvatar_SetFlag2(avatar, TRUE);
    }
    return speed == 3;
}

BOOL PlayerAvatar_DecelerateBike(PlayerAvatar *avatar) {
    BOOL ret = TRUE;
    int speed = PlayerAvatar_GetSpeed(avatar) - 1;
    if (speed < 0) {
        speed = 0;
        ret = FALSE;
    }
    PlayerAvatar_SetSpeed(avatar, speed);
    if (PlayerAvatar_CheckFlag2(avatar) == TRUE && speed == 0) {
        PlayerAvatar_SetFlag2(avatar, FALSE);
    }
    return ret;
}

void PlayerAvatar_DispatchBikeStep(PlayerAvatar *avatar, LocalMapObject *mapObject, MapLoadManager *mapLoadManager, int direction, u16 keys, u16 heldKeys) {
    switch (PlayerAvatar_UpdateBikeStepKind(avatar, direction)) {
    case 0:
        PlayerAvatar_BikeStop(avatar, mapObject, direction, keys, heldKeys);
        break;
    case 1:
        PlayerAvatar_BikeRide(avatar, mapObject, direction, keys, heldKeys);
        break;
    case 2:
        PlayerAvatar_BikeTurn(avatar, mapObject, direction, keys, heldKeys);
        break;
    case 3:
        PlayerAvatar_BikeCoast(avatar, mapObject, direction, keys, heldKeys);
        break;
    }
}

int PlayerAvatar_UpdateBikeStepKind(PlayerAvatar *avatar, int direction) {
    int stepKind = PlayerAvatar_GetBikeStepKind(avatar, direction);
    PlayerAvatar_SetMoveState(avatar, BikeStepKindToMoveState(stepKind));
    return stepKind;
}

int BikeStepKindToMoveState(int stepKind) {
    switch (stepKind) {
    case 0:
        return AVATAR_MOVE_STATE_NONE;
    case 1:
        return AVATAR_MOVE_STATE_MOVING;
    case 2:
        return AVATAR_MOVE_STATE_TURNING;
    case 3:
        return AVATAR_MOVE_STATE_MOVING;
    default:
        GF_AssertFail();
        return AVATAR_MOVE_STATE_NONE;
    }
}

#pragma force_active on

// 0: stop, 1: ride, 2: turn, 3: coast (still rolling at speed 2+).
int PlayerAvatar_GetBikeStepKind(PlayerAvatar *avatar, int direction) {
    int speed = PlayerAvatar_GetSpeed(avatar);
    int facing;
    if (direction == -1) {
        if (speed < 2) {
            PlayerAvatar_SetMoveState(avatar, AVATAR_MOVE_STATE_NONE);
            return 0;
        }
        PlayerAvatar_SetMoveState(avatar, AVATAR_MOVE_STATE_MOVING);
        return 3;
    }
    facing = PlayerAvatar_GetFacingDirection(avatar);
    if (facing != direction && PlayerAvatar_GetMoveState(avatar) != AVATAR_MOVE_STATE_MOVING) {
        if (speed < 2) {
            PlayerAvatar_SetMoveState(avatar, AVATAR_MOVE_STATE_TURNING);
            return 2;
        }
        PlayerAvatar_SetMoveState(avatar, AVATAR_MOVE_STATE_MOVING);
        return 3;
    }
    PlayerAvatar_SetMoveState(avatar, AVATAR_MOVE_STATE_MOVING);
    return 1;
}

#pragma force_active off

void PlayerAvatar_BikeStop(PlayerAvatar *avatar, LocalMapObject *mapObject, int direction, u16 keys, u16 heldKeys) {
    int movement = GetMovementForDirection(MapObject_GetFacingDirection(mapObject), MOVEMENT_FACE_UP);
    PlayerAvatar_SetObjectMovement(avatar, mapObject, movement);
    PlayerAvatar_ClearSpeedAndFlag2(avatar);
}

void PlayerAvatar_BikeRide(PlayerAvatar *avatar, LocalMapObject *mapObject, int direction, u16 keys, u16 heldKeys) {
    u32 collision = PlayerAvatar_CheckCollision(avatar, mapObject, direction);
    int movement;

    if (collision & 4) {
        movement = GetMovementForDirection(direction, 0x38);
        PlayerAvatar_AccelerateBike(avatar);
        PlayerAvatar_CountStep(avatar);
        PlayerAvatar_SetFlag6(avatar);
    } else if (collision & 0x10) {
        if (PlayerAvatar_GetSpeed(avatar) >= 3) {
            movement = PlayerAvatar_GetBikeMovement(avatar, direction);
            PlayerAvatar_SetFlag6(avatar);
            PlayerAvatar_SetForcedMovement(avatar, TRUE);
        } else {
            movement = GetMovementForDirection(direction, 0x1C);
            if (!(collision & 8)) {
                PlaySE(SEQ_SE_DP_WALL_HIT);
            }
            MapObject_SetNextFacingDirection(mapObject, direction);
            PlayerAvatar_ClearSpeedAndFlag2(avatar);
        }
    } else if (collision & 0x40) {
        int facing = MapObject_GetNextFacingDirection(mapObject);
        movement = GetMovementForDirection(facing, MOVEMENT_FACE_UP);
        MapObject_SetNextFacingDirection(mapObject, facing);
        PlayerAvatar_ClearSpeedAndFlag2(avatar);
        PlayerAvatar_SetLastInputs(avatar, -1, -1);
    } else if (collision != 0) {
        movement = GetMovementForDirection(direction, 0x1C);
        if (!(collision & 8)) {
            PlaySE(SEQ_SE_DP_WALL_HIT);
        }
        MapObject_SetNextFacingDirection(mapObject, direction);
        PlayerAvatar_ClearSpeedAndFlag2(avatar);
    } else {
        movement = PlayerAvatar_GetBikeMovement(avatar, direction);
        PlayerAvatar_AccelerateBike(avatar);
        PlayerAvatar_CountStep(avatar);
        PlayerAvatar_SetFlag6(avatar);
    }
    PlayerAvatar_SetObjectMovement(avatar, mapObject, movement);
}

void PlayerAvatar_BikeTurn(PlayerAvatar *avatar, LocalMapObject *mapObject, int direction, u16 keys, u16 heldKeys) {
    PlayerAvatar_ClearSpeedAndFlag2(avatar);
    PlayerAvatar_SetObjectMovement(avatar, mapObject, GetMovementForDirection(direction, 0x28));
    MapObject_SetNextFacingDirection(mapObject, direction);
    PlayerAvatar_ClearSpeedAndFlag2(avatar);
}

void PlayerAvatar_BikeCoast(PlayerAvatar *avatar, LocalMapObject *mapObject, int direction, u16 keys, u16 heldKeys) {
    int movement;
    BOOL rolling = PlayerAvatar_DecelerateBike(avatar);
    int facing = PlayerAvatar_GetNextFacingDirection(avatar);
    u32 collision = PlayerAvatar_CheckCollision(avatar, mapObject, facing);

    if (collision & 4) {
        movement = GetMovementForDirection(facing, 0x38);
    } else if (collision & 0x40) {
        movement = GetMovementForDirection(facing, MOVEMENT_FACE_UP);
        PlayerAvatar_ClearSpeedAndFlag2(avatar);
    } else if (collision != 0) {
        rolling = FALSE;
        movement = GetMovementForDirection(facing, 0x1C);
        if (!(collision & 8)) {
            PlaySE(SEQ_SE_DP_WALL_HIT);
        }
        MapObject_SetNextFacingDirection(mapObject, facing);
        PlayerAvatar_ClearSpeedAndFlag2(avatar);
    } else {
        movement = PlayerAvatar_GetBikeMovement(avatar, facing);
    }

    if (!rolling) {
        PlayerAvatar_SetFlag2(avatar, FALSE);
    }
    PlayerAvatar_SetObjectMovement(avatar, mapObject, movement);
}

void PlayerAvatar_SetObjectMovement(PlayerAvatar *avatar, LocalMapObject *mapObject, int movement) {
    PlayerAvatar_SetHeldMovement(avatar, movement);
    MapObject_SetHeldMovement(mapObject, movement);
}

// Collision bits: 1 blocked, 2 blocked by the map, 4 ledge, 8 blocked at a warp or door (no bump sound),
// 0x20 water ahead.
u32 PlayerAvatar_CheckCollision(PlayerAvatar *avatar, LocalMapObject *mapObject, int direction) {
    u32 result = 0;
    u32 collision = MapObject_GetCollisionInDirection(avatar, mapObject, direction);

    if (collision & 0xA) {
        result |= 1;
        if (MapObject_IsWarpInDirection(avatar, mapObject, direction)) {
            result |= 8;
        }
    }
    if (collision & 4) {
        result |= 2;
    }
    if (MapObject_IsLedgeInDirection(avatar, mapObject, direction)) {
        result |= 4;
    }
    if (MapObject_IsWaterInDirection(avatar, mapObject, direction) == TRUE) {
        result |= 0x20;
    }
    if (PlayerAvatar_IsBikeBlockedInDirection(avatar, mapObject, direction) == TRUE) {
        result |= 1;
    }
    return result;
}

u32 MapObject_GetCollisionInDirection(PlayerAvatar *avatar, LocalMapObject *mapObject, int direction) {
    int x = MapObject_GetXCoord(mapObject);
    int dx = GetDeltaXByFacingDirection(direction);
    int y = MapObject_GetYCoord(mapObject);
    int z = MapObject_GetZCoord(mapObject);
    int dz = GetDeltaYByFacingDirection(direction);
    VecFx32 position;
    s8 heightFlag;
    u32 result;

    MapObject_CopyPositionVector(mapObject, &position);
    result = 0;
    if (MapObject_IsOutsideMovementRange(mapObject, x + dx, y, z + dz) == TRUE) {
        result |= 1;
    }
    if (FieldSystem_CheckCollisionHeightAndGimmick(MapObject_GetFieldSystem(mapObject), &position, x + dx, z + dz, (u32 *)&heightFlag) == TRUE) {
        result |= 2;
        if (heightFlag != 0) {
            result |= 8;
        }
    }
    if (MapObject_IsMoveBlockedByBehavior(mapObject, x + dx, z + dz, direction) == TRUE) {
        result |= 2;
    }
    if (MapObject_IsTileOccupiedInLink(mapObject, x + dx, y, z + dz) == TRUE) {
        result |= 4;
    }
    return result;
}

BOOL MapObject_IsLedgeInDirection(PlayerAvatar *avatar, LocalMapObject *mapObject, int direction) {
    FieldSystem *fieldSystem;
    int x;
    int z;
    u8 behavior;

    if (direction != -1) {
        fieldSystem = MapObject_GetFieldSystem(mapObject);
        x = MapObject_GetXCoord(mapObject) + GetDeltaXByFacingDirection(direction);
        z = MapObject_GetZCoord(mapObject) + GetDeltaYByFacingDirection(direction);
        behavior = GetMetatileBehavior(fieldSystem, x, z);
        switch (direction) {
        case DIR_NORTH:
            if (MetatileBehavior_IsJumpNorth(behavior) == TRUE) {
                return TRUE;
            }
            break;
        case DIR_SOUTH:
            if (MetatileBehavior_IsJumpSouth(behavior) == TRUE) {
                return TRUE;
            }
            break;
        case DIR_WEST:
            if (MetatileBehavior_IsJumpWest(behavior) == TRUE) {
                return TRUE;
            }
            break;
        case DIR_EAST:
            if (MetatileBehavior_IsJumpEast(behavior) == TRUE) {
                return TRUE;
            }
            break;
        }
    }
    return FALSE;
}

BOOL MapObject_IsWarpInDirection(PlayerAvatar *avatar, LocalMapObject *mapObject, int direction) {
    FieldSystem *fieldSystem;
    int x;
    int z;
    u8 behavior;

    if (direction != -1) {
        fieldSystem = MapObject_GetFieldSystem(mapObject);
        x = MapObject_GetXCoord(mapObject);
        z = MapObject_GetZCoord(mapObject);
        behavior = GetMetatileBehavior(fieldSystem, x, z);
        switch (direction) { // falls through
        case DIR_NORTH:
            if (MetatileBehavior_IsWarpEntranceNorth(behavior) == TRUE) {
                return TRUE;
            }
        case DIR_SOUTH:
            if (MetatileBehavior_IsWarpEntranceSouth(behavior) == TRUE) {
                return TRUE;
            }
        case DIR_WEST:
            if (MetatileBehavior_IsWarpEntranceWest(behavior) == TRUE) {
                return TRUE;
            }
        case DIR_EAST:
            if (MetatileBehavior_IsWarpEntranceEast(behavior) == TRUE) {
                return TRUE;
            }
        }
        x += GetDeltaXByFacingDirection(direction);
        z += GetDeltaYByFacingDirection(direction);
        if (MetatileBehavior_IsDoor(GetMetatileBehavior(fieldSystem, x, z)) == TRUE) {
            return TRUE;
        }
    }
    return FALSE;
}

BOOL MapObject_IsWaterInDirection(PlayerAvatar *avatar, LocalMapObject *mapObject, int direction) {
    FieldSystem *fieldSystem;
    int x;
    int dx;
    int z;
    int dz;
    u8 behavior;
    if (direction != -1) {
        fieldSystem = MapObject_GetFieldSystem(mapObject);
        x = MapObject_GetXCoord(mapObject);
        dx = GetDeltaXByFacingDirection(direction);
        z = MapObject_GetZCoord(mapObject);
        dz = GetDeltaYByFacingDirection(direction);
        behavior = GetMetatileBehavior(fieldSystem, x + dx, z + dz);
        if (MapObject_CheckWaterBehavior(mapObject, behavior) != FALSE) {
            return TRUE;
        }
    }
    return FALSE;
}

BOOL PlayerAvatar_IsBikeBlockedInDirection(PlayerAvatar *avatar, LocalMapObject *mapObject, int direction) {
    FieldSystem *fieldSystem;
    int x;
    int z;
    int dx;
    int dz;
    u32 behavior;

    if (direction != -1 && PlayerAvatar_GetState(avatar) == PLAYER_STATE_CYCLING) {
        fieldSystem = MapObject_GetFieldSystem(mapObject);
        x = MapObject_GetXCoord(mapObject);
        dx = GetDeltaXByFacingDirection(direction);
        z = MapObject_GetZCoord(mapObject);
        dz = GetDeltaYByFacingDirection(direction);
        behavior = GetMetatileBehavior(fieldSystem, x + dx, z + dz);
        if (MapObject_CheckWaterBehavior(mapObject, behavior)) {
            return TRUE;
        }
        if (MapObject_CheckSnowBehavior(mapObject, behavior)) {
            return TRUE;
        }
        if (MetatileBehavior_IsVeryTallGrass_Int(behavior)) {
            return TRUE;
        }
        if (MetatileBehavior_IsMud_Int(behavior)) {
            return TRUE;
        }
    }
    return FALSE;
}

int PlayerAvatar_GetInputDirection(PlayerAvatar *avatar, u16 keys, u16 heldKeys) {
    return PlayerAvatar_ChooseInputDirection(avatar, keys, heldKeys);
}

int GetHorizontalDirectionFromKeys(int keys) {
    if (keys & PAD_KEY_LEFT) {
        return DIR_WEST;
    }
    if (keys & PAD_KEY_RIGHT) {
        return DIR_EAST;
    }
    return -1;
}

int GetVerticalDirectionFromKeys(int keys) {
    if (keys & PAD_KEY_UP) {
        return DIR_NORTH;
    }
    if (keys & PAD_KEY_DOWN) {
        return DIR_SOUTH;
    }
    return -1;
}

// With two directions held, keeps the current one until the other changes.
int PlayerAvatar_ChooseInputDirection(PlayerAvatar *avatar, int keys, int heldKeys) {
    int horizontal = GetHorizontalDirectionFromKeys(heldKeys);
    int vertical = GetVerticalDirectionFromKeys(heldKeys);
    int facing;
    int lastHorizontal;
    int lastVertical;

    if (horizontal == -1) {
        return vertical;
    }
    if (vertical == -1) {
        return horizontal;
    }

    facing = PlayerAvatar_GetNextFacingDirection(avatar);
    lastHorizontal = PlayerAvatar_GetLastHorizontalInput(avatar);
    lastVertical = PlayerAvatar_GetLastVerticalInput(avatar);
    if (facing != -1) {
        if (horizontal == lastHorizontal && vertical == lastVertical) {
            return facing;
        }
        if (vertical != lastVertical) {
            return vertical;
        }
        return horizontal;
    }
    return vertical;
}

BOOL PlayerAvatar_IsReadyOrBumpingWall(PlayerAvatar *avatar) {
    LocalMapObject *mapObject = PlayerAvatar_GetMapObject(avatar);
    if (MapObject_AreBitsSetForMovementScriptInit(mapObject) == TRUE) {
        return TRUE;
    }
    if (IsWallBumpMovement(MapObject_GetMovementCommand(mapObject)) == TRUE) {
        return TRUE;
    }
    return FALSE;
}

BOOL IsWallBumpMovement(u32 movement) {
    switch (movement) {
    case 0x1C:
    case 0x1D:
    case 0x1E:
    case 0x1F:
        return TRUE;
    default:
        return FALSE;
    }
}

BOOL IsRunButtonHeld(PlayerAvatar *avatar, int heldKeys) {
    return (heldKeys & PAD_BUTTON_B) != 0;
}

BOOL PlayerAvatar_IsRunning(PlayerAvatar *avatar) {
    switch (PlayerAvatar_GetHeldMovement(avatar)) {
    case 0x58:
    case 0x59:
    case 0x5A:
    case 0x5B:
        return TRUE;
    default:
        return FALSE;
    }
}

int PlayerAvatar_GetMoveStateForDirection(PlayerAvatar *avatar, int direction) {
    int moveState;
    switch (PlayerAvatar_GetState(avatar)) {
    case PLAYER_STATE_WALKING:
    case PLAYER_STATE_SURFING:
    case PLAYER_STATE_ROCKET:
        moveState = FootStepKindToMoveState(PlayerAvatar_GetFootStepKind(avatar, direction));
        break;
    case PLAYER_STATE_CYCLING:
        moveState = BikeStepKindToMoveState(PlayerAvatar_GetBikeStepKind(avatar, direction));
        break;
    default:
        moveState = AVATAR_MOVE_STATE_NONE;
        GF_AssertFail();
        break;
    }
    return moveState;
}

#pragma force_active on

u32 PlayerAvatar_GetMovementFromInput(PlayerAvatar *avatar, int keys, int heldKeys, u32 speed, int allowRun, int collision) {
    struct {
        volatile int direction;
    } input;

    int moveState;
    int movement;

    input.direction = PlayerAvatar_ChooseInputDirection(avatar, keys, heldKeys);
    moveState = PlayerAvatar_GetMoveStateForDirection(avatar, input.direction);
    PlayerAvatar_SetMoveState(avatar, moveState);
    if (moveState == AVATAR_MOVE_STATE_NONE) {
        return GetMovementForDirection(PlayerAvatar_GetFacingDirection(avatar), MOVEMENT_FACE_UP);
    }
    if (moveState == AVATAR_MOVE_STATE_TURNING) {
        return GetMovementForDirection(input.direction, 0x28);
    }
    PlayerAvatar_GetMapObject(avatar);
    movement = 4;
    {
        u32 blocked = collision;
        if (blocked & 4) {
            movement = 0x38;
        } else if (blocked) {
            movement = 0x1C;
            if (!(blocked & 8)) {
                PlaySE(SEQ_SE_DP_WALL_HIT);
            }
        } else {
            switch (speed) {
            case 0:
                movement = 4;
                break;
            case 1:
                movement = 8;
                break;
            case 2:
                movement = MOVEMENT_STEP_UP;
                break;
            case 3:
                movement = 0x4C;
                break;
            case 4:
                movement = MOVEMENT_RUN_UP;
                break;
            case 5:
                movement = 0x14;
                break;
            default:
                movement = 4;
                break;
            }
            if (allowRun == TRUE && IsRunButtonHeld(avatar, heldKeys) == TRUE) {
                movement = 0x58;
            }
        }
    }
    return GetMovementForDirection(input.direction, movement);
}

#pragma force_active off

BOOL PlayerAvatar_IsReadyForMovement(PlayerAvatar *avatar) {
    return MapObject_AreBitsSetForMovementScriptInit(PlayerAvatar_GetMapObject(avatar));
}

void PlayerAvatar_SetMovement(PlayerAvatar *avatar, int movement) {
    LocalMapObject *mapObject = PlayerAvatar_GetMapObject(avatar);
    PlayerAvatar_SetObjectMovement(avatar, mapObject, movement);
}

void PlayerAvatar_GetMetatileBehaviorInDirection(PlayerAvatar *avatar, int direction) {
    MapObject_GetMetatileBehaviorInDirection(PlayerAvatar_GetMapObject(avatar), direction);
}

void PlayerAvatar_GetCoordsInDirection(PlayerAvatar *avatar, int direction, int *x, int *z) {
    *x = PlayerAvatar_GetXCoord(avatar) + GetDeltaXByFacingDirection(direction);
    *z = PlayerAvatar_GetZCoord(avatar) + GetDeltaYByFacingDirection(direction);
}

void PlayerAvatar_GetCoordsInFront(PlayerAvatar *avatar, int *x, int *z) {
    int direction = PlayerAvatar_GetFacingDirection(avatar);
    PlayerAvatar_GetCoordsInDirection(avatar, direction, x, z);
}

void PlayerAvatar_CountStep(PlayerAvatar *avatar) {
    FieldSystem *fieldSystem = MapObject_GetFieldSystem(PlayerAvatar_GetMapObject(avatar));
    GameStats *stats = Save_GameStats_Get(fieldSystem->saveData);
    GameStats_Inc(stats, GAME_STAT_STEPS_WALKED);
    if (PlayerAvatar_GetState(fieldSystem->playerAvatar) == PLAYER_STATE_CYCLING) {
        GameStats_Inc(stats, GAME_STAT_STEPS_BIKED);
    }
}

BOOL PlayerAvatar_IsOnCyclingRoadSlope(PlayerAvatar *avatar, int behavior, int value) {
    if (value != -1) {
        return FALSE;
    }
    if (PlayerAvatar_GetState(avatar) != PLAYER_STATE_CYCLING) {
        return FALSE;
    }
    if (!PlayerAvatar_CheckBikeStateLocked(avatar)) {
        return FALSE;
    }
    if (MapObject_IsOnBridge(PlayerAvatar_GetMapObject(avatar), behavior) == TRUE) {
        return TRUE;
    }
    return FALSE;
}
