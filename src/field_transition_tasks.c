#include "field_transition_tasks.h"

#include "global.h"

#include "field/fieldmap.h"

#include "camera.h"
#include "field_system.h"
#include "follow_mon.h"
#include "heap.h"
#include "map_object.h"
#include "metatile_behavior.h"
#include "overlay_01.h"
#include "overlay_01_021E90C0.h"
#include "overlay_01_021F1AFC.h"
#include "player_avatar.h"
#include "screen_fade.h"
#include "task.h"
#include "unk_02054648.h"
#include "unk_02055BF0.h"
#include "unk_02062108.h"

// Map enter routines (sMapEnterRoutines) and the player/camera placement run before them (_020FC76C), indexed by
// FieldTransitionEnvironment.transitionNo, plus the Wi-Fi Plaza launch task (ScrCmd_663).

typedef struct WiFiPlazaTaskEnv {
    u16 state;
    u16 parameter;
    void *fieldArg; // &fieldSystem->unkB4, passed to the app as its args
    SaveData *saveData;
    u32 value;
} WiFiPlazaTaskEnv;

// Defined in field overlay / Wi-Fi Plaza sources that have no header yet.
BOOL ov01_021E971C(FieldSystem *fieldSystem, FieldEnvSubUnk18 *work, u8 direction);
BOOL ov01_021E9C40(TaskManager *taskMan);
int sub_02054940(FieldSystem *fieldSystem, int y, fx32 x, fx32 z, u8 *outKind);
BOOL WiFiPlaza_Init(OverlayManager *man, int *state);
BOOL WiFiPlaza_Main(OverlayManager *man, int *state);
BOOL WiFiPlaza_Exit(OverlayManager *man, int *state);

FS_EXTERN_OVERLAY(OVY_45);

BOOL Task_WiFiPlaza(TaskManager *taskMan);

BOOL FieldTransition_EnterThroughSideDoor(TaskManager *taskMan) {
    FieldSystem *fieldSystem = TaskManager_GetFieldSystem(taskMan);
    FieldTransitionEnvironment *env = TaskManager_GetEnvironment(taskMan);
    switch (env->transitionState) {
    case 0: {
        int x;
        env->unk18 = ov01_021E90C0();
        x = PlayerAvatar_GetXCoord(fieldSystem->playerAvatar);
        ov01_021E90DC(x, PlayerAvatar_GetZCoord(fieldSystem->playerAvatar), env->unk18);
        env->transitionState++;
        break;
    }
    case 1: {
        FieldEnvSubUnk18 *work = env->unk18;
        int direction = PlayerAvatar_GetFacingDirection(fieldSystem->playerAvatar);
        if (ov01_021E971C(fieldSystem, work, direction)) {
            ov01_021E90D4(env->unk18);
            env->transitionState++;
        }
        break;
    }
    case 2:
        return TRUE;
    }
    return FALSE;
}

BOOL FieldTransition_EnterFromWarpStairs(TaskManager *taskMan) {
    FieldSystem *fieldSystem = TaskManager_GetFieldSystem(taskMan);
    FieldTransitionEnvironment *env = TaskManager_GetEnvironment(taskMan);
    switch (env->transitionState) {
    case 0: {
        LocalMapObject *object;
        int direction;
        FieldMap_FadeScreen(1);
        object = PlayerAvatar_GetMapObject(fieldSystem->playerAvatar);
        direction = PlayerAvatar_GetFacingDirection(fieldSystem->playerAvatar);
        if (direction == DIR_WEST) {
            MapObject_SetHeldMovement(object, 10);
        } else if (direction == DIR_EAST) {
            MapObject_SetHeldMovement(object, 11);
        } else {
            GF_ASSERT(FALSE);
        }
        env->transitionState++;
        break;
    }
    case 1: {
        LocalMapObject *object = PlayerAvatar_GetMapObject(fieldSystem->playerAvatar);
        if (MapObject_IsMovementPaused(object) == TRUE) {
            MapObject_ClearHeldMovementIfActive(object);
            if (FollowMon_IsActive(fieldSystem)) {
                ov01_02205790(fieldSystem, (u8)PlayerAvatar_GetFacingDirection(fieldSystem->playerAvatar));
            }
            env->transitionState++;
        }
        break;
    }
    case 2:
        if (IsPaletteFadeFinished()) {
            env->transitionState++;
        }
        break;
    case 3:
        return TRUE;
    }
    return FALSE;
}

