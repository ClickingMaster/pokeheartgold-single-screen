#include "field_warp_state.h"

#include "constants/pokemon.h"

#include "field/field_follow_mon.h"

#include "field_roamer.h"
#include "field_system.h"
#include "field_warp_reset.h"
#include "field_warp_reset_suppress.h"
#include "field_warp_tasks.h"
#include "follow_mon.h"
#include "heap.h"
#include "map_header.h"
#include "map_object_movement_commands.h"
#include "overlay_01.h"
#include "overlay_02.h"
#include "party.h"
#include "player_avatar.h"
#include "pokemon.h"
#include "roamer.h"
#include "save_local_field_data.h"
#include "save_vars_flags.h"
#include "script_pokemon_util.h"
#include "sound_chatot.h"
#include "sound_playback.h"
#include "spawn_warp.h"
#include "sys_flags.h"
#include "sys_vars.h"
#include "task.h"

struct FlyTaskEnv {
    FieldSystem *fieldSystem;
    u16 state;
    u16 flag;
    u16 mapNo;
    s16 x;
    s16 y;
    u16 partySlot;
    Pokemon *mon;
    void *effect;
    EventObjectMovementMan *movement;
};

u32 GetSpawnIdForFlyMap(u32 mapId);
void FlypointFlagAction(FieldSystem *fieldSystem, u32 mapId);

static const MovementScriptCommand sFollowMovement[] = {
    { 73, 1 },
    { 33, 1 },
    { 74, 1 },
    { 254, 0 },
};

struct BattleSetupSub_134 *BattleSetupSub134_New(enum HeapID heapId) {
    struct BattleSetupSub_134 *p = Heap_Alloc(heapId, 0x24);
    MI_CpuClearFast(p, 0x24);
    return p;
}

void BattleSetupSub134_Delete(struct BattleSetupSub_134 *p) {
    Heap_Free(p);
}

void FieldSystem_SetWarpResetSuppressed(FieldSystem *fieldSystem, u8 value) {
    fieldSystem->unkAC = value;
}

void FieldSystem_ResetStateOnMapConnection(FieldSystem *fieldSystem) {
    SaveVarsFlags *flags;
    RoamerSaveData *roamers;
    flags = Save_VarsFlags_Get(fieldSystem->saveData);
    StrengthFlagAction(flags, 0);
    FlypointFlagAction(fieldSystem, fieldSystem->location->mapId);
    roamers = Save_Roamers_Get(fieldSystem->saveData);
    RoamerSave_SetFlute(roamers, 0);
    fieldSystem->encounterInhibitSteps = 0;
    fieldSystem->reverseTurnFrameSteps = 0;
    flags = Save_VarsFlags_Get(fieldSystem->saveData);
    if (!Save_VarsFlags_CheckSafariSysFlag(flags)) {
        roamers = Save_Roamers_Get(fieldSystem->saveData);
        UpdatePlayerLocationHistoryIfAnyRoamersActive(roamers, fieldSystem->location->mapId);
        Save_UpdateRoamersLocation(roamers);
    }
}

void FieldSystem_ResetStateOnWarp(FieldSystem *fieldSystem) {
    SaveVarsFlags *flags;
    RoamerSaveData *roamers;
    PlayerSaveData *player;
    if (fieldSystem->unkAC == 1) {
        return;
    }
    flags = Save_VarsFlags_Get(fieldSystem->saveData);
    ClearFlag972(flags);
    flags = Save_VarsFlags_Get(fieldSystem->saveData);
    StrengthFlagAction(flags, 0);
    FlypointFlagAction(fieldSystem, fieldSystem->location->mapId);
    roamers = Save_Roamers_Get(fieldSystem->saveData);
    RoamerSave_SetFlute(roamers, 0);
    fieldSystem->encounterInhibitSteps = 0;
    fieldSystem->reverseTurnFrameSteps = 0;
    roamers = Save_Roamers_Get(fieldSystem->saveData);
    UpdatePlayerLocationHistoryIfAnyRoamersActive(roamers, fieldSystem->location->mapId);
    if (!MapHeader_IsCave(fieldSystem->location->mapId)) {
        flags = Save_VarsFlags_Get(fieldSystem->saveData);
        SysFlagFlashClear(flags);
        SysFlagDefogClear(flags);
    }
    player = LocalFieldData_GetPlayer(Save_LocalFieldData_Get(fieldSystem->saveData));
    if (player->state == 1 && !MapHeader_IsBikeAllowed(fieldSystem->location->mapId)) {
        player->state = 0;
    } else if (player->state == 2) {
        player->state = 0;
    }
}

void FieldSystem_ResetStateOnFlyWarp(FieldSystem *fieldSystem) {
    Save_VarsFlags_ClearSafariSysFlag(Save_VarsFlags_Get(fieldSystem->saveData));
    Save_RandomizeRoamersLocation(Save_Roamers_Get(fieldSystem->saveData));
}

