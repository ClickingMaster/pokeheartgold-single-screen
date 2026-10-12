#include "field_warp_tasks.h"

#include "constants/field/map_load.h"
#include "constants/init_script_types.h"
#include "constants/maps.h"

#include "field/warp_spin.h"
#include "field_bgm.h"
#include "field_system_rtc_weather.h"
#include "follow_mon.h"
#include "map_events.h"
#include "map_object.h"
#include "overlay_01.h"
#include "overlay_02.h"
#include "save_follow_mon.h"
#include "save_local_field_data.h"
#include "save_vars_flags.h"
#include "screen_fade.h"
#include "script.h"
#include "sound.h"
#include "sound_system.h"
#include "sys_flags.h"
#include "sys_task.h"
#include "sys_vars.h"
#include "system.h"
#include "task.h"
#include "terrain_attributes.h"
#include "sound_playback.h"
#include "spawn_warp.h"
#include "field_terrain.h"
#include "field_overworld_transition.h"
#include "unk_020552A4.h"
#include "unk_02055418.h"
#include "comm_player_manager.h"
#include "unk_02058AEC.h"
#include "union_room_objects.h"
#include "unk_0206793C.h"

struct UnkTaskEnv_02053688 {
    int unk0;
    Location location;
    int unk18;
};

struct UnkTaskEnv_02053950 {
    int unk0;
    SysTask *unk4;
    Location location;
    u8 padding[4];
};

struct UnkTaskEnv_02053B3C {
    int unk0;
    int unk4;
    SysTask *unk8;
    Location location;
};

struct UnkTaskEnv_02053CCC {
    int unk0;
    int unk4;
    Location location;
    UnkStruct_0206793C *unk1C;
};

struct UnkTaskEnv_02053E5C {
    int unk0;
    int unk4;
    Location location;
};

static void sub_02052F30(FieldSystem *fieldSystem);
static void sub_02052F94(FieldSystem *fieldSystem, Location *location);
static void sub_0205316C(FieldSystem *fieldSystem);
static void sub_02053210(FieldSystem *fieldSystem);
static void sub_0205323C(FieldSystem *fieldSystem);
static void sub_02053284(FieldSystem *fieldSystem);
static void sub_02053324(FieldSystem *fieldSystem);
static void _CopyPlayerPosToLocationWorkFacingSouth(Location *location, FieldSystem *fieldSystem);
static BOOL _IsPlayerStandingInFrontOfUnionRoomReception(FieldSystem *fieldSystem);
static void _SetDynamicWarpToUnionRoomExit(FieldSystem *fieldSystem);
static BOOL FieldTask_NewGame(TaskManager *taskManager);
static BOOL FieldTask_ContinueGame_Normal(TaskManager *taskManager);
static BOOL FieldTask_ContinueGame_CommError(TaskManager *taskManager);
static BOOL sub_02053688(TaskManager *taskManager);
static void sub_02053710(TaskManager *taskManager, Location *location);
static BOOL sub_02053740(TaskManager *taskManager);
static BOOL Task_ScriptWarp(TaskManager *taskManager);
static BOOL sub_02053950(TaskManager *taskManager);
static void sub_020539D8(TaskManager *taskManager);
static BOOL sub_020539E8(TaskManager *taskManager);
static void sub_02053A14(TaskManager *taskManager);
static BOOL sub_02053A2C(TaskManager *taskManager);
static void sub_02053AA0(TaskManager *taskManager);
static BOOL sub_02053AE4(TaskManager *taskManager);
static BOOL sub_02053B3C(TaskManager *taskManager);
static void sub_02053BE8(TaskManager *taskManager);
static BOOL sub_02053BF8(TaskManager *taskManager);
static void sub_02053C24(TaskManager *taskManager);
static BOOL sub_02053C3C(TaskManager *taskManager);
static void sub_02053C90(TaskManager *taskManager);
static BOOL sub_02053CCC(TaskManager *taskManager);
static BOOL sub_02053E5C(TaskManager *taskManager);
static BOOL sub_02053F70(TaskManager *taskManager);
static BOOL sub_020540A4(TaskManager *taskManager);

static const struct MapLoadMode sMapLoadModes[] = {
    { .fieldBottomScreen = 1,
     .skipMapAttributes = FALSE,
     .useSimpleTerrainCollisions = FALSE,
     .switchScreens = FALSE,
     .useSeparateTerrainAttributes = FALSE,
     .loadExtOverlay = TRUE,
     .separateTerrainAttributesBlockCount = 0,
     .unk4 = 0,
     .unk5 = 64,
     .unk6 = 12 },
    { .fieldBottomScreen = 1,
     .skipMapAttributes = 2,
     .useSimpleTerrainCollisions = FALSE,
     .switchScreens = FALSE,
     .useSeparateTerrainAttributes = FALSE,
     .loadExtOverlay = TRUE,
     .separateTerrainAttributesBlockCount = 0,
     .unk4 = 0,
     .unk5 = 64,
     .unk6 = 12 },
    { .fieldBottomScreen = 3,
     .skipMapAttributes = FALSE,
     .useSimpleTerrainCollisions = FALSE,
     .switchScreens = FALSE,
     .useSeparateTerrainAttributes = FALSE,
     .loadExtOverlay = TRUE,
     .separateTerrainAttributesBlockCount = 0,
     .unk4 = 0,
     .unk5 = 64,
     .unk6 = 12 },
    { .fieldBottomScreen = 1,
     .skipMapAttributes = TRUE,
     .useSimpleTerrainCollisions = TRUE,
     .switchScreens = FALSE,
     .useSeparateTerrainAttributes = TRUE,
     .loadExtOverlay = TRUE,
     .separateTerrainAttributesBlockCount = 1,
     .unk4 = 0,
     .unk5 = 64,
     .unk6 = 12 },
    { .fieldBottomScreen = 1,
     .skipMapAttributes = TRUE,
     .useSimpleTerrainCollisions = TRUE,
     .switchScreens = FALSE,
     .useSeparateTerrainAttributes = TRUE,
     .loadExtOverlay = TRUE,
     .separateTerrainAttributesBlockCount = 1,
     .unk4 = 0,
     .unk5 = 0,
     .unk6 = 10 },
    { .fieldBottomScreen = 6,
     .skipMapAttributes = FALSE,
     .useSimpleTerrainCollisions = FALSE,
     .switchScreens = FALSE,
     .useSeparateTerrainAttributes = FALSE,
     .loadExtOverlay = TRUE,
     .separateTerrainAttributesBlockCount = 0,
     .unk4 = 0,
     .unk5 = 64,
     .unk6 = 12 },
};