BOOL FieldTransition_EnterWithDoorExit(TaskManager *taskMan) {
    FieldSystem *fieldSystem = TaskManager_GetFieldSystem(taskMan);
    FieldTransitionEnvironment *env = TaskManager_GetEnvironment(taskMan);
    PlayerAvatar_GetMapObject(fieldSystem->playerAvatar);
    switch (env->transitionState) {
    case 0: {
        FieldEnvSubUnk18 *work;
        int x;
        env->unk18 = ov01_021E90C0();
        work = env->unk18;
        x = PlayerAvatar_GetXCoord(fieldSystem->playerAvatar);
        ov01_021E90DC(x, PlayerAvatar_GetZCoord(fieldSystem->playerAvatar), work);
        TaskManager_Call(taskMan, ov01_021E9C40, work);
        env->transitionState++;
        break;
    }
    case 1:
        ov01_021E90D4(env->unk18);
        return TRUE;
    }
    return FALSE;
}

// The player rises 32 units over 16 frames (placed below the floor by FieldTransition_PlaceBelowFloor).
BOOL FieldTransition_EnterRising(TaskManager *taskMan) {
    FieldSystem *fieldSystem = TaskManager_GetFieldSystem(taskMan);
    FieldTransitionEnvironment *env = TaskManager_GetEnvironment(taskMan);
    switch (env->transitionState) {
    case 0:
        env->unk18 = Heap_AllocAtEnd(HEAP_ID_FIELD2, 8);
        *(u32 *)env->unk18 = 0;
        FieldMap_FadeScreen(1);
        env->transitionState++;
        break;
    case 1: {
        LocalMapObject *object = PlayerAvatar_GetMapObject(fieldSystem->playerAvatar);
        int *counter = (int *)env->unk18;
        VecFx32 position;
        (*counter)++;
        MapObject_CopyPositionVector(object, &position);
        position.y += 2 << 12;
        MapObject_SetPositionVector(object, &position);
        if (*counter >= 16) {
            PlayerAvatar_ToggleAutomaticHeightUpdatingImmediate(fieldSystem->playerAvatar, TRUE);
            env->transitionState++;
        }
        break;
    }
    case 2:
        Field_PlayerAvatar_OrrTransitionFlags(fieldSystem->playerAvatar, 1);
        Field_PlayerAvatar_ApplyTransitionFlags(fieldSystem->playerAvatar);
        env->transitionState++;
        break;
    case 3: {
        LocalMapObject *object = PlayerAvatar_GetMapObject(fieldSystem->playerAvatar);
        if (MapObject_AreBitsSetForMovementScriptInit(object)) {
            MapObject_SetHeldMovement(object, 12);
            env->transitionState++;
        }
        break;
    }
    case 4:
        if (MapObject_AreBitsSetForMovementScriptInit(PlayerAvatar_GetMapObject(fieldSystem->playerAvatar)) && IsPaletteFadeFinished()) {
            if (FollowMon_IsActive(fieldSystem)) {
                ov01_02205790(fieldSystem, 0);
                sub_0205FC94(FollowMon_GetMapObject(fieldSystem), 0x30);
                sub_02069DC8(FollowMon_GetMapObject(fieldSystem), TRUE);
            }
            env->transitionState++;
        }
        break;
    case 5:
        Heap_Free(env->unk18);
        return TRUE;
    }
    return FALSE;
}

// The player sinks 32 units over 16 frames (placed above the floor by FieldTransition_PlaceAboveFloor).
BOOL FieldTransition_EnterSinking(TaskManager *taskMan) {
    FieldSystem *fieldSystem = TaskManager_GetFieldSystem(taskMan);
    FieldTransitionEnvironment *env = TaskManager_GetEnvironment(taskMan);
    switch (env->transitionState) {
    case 0:
        env->unk18 = Heap_AllocAtEnd(HEAP_ID_FIELD2, 8);
        *(u32 *)env->unk18 = 0;
        FieldMap_FadeScreen(1);
        env->transitionState++;
        break;
    case 1: {
        LocalMapObject *object = PlayerAvatar_GetMapObject(fieldSystem->playerAvatar);
        int *counter = (int *)env->unk18;
        VecFx32 position;
        (*counter)++;
        MapObject_CopyPositionVector(object, &position);
        position.y -= 2 << 12;
        MapObject_SetPositionVector(object, &position);
        if (*counter >= 16) {
            PlayerAvatar_ToggleAutomaticHeightUpdatingImmediate(fieldSystem->playerAvatar, TRUE);
            env->transitionState++;
        }
        break;
    }
    case 2:
        Field_PlayerAvatar_OrrTransitionFlags(fieldSystem->playerAvatar, 1);
        Field_PlayerAvatar_ApplyTransitionFlags(fieldSystem->playerAvatar);
        env->transitionState++;
        break;
    case 3: {
        LocalMapObject *object = PlayerAvatar_GetMapObject(fieldSystem->playerAvatar);
        if (MapObject_AreBitsSetForMovementScriptInit(object)) {
            MapObject_SetHeldMovement(object, 13);
            env->transitionState++;
        }
        break;
    }
    case 4:
        if (MapObject_AreBitsSetForMovementScriptInit(PlayerAvatar_GetMapObject(fieldSystem->playerAvatar)) && IsPaletteFadeFinished()) {
            if (FollowMon_IsActive(fieldSystem)) {
                ov01_02205790(fieldSystem, 1);
                sub_0205FC94(FollowMon_GetMapObject(fieldSystem), 0x30);
                sub_02069DC8(FollowMon_GetMapObject(fieldSystem), TRUE);
            }
            env->transitionState++;
        }
        break;
    case 5:
        Heap_Free(env->unk18);
        return TRUE;
    }
    return FALSE;
}

