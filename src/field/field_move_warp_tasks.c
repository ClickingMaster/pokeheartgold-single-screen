#include "global.h"

#include "constants/global_fieldmap.h"
#include "constants/pokemon.h"
#include "constants/sndseq.h"

#include "field/field_3d_object_task.h"
#include "field/field_camera_zoom.h"
#include "field/field_follow_mon.h"

#include "field_system.h"
#include "field_warp_tasks.h"
#include "follow_mon.h"
#include "heap.h"
#include "map_object.h"
#include "map_object_movement_commands.h"
#include "overlay_02.h"
#include "player_avatar.h"
#include "pokemon.h"
#include "save_local_field_data.h"
#include "screen_fade.h"
#include "sound_chatot.h"
#include "sound_playback.h"
#include "spawn_warp.h"
#include "task.h"

// The field animations of the warping field moves: Escape Rope, Dig and Teleport take the player (and the following
// Pokemon) away, spinning while the camera zooms in and the screen fades out, then warp; the arrival task plays the
// spin the other way round after the warp. Dig and Teleport either let the following Pokemon perform the move (when it
// is the Pokemon that knows it and walks next to the player) or show the Pokemon's cut-in first.

// Return values of the state functions.
enum FieldMoveStepResult {
    FIELD_MOVE_STEP_WAIT,  // run the same state again next frame
    FIELD_MOVE_STEP_NEXT,  // run the next state now
    FIELD_MOVE_STEP_DONE,  // the task is over (warp started): free the environment
};

// What JumpTask_FieldMoveWarp is told about the warp, and which animation the arrival task plays.
enum FieldMoveWarpKind {
    FIELD_MOVE_WARP_ESCAPE_ROPE,
    FIELD_MOVE_WARP_DIG,
    FIELD_MOVE_WARP_TELEPORT,
};

struct FieldMoveTaskEnvironment {
    int state;
    int spinCount;
    BOOL withFollowMon; // Escape Rope: the following Pokemon spins along; Dig/Teleport: it performs the move
    int kind;           // enum FieldMoveWarpKind
    EventObjectMovementMan *playerMovement;
    EventObjectMovementMan *followMonMovement;
    void *cutIn; // FieldMoveCutIn_Start
    SysTask *cameraZoom;
    LocalMapObject *playerObject;
    FieldSystem *fieldSystem;
    Pokemon *mon;
    Field3dObjectTask *digEffect;
};

typedef int (*FieldMoveStepFunc)(TaskManager *taskManager, FieldSystem *fieldSystem, FieldMoveTaskEnvironment *env);

// state tables and movement scripts (asm data)
extern const FieldMoveStepFunc sFieldMoveArrivalSteps[]; // arrival
extern const FieldMoveStepFunc sEscapeRopeSteps[]; // Escape Rope
extern const FieldMoveStepFunc sDigWithFollowMonSteps[]; // Dig by the following Pokemon
extern const FieldMoveStepFunc sTeleportSteps[]; // Teleport after the cut-in
extern const FieldMoveStepFunc sTeleportWithFollowMonSteps[]; // Teleport by the following Pokemon
extern const FieldMoveStepFunc sDigSteps[]; // Dig after the cut-in
extern const MovementScriptCommand sTeleportFollowMonSpinMovement[];
extern const MovementScriptCommand sEscapeRopeSpinMovement[];
extern const MovementScriptCommand sDigHoleMovement[];
extern const MovementScriptCommand sDigSinkMovement[];
extern const MovementScriptCommand sEscapeRopeSpinStartMovement[];
extern const MovementScriptCommand sArrivalSpinMovement[];

FieldMoveTaskEnvironment *FieldMoveTaskEnvironment_New(enum HeapID heapID, u32 size);

