#include "mart_bag_task_private.h"

BOOL Task_MartBag(TaskManager *taskManager) {
    FieldSystem *fieldSystem = TaskManager_GetFieldSystem(taskManager);
    MartReturnEnv *env = TaskManager_GetEnvironment(taskManager);
    switch (env->state) {
    case 24:
        env->state = MartBag_WaitBagViewExit(fieldSystem, env);
        break;
    case 25:
        MartBag_ReturnToMart(taskManager);
        break;
    }
    return FALSE;
}

BOOL MartBag_WaitBagViewExit(FieldSystem *fieldSystem, MartReturnEnv *env) {
    if (FieldSystem_ApplicationIsRunning(fieldSystem)) {
        return 24;
    }
    if (env->result != NULL) {
        env->result[1] = sub_0207791C(env->bagView);
    }
    Heap_Free(env->bagView);
    FieldSystem_LoadFieldOverlay(fieldSystem);
    return 25;
}

void MartBag_ReturnToMart(TaskManager *taskManager) {
    FieldSystem *fieldSystem = TaskManager_GetFieldSystem(taskManager);
    MartReturnEnv *env = TaskManager_GetEnvironment(taskManager);
    if (sub_020505C8(fieldSystem)) {
        FieldMap_FadeScreen(1);
        TaskManager_Jump(taskManager, Task_Mart, env);
        env->state = 26;
    }
}