// Moves the player one tile back against its facing direction (east or west), keeping the camera offset;
// FieldTransition_EnterThroughSideDoor then plays the door animation.
void FieldTransition_PlaceInsideSideDoor(FieldSystem *fieldSystem) {
    const VecFx32 *cameraTarget;
    VecFx32 playerPosition;
    VecFx32 delta;
    VecFx32 oldLookAt;
    VecFx32 lookAt;
    int direction;

    cameraTarget = Camera_GetCurrentTarget(fieldSystem->camera);
    lookAt = Camera_GetLookAtCamTarget(fieldSystem->camera);
    oldLookAt = lookAt;
    VEC_Subtract(&oldLookAt, cameraTarget, &delta);
    direction = PlayerAvatar_GetFacingDirection(fieldSystem->playerAvatar);
    PlayerAvatar_CopyPositionVector(fieldSystem->playerAvatar, &playerPosition);
    if (direction == DIR_EAST) {
        playerPosition.x -= FX32_ONE * 16;
    } else {
        playerPosition.x += FX32_ONE * 16;
    }
    playerPosition.y = sub_02054940(fieldSystem, playerPosition.y, playerPosition.x, playerPosition.z, NULL);
    sub_0205C810(fieldSystem->playerAvatar, &playerPosition, direction);
    Camera_SetLookAtTargetAndRecalcPos(PlayerAvatar_GetPositionVector(fieldSystem->playerAvatar), fieldSystem->camera);
    Camera_SetFixedTarget(PlayerAvatar_GetPositionVector(fieldSystem->playerAvatar), fieldSystem->camera);
    Camera_OffsetLookAtPosAndTarget(&delta, fieldSystem->camera);
}

// Moves the player off the warp stairs it arrived on (east stairs: one tile east, facing west; west stairs: the
// opposite), keeping the camera offset; the following Pokemon faces the same way.
void FieldTransition_PlaceOffWarpStairs(FieldSystem *fieldSystem) {
    const VecFx32 *cameraTarget;
    VecFx32 playerPosition;
    VecFx32 delta;
    VecFx32 oldLookAt;
    VecFx32 lookAt;
    int direction;
    int x;
    u8 behavior;

    cameraTarget = Camera_GetCurrentTarget(fieldSystem->camera);
    lookAt = Camera_GetLookAtCamTarget(fieldSystem->camera);
    oldLookAt = lookAt;
    VEC_Subtract(&oldLookAt, cameraTarget, &delta);
    direction = PlayerAvatar_GetFacingDirection(fieldSystem->playerAvatar);
    PlayerAvatar_CopyPositionVector(fieldSystem->playerAvatar, &playerPosition);
    x = PlayerAvatar_GetXCoord(fieldSystem->playerAvatar);
    behavior = GetMetatileBehavior(fieldSystem, x, PlayerAvatar_GetZCoord(fieldSystem->playerAvatar));
    if (MetatileBehavior_IsWarpStairsEast(behavior)) {
        playerPosition.x += FX32_ONE * 16;
        direction = DIR_WEST;
    } else if (MetatileBehavior_IsWarpStairsWest(behavior)) {
        playerPosition.x -= FX32_ONE * 16;
        direction = DIR_EAST;
    }
    playerPosition.y = sub_02054940(fieldSystem, playerPosition.y, playerPosition.x, playerPosition.z, NULL);
    sub_0205C810(fieldSystem->playerAvatar, &playerPosition, direction);
    Camera_SetLookAtTargetAndRecalcPos(PlayerAvatar_GetPositionVector(fieldSystem->playerAvatar), fieldSystem->camera);
    Camera_SetFixedTarget(PlayerAvatar_GetPositionVector(fieldSystem->playerAvatar), fieldSystem->camera);
    Camera_OffsetLookAtPosAndTarget(&delta, fieldSystem->camera);
    if (FollowMon_IsActive(fieldSystem)) {
        MapObject_SetFacingDirectionDirect(FollowMon_GetMapObject(fieldSystem), direction);
    }
}