// state functions (referenced by the state tables)
int FieldEscapeRope_SpinStart(TaskManager *taskManager, FieldSystem *fieldSystem, FieldMoveTaskEnvironment *env);
int FieldEscapeRope_SpinAndFadeOut(TaskManager *taskManager, FieldSystem *fieldSystem, FieldMoveTaskEnvironment *env);
int FieldEscapeRope_WaitFadeOut(TaskManager *taskManager, FieldSystem *fieldSystem, FieldMoveTaskEnvironment *env);
int FieldEscapeRope_Warp(TaskManager *taskManager, FieldSystem *fieldSystem, FieldMoveTaskEnvironment *env);
int FieldMoveArrival_FadeIn(TaskManager *taskManager, FieldSystem *fieldSystem, FieldMoveTaskEnvironment *env);
int FieldMoveArrival_WaitFadeIn(TaskManager *taskManager, FieldSystem *fieldSystem, FieldMoveTaskEnvironment *env);
int FieldMoveArrival_Spin(TaskManager *taskManager, FieldSystem *fieldSystem, FieldMoveTaskEnvironment *env);
int FieldMoveArrival_Finish(TaskManager *taskManager, FieldSystem *fieldSystem, FieldMoveTaskEnvironment *env);
int FieldMove_ShowCutIn(TaskManager *taskManager, FieldSystem *fieldSystem, FieldMoveTaskEnvironment *env);
int FieldMove_WaitCutIn(TaskManager *taskManager, FieldSystem *fieldSystem, FieldMoveTaskEnvironment *env);
int FieldTeleport_FollowMonFlash(TaskManager *taskManager, FieldSystem *fieldSystem, FieldMoveTaskEnvironment *env);
int FieldTeleport_FollowMonSpinStart(TaskManager *taskManager, FieldSystem *fieldSystem, FieldMoveTaskEnvironment *env);
int FieldTeleport_FollowMonWaitSpin(TaskManager *taskManager, FieldSystem *fieldSystem, FieldMoveTaskEnvironment *env);
int FieldTeleport_SpinStart(TaskManager *taskManager, FieldSystem *fieldSystem, FieldMoveTaskEnvironment *env);
int FieldTeleport_SpinAndFadeOut(TaskManager *taskManager, FieldSystem *fieldSystem, FieldMoveTaskEnvironment *env);
int FieldTeleport_WaitFadeOut(TaskManager *taskManager, FieldSystem *fieldSystem, FieldMoveTaskEnvironment *env);
int FieldTeleport_Warp(TaskManager *taskManager, FieldSystem *fieldSystem, FieldMoveTaskEnvironment *env);
int FieldDig_SinkStart(TaskManager *taskManager, FieldSystem *fieldSystem, FieldMoveTaskEnvironment *env);
int FieldDig_DigHole(TaskManager *taskManager, FieldSystem *fieldSystem, FieldMoveTaskEnvironment *env);
int FieldDig_SpinAndFadeOut(TaskManager *taskManager, FieldSystem *fieldSystem, FieldMoveTaskEnvironment *env);
int FieldDig_WaitFadeOut(TaskManager *taskManager, FieldSystem *fieldSystem, FieldMoveTaskEnvironment *env);
int FieldDig_Warp(TaskManager *taskManager, FieldSystem *fieldSystem, FieldMoveTaskEnvironment *env);

FieldMoveTaskEnvironment *CreateFieldEscapeRopeTaskEnv(FieldSystem *fieldSystem, enum HeapID heapID) {
    FieldMoveTaskEnvironment *env = FieldMoveTaskEnvironment_New(heapID, sizeof(FieldMoveTaskEnvironment));
    int state;

    env->kind = FIELD_MOVE_WARP_ESCAPE_ROPE;
    env->fieldSystem = fieldSystem;
    env->playerObject = PlayerAvatar_GetMapObject(fieldSystem->playerAvatar);
    state = PlayerAvatar_GetState(fieldSystem->playerAvatar);
    if (state == PLAYER_STATE_CYCLING || state == PLAYER_STATE_SURFING) {
        env->withFollowMon = FALSE;
    } else if (FollowMon_IsActive(fieldSystem)) {
        env->withFollowMon = TRUE;
    } else {
        env->withFollowMon = FALSE;
    }
    return env;
}

BOOL Task_FieldEscapeRope(TaskManager *taskManager) {
    int result;
    FieldSystem *fieldSystem = TaskManager_GetFieldSystem(taskManager);
    FieldMoveTaskEnvironment *env = TaskManager_GetEnvironment(taskManager);

    do {
        result = sEscapeRopeSteps[env->state](taskManager, fieldSystem, env);
        if (result == FIELD_MOVE_STEP_DONE) {
            Heap_Free(env);
        }
    } while (result == FIELD_MOVE_STEP_NEXT);
    return FALSE;
}

