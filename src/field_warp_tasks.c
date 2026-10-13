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

struct WarpWithStairsSoundEnv {
    int state;
    Location location;
    int unk18;
};

struct FlyWarpEnv {
    int state;
    SysTask *flyLanding;
    Location location;
    u8 padding[4];
};

struct FieldMoveWarpEnv {
    int state;
    int kind;
    SysTask *unk8;
    Location location;
};

struct WarpPanelEnv {
    int state;
    int spinDone;
    Location location;
    FieldEffect *followMonBall;
};

struct UnionRoomWarpEnv {
    int unk0;
    int spinDone;
    Location location;
};

static void FieldSystem_UpdateMapLoadTypeForMap(FieldSystem *fieldSystem);
static void FieldSystem_EnterLocation(FieldSystem *fieldSystem, Location *location);
static void FieldSystem_CreatePlayerAndMapObjects(FieldSystem *fieldSystem);
static void FieldSystem_DeletePlayerAndMapObjects(FieldSystem *fieldSystem);
static void FieldSystem_RestorePlayerAndMapObjects(FieldSystem *fieldSystem);
static void FieldSystem_LoadMapMatrixAndMode(FieldSystem *fieldSystem);
static void FieldSystem_UnloadMapMode(FieldSystem *fieldSystem);
static void _CopyPlayerPosToLocationWorkFacingSouth(Location *location, FieldSystem *fieldSystem);
static BOOL _IsPlayerStandingInFrontOfUnionRoomReception(FieldSystem *fieldSystem);
static void _SetDynamicWarpToUnionRoomExit(FieldSystem *fieldSystem);
static BOOL FieldTask_NewGame(TaskManager *taskManager);
static BOOL FieldTask_ContinueGame_Normal(TaskManager *taskManager);
static BOOL FieldTask_ContinueGame_CommError(TaskManager *taskManager);
static BOOL Task_WarpWithStairsSound(TaskManager *taskManager);
static void CallTask_WarpWithStairsSound(TaskManager *taskManager, Location *location);
static BOOL Task_ChangeMap(TaskManager *taskManager);
static BOOL Task_ScriptWarp(TaskManager *taskManager);
static BOOL Task_FlyWarp(TaskManager *taskManager);
static void CallTask_FlyWarpLeaveOverworld(TaskManager *taskManager);
static BOOL Task_FlyWarpLeaveOverworld(TaskManager *taskManager);
static void CallTask_FlyWarpArrive(TaskManager *taskManager);
static BOOL Task_FlyWarpArrive(TaskManager *taskManager);
static void CallTask_FlyLanding(TaskManager *taskManager);
static BOOL Task_WaitFlyLanding(TaskManager *taskManager);
static BOOL Task_FieldMoveWarp(TaskManager *taskManager);
static void CallTask_FieldMoveWarpLeaveOverworld(TaskManager *taskManager);
static BOOL Task_FieldMoveWarpLeaveOverworld(TaskManager *taskManager);
static void CallTask_FieldMoveWarpArrive(TaskManager *taskManager);
static BOOL Task_FieldMoveWarpArrive(TaskManager *taskManager);
static void CallTask_FieldMoveArrivalAnim(TaskManager *taskManager);
static BOOL Task_WarpPanel(TaskManager *taskManager);
static BOOL Task_ExitUnionRoom(TaskManager *taskManager);
static BOOL Task_EnterUnionRoom(TaskManager *taskManager);
static BOOL Task_WarpToColosseum(TaskManager *taskManager);

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