static void sub_02052F30(FieldSystem *fieldSystem) {
    BOOL battleTower = FALSE;

    switch (fieldSystem->location->mapId) {
    case MAP_SAFARI_ZONE_ENTRANCE_EXTERIOR:
        fieldSystem->mapLoadType = MAP_LOAD_TYPE_SAFARI;
        return;
    case MAP_BATTLE_TOWER:
    case MAP_BATTLE_TOWER_ELEVATOR:
    case MAP_BATTLE_TOWER_UNUSED_1:
    case MAP_BATTLE_TOWER_UNUSED_2:
    case MAP_BATTLE_TOWER_UNUSED_3:
    case MAP_BATTLE_TOWER_UNUSED_4:
    case MAP_BATTLE_TOWER_PARTNER_ROOM:
        battleTower = TRUE;
        break;
    }
    if (fieldSystem->mapLoadType == MAP_LOAD_TYPE_SAFARI) {
        fieldSystem->mapLoadType = MAP_LOAD_TYPE_OVERWORLD;
    }
    if (!battleTower && fieldSystem->mapLoadType == MAP_LOAD_TYPE_BATTLE_TOWER) {
        fieldSystem->mapLoadType = MAP_LOAD_TYPE_OVERWORLD;
    }
    if (battleTower) {
        fieldSystem->mapLoadType = MAP_LOAD_TYPE_BATTLE_TOWER;
    }
}

static void sub_02052F94(FieldSystem *fieldSystem, Location *location) {
    LocalFieldData *localFieldData = Save_LocalFieldData_Get(fieldSystem->saveData);
    Location *r2 = LocalFieldData_GetPreviousPosition(localFieldData);
    const WarpEvent *warp;
    if (location != NULL) {
        *r2 = *fieldSystem->location;
        *fieldSystem->location = *location;
    }
    Field_InitMapEvents(fieldSystem, fieldSystem->location->mapId);
    if (fieldSystem->location->warpId != -1) {
        warp = Field_GetWarpEventI(fieldSystem, fieldSystem->location->warpId);
        fieldSystem->location->x = warp->x;
        fieldSystem->location->y = warp->z;
        if (warp->anchor == 0x100) {
            *LocalFieldData_GetDynamicWarp(localFieldData) = *LocalFieldData_GetEntrancePosition(localFieldData);
        }
    }
}

void FieldMapChange_Set3DDisplay(FieldSystem *fieldSystem) {
    GF_ASSERT(fieldSystem->mapLoadType < MAP_LOAD_TYPE_MAX);
    gSystem.screensFlipped = fieldSystem->mapLoadMode->switchScreens;
}

void sub_02053038(FieldSystem *fieldSystem, BOOL isConnection) {
    u32 mapId = fieldSystem->location->mapId;
    LocalFieldData *localFieldData = Save_LocalFieldData_Get(fieldSystem->saveData);
    SaveVarsFlags *varsFlags;
    u16 weather;
    u16 spawnId;

    FieldBGM_ClearOverride(fieldSystem);
    if (!fieldSystem->unkAC) {
        ClearTempFieldEventData(fieldSystem);
    }
    if (!isConnection) {
        FieldSystem_ResetStateOnWarp(fieldSystem);
    } else {
        FieldSystem_ResetStateOnMapConnection(fieldSystem);
    }
    if (!fieldSystem->unkAC && !isConnection) {
        FieldSystem_StartBugContestTimer(fieldSystem);
    }
    if (!isConnection) {
        Save_Gymmick_Clear(Save_GetGymmickPtr(fieldSystem->saveData));
        SetLakeOfRageWaterLevel(fieldSystem->mapMatrix, ShouldUseAlternateLakeOfRage(fieldSystem->saveData, mapId));
    }
    varsFlags = Save_VarsFlags_Get(fieldSystem->saveData);
    weather = FieldSystem_GetWeather_HandleDiamondDust(fieldSystem, mapId);
    if (sub_02066C74(varsFlags, 1) && mapId == MAP_LAKE_OF_RAGE) {
        weather = 0;
    }
    if (weather == 9 && SysFlagDefogCheck(varsFlags) == TRUE) {
        weather = 0;
    }
    if (weather == 11 && SysFlagFlashCheck(varsFlags) == TRUE) {
        weather = 12;
    }
    LocalFieldData_SetWeatherType(localFieldData, weather);
    if (isConnection) {
        GF_ASSERT(LocalFieldData_GetCameraType(localFieldData) == MapHeader_GetCameraType(mapId));
    } else {
        LocalFieldData_SetCameraType(localFieldData, MapHeader_GetCameraType(mapId));
    }
    if (!isConnection) {
        spawnId = MapHeader_GetSpawnIdForDeathWarp(mapId);
        if (spawnId != 0) {
            LocalFieldData_SetBlackoutSpawn(localFieldData, spawnId);
        }
    }
    TryStartMapScriptByType(fieldSystem, INIT_SCRIPT_ON_TRANSITION);
    fieldSystem->encounterInhibitSteps = 0;
    fieldSystem->reverseTurnFrameSteps = 0;
    fieldSystem->unk78 = 0;
    Save_FollowMon_SetInhibitFlagState(Save_FollowMon_Get(fieldSystem->saveData), FALSE);
    ClearFlag99A(Save_VarsFlags_Get(fieldSystem->saveData));
}