int FieldEscapeRope_SpinStart(TaskManager *taskManager, FieldSystem *fieldSystem, FieldMoveTaskEnvironment *env) {
    SysTask *cameraZoom = FieldCameraZoom_Start(fieldSystem, HEAP_ID_FIELD1);

    env->cameraZoom = cameraZoom;
    FieldCameraZoom_Set(cameraZoom, FIELD_CAMERA_ZOOM_BY, -0x96000, 15);
    env->playerMovement = EventObjectMovementMan_Create(env->playerObject, sEscapeRopeSpinStartMovement);
    if (env->withFollowMon) {
        env->followMonMovement = EventObjectMovementMan_Create(fieldSystem->followMon.mapObject, sEscapeRopeSpinStartMovement);
    }
    env->state++;
    PlaySE(SEQ_SE_DP_KAIDAN2);
    return FIELD_MOVE_STEP_WAIT;
}

int FieldEscapeRope_SpinAndFadeOut(TaskManager *taskManager, FieldSystem *fieldSystem, FieldMoveTaskEnvironment *env) {
    if (!EventObjectMovementMan_IsFinish(env->playerMovement)) {
        return FIELD_MOVE_STEP_WAIT;
    }
    EventObjectMovementMan_Delete(env->playerMovement);
    env->playerMovement = EventObjectMovementMan_Create(env->playerObject, sEscapeRopeSpinMovement);
    if (env->withFollowMon) {
        EventObjectMovementMan_Delete(env->followMonMovement);
        env->followMonMovement = EventObjectMovementMan_Create(fieldSystem->followMon.mapObject, sEscapeRopeSpinMovement);
    }
    env->spinCount++;
    if (env->spinCount < 8) {
        return FIELD_MOVE_STEP_WAIT;
    }
    if (env->kind == FIELD_MOVE_WARP_TELEPORT) {
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, RGB_BLACK, 6, 1, HEAP_ID_FIELD1);
    } else {
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, RGB_WHITE, 6, 1, HEAP_ID_FIELD1);
    }
    env->state++;
    return FIELD_MOVE_STEP_WAIT;
}

int FieldEscapeRope_WaitFadeOut(TaskManager *taskManager, FieldSystem *fieldSystem, FieldMoveTaskEnvironment *env) {
    if (EventObjectMovementMan_IsFinish(env->playerMovement) == TRUE) {
        EventObjectMovementMan_Delete(env->playerMovement);
        env->playerMovement = EventObjectMovementMan_Create(env->playerObject, sEscapeRopeSpinMovement);
        if (env->withFollowMon) {
            EventObjectMovementMan_Delete(env->followMonMovement);
            env->followMonMovement = EventObjectMovementMan_Create(fieldSystem->followMon.mapObject, sEscapeRopeSpinMovement);
        }
    }
    if (!IsPaletteFadeFinished()) {
        return FIELD_MOVE_STEP_WAIT;
    }
    EventObjectMovementMan_Delete(env->playerMovement);
    if (env->withFollowMon) {
        EventObjectMovementMan_Delete(env->followMonMovement);
    }
    FieldCameraZoom_End(env->cameraZoom);
    env->state++;
    return FIELD_MOVE_STEP_NEXT;
}

// Escape Rope warps to the special spawn point of the last cave entrance.
int FieldEscapeRope_Warp(TaskManager *taskManager, FieldSystem *fieldSystem, FieldMoveTaskEnvironment *env) {
    JumpTask_FieldMoveWarp(taskManager, LocalFieldData_GetSpecialSpawnWarpPtr(Save_LocalFieldData_Get(fieldSystem->saveData)), env->kind);
    return FIELD_MOVE_STEP_DONE;
}

// Arrival after the warp (field_warp_tasks.c); kind is the warp's FieldMoveWarpKind.
FieldMoveTaskEnvironment *FieldMoveTask_CreateArrivalEnvironment(FieldSystem *fieldSystem, enum HeapID heapID, int kind) {
    FieldMoveTaskEnvironment *env = FieldMoveTaskEnvironment_New(heapID, sizeof(FieldMoveTaskEnvironment));
    env->kind = kind;
    env->fieldSystem = fieldSystem;
    env->playerObject = PlayerAvatar_GetMapObject(fieldSystem->playerAvatar);
    return env;
}

