#include "area_light_tasks_internal.h"
#include "field_bgm.h"
#include "field_time_events.h"
#include "field_warp_tasks.h"
#include "heap.h"
#include "screen_fade.h"
#include "sound.h"
#include "sound_playback.h"
#include "sound_system.h"

enum ShipVoyageState {
    SHIP_VOYAGE_FADE_OUT,
    SHIP_VOYAGE_LEAVE_OVERWORLD,
    SHIP_VOYAGE_DEPARTURE_CUTSCENE,
    SHIP_VOYAGE_CHANGE_MAP,
    SHIP_VOYAGE_RETURN_TO_OVERWORLD,
    SHIP_VOYAGE_FREE,
};

static void CallTask_ShipDepartureCutscene(FieldSystem *fieldSystem, u8 port, void *modelAttributes);
static BOOL Task_ShipDepartureCutscene(TaskManager *taskManager);
static BOOL Task_ShipVoyage(TaskManager *taskManager);

void CallTask_ShipVoyage(FieldSystem *fieldSystem, u8 port, u8 direction, u32 mapId, u32 x, u32 y) {
    AreaLightTaskEnv *env = Heap_AllocAtEnd(HEAP_ID_FIELD2, sizeof(AreaLightTaskEnv));

    env->variant = port;
    env->direction = direction;
    env->mapId = mapId;
    env->x = x;
    env->y = y;
    LoadAreaOrDungeonLightTxt(0, &env->light);
    env->state = SHIP_VOYAGE_FADE_OUT;
    TaskManager_Call(fieldSystem->taskman, Task_ShipVoyage, env);
}

static void CallTask_ShipDepartureCutscene(FieldSystem *fieldSystem, u8 port, void *modelAttributes) {
    NightTaskEnv *env = Heap_AllocAtEnd(HEAP_ID_FIELD2, sizeof(NightTaskEnv));

    env->modelAttributes = modelAttributes;
    env->port = port;
    switch (Field_GetTimeOfDay(fieldSystem)) {
    case RTC_TIMEOFDAY_MORN:
    case RTC_TIMEOFDAY_DAY:
        env->night = FALSE;
        break;
    case RTC_TIMEOFDAY_EVE:
    case RTC_TIMEOFDAY_NITE:
    case RTC_TIMEOFDAY_LATE:
        env->night = TRUE;
        break;
    default:
        GF_ASSERT(FALSE);
        env->night = FALSE;
        break;
    }
    TaskManager_Call(fieldSystem->taskman, Task_ShipDepartureCutscene, env);
}

static BOOL Task_ShipDepartureCutscene(TaskManager *taskManager) {
    FieldSystem *fieldSystem = TaskManager_GetFieldSystem(taskManager);
    NightTaskEnv *env = TaskManager_GetEnvironment(taskManager);
    u32 *state = TaskManager_GetStatePtr(taskManager);

    switch (*state) {
    case 0:
        ShipDepartureCutscene_LaunchApp(fieldSystem, env);
        break;
    case 1:
        if (FieldSystem_ApplicationIsRunning(fieldSystem)) {
            return FALSE;
        }
        break;
    case 2:
        Heap_Free(env);
        return TRUE;
    }
    (*state)++;
    return FALSE;
}

static BOOL Task_ShipVoyage(TaskManager *taskManager) {
    FieldSystem *fieldSystem = TaskManager_GetFieldSystem(taskManager);
    AreaLightTaskEnv *env = TaskManager_GetEnvironment(taskManager);

    switch (env->state) {
    case SHIP_VOYAGE_FADE_OUT:
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, 0, 6, 1, HEAP_ID_FIELD2);
        GF_SndStartFadeOutBGM(0, 6);
        env->state = SHIP_VOYAGE_LEAVE_OVERWORLD;
        break;
    case SHIP_VOYAGE_LEAVE_OVERWORLD:
        if (!IsPaletteFadeFinished() || GF_SndGetFadeTimer() != 0) {
            return FALSE;
        }
        StopBGM(GF_GetCurrentPlayingBGM(), 0);
        CallTask_LeaveOverworld(taskManager);
        env->state = SHIP_VOYAGE_DEPARTURE_CUTSCENE;
        break;
    case SHIP_VOYAGE_DEPARTURE_CUTSCENE:
        CallTask_ShipDepartureCutscene(fieldSystem, env->variant, &env->light);
        env->state = SHIP_VOYAGE_CHANGE_MAP;
        break;
    case SHIP_VOYAGE_CHANGE_MAP:
        CallTask_ChangeMapToCoords(taskManager, env->mapId, -1, env->x, env->y, env->direction);
        env->state = SHIP_VOYAGE_RETURN_TO_OVERWORLD;
        break;
    case SHIP_VOYAGE_RETURN_TO_OVERWORLD:
        FieldBGM_PlayForMapHeader(fieldSystem, env->mapId, FALSE);
        CallTask_RestoreOverworldAndFadeIn(taskManager);
        env->state = SHIP_VOYAGE_FREE;
        break;
    case SHIP_VOYAGE_FREE:
        Heap_Free(env);
        return TRUE;
    }
    return FALSE;
}
