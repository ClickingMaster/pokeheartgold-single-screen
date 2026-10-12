#include "field_task_launch_internal.h"
#include "heap.h"

void sub_02059D44(FieldSystem *fieldSystem) {
    int i;
    u32 localPlayer = Comm_GetLocalNetId();
    int group = CommPlayerManager_GetPlayerFrontX();
    int position = CommPlayerManager_GetPlayerFrontZ(localPlayer);

    for (i = 0; i < Comm_CountConnected(); i++) {
        if (i != localPlayer && group == CommPlayerManager_GetPlayerX(i) && position == CommPlayerManager_GetPlayerZ(i)) {
            TrainerCardTaskEnv *env = Heap_AllocAtEnd(HEAP_ID_FIELD2, sizeof(TrainerCardTaskEnv));
            env->player = i;
            env->state = 0;
            FieldSystem_CreateTask(fieldSystem, sub_02059B64, env);
            sub_0203E2F4();
            return;
        }
    }
}