BOOL Task_FieldMoveArrival(TaskManager *taskManager) {
    FieldSystem *fieldSystem = TaskManager_GetFieldSystem(taskManager);
    FieldMoveTaskEnvironment *env = TaskManager_GetEnvironment(taskManager);
    int result;

    do {
        result = sFieldMoveArrivalSteps[env->state](taskManager, fieldSystem, env);
        if (result == FIELD_MOVE_STEP_DONE) {
            Heap_Free(env);
            return TRUE;
        }
    } while (result == FIELD_MOVE_STEP_NEXT);
    return FALSE;
}

int FieldMoveArrival_FadeIn(TaskManager *taskManager, FieldSystem *fieldSystem, FieldMoveTaskEnvironment *env) {
    SysTask *cameraZoom;

    if (env->kind == FIELD_MOVE_WARP_TELEPORT) {
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, RGB_BLACK, 6, 1, HEAP_ID_FIELD1);
    } else {
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, RGB_WHITE, 6, 1, HEAP_ID_FIELD1);
    }
    cameraZoom = FieldCameraZoom_Start(fieldSystem, HEAP_ID_FIELD1);
    env->cameraZoom = cameraZoom;
    FieldCameraZoom_Set(cameraZoom, FIELD_CAMERA_ZOOM_BY, -0x96000, 1);
    env->playerMovement = EventObjectMovementMan_Create(env->playerObject, sEscapeRopeSpinMovement);
    env->state++;
    return FIELD_MOVE_STEP_WAIT;
}

int FieldMoveArrival_WaitFadeIn(TaskManager *taskManager, FieldSystem *fieldSystem, FieldMoveTaskEnvironment *env) {
    if (EventObjectMovementMan_IsFinish(env->playerMovement) == TRUE) {
        EventObjectMovementMan_Delete(env->playerMovement);
        env->playerMovement = EventObjectMovementMan_Create(env->playerObject, sEscapeRopeSpinMovement);
    }
    if (!IsPaletteFadeFinished()) {
        return FIELD_MOVE_STEP_WAIT;
    }
    FieldCameraZoom_Set(env->cameraZoom, FIELD_CAMERA_ZOOM_BACK, 0, 60);
    env->state++;
    return FIELD_MOVE_STEP_NEXT;
}

int FieldMoveArrival_Spin(TaskManager *taskManager, FieldSystem *fieldSystem, FieldMoveTaskEnvironment *env) {
    if (!EventObjectMovementMan_IsFinish(env->playerMovement)) {
        return FIELD_MOVE_STEP_WAIT;
    }
    EventObjectMovementMan_Delete(env->playerMovement);
    if (++env->spinCount < 4) {
        env->playerMovement = EventObjectMovementMan_Create(env->playerObject, sEscapeRopeSpinMovement);
        return FIELD_MOVE_STEP_WAIT;
    }
    env->playerMovement = EventObjectMovementMan_Create(env->playerObject, sArrivalSpinMovement);
    env->state++;
    return FIELD_MOVE_STEP_WAIT;
}

int FieldMoveArrival_Finish(TaskManager *taskManager, FieldSystem *fieldSystem, FieldMoveTaskEnvironment *env) {
    if (!EventObjectMovementMan_IsFinish(env->playerMovement)) {
        return FIELD_MOVE_STEP_WAIT;
    }
    if (!FieldCameraZoom_IsDone(env->cameraZoom)) {
        return FIELD_MOVE_STEP_WAIT;
    }
    FieldCameraZoom_End(env->cameraZoom);
    EventObjectMovementMan_Delete(env->playerMovement);
    return FIELD_MOVE_STEP_DONE;
}