static void sub_0205316C(FieldSystem *fieldSystem) {
    u32 gender;
    struct PlayerSaveData *playerSaveData;
    if (fieldSystem->unkAC) {
        gender = PlayerProfile_GetTrainerGender(Save_PlayerData_GetProfile(fieldSystem->saveData));
        playerSaveData = LocalFieldData_GetPlayer(Save_LocalFieldData_Get(fieldSystem->saveData));
        fieldSystem->playerAvatar = PlayerAvatar_CreateWithParams(fieldSystem->mapObjectManager, fieldSystem->location->x, fieldSystem->location->y, fieldSystem->location->direction, playerSaveData->state, gender, 2, playerSaveData);
    } else {
        fieldSystem->mapObjectManager = MapObjectManager_Init(fieldSystem, 64, 5);
        gender = PlayerProfile_GetTrainerGender(Save_PlayerData_GetProfile(fieldSystem->saveData));
        playerSaveData = LocalFieldData_GetPlayer(Save_LocalFieldData_Get(fieldSystem->saveData));
        fieldSystem->playerAvatar = PlayerAvatar_CreateWithParams(fieldSystem->mapObjectManager, fieldSystem->location->x, fieldSystem->location->y, fieldSystem->location->direction, playerSaveData->state, gender, 2, playerSaveData);
        FollowMon_InitMapObject(fieldSystem->mapObjectManager, fieldSystem->location->x, fieldSystem->location->y, fieldSystem->location->direction, fieldSystem->location->mapId);
        Field_InitMapObjectsFromZoneEventData(fieldSystem);
        sub_0205F55C(fieldSystem->mapObjectManager);
    }
}

static void sub_02053210(FieldSystem *fieldSystem) {
    if (fieldSystem->unkAC) {
        PlayerAvatar_DeleteFromMap(fieldSystem->playerAvatar);
    } else {
        CommPlayerManager_RemoveAllAvatars();
        PlayerAvatar_FreeToHeap(fieldSystem->playerAvatar);
        MapObjectManager_RemoveAllActiveObjects(fieldSystem->mapObjectManager);
        MapObjectManager_Delete(fieldSystem->mapObjectManager);
    }
}

static void sub_0205323C(FieldSystem *fieldSystem) {
    u32 gender;
    struct PlayerSaveData *playerSaveData;

    fieldSystem->mapObjectManager = MapObjectManager_Init(fieldSystem, 64, 5);
    FieldSystem_RestoreMapObjectsFromSave(fieldSystem);
    playerSaveData = LocalFieldData_GetPlayer(Save_LocalFieldData_Get(fieldSystem->saveData));
    gender = PlayerProfile_GetTrainerGender(Save_PlayerData_GetProfile(fieldSystem->saveData));
    fieldSystem->playerAvatar = PlayerAvatar_CreateWithActiveMapObject(fieldSystem->mapObjectManager, playerSaveData, gender);
    FollowMon_ChangeMon(fieldSystem->mapObjectManager, fieldSystem->location->mapId);
    sub_0205F55C(fieldSystem->mapObjectManager);
}

static void sub_02053284(FieldSystem *fieldSystem) {
    SaveVarsFlags *varsFlags;

    sub_02052F30(fieldSystem);

    GF_ASSERT(fieldSystem->unk60 == NULL);

    MapMatrix_Load(fieldSystem->location->mapId, fieldSystem->mapMatrix);
    varsFlags = Save_VarsFlags_Get(fieldSystem->saveData);
    if (sub_02066C74(varsFlags, 0)) {
        RemoveMahoganyTownAntennaTree(fieldSystem->mapMatrix);
    }
    SetLakeOfRageWaterLevel(fieldSystem->mapMatrix, sub_02066C74(varsFlags, 1));
    PlaceSafariZoneAreas(fieldSystem->mapMatrix, fieldSystem->saveData);

    GF_ASSERT(fieldSystem->mapLoadType < 6);

    fieldSystem->mapLoadMode = &sMapLoadModes[fieldSystem->mapLoadType];
    fieldSystem->skipMapAttributes = fieldSystem->mapLoadMode->skipMapAttributes;
    fieldSystem->bottomScreenType = fieldSystem->mapLoadMode->fieldBottomScreen;
    FieldSystem_SetTerrainQueries(&fieldSystem->unk60, fieldSystem->mapLoadMode->useSimpleTerrainCollisions);
    if (fieldSystem->mapLoadMode->useSeparateTerrainAttributes) {
        TerrainAttributes_New(fieldSystem, fieldSystem->mapLoadMode->separateTerrainAttributesBlockCount);
    }
}

static void sub_02053324(FieldSystem *fieldSystem) {
    GF_ASSERT(fieldSystem->unk60 != NULL);
    fieldSystem->unk60 = NULL;
    fieldSystem->bottomScreenType = 7;
    if (fieldSystem->mapLoadMode->useSeparateTerrainAttributes) {
        TerrainAttributes_Free(fieldSystem);
    }
    fieldSystem->mapLoadMode = 0;
}

static void _CopyPlayerPosToLocationWorkFacingSouth(Location *location, FieldSystem *fieldSystem) {
    int x, z;

    z = PlayerAvatar_GetZCoord(fieldSystem->playerAvatar);
    x = PlayerAvatar_GetXCoord(fieldSystem->playerAvatar);

    InitLocation(location, fieldSystem->location->mapId, -1, x, z, DIR_SOUTH);
}

