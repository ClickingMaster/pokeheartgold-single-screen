#include "field_warp_state_internal.h"

#include "constants/pokemon.h"

static const u16 sFollowMovement[] = { 73, 1, 33, 1, 74, 1, 254, 0 };

void *BattleSetupSub134_New(int heapId) {
    void *p = Heap_Alloc(heapId, 0x24);
    MIi_CpuClearFast(0, p, 0x24);
    return p;
}

void BattleSetupSub134_Delete(void *p) {
    Heap_Free(p);
}

void FieldSystem_SetWarpResetSuppressed(void *fs, void *value) {
    *(void **)((u8 *)fs + 0xAC) = value;
}

void FieldSystem_ResetStateOnMapConnection(u8 *fs) {
    void *flags;
    void *roamers;
    flags = Save_VarsFlags_Get(*(void **)(fs + 0xC));
    StrengthFlagAction(flags, 0);
    FlypointFlagAction(fs, **(u32 ***)(fs + 0x20));
    roamers = Save_Roamers_Get(*(void **)(fs + 0xC));
    RoamerSave_SetFlute(roamers, 0);
    *(u16 *)(fs + 0x7E) = 0;
    *(u16 *)(fs + 0x7C) = 0;
    flags = Save_VarsFlags_Get(*(void **)(fs + 0xC));
    if (!Save_VarsFlags_CheckSafariSysFlag(flags)) {
        roamers = Save_Roamers_Get(*(void **)(fs + 0xC));
        UpdatePlayerLocationHistoryIfAnyRoamersActive(roamers, **(u32 ***)(fs + 0x20));
        Save_UpdateRoamersLocation(roamers);
    }
}

void FieldSystem_ResetStateOnWarp(u8 *fs) {
    void *flags;
    void *roamers;
    u8 *player;
    if (*(u32 *)(fs + 0xAC) == 1) {
        return;
    }
    flags = Save_VarsFlags_Get(*(void **)(fs + 0xC));
    ClearFlag972(flags);
    flags = Save_VarsFlags_Get(*(void **)(fs + 0xC));
    StrengthFlagAction(flags, 0);
    FlypointFlagAction(fs, **(u32 ***)(fs + 0x20));
    roamers = Save_Roamers_Get(*(void **)(fs + 0xC));
    RoamerSave_SetFlute(roamers, 0);
    *(u16 *)(fs + 0x7E) = 0;
    *(u16 *)(fs + 0x7C) = 0;
    roamers = Save_Roamers_Get(*(void **)(fs + 0xC));
    UpdatePlayerLocationHistoryIfAnyRoamersActive(roamers, **(u32 ***)(fs + 0x20));
    if (!MapHeader_IsCave(**(u32 ***)(fs + 0x20))) {
        flags = Save_VarsFlags_Get(*(void **)(fs + 0xC));
        SysFlagFlashClear(flags);
        SysFlagDefogClear(flags);
    }
    player = LocalFieldData_GetPlayer(Save_LocalFieldData_Get(*(void **)(fs + 0xC)));
    if (*(u32 *)(player + 4) == 1 && !MapHeader_IsBikeAllowed(**(u32 ***)(fs + 0x20))) {
        *(u32 *)(player + 4) = 0;
    } else if (*(u32 *)(player + 4) == 2) {
        *(u32 *)(player + 4) = 0;
    }
}

void FieldSystem_ResetStateOnFlyWarp(u8 *fs) {
    Save_VarsFlags_ClearSafariSysFlag(Save_VarsFlags_Get(*(void **)(fs + 0xC)));
    Save_RandomizeRoamersLocation(Save_Roamers_Get(*(void **)(fs + 0xC)));
}

void FieldSystem_ClearSafariAndRandomizeRoamers(u8 *fs) {
    Save_VarsFlags_ClearSafariSysFlag(Save_VarsFlags_Get(*(void **)(fs + 0xC)));
    Save_RandomizeRoamersLocation(Save_Roamers_Get(*(void **)(fs + 0xC)));
}

void FieldSystem_ClearSafariSysFlag(u8 *fs) {
    Save_VarsFlags_ClearSafariSysFlag(Save_VarsFlags_Get(*(void **)(fs + 0xC)));
}