FieldMoveTaskEnvironment *FieldMoveTask_CreateDigEnvironment(FieldSystem *fieldSystem, Pokemon *mon, u8 slotno, enum HeapID heapID) {
    FieldMoveTaskEnvironment *env = FieldMoveTaskEnvironment_New(heapID, sizeof(FieldMoveTaskEnvironment));
    env->kind = FIELD_MOVE_WARP_DIG;
    env->fieldSystem = fieldSystem;
    env->playerObject = PlayerAvatar_GetMapObject(fieldSystem->playerAvatar);
    env->mon = mon;
    if (FollowMon_IsNextToPlayer(fieldSystem) && slotno == FieldSystem_GetLeadMonIndex(fieldSystem)) {
        env->withFollowMon = TRUE;
    } else {
        env->withFollowMon = FALSE;
    }
    return env;
}

BOOL Task_FieldDig(TaskManager *taskManager) {
    FieldSystem *fieldSystem = TaskManager_GetFieldSystem(taskManager);
    FieldMoveTaskEnvironment *env = TaskManager_GetEnvironment(taskManager);
    u32 *state = TaskManager_GetStatePtr(taskManager);
    int result;

    switch (*state) {
    case 0:
        if (env->withFollowMon) {
            TaskManager_Call(taskManager, Task_WaitFollowMonMovement, NULL);
        }
        (*state)++;
        break;
    case 1:
        (*state)++;
        if (env->withFollowMon) {
            u8 count;
            if (FieldSystem_FirstAliveMonHasType(fieldSystem, TYPE_GROUND)) {
                count = 2;
                FollowMonMood_Add(fieldSystem->followMonMood, 1);
            } else {
                count = 1;
            }
            FollowMon_JumpOnSpot(fieldSystem, count);
            break;
        }
        // fallthrough
    case 2:
        if (env->withFollowMon) {
            u32 species = GetMonData(env->mon, MON_DATA_SPECIES, NULL);
            u32 form = GetMonData(env->mon, MON_DATA_FORM, NULL);
            PlayCry((u16)species, (u8)form);
        }
        (*state)++;
        // fallthrough
    case 3:
        do {
            if (env->withFollowMon) {
                if (IsCryFinished()) {
                    result = FIELD_MOVE_STEP_WAIT;
                } else {
                    result = sDigWithFollowMonSteps[env->state](taskManager, fieldSystem, env);
                }
            } else {
                result = sDigSteps[env->state](taskManager, fieldSystem, env);
            }
            if (result == FIELD_MOVE_STEP_DONE) {
                Heap_Free(env);
            }
        } while (result == FIELD_MOVE_STEP_NEXT);
        break;
    }
    return FALSE;
}

// Dig / Teleport without the following Pokemon: the cut-in of the Pokemon using the move first.
int FieldMove_ShowCutIn(TaskManager *taskManager, FieldSystem *fieldSystem, FieldMoveTaskEnvironment *env) {
    int gender = PlayerAvatar_GetGender(fieldSystem->playerAvatar);
    env->cutIn = FieldMoveCutIn_Start(fieldSystem, 0, env->mon, gender);
    env->state++;
    return FIELD_MOVE_STEP_WAIT;
}

int FieldMove_WaitCutIn(TaskManager *taskManager, FieldSystem *fieldSystem, FieldMoveTaskEnvironment *env) {
    if (!FieldMoveCutIn_IsFinished(env->cutIn)) {
        return FIELD_MOVE_STEP_WAIT;
    }
    FieldMoveCutIn_Delete(env->cutIn);
    FollowMon_StartRecall(env->fieldSystem);
    env->state++;
    return FIELD_MOVE_STEP_WAIT;
}

#pragma force_active on
FieldMoveTaskEnvironment *FieldMoveTask_CreateTeleportEnvironment(FieldSystem *fieldSystem, Pokemon *mon, u8 slotno, enum HeapID heapID) {
    FieldMoveTaskEnvironment *env = FieldMoveTaskEnvironment_New(heapID, sizeof(FieldMoveTaskEnvironment));
    env->kind = FIELD_MOVE_WARP_TELEPORT;
    env->fieldSystem = fieldSystem;
    env->playerObject = PlayerAvatar_GetMapObject(fieldSystem->playerAvatar);
    env->mon = mon;
    if ((u32)(PlayerAvatar_GetState(fieldSystem->playerAvatar) - PLAYER_STATE_CYCLING) <= PLAYER_STATE_SURFING - PLAYER_STATE_CYCLING) {
        env->withFollowMon = FALSE;
    } else if (FollowMon_IsNextToPlayer(fieldSystem) && slotno == FieldSystem_GetLeadMonIndex(fieldSystem)) {
        env->withFollowMon = TRUE;
    } else {
        env->withFollowMon = FALSE;
    }
    return env;
}

