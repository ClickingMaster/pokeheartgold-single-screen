#include "global.h"

#include "field_overworld_restore.h"
#include "field_overworld_transition.h"
#include "field_warp_reset.h"
#include "heap.h"
#include "npc_trade.h"
#include "task.h"
#include "trade_sequence.h"

FS_EXTERN_OVERLAY(OVY_71);

typedef struct NPCTradeTaskEnv {
    NPCTradeAppData *tradeWork;
    int state;
    int slot;
    TRADE_ANIM_WORK animWork;
    Pokemon *myMon;
    Pokemon *tradeMon;
} NPCTradeTaskEnv;

static void NPCTrade_CallTradeSequenceApp(TaskManager *taskManager);
static BOOL Task_NPCTrade(TaskManager *taskManager);

static const OverlayManagerTemplate sNPCTradeSequenceAppTemplate = {
    TradeSequence_Init,
    TradeSequence_Main,
    TradeSequence_Exit,
    FS_OVERLAY_ID(OVY_71),
};

static void NPCTrade_CallTradeSequenceApp(TaskManager *taskManager) {
    NPCTradeTaskEnv *env;
    TaskManager_GetFieldSystem(taskManager);
    env = TaskManager_GetEnvironment(taskManager);
    CallApplicationAsTask(taskManager, &sNPCTradeSequenceAppTemplate, &env->animWork);
}

static BOOL Task_NPCTrade(TaskManager *taskManager) {
    FieldSystem *fieldSystem = TaskManager_GetFieldSystem(taskManager);
    NPCTradeTaskEnv *env = TaskManager_GetEnvironment(taskManager);

    switch (env->state) {
    case 0:
        NPCTrade_CreateTradeAnim(fieldSystem, env->tradeWork, env->slot, &env->animWork, env->myMon, env->tradeMon);
        NPCTrade_ReceiveMonToSlot(fieldSystem, env->tradeWork, env->slot);
        env->state++;
        break;
    case 1:
        PaletteFadeUntilFinished(taskManager);
        env->state++;
        break;
    case 2:
        CallTask_LeaveOverworld(taskManager);
        env->state++;
        break;
    case 3:
        NPCTrade_CallTradeSequenceApp(taskManager);
        env->state++;
        break;
    case 4:
        CallTask_RestoreOverworld(taskManager);
        env->state++;
        break;
    case 5:
        CallTask_FadeFromBlack(taskManager);
        env->state++;
        break;
    case 6:
        Heap_Free(env->myMon);
        Heap_Free(env->tradeMon);
        Heap_Free(env);
        return TRUE;
    }
    return FALSE;
}

void CallTask_NPCTrade(TaskManager *taskManager, NPCTradeAppData *tradeWork, u16 slot, enum HeapID heapID) {
    NPCTradeTaskEnv *env = Heap_Alloc(heapID, sizeof(NPCTradeTaskEnv));
    memset(env, 0, sizeof(NPCTradeTaskEnv));
    env->state = 0;
    env->tradeWork = tradeWork;
    env->slot = slot;
    env->myMon = AllocMonZeroed(heapID);
    env->tradeMon = AllocMonZeroed(heapID);
    TaskManager_Call(taskManager, Task_NPCTrade, env);
}
