#include "safari_decoration_task.h"

#include "launch_application.h"
#include "unk_020552A4.h"

static BOOL Task_SafariDecoration(TaskManager *taskManager);

void CallTask_SafariDecoration(TaskManager *taskManager, u16 *unknownPtr) {
    FieldSystem *fieldSystem = TaskManager_GetFieldSystem(taskManager);
    SafariDecorationTaskEnv *data = Heap_AllocAtEnd(HEAP_ID_FIELD2, sizeof(SafariDecorationTaskEnv));
    MI_CpuClear8(data, sizeof(SafariDecorationTaskEnv));
    data->state = 0;
    data->unknown_4 = unknownPtr;
    TaskManager_Call(taskManager, Task_SafariDecoration, data);
}

static BOOL Task_SafariDecoration(TaskManager *taskManager) {
    FieldSystem *fieldSystem = TaskManager_GetFieldSystem(taskManager);
    SafariDecorationTaskEnv *env = TaskManager_GetEnvironment(taskManager);

    switch (env->state) {
    case 0:
        env->safariDecorationArgs = SafariDecoration_LaunchApp(fieldSystem);
        env->state++;
        break;
    case 1:
        if (FieldSystem_ApplicationIsRunning(fieldSystem) == FALSE) {
            *env->unknown_4 = env->safariDecorationArgs->placedObject;
            Heap_Free(env->safariDecorationArgs);
            env->state++;
        }
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