BOOL Task_FieldTeleport(TaskManager *taskManager) {
    FieldSystem *fieldSystem = TaskManager_GetFieldSystem(taskManager);
    FieldMoveTaskEnvironment *env = TaskManager_GetEnvironment(taskManager);
    u32 *state = TaskManager_GetStatePtr(taskManager);

    switch (*state) {
    case 0:
        if (env->withFollowMon) {
            TaskManager_Call(taskManager, Task_WaitFollowMonMovement, NULL);
        }
        ++*state;
        break;
    case 1:
        ++*state;
        if (env->withFollowMon) {
            u8 count;
            if (FieldSystem_FirstAliveMonHasType(fieldSystem, TYPE_PSYCHIC)) {
                count = 2;
                FollowMonMood_Add(fieldSystem->followMonMood, 1);
            } else {
                count = 1;
            }
            FollowMon_JumpOnSpot(fieldSystem, count);
            break;
        }
        // fallthrough
    case 2:
        if (env->withFollowMon) {
            u32 species = GetMonData(env->mon, MON_DATA_SPECIES, NULL);
            u32 form = GetMonData(env->mon, MON_DATA_FORM, NULL);
            PlayCry((u16)species, (u8)form);
        }
        ++*state;
        // fallthrough
    case 3: {
        int result;
        do {
            if (env->withFollowMon) {
                if (IsCryFinished()) {
                    result = FIELD_MOVE_STEP_WAIT;
                } else {
                    result = sTeleportWithFollowMonSteps[env->state](taskManager, fieldSystem, env);
                }
            } else {
                result = sTeleportSteps[env->state](taskManager, fieldSystem, env);
            }
            if (result == FIELD_MOVE_STEP_DONE) {
                Heap_Free(env);
            }
        } while (result == FIELD_MOVE_STEP_NEXT);
    } break;
    }
    return FALSE;
}
#pragma force_active reset

FieldMoveTaskEnvironment *FieldMoveTaskEnvironment_New(enum HeapID heapID, u32 size) {
    FieldMoveTaskEnvironment *env = Heap_AllocAtEnd(heapID, size);
    if (env == NULL) {
        GF_AssertFail();
    }
    memset(env, 0, size);
    return env;
}

// Teleport by the following Pokemon: it flashes, spins and vanishes, then the player spins away.
int FieldTeleport_FollowMonFlash(TaskManager *taskManager, FieldSystem *fieldSystem, FieldMoveTaskEnvironment *env) {
    FollowMon_StartFlash(fieldSystem, 2, 1);
    env->state++;
    return FIELD_MOVE_STEP_WAIT;
}

int FieldTeleport_FollowMonSpinStart(TaskManager *taskManager, FieldSystem *fieldSystem, FieldMoveTaskEnvironment *env) {
    SysTask *cameraZoom = FieldCameraZoom_Start(fieldSystem, HEAP_ID_FIELD1);
    env->cameraZoom = cameraZoom;
    FieldCameraZoom_Set(cameraZoom, FIELD_CAMERA_ZOOM_BY, -0x96000, 15);
    env->followMonMovement = EventObjectMovementMan_Create(fieldSystem->followMon.mapObject, sTeleportFollowMonSpinMovement);
    env->state++;
    PlaySE(SEQ_SE_DP_TELE);
    return FIELD_MOVE_STEP_WAIT;
}

int FieldTeleport_FollowMonWaitSpin(TaskManager *taskManager, FieldSystem *fieldSystem, FieldMoveTaskEnvironment *env) {
    if (!EventObjectMovementMan_IsFinish(env->followMonMovement)) {
        return FIELD_MOVE_STEP_WAIT;
    }
    EventObjectMovementMan_Delete(env->followMonMovement);
    env->playerMovement = EventObjectMovementMan_Create(env->playerObject, sEscapeRopeSpinStartMovement);
    env->followMonMovement = EventObjectMovementMan_Create(fieldSystem->followMon.mapObject, sEscapeRopeSpinStartMovement);
    env->state++;
    return FIELD_MOVE_STEP_WAIT;
}