static const OverlayManagerTemplate sOverlayTemplate_WiFiPlaza = { WiFiPlaza_Init, WiFiPlaza_Main, WiFiPlaza_Exit, FS_OVERLAY_ID(OVY_45) };

void FieldTransition_PlaceBelowFloor(FieldSystem *fieldSystem) {
    VecFx32 position;
    VecFx32 delta;
    const VecFx32 *currentTarget = Camera_GetCurrentTarget(fieldSystem->camera);
    VecFx32 lookAt = Camera_GetLookAtCamTarget(fieldSystem->camera);
    LocalMapObject *object;
    VEC_Subtract(&lookAt, currentTarget, &delta);
    object = PlayerAvatar_GetMapObject(fieldSystem->playerAvatar);
    PlayerAvatar_CopyPositionVector(fieldSystem->playerAvatar, &position);
    PlayerAvatar_ToggleAutomaticHeightUpdating(fieldSystem->playerAvatar, FALSE);
    position.y -= 2 << 16;
    sub_0205C810(fieldSystem->playerAvatar, &position, 0);
    Camera_SetLookAtTargetAndRecalcPos(PlayerAvatar_GetPositionVector(fieldSystem->playerAvatar), fieldSystem->camera);
    Camera_SetFixedTarget(PlayerAvatar_GetPositionVector(fieldSystem->playerAvatar), fieldSystem->camera);
    Camera_OffsetLookAtPosAndTarget(&delta, fieldSystem->camera);
    MapObject_ClearHeldMovementIfActive(object);
    Field_PlayerAvatar_OrrTransitionFlags(fieldSystem->playerAvatar, 2 << 8);
    Field_PlayerAvatar_ApplyTransitionFlags(fieldSystem->playerAvatar);
    sub_0205F328(object, 0);
}

void FieldTransition_PlaceAboveFloor(FieldSystem *fieldSystem) {
    VecFx32 position;
    VecFx32 delta;
    const VecFx32 *currentTarget = Camera_GetCurrentTarget(fieldSystem->camera);
    VecFx32 lookAt = Camera_GetLookAtCamTarget(fieldSystem->camera);
    LocalMapObject *object;
    VEC_Subtract(&lookAt, currentTarget, &delta);
    object = PlayerAvatar_GetMapObject(fieldSystem->playerAvatar);
    PlayerAvatar_CopyPositionVector(fieldSystem->playerAvatar, &position);
    PlayerAvatar_ToggleAutomaticHeightUpdating(fieldSystem->playerAvatar, FALSE);
    position.y += 2 << 16;
    sub_0205C810(fieldSystem->playerAvatar, &position, 0);
    Camera_SetLookAtTargetAndRecalcPos(PlayerAvatar_GetPositionVector(fieldSystem->playerAvatar), fieldSystem->camera);
    Camera_SetFixedTarget(PlayerAvatar_GetPositionVector(fieldSystem->playerAvatar), fieldSystem->camera);
    Camera_OffsetLookAtPosAndTarget(&delta, fieldSystem->camera);
    MapObject_ClearHeldMovementIfActive(object);
    Field_PlayerAvatar_OrrTransitionFlags(fieldSystem->playerAvatar, 2 << 8);
    Field_PlayerAvatar_ApplyTransitionFlags(fieldSystem->playerAvatar);
    sub_0205F328(object, 0);
}

void CallTask_WiFiPlaza(TaskManager *taskMan, u16 parameter) {
    WiFiPlazaTaskEnv *env = Heap_AllocAtEnd(HEAP_ID_FIELD2, sizeof(WiFiPlazaTaskEnv));
    memset(env, 0, sizeof(WiFiPlazaTaskEnv));
    env->parameter = parameter;
    TaskManager_Call(taskMan, Task_WiFiPlaza, env);
}

BOOL Task_WiFiPlaza(TaskManager *taskMan) {
    FieldSystem *fieldSystem = TaskManager_GetFieldSystem(taskMan);
    WiFiPlazaTaskEnv *env = TaskManager_GetEnvironment(taskMan);
    switch (env->state) {
    case 0:
        env->saveData = fieldSystem->saveData;
        env->value = env->parameter;
        env->fieldArg = &fieldSystem->unkB4;
        CallApplicationAsTask(taskMan, &sOverlayTemplate_WiFiPlaza, &env->fieldArg);
        env->state++;
        break;
    case 1:
        Heap_Free(env);
        return TRUE;
    }
    return FALSE;
}