static void FieldSystem_UpdateMapLoadTypeForMap(FieldSystem *fieldSystem) {
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

static void FieldSystem_EnterLocation(FieldSystem *fieldSystem, Location *location) {
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

void FieldSystem_InitMapOnEnter(FieldSystem *fieldSystem, BOOL isConnection) {
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
    if (Save_VarsFlags_CheckMapEventKey(varsFlags, 1) && mapId == MAP_LAKE_OF_RAGE) {
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

static void FieldSystem_CreatePlayerAndMapObjects(FieldSystem *fieldSystem) {
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
        MapObjectManager_SuspendMovement(fieldSystem->mapObjectManager);
    }
}

static void FieldSystem_DeletePlayerAndMapObjects(FieldSystem *fieldSystem) {
    if (fieldSystem->unkAC) {
        PlayerAvatar_DeleteFromMap(fieldSystem->playerAvatar);
    } else {
        CommPlayerManager_RemoveAllAvatars();
        PlayerAvatar_FreeToHeap(fieldSystem->playerAvatar);
        MapObjectManager_RemoveAllActiveObjects(fieldSystem->mapObjectManager);
        MapObjectManager_Delete(fieldSystem->mapObjectManager);
    }
}

static void FieldSystem_RestorePlayerAndMapObjects(FieldSystem *fieldSystem) {
    u32 gender;
    struct PlayerSaveData *playerSaveData;

    fieldSystem->mapObjectManager = MapObjectManager_Init(fieldSystem, 64, 5);
    FieldSystem_RestoreMapObjectsFromSave(fieldSystem);
    playerSaveData = LocalFieldData_GetPlayer(Save_LocalFieldData_Get(fieldSystem->saveData));
    gender = PlayerProfile_GetTrainerGender(Save_PlayerData_GetProfile(fieldSystem->saveData));
    fieldSystem->playerAvatar = PlayerAvatar_CreateWithActiveMapObject(fieldSystem->mapObjectManager, playerSaveData, gender);
    FollowMon_ChangeMon(fieldSystem->mapObjectManager, fieldSystem->location->mapId);
    MapObjectManager_SuspendMovement(fieldSystem->mapObjectManager);
}

static void FieldSystem_LoadMapMatrixAndMode(FieldSystem *fieldSystem) {
    SaveVarsFlags *varsFlags;

    FieldSystem_UpdateMapLoadTypeForMap(fieldSystem);

    GF_ASSERT(fieldSystem->terrainQueries == NULL);

    MapMatrix_Load(fieldSystem->location->mapId, fieldSystem->mapMatrix);
    varsFlags = Save_VarsFlags_Get(fieldSystem->saveData);
    if (Save_VarsFlags_CheckMapEventKey(varsFlags, 0)) {
        RemoveMahoganyTownAntennaTree(fieldSystem->mapMatrix);
    }
    SetLakeOfRageWaterLevel(fieldSystem->mapMatrix, Save_VarsFlags_CheckMapEventKey(varsFlags, 1));
    PlaceSafariZoneAreas(fieldSystem->mapMatrix, fieldSystem->saveData);

    GF_ASSERT(fieldSystem->mapLoadType < 6);

    fieldSystem->mapLoadMode = &sMapLoadModes[fieldSystem->mapLoadType];
    fieldSystem->skipMapAttributes = fieldSystem->mapLoadMode->skipMapAttributes;
    fieldSystem->bottomScreenType = fieldSystem->mapLoadMode->fieldBottomScreen;
    FieldSystem_SetTerrainQueries(&fieldSystem->terrainQueries, fieldSystem->mapLoadMode->useSimpleTerrainCollisions);
    if (fieldSystem->mapLoadMode->useSeparateTerrainAttributes) {
        TerrainAttributes_New(fieldSystem, fieldSystem->mapLoadMode->separateTerrainAttributesBlockCount);
    }
}

static void FieldSystem_UnloadMapMode(FieldSystem *fieldSystem) {
    GF_ASSERT(fieldSystem->terrainQueries != NULL);
    fieldSystem->terrainQueries = NULL;
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
        FieldSystem_EnterLocation(fieldSystem, fieldSystem->location);
        FieldSystem_LoadMapMatrixAndMode(fieldSystem);
        FieldSystem_InitMapOnEnter(fieldSystem, FALSE);
        FieldSystem_CreatePlayerAndMapObjects(fieldSystem);
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
            FieldSystem_EnterLocation(fieldSystem, LocalFieldData_GetDynamicWarp(localFieldData));
            FieldSystem_LoadMapMatrixAndMode(fieldSystem);
            FieldSystem_InitMapOnEnter(fieldSystem, FALSE);
            FieldSystem_CreatePlayerAndMapObjects(fieldSystem);
        } else {
            FieldSystem_EnterLocation(fieldSystem, NULL);
            FieldSystem_LoadMapMatrixAndMode(fieldSystem);
            FieldSystem_StartBugContestTimer(fieldSystem);
            FieldSystem_RestorePlayerAndMapObjects(fieldSystem);
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
        FieldSystem_EnterLocation(fieldSystem, &env->location);
        FieldSystem_LoadMapMatrixAndMode(fieldSystem);
        FieldSystem_InitMapOnEnter(fieldSystem, FALSE);
        FieldSystem_CreatePlayerAndMapObjects(fieldSystem);
        (*state_p)++;
        break;
    case 2:
        fieldSystem->unionRoom = UnionRoom_Start(fieldSystem);
        fieldSystem->unionRoomObjects = UnionRoomObjects_New(fieldSystem->unionRoom);
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

static BOOL Task_WarpWithStairsSound(TaskManager *taskManager) {
    FieldSystem *fieldSystem = TaskManager_GetFieldSystem(taskManager);
    struct WarpWithStairsSoundEnv *env = TaskManager_GetEnvironment(taskManager);

    switch (env->state) {
    case 0:
        PlaySE(SEQ_SE_DP_KAIDAN2);
        FieldBGM_TryFadeIn(fieldSystem, env->location.mapId);
        CallTask_FadeOutAndLeaveOverworld(taskManager);
        env->state++;
        break;
    case 1:
        CallTask_ChangeMap(taskManager, &env->location);
        env->state++;
        break;
    case 2:
        if (GF_SndGetFadeTimer() != 0) {
            break;
        }
        FieldBGM_PlayForMapHeader(fieldSystem, env->location.mapId, FALSE);
        CallTask_RestoreOverworldAndFadeIn(taskManager);
        env->state++;
        break;
    case 3:
        Heap_Free(env);
        return TRUE;
    }

    return FALSE;
}

static void CallTask_WarpWithStairsSound(TaskManager *taskManager, Location *location) {
    struct WarpWithStairsSoundEnv *env = Heap_AllocAtEnd(HEAP_ID_FIELD2, sizeof(struct WarpWithStairsSoundEnv));
    env->state = 0;
    env->location = *location;
    TaskManager_Call(taskManager, Task_WarpWithStairsSound, env);
}

static BOOL Task_ChangeMap(TaskManager *taskManager) {
    FieldSystem *fieldSystem = TaskManager_GetFieldSystem(taskManager);
    struct ErrorContinueEnv *env = TaskManager_GetEnvironment(taskManager);

    switch (env->state) {
    case 0:
        FieldSystem_DeletePlayerAndMapObjects(fieldSystem);
        FieldSystem_UnloadMapMode(fieldSystem);
        env->state++;
        break;
    case 1:
        FieldSystem_EnterLocation(fieldSystem, &env->location);
        FieldSystem_LoadMapMatrixAndMode(fieldSystem);
        FieldSystem_InitMapOnEnter(fieldSystem, FALSE);
        env->state++;
        break;
    case 2:
        FieldSystem_CreatePlayerAndMapObjects(fieldSystem);
        Heap_Free(env);
        return TRUE;
    }

    return FALSE;
}

void CallTask_ChangeMap(TaskManager *taskManager, const Location *location) {
    FieldSystem *fieldSystem = TaskManager_GetFieldSystem(taskManager);
    struct ErrorContinueEnv *env = Heap_AllocAtEnd(HEAP_ID_FIELD2, sizeof(struct ErrorContinueEnv));
    if (FieldSystem_IsFieldMapLoaded(fieldSystem)) {
        GF_ASSERT(FALSE);
        return;
    }
    env->state = 0;
    env->location = *location;
    TaskManager_Call(taskManager, Task_ChangeMap, env);
}

void CallTask_ChangeMapToCoords(TaskManager *taskManager, u32 mapId, int warpId, int x, int y, int direction) {
    Location location = {
        mapId,
        warpId,
        x,
        y,
        direction
    };

    CallTask_ChangeMap(taskManager, &location);
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
        CallTask_ChangeMap(taskManager, &env->location);
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

TaskManager *CallFieldTask_FlyWarp(FieldSystem *fieldSystem, u32 mapId, int warpId, int x, int y, int direction) {
    Location location = {
        mapId,
        warpId,
        x,
        y,
        direction
    };
    struct FlyWarpEnv *env = Heap_AllocAtEnd(HEAP_ID_FIELD2, sizeof(struct FlyWarpEnv));
    env->state = 0;
    env->flyLanding = 0;
    env->location = location;
    return FieldSystem_CreateTask(fieldSystem, Task_FlyWarp, env);
}

void JumpTask_FlyWarp(TaskManager *taskManager, u32 mapId, int warpId, int x, int y, int direction) {
    Location location = {
        mapId,
        warpId,
        x,
        y,
        direction
    };
    struct FlyWarpEnv *env = Heap_AllocAtEnd(HEAP_ID_FIELD2, sizeof(struct FlyWarpEnv));
    env->state = 0;
    env->flyLanding = NULL;
    env->location = location;
    TaskManager_Jump(taskManager, Task_FlyWarp, env);
}

static BOOL Task_FlyWarp(TaskManager *taskManager) {
    FieldSystem *fieldSystem = TaskManager_GetFieldSystem(taskManager);
    struct FlyWarpEnv *env = TaskManager_GetEnvironment(taskManager);
    Location *location = &env->location;
    switch (env->state) {
    case 0:
        FieldBGM_TryFadeIn(fieldSystem, location->mapId);
        CallTask_FlyWarpLeaveOverworld(taskManager);
        env->state++;
        break;
    case 1:
        PlayerAvatar_SetState(fieldSystem->playerAvatar, PLAYER_STATE_WALKING);
        CallTask_ChangeMap(taskManager, &env->location);
        env->state++;
        break;
    case 2:
        if (GF_SndGetFadeTimer() != 0) {
            break;
        }
        FieldSystem_ResetStateOnFlyWarp(fieldSystem);
        CallTask_FlyWarpArrive(taskManager);
        env->state++;
        break;
    case 3:
        Heap_Free(env);
        return TRUE;
    }

    return FALSE;
}

static void CallTask_FlyWarpLeaveOverworld(TaskManager *taskManager) {
    TaskManager_Call(taskManager, Task_FlyWarpLeaveOverworld, NULL);
}

static BOOL Task_FlyWarpLeaveOverworld(TaskManager *taskManager) {
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

static void CallTask_FlyWarpArrive(TaskManager *taskManager) {
    TaskManager_Call(taskManager, Task_FlyWarpArrive, TaskManager_GetEnvironment(taskManager));
}

static BOOL Task_FlyWarpArrive(TaskManager *taskManager) {
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
        CallTask_FlyLanding(taskManager);
        (*state_p)++;
        break;
    case 2:
        return TRUE;
    }

    return FALSE;
}

static void CallTask_FlyLanding(TaskManager *taskManager) {
    FieldSystem *fieldSystem = TaskManager_GetFieldSystem(taskManager);
    struct FlyWarpEnv *env = TaskManager_GetEnvironment(taskManager);
    if (!FieldSystem_IsFieldMapLoaded(fieldSystem)) {
        GF_ASSERT(FALSE);
        return;
    }
    env->flyLanding = FlyLanding_Start(fieldSystem, PlayerAvatar_GetGender(fieldSystem->playerAvatar));
    TaskManager_Call(taskManager, Task_WaitFlyLanding, env);
}

static BOOL Task_WaitFlyLanding(TaskManager *taskManager) {
    struct FlyWarpEnv *env = TaskManager_GetEnvironment(taskManager);
    if (FlyLanding_IsDone(env->flyLanding) == TRUE) {
        FlyLanding_Delete(env->flyLanding);
        return TRUE;
    }

    return FALSE;
}

void JumpTask_FieldMoveWarp(TaskManager *taskManager, Location *location, int kind) {
    struct FieldMoveWarpEnv *env = Heap_AllocAtEnd(HEAP_ID_FIELD2, sizeof(struct FieldMoveWarpEnv));
    env->state = 0;
    env->kind = kind;
    env->unk8 = NULL;
    env->location = *location;
    TaskManager_Jump(taskManager, Task_FieldMoveWarp, env);
}

static BOOL Task_FieldMoveWarp(TaskManager *taskManager) {
    FieldSystem *fieldSystem = TaskManager_GetFieldSystem(taskManager);
    struct FieldMoveWarpEnv *env = TaskManager_GetEnvironment(taskManager);
    Location *location = &env->location;

    switch (env->state) {
    case 0:
        FieldBGM_TryFadeIn(fieldSystem, location->mapId);
        CallTask_FieldMoveWarpLeaveOverworld(taskManager);
        env->state++;
        break;
    case 1:
        PlayerAvatar_SetState(fieldSystem->playerAvatar, PLAYER_STATE_WALKING);
        CallTask_ChangeMap(taskManager, &env->location);
        env->state++;
        break;
    case 2:
        if (GF_SndGetFadeTimer() != 0) {
            break;
        }
        FieldBGM_PlayForMapHeader(fieldSystem, location->mapId, FALSE);
        if (env->kind == 2) {
            FieldSystem_ClearSafariAndRandomizeRoamers(fieldSystem);
        } else if (env->kind == 0 || env->kind == 1) {
            FieldSystem_ClearSafariSysFlag(fieldSystem);
        } else {
            GF_ASSERT(FALSE);
        }
        CallTask_FieldMoveWarpArrive(taskManager);
        env->state++;
        break;
    case 3:
        Heap_Free(env);
        return TRUE;
    }

    return FALSE;
}

static void CallTask_FieldMoveWarpLeaveOverworld(TaskManager *taskManager) {
    TaskManager_Call(taskManager, Task_FieldMoveWarpLeaveOverworld, NULL);
}

static BOOL Task_FieldMoveWarpLeaveOverworld(TaskManager *taskManager) {
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

static void CallTask_FieldMoveWarpArrive(TaskManager *taskManager) {
    struct FieldMoveWarpEnv *env = TaskManager_GetEnvironment(taskManager);
    TaskManager_Call(taskManager, Task_FieldMoveWarpArrive, env);
}

static BOOL Task_FieldMoveWarpArrive(TaskManager *taskManager) {
    u32 *state_p = TaskManager_GetStatePtr(taskManager);
    FieldSystem *fieldSystem = TaskManager_GetFieldSystem(taskManager);
    struct FieldMoveWarpEnv *env = TaskManager_GetEnvironment(taskManager); // unused

    switch (*state_p) {
    case 0:
        CallTask_RestoreOverworld(taskManager);
        (*state_p)++;
        break;
    case 1:
        FieldSystem_DrawMapNameAnimation(fieldSystem);
        CallTask_FieldMoveArrivalAnim(taskManager);
        (*state_p)++;
        break;
    case 2:
        return TRUE;
    }

    return FALSE;
}

static void CallTask_FieldMoveArrivalAnim(TaskManager *taskManager) {
    FieldSystem *fieldSystem = TaskManager_GetFieldSystem(taskManager);
    struct FieldMoveWarpEnv *env = TaskManager_GetEnvironment(taskManager);

    if (!FieldSystem_IsFieldMapLoaded(fieldSystem)) {
        GF_ASSERT(FALSE);
        return;
    }
    TaskManager_Call(taskManager, Task_FieldMoveArrival, FieldMoveTask_CreateArrivalEnvironment(fieldSystem, HEAP_ID_FIELD1, env->kind));
}

static BOOL Task_WarpPanel(TaskManager *taskManager) {
    FieldSystem *fieldSystem = TaskManager_GetFieldSystem(taskManager);
    struct WarpPanelEnv *env = TaskManager_GetEnvironment(taskManager);
    Location *location = &env->location;
    struct LocalMapObject *follow_mon_obj;

    switch (env->state) {
    case 0:
        env->followMonBall = 0;
        if (FollowMon_IsActive(fieldSystem)
            && !FollowMon_CheckVisibleFlag(fieldSystem)
            && PlayerAvatar_GetState(fieldSystem->playerAvatar) != 1) {
            env->followMonBall = FollowMonBall_Spawn(FollowMon_GetMapObject(fieldSystem), 1);
        }
        env->state++;
        break;
    case 1:
        if (env->followMonBall == 0 || !FieldEffect_IsActive(env->followMonBall)) {
            FieldSystem_StartWarpSpin(fieldSystem, 1, &env->spinDone);
            env->state++;
        }
        break;
    case 2:
        if (env->spinDone) {
            FieldBGM_TryFadeIn(fieldSystem, location->mapId);
            CallTask_LeaveOverworld(taskManager);
            env->state++;
        }
        break;
    case 3:
        CallTask_ChangeMap(taskManager, location);
        env->state++;
        break;
    case 4:
        if (GF_SndGetFadeTimer() != 0) {
            break;
        }
        FieldBGM_PlayForMapHeader(fieldSystem, location->mapId, FALSE);
        CallTask_RestoreOverworld(taskManager);
        env->state++;
        break;
    case 5:
        env->spinDone = 0;
        FieldSystem_StartWarpSpin(fieldSystem, 0, &env->spinDone);
        if (FollowMon_IsActive(fieldSystem)) {
            follow_mon_obj = FollowMon_GetMapObject(fieldSystem);
            if (FollowMon_GetPermissionBySpeciesAndMap(FollowMon_GetSpecies(follow_mon_obj), location->mapId)) {
                FollowMon_SetReleaseBallPending(follow_mon_obj, 1);
                FollowMon_MoveToPlayer(fieldSystem, 1);
            }
        }
        env->state++;
        break;
    case 6:
        if (env->spinDone) {
            env->state++;
        }
        break;
    case 7:
        Heap_Free(env);
        return TRUE;
    }

    return FALSE;
}

TaskManager *CallFieldTask_WarpPanel(FieldSystem *fieldSystem, u32 mapId, int warpId) {
    struct WarpPanelEnv *env;
    Location location;
    int direction;

    env = Heap_AllocAtEnd(HEAP_ID_FIELD2, sizeof(struct WarpPanelEnv));
    MI_CpuClear8(env, sizeof(struct WarpPanelEnv));
    direction = PlayerAvatar_GetFacingDirection(fieldSystem->playerAvatar);
    InitLocation(&location, mapId, warpId, 0, 0, direction);
    env->location = location;
    return FieldSystem_CreateTask(fieldSystem, Task_WarpPanel, env);
}

static BOOL Task_ExitUnionRoom(TaskManager *taskManager) {
    FieldSystem *fieldSystem = TaskManager_GetFieldSystem(taskManager);
    struct UnionRoomWarpEnv *env = TaskManager_GetEnvironment(taskManager);
    u32 *state_p = TaskManager_GetStatePtr(taskManager);
    Location *location = &env->location;

    switch (*state_p) {
    case 0:
        FieldBGM_TryFadeIn(fieldSystem, location->mapId);
        FieldSystem_StartWarpSpin(fieldSystem, 1, &env->spinDone);
        (*state_p)++;
        break;
    case 1:
        if (env->spinDone) {
            CallTask_LeaveOverworld(taskManager);
            (*state_p)++;
        }
        break;
    case 2:
        CallTask_ChangeMap(taskManager, location);
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

void CallFieldTask_ExitUnionRoom(FieldSystem *fieldSystem) {
    Location *location = LocalFieldData_GetDynamicWarp(Save_LocalFieldData_Get(fieldSystem->saveData));
    struct UnionRoomWarpEnv *env = Heap_AllocAtEnd(HEAP_ID_FIELD2, sizeof(struct UnionRoomWarpEnv));
    MI_CpuClear8(env, sizeof(struct UnionRoomWarpEnv));
    env->location = *location;
    UnionRoom_Stop(fieldSystem);
    UnionRoomObjects_Delete(fieldSystem->unionRoomObjects);
    fieldSystem->mapLoadType = 0;
    FieldSystem_CreateTask(fieldSystem, Task_ExitUnionRoom, env);
    fieldSystem->unionRoom = NULL;
}

static BOOL Task_EnterUnionRoom(TaskManager *taskManager) {
    FieldSystem *fieldSystem = TaskManager_GetFieldSystem(taskManager);
    struct UnionRoomWarpEnv *env = TaskManager_GetEnvironment(taskManager);
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
        CallTask_ChangeMap(taskManager, location);
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
        env->spinDone = 0;
        FieldSystem_StartWarpSpin(fieldSystem, 0, &env->spinDone);
        (*state_p)++;
        break;
    case 5:
        if (env->spinDone) {
            (*state_p)++;
        }
        break;
    case 6:
        Heap_Free(env);
        return TRUE;
    }

    return FALSE;
}

void CallTask_EnterUnionRoom(TaskManager *taskManager) {
    FieldSystem *fieldSystem = TaskManager_GetFieldSystem(taskManager);
    Location *location = LocalFieldData_GetDynamicWarp(Save_LocalFieldData_Get(fieldSystem->saveData));
    struct UnionRoomWarpEnv *env = Heap_AllocAtEnd(HEAP_ID_FIELD2, sizeof(struct UnionRoomWarpEnv));
    MI_CpuClear8(env, sizeof(struct UnionRoomWarpEnv));
    _CopyPlayerPosToLocationWorkFacingSouth(location, fieldSystem);
    InitLocation(&env->location, MAP_UNION, -1, 8, 14, DIR_NORTH);
    fieldSystem->unionRoom = UnionRoom_Start(fieldSystem);
    fieldSystem->unionRoomObjects = UnionRoomObjects_New(fieldSystem->unionRoom);
    fieldSystem->mapLoadType = MAP_LOAD_TYPE_UNION;
    TaskManager_Call(taskManager, Task_EnterUnionRoom, env);
}

static BOOL Task_WarpToColosseum(TaskManager *taskManager) {
    FieldSystem *fieldSystem = TaskManager_GetFieldSystem(taskManager);
    struct WarpWithStairsSoundEnv *env = TaskManager_GetEnvironment(taskManager);

    switch (env->state) {
    case 0:
        PlaySE(SEQ_SE_DP_KAIDAN2);
        FieldBGM_TryFadeIn(fieldSystem, env->location.mapId);
        CallTask_FadeOutAndLeaveOverworld(taskManager);
        env->state++;
        break;
    case 1:
        CallTask_ChangeMap(taskManager, &env->location);
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

void CallTask_WarpToColosseum(TaskManager *taskManager, u32 mapId, int warpId, int x, int y, int direction) {
    struct WarpWithStairsSoundEnv *env;
    FieldSystem *fieldSystem = TaskManager_GetFieldSystem(taskManager);
    _CopyPlayerPosToLocationWorkFacingSouth(LocalFieldData_GetDynamicWarp(Save_LocalFieldData_Get(fieldSystem->saveData)), fieldSystem);
    fieldSystem->mapLoadType = MAP_LOAD_TYPE_COLOSSEUM;
    env = Heap_AllocAtEnd(HEAP_ID_FIELD2, sizeof(struct WarpWithStairsSoundEnv));
    {
        Location location = {
            mapId,
            warpId,
            x,
            y,
            direction
        };
        env->state = 0;
        env->location = location;
    }
    TaskManager_Call(taskManager, Task_WarpToColosseum, env);
}

void CallTask_ReturnFromColosseum(TaskManager *taskManager) {
    FieldSystem *fieldSystem = TaskManager_GetFieldSystem(taskManager);
    Location *location = LocalFieldData_GetDynamicWarp(Save_LocalFieldData_Get(fieldSystem->saveData));
    fieldSystem->mapLoadType = 0;
    CallTask_WarpWithStairsSound(fieldSystem->taskman, location);
}