// Teleport after the cut-in.
int FieldTeleport_SpinStart(TaskManager *taskManager, FieldSystem *fieldSystem, FieldMoveTaskEnvironment *env) {
    SysTask *cameraZoom = FieldCameraZoom_Start(fieldSystem, HEAP_ID_FIELD1);
    env->cameraZoom = cameraZoom;
    FieldCameraZoom_Set(cameraZoom, FIELD_CAMERA_ZOOM_BY, -0x96000, 15);
    env->playerMovement = EventObjectMovementMan_Create(env->playerObject, sEscapeRopeSpinStartMovement);
    env->state++;
    PlaySE(SEQ_SE_DP_TELE);
    return FIELD_MOVE_STEP_WAIT;
}

int FieldTeleport_SpinAndFadeOut(TaskManager *taskManager, FieldSystem *fieldSystem, FieldMoveTaskEnvironment *env) {
    if (!EventObjectMovementMan_IsFinish(env->playerMovement)) {
        return FIELD_MOVE_STEP_WAIT;
    }
    EventObjectMovementMan_Delete(env->playerMovement);
    env->playerMovement = EventObjectMovementMan_Create(env->playerObject, sEscapeRopeSpinMovement);
    if (env->withFollowMon) {
        EventObjectMovementMan_Delete(env->followMonMovement);
        env->followMonMovement = EventObjectMovementMan_Create(fieldSystem->followMon.mapObject, sEscapeRopeSpinMovement);
    }
    env->spinCount++;
    if (env->spinCount < 8) {
        return FIELD_MOVE_STEP_WAIT;
    }
    BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, RGB_BLACK, 6, 1, HEAP_ID_FIELD1);
    env->state++;
    return FIELD_MOVE_STEP_WAIT;
}

int FieldTeleport_WaitFadeOut(TaskManager *taskManager, FieldSystem *fieldSystem, FieldMoveTaskEnvironment *env) {
    if (EventObjectMovementMan_IsFinish(env->playerMovement) == TRUE) {
        EventObjectMovementMan_Delete(env->playerMovement);
        env->playerMovement = EventObjectMovementMan_Create(env->playerObject, sEscapeRopeSpinMovement);
        if (env->withFollowMon) {
            EventObjectMovementMan_Delete(env->followMonMovement);
            env->followMonMovement = EventObjectMovementMan_Create(fieldSystem->followMon.mapObject, sEscapeRopeSpinMovement);
        }
    }
    if (!IsPaletteFadeFinished()) {
        return FIELD_MOVE_STEP_WAIT;
    }
    EventObjectMovementMan_Delete(env->playerMovement);
    if (env->withFollowMon) {
        EventObjectMovementMan_Delete(env->followMonMovement);
    }
    FieldCameraZoom_End(env->cameraZoom);
    env->state++;
    return FIELD_MOVE_STEP_NEXT;
}

// Teleport warps to the last Pokemon Center (the blackout spawn point).
int FieldTeleport_Warp(TaskManager *taskManager, FieldSystem *fieldSystem, FieldMoveTaskEnvironment *env) {
    Location warp;
    LocalFieldData *localFieldData = Save_LocalFieldData_Get(fieldSystem->saveData);
    u16 spawnId = LocalFieldData_GetBlackoutSpawn(localFieldData);
    GetFlyWarpData(spawnId, &warp);
    GetSpecialSpawnWarpData(spawnId, LocalFieldData_GetSpecialSpawnWarpPtr(localFieldData));
    JumpTask_FieldMoveWarp(taskManager, &warp, env->kind);
    return FIELD_MOVE_STEP_DONE;
}

// Dig: the player (and the following Pokemon) spin down into the ground.
int FieldDig_SinkStart(TaskManager *taskManager, FieldSystem *fieldSystem, FieldMoveTaskEnvironment *env) {
    SysTask *cameraZoom = FieldCameraZoom_Start(fieldSystem, HEAP_ID_FIELD1);
    env->cameraZoom = cameraZoom;
    FieldCameraZoom_Set(cameraZoom, FIELD_CAMERA_ZOOM_BY, -0x96000, 15);
    env->playerMovement = EventObjectMovementMan_Create(env->playerObject, sDigSinkMovement);
    if (env->withFollowMon) {
        env->followMonMovement = EventObjectMovementMan_Create(fieldSystem->followMon.mapObject, sDigSinkMovement);
    }
    env->state++;
    PlaySE(SEQ_SE_DP_KAIDAN2);
    return FIELD_MOVE_STEP_WAIT;
}

