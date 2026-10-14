#ifndef POKEHEARTGOLD_TASK_H
#define POKEHEARTGOLD_TASK_H

#include "overlay_manager.h"
#include "script_manager.h"

typedef BOOL (*TaskFunc)(TaskManager *taskman);

typedef struct TaskManagerBgGoTo {
    u32 bgGoToId;
} TaskManagerBgGoTo;

struct ApplicationTaskEnv {
    int state;
    const OverlayManagerTemplate *template;
    void *work;
};

struct TaskManager { // declared in field_system.h
    TaskManager *prev;
    TaskFunc func;
    u32 state;
    void *env;
    u32 unk10;
    void *unk14;
    FieldSystem *fieldSystem;
    TaskManagerBgGoTo *bgGoTo; // size=4
};

TaskManager *FieldSystem_CreateTask(FieldSystem *fieldSystem, TaskFunc taskFunc, void *env);
void TaskManager_Jump(TaskManager *taskman, TaskFunc taskFunc, void *env);
void TaskManager_Call(TaskManager *taskman, TaskFunc taskFunc, void *env);
BOOL FieldSystem_RunTaskFrame(FieldSystem *fieldSystem);
BOOL FieldSystem_TaskIsRunning(FieldSystem *fieldSystem);
BOOL FieldSystem_ApplicationIsRunning(FieldSystem *fieldSystem);
void FieldSystem_LoadFieldOverlay(FieldSystem *fieldSystem);
BOOL FieldSystem_IsOverworldRestored(FieldSystem *fieldSystem);
void CallApplicationAsTask(TaskManager *taskManager, const OverlayManagerTemplate *template, void *work);
FieldSystem *TaskManager_GetFieldSystem(TaskManager *taskManager);
void *TaskManager_GetEnvironment(TaskManager *taskManager);
u32 *TaskManager_GetStatePtr(TaskManager *taskManager);
u32 TaskManager_GetBgGoToId(TaskManager *taskManager);

#endif // POKEHEARTGOLD_TASK_H
