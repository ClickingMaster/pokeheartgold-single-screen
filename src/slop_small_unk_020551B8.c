#include "global.h"

#include "heap.h"
#include "sound_02004A44.h"
#include "task.h"

typedef struct {
    int state;
    int unk_4;
    int unk_8;
    u32 unk_c;
} UnkStruct_020551B8;

extern void ov01_021EFB64(int param0);
extern void ov01_021EFC04(int param0);
extern void ov01_021EFC94(int param0, FieldSystem *fieldSystem, int *param2);

static BOOL sub_020551B8(TaskManager *taskMan);
void sub_02055218(TaskManager *taskMan, int param1, u32 param2);

static BOOL sub_020551B8(TaskManager *taskMan) {
    FieldSystem *fieldSystem = TaskManager_GetFieldSystem(taskMan);
    UnkStruct_020551B8 *env = TaskManager_GetEnvironment(taskMan);

    switch (env->state) {
    case 0:
        ov01_021EFB64(env->unk_8);
        ov01_021EFC94(env->unk_8, fieldSystem, &env->unk_4);
        Sound_SetSceneAndPlayBGM(5, env->unk_c, 1);
        env->state++;
        break;
    case 1:
        if (env->unk_4 == 1) {
            ov01_021EFC04(env->unk_8);
            Heap_Free(env);
            return TRUE;
        }
        break;
    }
    return FALSE;
}

void sub_02055218(TaskManager *taskMan, int param1, u32 param2) {
    UnkStruct_020551B8 *env = Heap_AllocAtEnd(HEAP_ID_FIELD2, sizeof(UnkStruct_020551B8));
    env->state = 0;
    env->unk_4 = 0;
    env->unk_8 = param1;
    env->unk_c = param2;
    TaskManager_Call(taskMan, sub_020551B8, env);
}