int FieldDig_DigHole(TaskManager *taskManager, FieldSystem *fieldSystem, FieldMoveTaskEnvironment *env) {
    if (!EventObjectMovementMan_IsFinish(env->playerMovement)) {
        return FIELD_MOVE_STEP_WAIT;
    }
    EventObjectMovementMan_Delete(env->playerMovement);
    env->playerMovement = EventObjectMovementMan_Create(env->playerObject, sDigHoleMovement);
    if (env->withFollowMon) {
        EventObjectMovementMan_Delete(env->followMonMovement);
        env->followMonMovement = EventObjectMovementMan_Create(fieldSystem->followMon.mapObject, sDigHoleMovement);
    }
    if (PlayerAvatar_GetState(fieldSystem->playerAvatar) != PLAYER_STATE_SURFING) {
        if (env->withFollowMon) {
            env->digEffect = RockDebris_CreateWithFollower(env->fieldSystem);
        } else {
            env->digEffect = RockDebris_Create(env->fieldSystem);
        }
    }
    env->state++;
    return FIELD_MOVE_STEP_WAIT;
}

int FieldDig_SpinAndFadeOut(TaskManager *taskManager, FieldSystem *fieldSystem, FieldMoveTaskEnvironment *env) {
    if (!EventObjectMovementMan_IsFinish(env->playerMovement)) {
        return FIELD_MOVE_STEP_WAIT;
    }
    EventObjectMovementMan_Delete(env->playerMovement);
    env->playerMovement = EventObjectMovementMan_Create(env->playerObject, sEscapeRopeSpinMovement);
    if (env->withFollowMon) {
        EventObjectMovementMan_Delete(env->followMonMovement);
        env->followMonMovement = EventObjectMovementMan_Create(fieldSystem->followMon.mapObject, sEscapeRopeSpinMovement);
    }
    env->spinCount++;
    if (env->spinCount < 8) {
        return FIELD_MOVE_STEP_WAIT;
    }
    BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, RGB_WHITE, 6, 1, HEAP_ID_FIELD1);
    env->state++;
    return FIELD_MOVE_STEP_WAIT;
}

int FieldDig_WaitFadeOut(TaskManager *taskManager, FieldSystem *fieldSystem, FieldMoveTaskEnvironment *env) {
    if (EventObjectMovementMan_IsFinish(env->playerMovement) == TRUE) {
        EventObjectMovementMan_Delete(env->playerMovement);
        env->playerMovement = EventObjectMovementMan_Create(env->playerObject, sEscapeRopeSpinMovement);
        if (env->withFollowMon) {
            EventObjectMovementMan_Delete(env->followMonMovement);
            env->followMonMovement = EventObjectMovementMan_Create(fieldSystem->followMon.mapObject, sEscapeRopeSpinMovement);
        }
    }
    if (!IsPaletteFadeFinished()) {
        return FIELD_MOVE_STEP_WAIT;
    }
    EventObjectMovementMan_Delete(env->playerMovement);
    if (PlayerAvatar_GetState(fieldSystem->playerAvatar) != PLAYER_STATE_SURFING) {
        RockDebris_Delete(env->digEffect);
        env->digEffect = NULL;
    }
    if (env->withFollowMon) {
        EventObjectMovementMan_Delete(env->followMonMovement);
    }
    FieldCameraZoom_End(env->cameraZoom);
    env->state++;
    return FIELD_MOVE_STEP_NEXT;
}

// Dig warps to the special spawn point of the last cave entrance, like Escape Rope.
int FieldDig_Warp(TaskManager *taskManager, FieldSystem *fieldSystem, FieldMoveTaskEnvironment *env) {
    JumpTask_FieldMoveWarp(taskManager, LocalFieldData_GetSpecialSpawnWarpPtr(Save_LocalFieldData_Get(fieldSystem->saveData)), env->kind);
    return FIELD_MOVE_STEP_DONE;
}