static BOOL _IsPlayerStandingInFrontOfUnionRoomReception(FieldSystem *fieldSystem) {
    if (MapHeader_MapIsPokemonCenter(fieldSystem->location->mapId)
        && fieldSystem->location->x == 6 && fieldSystem->location->y == 6) {
        return TRUE;
    }

    if (MapHeader_MapIsPokemonLeagueLobby(fieldSystem->location->mapId)
        && fieldSystem->location->x == 3 && fieldSystem->location->y == 15) {
        return TRUE;
    }

    return FALSE;
}

static void _SetDynamicWarpToUnionRoomExit(FieldSystem *fieldSystem) {
    Location *dynamicWarp = LocalFieldData_GetDynamicWarp(Save_LocalFieldData_Get(fieldSystem->saveData));
    SaveVarsFlags *varsFlags = Save_VarsFlags_Get(fieldSystem->saveData); // unused
    if (MapHeader_MapIsPokemonLeagueLobby(fieldSystem->location->mapId) == TRUE) {
        InitLocation(dynamicWarp, fieldSystem->location->mapId, -1, 4, 11, DIR_SOUTH);
    } else {
        InitLocation(dynamicWarp, fieldSystem->location->mapId, -1, 7, 2, DIR_SOUTH);
    }
}

static BOOL FieldTask_NewGame(TaskManager *taskManager) {
    FieldSystem *fieldSystem = TaskManager_GetFieldSystem(taskManager);
    u32 *state_p = TaskManager_GetStatePtr(taskManager);

    switch (*state_p) {
    case 0:
        sub_02052F94(fieldSystem, fieldSystem->location);
        sub_02053284(fieldSystem);
        sub_02053038(fieldSystem, FALSE);
        sub_0205316C(fieldSystem);
        (*state_p)++;
        break;
    case 1:
        CallTask_RestoreOverworldAndFadeIn(taskManager);
        (*state_p)++;
        break;
    case 2:
        return TRUE;
    }

    return FALSE;
}

TaskManager *CallFieldTask_NewGame(FieldSystem *fieldSystem) {
    fieldSystem->mapLoadType = 0;
    RunInitScript(fieldSystem);
    return FieldSystem_CreateTask(fieldSystem, FieldTask_NewGame, NULL);
}

static BOOL FieldTask_ContinueGame_Normal(TaskManager *taskManager) {
    FieldSystem *fieldSystem = TaskManager_GetFieldSystem(taskManager);
    SaveVarsFlags *varsFlags = Save_VarsFlags_Get(fieldSystem->saveData);
    LocalFieldData *localFieldData;
    u32 *state_p = TaskManager_GetStatePtr(taskManager);

    switch (*state_p) {
    case 0:
        if (CheckFlag966(varsFlags)) {
            localFieldData = Save_LocalFieldData_Get(fieldSystem->saveData);
            if (_IsPlayerStandingInFrontOfUnionRoomReception(fieldSystem)) {
                _SetDynamicWarpToUnionRoomExit(fieldSystem);
            }
            ClearFlag966(varsFlags);
            sub_02052F94(fieldSystem, LocalFieldData_GetDynamicWarp(localFieldData));
            sub_02053284(fieldSystem);
            sub_02053038(fieldSystem, FALSE);
            sub_0205316C(fieldSystem);
        } else {
            sub_02052F94(fieldSystem, NULL);
            sub_02053284(fieldSystem);
            FieldSystem_StartBugContestTimer(fieldSystem);
            sub_0205323C(fieldSystem);
        }
        FieldSystem_RandomizeRoamers(fieldSystem);
        *state_p = 2;
        break;
    case 2:
        CallTask_RestoreOverworldAndFadeIn(taskManager);
        *state_p = 3;
        break;
    case 3:
        return TRUE;
    }

    return FALSE;
}

TaskManager *CallFieldTask_ContinueGame_Normal(FieldSystem *fieldSystem) {
    fieldSystem->mapLoadType = 0;
    return FieldSystem_CreateTask(fieldSystem, FieldTask_ContinueGame_Normal, NULL);
}

static BOOL FieldTask_ContinueGame_CommError(TaskManager *taskManager) {
    FieldSystem *fieldSystem = TaskManager_GetFieldSystem(taskManager);
    struct ErrorContinueEnv *env = TaskManager_GetEnvironment(taskManager);
    SaveVarsFlags *varsFlags = Save_VarsFlags_Get(fieldSystem->saveData);
    u32 *state_p = TaskManager_GetStatePtr(taskManager);

    switch (*state_p) {
    case 0:
        SetMasterBrightnessByColor(PM_LCD_TOP, RGB_BLACK);
        SetMasterBrightnessByColor(PM_LCD_BOTTOM, RGB_BLACK);
        (*state_p)++;
        break;
    case 1:
        sub_02052F94(fieldSystem, &env->location);
        sub_02053284(fieldSystem);
        sub_02053038(fieldSystem, FALSE);
        sub_0205316C(fieldSystem);
        (*state_p)++;
        break;
    case 2:
        fieldSystem->unk80 = UnionRoom_Start(fieldSystem);
        fieldSystem->unk84 = UnionRoomObjects_New(fieldSystem->unk80);
        CallTask_RestoreOverworld(taskManager);
        (*state_p)++;
        break;
    case 3:
        env->state = 0;
        FieldSystem_StartWarpSpin(fieldSystem, 0, &env->state);
        (*state_p)++;
        break;
    case 4:
        if (env->state) {
            (*state_p)++;
        }
        break;
    case 5:
        Heap_Free(env);
        return TRUE;
    }

    return FALSE;
}

