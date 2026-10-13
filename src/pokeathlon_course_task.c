#include "pokeathlon_course_task.h"

#include "global.h"

#include "field_system.h"
#include "heap.h"
#include "launch_application.h"
#include "task.h"

typedef struct PokeathlonCourseTaskArgs {
    SaveData *saveData; // 0x00
    u32 mode;           // 0x04
    u16 unk08;          // 0x08
    u16 unk0A;          // 0x0A
    u8 unk0C;           // 0x0C
    u8 unk0D;           // 0x0D
    u8 unk0E;           // 0x0E
    u8 unk0F;           // 0x0F
} PokeathlonCourseTaskArgs;

typedef struct PokeathlonCourseTaskEnv {
    u32 state;
    PokeathlonCourseTaskArgs *args;
    u32 mode;
    u16 unk0C;
    u16 unk0E;
    u16 *unk10;
    u16 *unk14;
    u16 *unk18;
    u16 *unk1C;
} PokeathlonCourseTaskEnv;

static BOOL Task_PokeathlonCourse(TaskManager *taskman);

void CallTask_PokeathlonCourse(FieldSystem *fieldSystem, u8 mode, u8 a2, u8 a3, u16 *a4, u16 *a5, u16 *a6, u16 *a7) {
    PokeathlonCourseTaskEnv *work = Heap_AllocAtEnd(HEAP_ID_FIELD2, sizeof(PokeathlonCourseTaskEnv));
    work->state = 0;
    work->mode = mode;
    work->unk0E = a2;
    work->unk0C = a3;
    work->unk10 = a4;
    work->unk14 = a5;
    work->unk18 = a6;
    work->unk1C = a7;
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
        work->args->unk0C = work->unk0C;
        work->args->unk0F = work->unk0E;
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
        *work->unk10 = work->args->unk0E;
        *work->unk14 = work->args->unk0A;
        *work->unk18 = work->args->unk08;
        *work->unk1C = work->args->unk0D;
        Heap_Free(work->args);
        Heap_Free(work);
        return TRUE;
    }
    return FALSE;
}
