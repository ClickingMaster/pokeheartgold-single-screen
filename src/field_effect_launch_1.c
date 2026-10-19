#include "field_bgm.h"
#include "field_effect_launch_internal.h"
#include "field_time_events.h"
#include "field_warp_tasks.h"
#include "heap.h"
#include "player_data.h"
#include "screen_fade.h"
#include "sound.h"
#include "sound_playback.h"
#include "sound_system.h"

enum MagnetTrainRideState {
    MAGNET_TRAIN_RIDE_FADE_OUT,
    MAGNET_TRAIN_RIDE_LEAVE_OVERWORLD,
    MAGNET_TRAIN_RIDE_CUTSCENE,
    MAGNET_TRAIN_RIDE_CHANGE_MAP,
    MAGNET_TRAIN_RIDE_RETURN_TO_OVERWORLD,
    MAGNET_TRAIN_RIDE_FREE,
};

enum MagnetTrainCutsceneState {
    MAGNET_TRAIN_CUTSCENE_LAUNCH,
    MAGNET_TRAIN_CUTSCENE_WAIT,
    MAGNET_TRAIN_CUTSCENE_FREE,
};

void CallTask_MagnetTrainRide(FieldSystem *fieldSystem, u8 variant, u8 direction, u32 mapId, u32 x, u32 y) {
    AreaLightTaskEnv *env = Heap_AllocAtEnd(HEAP_ID_FIELD2, sizeof(AreaLightTaskEnv));

    env->variant = variant;
    env->direction = direction;
    env->mapId = mapId;
    env->x = x;
    env->y = y;
    LoadAreaOrDungeonLightTxt(0, &env->light);
    env->state = MAGNET_TRAIN_RIDE_FADE_OUT;
    TaskManager_Call(fieldSystem->taskman, Task_MagnetTrainRide, env);
}

void CallTask_MagnetTrainCutscene(FieldSystem *fieldSystem, u8 variant, void *lighting) {
    GenderTaskEnv *env = Heap_AllocAtEnd(HEAP_ID_FIELD2, sizeof(GenderTaskEnv));

    env->lighting = lighting;
    env->variant = variant;
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
    env->gender = PlayerProfile_GetTrainerGender(Save_PlayerData_GetProfile(fieldSystem->saveData));
    TaskManager_Call(fieldSystem->taskman, Task_MagnetTrainCutscene, env);
}

BOOL Task_MagnetTrainCutscene(TaskManager *taskManager) {
    FieldSystem *fieldSystem = TaskManager_GetFieldSystem(taskManager);
    GenderTaskEnv *env = TaskManager_GetEnvironment(taskManager);
    u32 *state = TaskManager_GetStatePtr(taskManager);

    switch (*state) {
    case MAGNET_TRAIN_CUTSCENE_LAUNCH:
        MagnetTrainCutscene_LaunchApp(fieldSystem, env);
        break;
    case MAGNET_TRAIN_CUTSCENE_WAIT:
        if (FieldSystem_ApplicationIsRunning(fieldSystem)) {
            return FALSE;
        }
        break;
    case MAGNET_TRAIN_CUTSCENE_FREE:
        Heap_Free(env);
        return TRUE;
    }
    (*state)++;
    return FALSE;
}

BOOL Task_MagnetTrainRide(TaskManager *taskManager) {
    FieldSystem *fieldSystem = TaskManager_GetFieldSystem(taskManager);
    AreaLightTaskEnv *env = TaskManager_GetEnvironment(taskManager);

    switch (env->state) {
    case MAGNET_TRAIN_RIDE_FADE_OUT:
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, 0, 6, 1, HEAP_ID_FIELD2);
        GF_SndStartFadeOutBGM(0, 6);
        env->state = MAGNET_TRAIN_RIDE_LEAVE_OVERWORLD;
        break;
    case MAGNET_TRAIN_RIDE_LEAVE_OVERWORLD:
        if (!IsPaletteFadeFinished() || GF_SndGetFadeTimer() != 0) {
            return FALSE;
        }
        StopBGM(GF_GetCurrentPlayingBGM(), 0);
        CallTask_LeaveOverworld(taskManager);
        env->state = MAGNET_TRAIN_RIDE_CUTSCENE;
        break;
    case MAGNET_TRAIN_RIDE_CUTSCENE:
        CallTask_MagnetTrainCutscene(fieldSystem, env->variant, &env->light);
        env->state = MAGNET_TRAIN_RIDE_CHANGE_MAP;
        break;
    case MAGNET_TRAIN_RIDE_CHANGE_MAP:
        CallTask_ChangeMapToCoords(taskManager, env->mapId, -1, env->x, env->y, env->direction);
        env->state = MAGNET_TRAIN_RIDE_RETURN_TO_OVERWORLD;
        break;
    case MAGNET_TRAIN_RIDE_RETURN_TO_OVERWORLD:
        FieldBGM_PlayForMapHeader(fieldSystem, env->mapId, FALSE);
        CallTask_RestoreOverworldAndFadeIn(taskManager);
        env->state = MAGNET_TRAIN_RIDE_FREE;
        break;
    case MAGNET_TRAIN_RIDE_FREE:
        Heap_Free(env);
        return TRUE;
    }
    return FALSE;
}
