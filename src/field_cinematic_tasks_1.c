#include "global.h"

#include "field_overworld_restore.h"
#include "field_overworld_transition.h"
#include "field_system.h"
#include "field_time_events.h"
#include "heap.h"
#include "legendary_cinematic_manager.h"
#include "legendary_cinematic_task.h"
#include "task.h"

typedef struct LegendaryCinematicTaskEnv {
    ModelAttributesStorage modelAttributes;
    u32 scene;
    u32 timeOfDay;
    u32 state;
} LegendaryCinematicTaskEnv;

typedef struct LegendaryCinematicAppTaskEnv {
    LegendaryCinematicTaskEnv *cinematic;
    LegendaryCinematicArgs *args;
    u32 state;
} LegendaryCinematicAppTaskEnv;

LegendaryCinematicArgs *LegendaryCinematic_LaunchApp(FieldSystem *fieldSystem, ModelAttributesStorage *modelAttributes, int scene, int timeOfDay, enum HeapID heapID);

static BOOL Task_LegendaryCinematic(TaskManager *taskManager);
static void CallTask_LegendaryCinematicApp(FieldSystem *fieldSystem, LegendaryCinematicTaskEnv *cinematic);
static BOOL Task_LegendaryCinematicApp(TaskManager *taskManager);

void CallTask_LegendaryCinematic(FieldSystem *fieldSystem, u16 scene) {
    LegendaryCinematicTaskEnv *env = Heap_AllocAtEnd(HEAP_ID_FIELD2, sizeof(LegendaryCinematicTaskEnv));
    env->modelAttributes = *(ModelAttributesStorage *)fieldSystem->modelAttributes;
    env->scene = scene;
    env->state = 0;

    // The scene is lit as day, evening or night.
    switch (Field_GetTimeOfDay(fieldSystem)) {
    case RTC_TIMEOFDAY_MORN:
    case RTC_TIMEOFDAY_DAY:
        env->timeOfDay = 0;
        break;
    case RTC_TIMEOFDAY_EVE:
        env->timeOfDay = 1;
        break;
    case RTC_TIMEOFDAY_NITE:
    case RTC_TIMEOFDAY_LATE:
        env->timeOfDay = 2;
        break;
    default:
        GF_ASSERT(FALSE);
        env->timeOfDay = 1;
        break;
    }

    TaskManager_Call(fieldSystem->taskman, Task_LegendaryCinematic, env);
}

static BOOL Task_LegendaryCinematic(TaskManager *taskManager) {
    FieldSystem *fieldSystem = TaskManager_GetFieldSystem(taskManager);
    LegendaryCinematicTaskEnv *env = TaskManager_GetEnvironment(taskManager);

    switch (env->state) {
    case 0:
        CallTask_LeaveOverworld(taskManager);
        env->state++;
        break;
    case 1:
        CallTask_LegendaryCinematicApp(fieldSystem, env);
        env->state++;
        break;
    case 2:
        CallTask_RestoreOverworld(taskManager);
        env->state++;
        break;
    case 3:
        Heap_Free(env);
        return TRUE;
    }
    return FALSE;
}

static void CallTask_LegendaryCinematicApp(FieldSystem *fieldSystem, LegendaryCinematicTaskEnv *cinematic) {
    LegendaryCinematicAppTaskEnv *env = Heap_AllocAtEnd(HEAP_ID_FIELD2, sizeof(LegendaryCinematicAppTaskEnv));
    env->cinematic = cinematic;
    env->state = 0;
    TaskManager_Call(fieldSystem->taskman, Task_LegendaryCinematicApp, env);
}

static BOOL Task_LegendaryCinematicApp(TaskManager *taskManager) {
    FieldSystem *fieldSystem = TaskManager_GetFieldSystem(taskManager);
    LegendaryCinematicAppTaskEnv *env = TaskManager_GetEnvironment(taskManager);
    LegendaryCinematicTaskEnv *cinematic = env->cinematic;

    switch (env->state) {
    case 0:
        env->args = LegendaryCinematic_LaunchApp(fieldSystem, &cinematic->modelAttributes, cinematic->scene, cinematic->timeOfDay, HEAP_ID_FIELD2);
        env->state++;
        break;
    case 1:
        if (!FieldSystem_ApplicationIsRunning(fieldSystem)) {
            env->state++;
        }
        break;
    case 2:
        Heap_Free(env->args);
        Heap_Free(env);
        return TRUE;
    }
    return FALSE;
}