void FieldSystem_ClearSafariAndRandomizeRoamers(FieldSystem *fieldSystem) {
    Save_VarsFlags_ClearSafariSysFlag(Save_VarsFlags_Get(fieldSystem->saveData));
    Save_RandomizeRoamersLocation(Save_Roamers_Get(fieldSystem->saveData));
}

void FieldSystem_ClearSafariSysFlag(FieldSystem *fieldSystem) {
    Save_VarsFlags_ClearSafariSysFlag(Save_VarsFlags_Get(fieldSystem->saveData));
}

void FieldSystem_ClearFollowingTrainer(FieldSystem *fieldSystem) {
    SaveVarsFlags *f = Save_VarsFlags_Get(fieldSystem->saveData);
    Save_VarsFlags_ClearHaveFollowerFlag(f);
    Save_VarsFlags_SetFollowerTrainerNum(f, 0);
}

void FieldSystem_RandomizeRoamers(FieldSystem *fieldSystem) {
    Save_RandomizeRoamersLocation(Save_Roamers_Get(fieldSystem->saveData));
}

struct FlyTaskEnv *FlyTaskEnv_New(enum HeapID heapId, FieldSystem *fieldSystem, Pokemon *mon, u16 value, u16 mapNo, s16 x, s16 y) {
    struct FlyTaskEnv *p = Heap_AllocAtEnd(heapId, sizeof(struct FlyTaskEnv));
    memset(p, 0, sizeof(struct FlyTaskEnv));
    p->fieldSystem = fieldSystem;
    p->mon = mon;
    p->mapNo = mapNo;
    p->x = x;
    p->y = y;
    p->partySlot = value;
    return p;
}

BOOL FieldTask_Fly(TaskManager *task) {
    Location warp;
    u8 kind;
    FieldSystem *fieldSystem = TaskManager_GetFieldSystem(task);
    struct FlyTaskEnv *env = TaskManager_GetEnvironment(task);
    switch (env->state) {
    case 0:
        TaskManager_Call(task, Task_WaitFollowMonMovement, NULL);
        env->state++;
        break;
    case 1: {
        int avatarState = PlayerAvatar_GetState(fieldSystem->playerAvatar);
        if (avatarState == 1 || avatarState == 2) {
            env->state = 4;
            env->flag = 0;
            break;
        }
    }
        if (env->partySlot != GetIdxOfFirstAliveMonInParty_CrashIfNone(SaveArray_Party_Get(fieldSystem->saveData))) {
            FollowMon_StartRecall(env->fieldSystem);
            env->state = 4;
            env->flag = 0;
            break;
        }
        if (FollowMon_IsVisible(fieldSystem)) {
            if (FieldSystem_FirstAliveMonHasType(fieldSystem, TYPE_FLYING)) {
                kind = 2;
                FollowMonMood_Add(fieldSystem->followMonMood, 1);
            } else {
                kind = 1;
            }
            FollowMon_JumpOnSpot(fieldSystem, kind);
            env->flag = 1;
            env->state = 2;
        } else {
            env->state = 4;
            env->flag = 0;
        }
        break;
    case 2: {
        u32 species = GetMonData(env->mon, MON_DATA_SPECIES, NULL);
        u32 form = GetMonData(env->mon, MON_DATA_FORM, NULL);
        PlayCry((u16)species, (u8)form);
        env->state++;
        break;
    }
    case 3:
        if (!IsCryFinished()) {
            env->state++;
        }
        break;
    case 4:
        env->movement = EventObjectMovementMan_Create(PlayerAvatar_GetMapObject(fieldSystem->playerAvatar), sFollowMovement);
        env->state++;
        break;
    case 5:
        if (EventObjectMovementMan_IsFinish(env->movement)) {
            EventObjectMovementMan_Delete(env->movement);
            if (env->flag == 0) {
                env->effect = FieldMoveCutIn_Start(env->fieldSystem, 1, env->mon, PlayerAvatar_GetGender(env->fieldSystem->playerAvatar));
            } else {
                env->effect = FieldMoveCutIn_Start(env->fieldSystem, 2, env->mon, PlayerAvatar_GetGender(env->fieldSystem->playerAvatar));
            }
            env->state++;
        }
        break;
    case 6:
        if (FieldMoveCutIn_IsFinished(env->effect)) {
            LocalFieldData *local;
            u16 map;
            FieldMoveCutIn_Delete(env->effect);
            local = Save_LocalFieldData_Get(fieldSystem->saveData);
            map = GetSpawnIdForFlyMap(env->mapNo);
            if (map == 0) {
                GF_AssertFail();
            }
            GetFlyWarpData(map, &warp);
            GetSpecialSpawnWarpData(map, LocalFieldData_GetSpecialSpawnWarpPtr(local));
            JumpTask_FlyWarp(task, warp.mapId, -1, warp.x, warp.y, 1);
            Heap_Free(env);
        }
        break;
    }
    return FALSE;
}
