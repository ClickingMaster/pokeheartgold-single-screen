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

extern void EncounterEffect_LoadOverlay(int effect);
extern void EncounterEffect_UnloadOverlay(int effect);
extern void EncounterEffect_Start(int effect, FieldSystem *fieldSystem, int *done);

static BOOL Task_BattleTransition(TaskManager *taskMan);
void CallTask_BattleTransition(TaskManager *taskMan, int effect, int bgm);

static BOOL Task_BattleTransition(TaskManager *taskMan) {
    FieldSystem *fieldSystem = TaskManager_GetFieldSystem(taskMan);
    BattleTransitionTaskEnv *env = TaskManager_GetEnvironment(taskMan);

    switch (env->state) {
    case 0:
        EncounterEffect_LoadOverlay(env->effect);
        EncounterEffect_Start(env->effect, fieldSystem, &env->done);
        Sound_SetSceneAndPlayBGM(5, env->bgm, 1);
        env->state++;
        break;
    case 1:
        if (env->done == 1) {
            EncounterEffect_UnloadOverlay(env->effect);
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