TaskManager *CallFieldTask_ContinueGame_CommError(FieldSystem *fieldSystem) {
    SaveVarsFlags *varsFlags;
    struct ErrorContinueEnv *env;
    if (!MapHeader_MapIsUnionRoom(fieldSystem->location->mapId)) {
        if (_IsPlayerStandingInFrontOfUnionRoomReception(fieldSystem)) {
            varsFlags = Save_VarsFlags_Get(fieldSystem->saveData);
            _SetDynamicWarpToUnionRoomExit(fieldSystem);
            SetFlag966(varsFlags);
        } else {
            return CallFieldTask_ContinueGame_Normal(fieldSystem);
        }
    }
    env = Heap_AllocAtEnd(HEAP_ID_FIELD2, sizeof(struct ErrorContinueEnv));
    env->state = 0;
    InitLocation(&env->location, MAP_UNION, -1, 8, 14, DIR_NORTH);
    fieldSystem->mapLoadType = MAP_LOAD_TYPE_UNION;
    return FieldSystem_CreateTask(fieldSystem, FieldTask_ContinueGame_CommError, env);
}

static BOOL sub_02053688(TaskManager *taskManager) {
    FieldSystem *fieldSystem = TaskManager_GetFieldSystem(taskManager);
    struct UnkTaskEnv_02053688 *env = TaskManager_GetEnvironment(taskManager);

    switch (env->unk0) {
    case 0:
        PlaySE(SEQ_SE_DP_KAIDAN2);
        FieldBGM_TryFadeIn(fieldSystem, env->location.mapId);
        CallTask_FadeOutAndLeaveOverworld(taskManager);
        env->unk0++;
        break;
    case 1:
        sub_020537A8(taskManager, &env->location);
        env->unk0++;
        break;
    case 2:
        if (GF_SndGetFadeTimer() != 0) {
            break;
        }
        FieldBGM_PlayForMapHeader(fieldSystem, env->location.mapId, FALSE);
        CallTask_RestoreOverworldAndFadeIn(taskManager);
        env->unk0++;
        break;
    case 3:
        Heap_Free(env);
        return TRUE;
    }

    return FALSE;
}

static void sub_02053710(TaskManager *taskManager, Location *location) {
    struct UnkTaskEnv_02053688 *env = Heap_AllocAtEnd(HEAP_ID_FIELD2, sizeof(struct UnkTaskEnv_02053688));
    env->unk0 = 0;
    env->location = *location;
    TaskManager_Call(taskManager, sub_02053688, env);
}

static BOOL sub_02053740(TaskManager *taskManager) {
    FieldSystem *fieldSystem = TaskManager_GetFieldSystem(taskManager);
    struct ErrorContinueEnv *env = TaskManager_GetEnvironment(taskManager);

    switch (env->state) {
    case 0:
        sub_02053210(fieldSystem);
        sub_02053324(fieldSystem);
        env->state++;
        break;
    case 1:
        sub_02052F94(fieldSystem, &env->location);
        sub_02053284(fieldSystem);
        sub_02053038(fieldSystem, FALSE);
        env->state++;
        break;
    case 2:
        sub_0205316C(fieldSystem);
        Heap_Free(env);
        return TRUE;
    }

    return FALSE;
}

void sub_020537A8(TaskManager *taskManager, const Location *location) {
    FieldSystem *fieldSystem = TaskManager_GetFieldSystem(taskManager);
    struct ErrorContinueEnv *env = Heap_AllocAtEnd(HEAP_ID_FIELD2, sizeof(struct ErrorContinueEnv));
    if (sub_0203DF7C(fieldSystem)) {
        GF_ASSERT(FALSE);
        return;
    }
    env->state = 0;
    env->location = *location;
    TaskManager_Call(taskManager, sub_02053740, env);
}

void sub_020537F0(TaskManager *taskManager, u32 mapId, int warpId, int x, int y, int direction) {
    Location location = {
        mapId,
        warpId,
        x,
        y,
        direction
    };

    sub_020537A8(taskManager, &location);
}

static BOOL Task_ScriptWarp(TaskManager *taskManager) {
    FieldSystem *fieldSystem = TaskManager_GetFieldSystem(taskManager);
    struct ErrorContinueEnv *env = TaskManager_GetEnvironment(taskManager);

    switch (env->state) {
    case 0:
        FieldBGM_TryFadeIn(fieldSystem, env->location.mapId);
        CallTask_LeaveOverworld(taskManager);
        env->state++;
        break;
    case 1:
        sub_020537A8(taskManager, &env->location);
        env->state++;
        break;
    case 2:
        if (GF_SndGetFadeTimer() != 0) {
            break;
        }
        FieldBGM_PlayForMapHeader(fieldSystem, env->location.mapId, FALSE);
        CallTask_RestoreOverworld(taskManager);
        env->state++;
        break;
    case 3:
        Heap_Free(env);
        return TRUE;
    }

    return FALSE;
}

void CallTask_ScriptWarp(TaskManager *taskManager, u32 mapId, int warpId, int x, int y, int direction) {
    struct ErrorContinueEnv *env = Heap_AllocAtEnd(HEAP_ID_FIELD2, sizeof(struct ErrorContinueEnv));
    env->state = 0;
    InitLocation(&env->location, mapId, warpId, x, y, direction);
    TaskManager_Call(taskManager, Task_ScriptWarp, env);
}

TaskManager *sub_020538C0(FieldSystem *fieldSystem, u32 mapId, int warpId, int x, int y, int direction) {
    Location location = {
        mapId,
        warpId,
        x,
        y,
        direction
    };
    struct UnkTaskEnv_02053950 *env = Heap_AllocAtEnd(HEAP_ID_FIELD2, sizeof(struct UnkTaskEnv_02053950));
    env->unk0 = 0;
    env->unk4 = 0;
    env->location = location;
    return FieldSystem_CreateTask(fieldSystem, sub_02053950, env);
}

