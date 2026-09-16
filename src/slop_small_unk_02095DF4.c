#include "global.h"

#include "field_system.h"
#include "heap.h"
#include "launch_application.h"
#include "task.h"
#include "unk_02095DF4.h"

typedef struct UnkStruct_02095E30 {
    SaveData *saveData; // 0x00
    u32 mode;           // 0x04
    u16 unk08;          // 0x08
    u16 unk0A;          // 0x0A
    u8 unk0C;           // 0x0C
    u8 unk0D;           // 0x0D
    u8 unk0E;           // 0x0E
    u8 unk0F;           // 0x0F
} UnkStruct_02095E30;

typedef struct UnkStruct_02095DF4 {
    u32 unk00; // state
    UnkStruct_02095E30 *unk04;
    u32 unk08;
    u16 unk0C;
    u16 unk0E;
    u16 *unk10;
    u16 *unk14;
    u16 *unk18;
    u16 *unk1C;
} UnkStruct_02095DF4;

static BOOL sub_02095E30(TaskManager *taskman);

void sub_02095DF4(FieldSystem *fieldSystem, u8 a1, u8 a2, u8 a3, u16 *a4, u16 *a5, u16 *a6, u16 *a7) {
    UnkStruct_02095DF4 *work = Heap_AllocAtEnd(HEAP_ID_FIELD2, sizeof(UnkStruct_02095DF4));
    work->unk00 = 0;
    work->unk08 = a1;
    work->unk0E = a2;
    work->unk0C = a3;
    work->unk10 = a4;
    work->unk14 = a5;
    work->unk18 = a6;
    work->unk1C = a7;
    TaskManager_Call(fieldSystem->taskman, sub_02095E30, work);
}

static BOOL sub_02095E30(TaskManager *taskman) {
    FieldSystem *fieldSystem = TaskManager_GetFieldSystem(taskman);
    UnkStruct_02095DF4 *work = TaskManager_GetEnvironment(taskman);
    switch (work->unk00) {
    case 0:
        work->unk04 = Heap_AllocAtEnd(HEAP_ID_FIELD2, sizeof(UnkStruct_02095E30));
        work->unk04->mode = (work->unk08 != 0);
        work->unk04->saveData = FieldSystem_GetSaveData(fieldSystem);
        work->unk04->unk0C = work->unk0C;
        work->unk04->unk0F = work->unk0E;
        PokeathlonCourse_LaunchApp(fieldSystem, (PokeathlonCourseArgs *)work->unk04);
        sub_0203E30C();
        work->unk00 = 1;
        break;
    case 1:
        if (FieldSystem_ApplicationIsRunning(fieldSystem)) {
            break;
        }
        work->unk00 = 2;
        FieldSystem_LoadFieldOverlay(fieldSystem);
        break;
    case 2:
        if (!sub_020505C8(fieldSystem)) {
            break;
        }
        work->unk00 = 3;
        break;
    case 3:
        *work->unk10 = work->unk04->unk0E;
        *work->unk14 = work->unk04->unk0A;
        *work->unk18 = work->unk04->unk08;
        *work->unk1C = work->unk04->unk0D;
        Heap_Free(work->unk04);
        Heap_Free(work);
        return TRUE;
    }
    return FALSE;
}
