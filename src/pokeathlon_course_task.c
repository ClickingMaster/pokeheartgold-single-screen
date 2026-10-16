#include "pokeathlon_course_task.h"

#include "global.h"

#include "field_system.h"
#include "heap.h"
#include "launch_application.h"
#include "task.h"

typedef struct PokeathlonCourseTaskArgs {
    SaveData *saveData; // 0x00
    u32 mode;           // 0x04
    u16 points;         // 0x08
    u16 totalPoints;    // 0x0A
    u8 course;          // 0x0C
    u8 rank;            // 0x0D
    u8 shouldFreeHeap;  // 0x0E
    u8 isSupreme;       // 0x0F
} PokeathlonCourseTaskArgs;

typedef struct PokeathlonCourseTaskEnv {
    u32 state;
    PokeathlonCourseTaskArgs *args;
    u32 mode;
    u16 course;
    u16 isSupreme;
    u16 *shouldFreeHeapOut;
    u16 *totalPointsOut;
    u16 *pointsOut;
    u16 *rankOut;
} PokeathlonCourseTaskEnv;

static BOOL Task_PokeathlonCourse(TaskManager *taskman);

void CallTask_PokeathlonCourse(FieldSystem *fieldSystem, u8 mode, u8 a2, u8 a3, u16 *a4, u16 *a5, u16 *a6, u16 *a7) {
    PokeathlonCourseTaskEnv *work = Heap_AllocAtEnd(HEAP_ID_FIELD2, sizeof(PokeathlonCourseTaskEnv));
    work->state = 0;
    work->mode = mode;
    work->isSupreme = a2;
    work->course = a3;
    work->shouldFreeHeapOut = a4;
    work->totalPointsOut = a5;
    work->pointsOut = a6;
    work->rankOut = a7;
    TaskManager_Call(fieldSystem->taskman, Task_PokeathlonCourse, work);
}

static BOOL Task_PokeathlonCourse(TaskManager *taskman) {
    FieldSystem *fieldSystem = TaskManager_GetFieldSystem(taskman);
    PokeathlonCourseTaskEnv *work = TaskManager_GetEnvironment(taskman);
    switch (work->state) {
    case 0:
        work->args = Heap_AllocAtEnd(HEAP_ID_FIELD2, sizeof(PokeathlonCourseTaskArgs));
        work->args->mode = (work->mode != 0);
        work->args->saveData = FieldSystem_GetSaveData(fieldSystem);
        work->args->course = work->course;
        work->args->isSupreme = work->isSupreme;
        PokeathlonCourse_LaunchApp(fieldSystem, (PokeathlonCourseArgs *)work->args);
        FieldSystem_ResumeProcesses();
        work->state = 1;
        break;
    case 1:
        if (FieldSystem_ApplicationIsRunning(fieldSystem)) {
            break;
        }
        work->state = 2;
        FieldSystem_LoadFieldOverlay(fieldSystem);
        break;
    case 2:
        if (!FieldSystem_IsOverworldRestored(fieldSystem)) {
            break;
        }
        work->state = 3;
        break;
    case 3:
        *work->shouldFreeHeapOut = work->args->shouldFreeHeap;
        *work->totalPointsOut = work->args->totalPoints;
        *work->pointsOut = work->args->points;
        *work->rankOut = work->args->rank;
        Heap_Free(work->args);
        Heap_Free(work);
        return TRUE;
    }
    return FALSE;
}
