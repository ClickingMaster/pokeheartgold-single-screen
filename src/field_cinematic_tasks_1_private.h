#ifndef PRIVATE_H
#define PRIVATE_H
#include "global.h"
typedef struct TaskManager TaskManager;
extern void *Heap_AllocAtEnd(int,int); extern void Heap_Free(void *); extern int Field_GetTimeOfDay(void *); extern void GF_AssertFail(void);
extern void TaskManager_Call(TaskManager *, BOOL (*)(TaskManager *), void *); extern void *TaskManager_GetFieldSystem(TaskManager *); extern void *TaskManager_GetEnvironment(TaskManager *);
extern void CallTask_LeaveOverworld(TaskManager *); extern void CallTask_RestoreOverworld(TaskManager *);
extern void *LegendaryCinematic_LaunchApp(void *,void *,int,int,int); extern BOOL FieldSystem_ApplicationIsRunning(void *);
void CallTask_LegendaryCinematic(void *,u16); BOOL Task_LegendaryCinematic(TaskManager *); void CallTask_LegendaryCinematicApp(void *,void *); BOOL Task_LegendaryCinematicApp(TaskManager *);
#endif