void sub_02053908(TaskManager *taskManager, u32 mapId, int warpId, int x, int y, int direction) {
    Location location = {
        mapId,
        warpId,
        x,
        y,
        direction
    };
    struct UnkTaskEnv_02053950 *env = Heap_AllocAtEnd(HEAP_ID_FIELD2, sizeof(struct UnkTaskEnv_02053950));
    env->unk0 = 0;
    env->unk4 = NULL;
    env->location = location;
    TaskManager_Jump(taskManager, sub_02053950, env);
}

static BOOL sub_02053950(TaskManager *taskManager) {
    FieldSystem *fieldSystem = TaskManager_GetFieldSystem(taskManager);
    struct UnkTaskEnv_02053950 *env = TaskManager_GetEnvironment(taskManager);
    Location *location = &env->location;
    switch (env->unk0) {
    case 0:
        FieldBGM_TryFadeIn(fieldSystem, location->mapId);
        sub_020539D8(taskManager);
        env->unk0++;
        break;
    case 1:
        PlayerAvatar_SetState(fieldSystem->playerAvatar, PLAYER_STATE_WALKING);
        sub_020537A8(taskManager, &env->location);
        env->unk0++;
        break;
    case 2:
        if (GF_SndGetFadeTimer() != 0) {
            break;
        }
        FieldSystem_ResetStateOnFlyWarp(fieldSystem);
        sub_02053A14(taskManager);
        env->unk0++;
        break;
    case 3:
        Heap_Free(env);
        return TRUE;
    }

    return FALSE;
}

static void sub_020539D8(TaskManager *taskManager) {
    TaskManager_Call(taskManager, sub_020539E8, NULL);
}

static BOOL sub_020539E8(TaskManager *taskManager) {
    u32 *state_p = TaskManager_GetStatePtr(taskManager);

    switch (*state_p) {
    case 0:
        CallTask_LeaveOverworld(taskManager);
        (*state_p)++;
        break;
    case 1:
        return TRUE;
    }

    return FALSE;
}

static void sub_02053A14(TaskManager *taskManager) {
    TaskManager_Call(taskManager, sub_02053A2C, TaskManager_GetEnvironment(taskManager));
}

static BOOL sub_02053A2C(TaskManager *taskManager) {
    u32 *state_p = TaskManager_GetStatePtr(taskManager);
    FieldSystem *fieldSystem = TaskManager_GetFieldSystem(taskManager);
    switch (*state_p) {
    case 0:
        CallTask_RestoreOverworld(taskManager);
        (*state_p)++;
        break;
    case 1:
        StopSEByHandle(5, 10);
        GF_SndHandleMoveVolume(0, 128, 15);
        fieldSystem->environmentSoundState = ENVIRONMENT_SOUND_NONE;
        FieldBGM_PlayForMapHeader(fieldSystem, fieldSystem->location->mapId, TRUE);
        FieldSystem_DrawMapNameAnimation(fieldSystem);
        sub_02053AA0(taskManager);
        (*state_p)++;
        break;
    case 2:
        return TRUE;
    }

    return FALSE;
}

static void sub_02053AA0(TaskManager *taskManager) {
    FieldSystem *fieldSystem = TaskManager_GetFieldSystem(taskManager);
    struct UnkTaskEnv_02053950 *env = TaskManager_GetEnvironment(taskManager);
    if (!sub_0203DF7C(fieldSystem)) {
        GF_ASSERT(FALSE);
        return;
    }
    env->unk4 = FlyLanding_Start(fieldSystem, PlayerAvatar_GetGender(fieldSystem->playerAvatar));
    TaskManager_Call(taskManager, sub_02053AE4, env);
}

static BOOL sub_02053AE4(TaskManager *taskManager) {
    struct UnkTaskEnv_02053950 *env = TaskManager_GetEnvironment(taskManager);
    if (FlyLanding_IsDone(env->unk4) == TRUE) {
        FlyLanding_Delete(env->unk4);
        return TRUE;
    }

    return FALSE;
}

void sub_02053B04(TaskManager *taskManager, Location *location, int a2) {
    struct UnkTaskEnv_02053B3C *env = Heap_AllocAtEnd(HEAP_ID_FIELD2, sizeof(struct UnkTaskEnv_02053B3C));
    env->unk0 = 0;
    env->unk4 = a2;
    env->unk8 = NULL;
    env->location = *location;
    TaskManager_Jump(taskManager, sub_02053B3C, env);
}

static BOOL sub_02053B3C(TaskManager *taskManager) {
    FieldSystem *fieldSystem = TaskManager_GetFieldSystem(taskManager);
    struct UnkTaskEnv_02053B3C *env = TaskManager_GetEnvironment(taskManager);
    Location *location = &env->location;

    switch (env->unk0) {
    case 0:
        FieldBGM_TryFadeIn(fieldSystem, location->mapId);
        sub_02053BE8(taskManager);
        env->unk0++;
        break;
    case 1:
        PlayerAvatar_SetState(fieldSystem->playerAvatar, PLAYER_STATE_WALKING);
        sub_020537A8(taskManager, &env->location);
        env->unk0++;
        break;
    case 2:
        if (GF_SndGetFadeTimer() != 0) {
            break;
        }
        FieldBGM_PlayForMapHeader(fieldSystem, location->mapId, FALSE);
        if (env->unk4 == 2) {
            sub_02067BA4(fieldSystem);
        } else if (env->unk4 == 0 || env->unk4 == 1) {
            FieldSystem_ClearSafariSysFlag(fieldSystem);
        } else {
            GF_ASSERT(FALSE);
        }
        sub_02053C24(taskManager);
        env->unk0++;
        break;
    case 3:
        Heap_Free(env);
        return TRUE;
    }

    return FALSE;
}

static void sub_02053BE8(TaskManager *taskManager) {
    TaskManager_Call(taskManager, sub_02053BF8, NULL);
}

