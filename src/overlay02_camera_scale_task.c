#include "overlay02_4_landing_private.h"

WIP_LOCAL BOOL ov02_022508D8(TaskManager *taskManager) {
    FieldSystem *fieldSystem = TaskManager_GetFieldSystem(taskManager);
    u32 *state = TaskManager_GetStatePtr(taskManager);
    void *env = TaskManager_GetEnvironment(taskManager);

    switch (*state) {
    case 0: {
        VecFx32 v;
        v = ov02_02253B24;
        (*(u16 *)((u8 *)env + 2))++;
        v.x = (s32)((double)*(u16 *)((u8 *)env + 2) * 2048.0 / 10.0 + 4096.0);
        v.y = (s32)((double)*(u16 *)((u8 *)env + 2) * 2048.0 / 10.0 + 4096.0);
        BillboardObject_SetScale(ov01_021F771C(*(void **)((u8 *)fieldSystem + 0x3c)), &v);
        if (*(u16 *)((u8 *)env + 2) >= 10) {
            *(u16 *)((u8 *)env + 2) = 0;
            *(u16 *)env = 0;
            (*state)++;
        }
        break;
    }
    case 1:
        (*(u16 *)env)++;
        if (*(u16 *)env >= 10) {
            (*state)++;
        }
        break;
    case 2: {
        VecFx32 v;
        v = ov02_02253B30;
        (*(u16 *)((u8 *)env + 2))++;
        if (*(u16 *)((u8 *)env + 2) >= 10) {
            (*state)++;
        } else {
            v.x = (s32)((double)(10 - *(u16 *)((u8 *)env + 2)) * 2048.0 / 10.0 + 4096.0);
            v.y = (s32)((double)(10 - *(u16 *)((u8 *)env + 2)) * 2048.0 / 10.0 + 4096.0);
        }
        BillboardObject_SetScale(ov01_021F771C(*(void **)((u8 *)fieldSystem + 0x3c)), &v);
        break;
    }
    case 3:
        Heap_Free(env);
        return TRUE;
    }
    return FALSE;
}