void FieldSystem_ClearFollowingTrainer(u8 *fs) {
    void *f = Save_VarsFlags_Get(*(void **)(fs + 0xC));
    Save_VarsFlags_ClearHaveFollowerFlag(f);
    Save_VarsFlags_SetFollowerTrainerNum(f, 0);
}

void FieldSystem_RandomizeRoamers(u8 *fs) {
    Save_RandomizeRoamersLocation(Save_Roamers_Get(*(void **)(fs + 0xC)));
}

void *FlyTaskEnv_New(int heapId, void *fs, void *mon, int value, u16 mapNo, s16 x, s16 y) {
    FlyTaskEnv *p = Heap_AllocAtEnd(heapId, sizeof(FlyTaskEnv));
    memset(p, 0, sizeof(FlyTaskEnv));
    p->fieldSystem = fs;
    p->mon = mon;
    p->mapNo = mapNo;
    p->x = x;
    p->y = y;
    p->partySlot = value;
    return p;
}

BOOL FieldTask_Fly(void *task) {
    WarpData warp;
    int kind;
    u8 *fs = TaskManager_GetFieldSystem(task);
    FlyTaskEnv *env = TaskManager_GetEnvironment(task);
    switch (env->state) {
    case 0:
        TaskManager_Call(task, Task_WaitFollowMonMovement, 0);
        env->state++;
        break;
    case 1: {
        int avatarState = PlayerAvatar_GetState(*(void **)(fs + 0x40));
        if (avatarState == 1 || avatarState == 2) {
            env->state = 4;
            env->flag = 0;
            break;
        }
    }
        if (env->partySlot != GetIdxOfFirstAliveMonInParty_CrashIfNone(SaveArray_Party_Get(*(void **)(fs + 0xC)))) {
            FollowMon_StartRecall(env->fieldSystem);
            env->state = 4;
            env->flag = 0;
            break;
        }
        if (FollowMon_IsVisible(fs)) {
            if (FieldSystem_FirstAliveMonHasType(fs, TYPE_FLYING)) {
                kind = 2;
                FollowMonMood_Add(*(void **)(fs + 0x108), 1);
            } else {
                kind = 1;
            }
            FollowMon_JumpOnSpot(fs, kind);
            env->flag = 1;
            env->state = 2;
        } else {
            env->state = 4;
            env->flag = 0;
        }
        break;
    case 2: {
        u32 species = GetMonData(env->mon, MON_DATA_SPECIES, 0);
        u32 form = GetMonData(env->mon, MON_DATA_FORM, 0);
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
        env->movement = EventObjectMovementMan_Create(PlayerAvatar_GetMapObject(*(void **)(fs + 0x40)), sFollowMovement);
        env->state++;
        break;
    case 5:
        if (EventObjectMovementMan_IsFinish(env->movement)) {
            EventObjectMovementMan_Delete(env->movement);
            if (env->flag == 0) {
                env->effect = FieldMoveCutIn_Start(env->fieldSystem, 1, env->mon, PlayerAvatar_GetGender(*(void **)((u8 *)env->fieldSystem + 0x40)));
            } else {
                env->effect = FieldMoveCutIn_Start(env->fieldSystem, 2, env->mon, PlayerAvatar_GetGender(*(void **)((u8 *)env->fieldSystem + 0x40)));
            }
            env->state++;
        }
        break;
    case 6:
        if (FieldMoveCutIn_IsFinished(env->effect)) {
            void *local;
            u16 map;
            FieldMoveCutIn_Delete(env->effect);
            local = Save_LocalFieldData_Get(*(void **)(fs + 0xC));
            map = GetSpawnIdForFlyMap(env->mapNo);
            if (map == 0) {
                GF_AssertFail();
            }
            GetFlyWarpData(map, &warp);
            GetSpecialSpawnWarpData(map, LocalFieldData_GetSpecialSpawnWarpPtr(local));
            JumpTask_FlyWarp(task, warp.map, -1, warp.y, warp.dir, 1);
            Heap_Free(env);
        }
        break;
    }
    return FALSE;
}