static BOOL sub_02053BF8(TaskManager *taskManager) {
    u32 *state_p = TaskManager_GetStatePtr(taskManager);

    switch (*state_p) {
    case 0:
        CallTask_LeaveOverworld(taskManager);
        (*state_p)++;
        break;
    case 1:
        return TRUE;
    }

    return FALSE;
}

static void sub_02053C24(TaskManager *taskManager) {
    struct UnkTaskEnv_02053B3C *env = TaskManager_GetEnvironment(taskManager);
    TaskManager_Call(taskManager, sub_02053C3C, env);
}

static BOOL sub_02053C3C(TaskManager *taskManager) {
    u32 *state_p = TaskManager_GetStatePtr(taskManager);
    FieldSystem *fieldSystem = TaskManager_GetFieldSystem(taskManager);
    struct UnkTaskEnv_02053B3C *env = TaskManager_GetEnvironment(taskManager); // unused

    switch (*state_p) {
    case 0:
        CallTask_RestoreOverworld(taskManager);
        (*state_p)++;
        break;
    case 1:
        FieldSystem_DrawMapNameAnimation(fieldSystem);
        sub_02053C90(taskManager);
        (*state_p)++;
        break;
    case 2:
        return TRUE;
    }

    return FALSE;
}

static void sub_02053C90(TaskManager *taskManager) {
    FieldSystem *fieldSystem = TaskManager_GetFieldSystem(taskManager);
    struct UnkTaskEnv_02053B3C *env = TaskManager_GetEnvironment(taskManager);

    if (!sub_0203DF7C(fieldSystem)) {
        GF_ASSERT(FALSE);
        return;
    }
    TaskManager_Call(taskManager, Task_FieldMoveArrival, FieldMoveTask_CreateArrivalEnvironment(fieldSystem, HEAP_ID_FIELD1, env->unk4));
}

static BOOL sub_02053CCC(TaskManager *taskManager) {
    FieldSystem *fieldSystem = TaskManager_GetFieldSystem(taskManager);
    struct UnkTaskEnv_02053CCC *env = TaskManager_GetEnvironment(taskManager);
    Location *location = &env->location;
    struct LocalMapObject *follow_mon_obj;

    switch (env->unk0) {
    case 0:
        env->unk1C = 0;
        if (FollowMon_IsActive(fieldSystem)
            && !FollowMon_CheckVisibleFlag(fieldSystem)
            && PlayerAvatar_GetState(fieldSystem->playerAvatar) != 1) {
            env->unk1C = FollowMonBall_Spawn(FollowMon_GetMapObject(fieldSystem), 1);
        }
        env->unk0++;
        break;
    case 1:
        if (env->unk1C == 0 || !FieldEffect_IsActive(env->unk1C)) {
            FieldSystem_StartWarpSpin(fieldSystem, 1, &env->unk4);
            env->unk0++;
        }
        break;
    case 2:
        if (env->unk4) {
            FieldBGM_TryFadeIn(fieldSystem, location->mapId);
            CallTask_LeaveOverworld(taskManager);
            env->unk0++;
        }
        break;
    case 3:
        sub_020537A8(taskManager, location);
        env->unk0++;
        break;
    case 4:
        if (GF_SndGetFadeTimer() != 0) {
            break;
        }
        FieldBGM_PlayForMapHeader(fieldSystem, location->mapId, FALSE);
        CallTask_RestoreOverworld(taskManager);
        env->unk0++;
        break;
    case 5:
        env->unk4 = 0;
        FieldSystem_StartWarpSpin(fieldSystem, 0, &env->unk4);
        if (FollowMon_IsActive(fieldSystem)) {
            follow_mon_obj = FollowMon_GetMapObject(fieldSystem);
            if (FollowMon_GetPermissionBySpeciesAndMap(FollowMon_GetSpecies(follow_mon_obj), location->mapId)) {
                sub_02069E84(follow_mon_obj, 1);
                FollowMon_MoveToPlayer(fieldSystem, 1);
            }
        }
        env->unk0++;
        break;
    case 6:
        if (env->unk4) {
            env->unk0++;
        }
        break;
    case 7:
        Heap_Free(env);
        return TRUE;
    }

    return FALSE;
}

TaskManager *sub_02053E08(FieldSystem *fieldSystem, u32 mapId, int warpId) {
    struct UnkTaskEnv_02053CCC *env;
    Location location;
    int direction;

    env = Heap_AllocAtEnd(HEAP_ID_FIELD2, sizeof(struct UnkTaskEnv_02053CCC));
    MI_CpuClear8(env, sizeof(struct UnkTaskEnv_02053CCC));
    direction = PlayerAvatar_GetFacingDirection(fieldSystem->playerAvatar);
    InitLocation(&location, mapId, warpId, 0, 0, direction);
    env->location = location;
    return FieldSystem_CreateTask(fieldSystem, sub_02053CCC, env);
}

static BOOL sub_02053E5C(TaskManager *taskManager) {
    FieldSystem *fieldSystem = TaskManager_GetFieldSystem(taskManager);
    struct UnkTaskEnv_02053E5C *env = TaskManager_GetEnvironment(taskManager);
    u32 *state_p = TaskManager_GetStatePtr(taskManager);
    Location *location = &env->location;

    switch (*state_p) {
    case 0:
        FieldBGM_TryFadeIn(fieldSystem, location->mapId);
        FieldSystem_StartWarpSpin(fieldSystem, 1, &env->unk4);
        (*state_p)++;
        break;
    case 1:
        if (env->unk4) {
            CallTask_LeaveOverworld(taskManager);
            (*state_p)++;
        }
        break;
    case 2:
        sub_020537A8(taskManager, location);
        (*state_p)++;
        break;
    case 3:
        if (GF_SndGetFadeTimer() != 0) {
            break;
        }
        FieldBGM_PlayForMapHeader(fieldSystem, location->mapId, FALSE);
        CallTask_RestoreOverworld(taskManager);
        (*state_p)++;
        break;
    case 4:
        CallTask_FadeFromBlack(taskManager);
        (*state_p)++;
        break;
    case 5:
        Heap_Free(env);
        return TRUE;
    }

    return FALSE;
}

