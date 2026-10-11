#include "battle_transition_task.h"

#include "global.h"

#include "heap.h"
#include "sound_system.h"
#include "task.h"

// Field task that plays a battle transition effect and the battle music, then waits for the effect to finish.

typedef struct BattleTransitionTaskEnv {
    int state;
    int done; // set to 1 by the transition effect
    int effect;
    u32 bgm;
} BattleTransitionTaskEnv;

extern void ov01_021EFB64(int effect);
extern void ov01_021EFC04(int effect);
extern void ov01_021EFC94(int effect, FieldSystem *fieldSystem, int *done);

static BOOL Task_BattleTransition(TaskManager *taskMan);
void CallTask_BattleTransition(TaskManager *taskMan, int effect, int bgm);

static BOOL Task_BattleTransition(TaskManager *taskMan) {
    FieldSystem *fieldSystem = TaskManager_GetFieldSystem(taskMan);
    BattleTransitionTaskEnv *env = TaskManager_GetEnvironment(taskMan);

    switch (env->state) {
    case 0:
        ov01_021EFB64(env->effect);
        ov01_021EFC94(env->effect, fieldSystem, &env->done);
        Sound_SetSceneAndPlayBGM(5, env->bgm, 1);
        env->state++;
        break;
    case 1:
        if (env->done == 1) {
            ov01_021EFC04(env->effect);
            Heap_Free(env);
            return TRUE;
        }
        break;
    }
    return FALSE;
}

void CallTask_BattleTransition(TaskManager *taskMan, int effect, int bgm) {
    BattleTransitionTaskEnv *env = Heap_AllocAtEnd(HEAP_ID_FIELD2, sizeof(BattleTransitionTaskEnv));
    env->state = 0;
    env->done = 0;
    env->effect = effect;
    env->bgm = bgm;
    TaskManager_Call(taskMan, Task_BattleTransition, env);
}