void sub_02053F14(FieldSystem *fieldSystem) {
    Location *location = LocalFieldData_GetDynamicWarp(Save_LocalFieldData_Get(fieldSystem->saveData));
    struct UnkTaskEnv_02053E5C *env = Heap_AllocAtEnd(HEAP_ID_FIELD2, sizeof(struct UnkTaskEnv_02053E5C));
    MI_CpuClear8(env, sizeof(struct UnkTaskEnv_02053E5C));
    env->location = *location;
    UnionRoom_Stop(fieldSystem);
    UnionRoomObjects_Delete(fieldSystem->unk84);
    fieldSystem->mapLoadType = 0;
    FieldSystem_CreateTask(fieldSystem, sub_02053E5C, env);
    fieldSystem->unk80 = NULL;
}

static BOOL sub_02053F70(TaskManager *taskManager) {
    FieldSystem *fieldSystem = TaskManager_GetFieldSystem(taskManager);
    struct UnkTaskEnv_02053E5C *env = TaskManager_GetEnvironment(taskManager);
    u32 *state_p = TaskManager_GetStatePtr(taskManager);
    Location *location = &env->location;

    switch (*state_p) {
    case 0:
        FieldBGM_TryFadeIn(fieldSystem, location->mapId);
        PaletteFadeUntilFinished(taskManager);
        (*state_p)++;
        break;
    case 1:
        CallTask_LeaveOverworld(taskManager);
        (*state_p)++;
        break;
    case 2:
        sub_020537A8(taskManager, location);
        (*state_p)++;
        break;
    case 3:
        if (GF_SndGetFadeTimer() != 0) {
            break;
        }
        FieldBGM_PlayForMapHeader(fieldSystem, location->mapId, FALSE);
        CallTask_RestoreOverworld(taskManager);
        (*state_p)++;
        break;
    case 4:
        env->unk4 = 0;
        FieldSystem_StartWarpSpin(fieldSystem, 0, &env->unk4);
        (*state_p)++;
        break;
    case 5:
        if (env->unk4) {
            (*state_p)++;
        }
        break;
    case 6:
        Heap_Free(env);
        return TRUE;
    }

    return FALSE;
}

void sub_02054030(TaskManager *taskManager) {
    FieldSystem *fieldSystem = TaskManager_GetFieldSystem(taskManager);
    Location *location = LocalFieldData_GetDynamicWarp(Save_LocalFieldData_Get(fieldSystem->saveData));
    struct UnkTaskEnv_02053E5C *env = Heap_AllocAtEnd(HEAP_ID_FIELD2, sizeof(struct UnkTaskEnv_02053E5C));
    MI_CpuClear8(env, sizeof(struct UnkTaskEnv_02053E5C));
    _CopyPlayerPosToLocationWorkFacingSouth(location, fieldSystem);
    InitLocation(&env->location, MAP_UNION, -1, 8, 14, DIR_NORTH);
    fieldSystem->unk80 = UnionRoom_Start(fieldSystem);
    fieldSystem->unk84 = UnionRoomObjects_New(fieldSystem->unk80);
    fieldSystem->mapLoadType = MAP_LOAD_TYPE_UNION;
    TaskManager_Call(taskManager, sub_02053F70, env);
}

static BOOL sub_020540A4(TaskManager *taskManager) {
    FieldSystem *fieldSystem = TaskManager_GetFieldSystem(taskManager);
    struct UnkTaskEnv_02053688 *env = TaskManager_GetEnvironment(taskManager);

    switch (env->unk0) {
    case 0:
        PlaySE(SEQ_SE_DP_KAIDAN2);
        FieldBGM_TryFadeIn(fieldSystem, env->location.mapId);
        CallTask_FadeOutAndLeaveOverworld(taskManager);
        env->unk0++;
        break;
    case 1:
        sub_020537A8(taskManager, &env->location);
        env->unk0++;
        break;
    case 2:
        if (GF_SndGetFadeTimer() != 0) {
            break;
        }
        FieldBGM_PlayForMapHeader(fieldSystem, env->location.mapId, FALSE);
        CallTask_RestoreOverworld(taskManager);
        env->unk0++;
        break;
    case 3:
        Heap_Free(env);
        return TRUE;
    }

    return FALSE;
}

void sub_0205412C(TaskManager *taskManager, u32 mapId, int warpId, int x, int y, int direction) {
    struct UnkTaskEnv_02053688 *env;
    FieldSystem *fieldSystem = TaskManager_GetFieldSystem(taskManager);
    _CopyPlayerPosToLocationWorkFacingSouth(LocalFieldData_GetDynamicWarp(Save_LocalFieldData_Get(fieldSystem->saveData)), fieldSystem);
    fieldSystem->mapLoadType = MAP_LOAD_TYPE_COLOSSEUM;
    env = Heap_AllocAtEnd(HEAP_ID_FIELD2, sizeof(struct UnkTaskEnv_02053688));
    {
        Location location = {
            mapId,
            warpId,
            x,
            y,
            direction
        };
        env->unk0 = 0;
        env->location = location;
    }
    TaskManager_Call(taskManager, sub_020540A4, env);
}

void sub_02054190(TaskManager *taskManager) {
    FieldSystem *fieldSystem = TaskManager_GetFieldSystem(taskManager);
    Location *location = LocalFieldData_GetDynamicWarp(Save_LocalFieldData_Get(fieldSystem->saveData));
    fieldSystem->mapLoadType = 0;
    sub_02053710(fieldSystem->taskman, location);
}
