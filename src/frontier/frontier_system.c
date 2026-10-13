#include "frontier/frontier_system.h"

#include "global.h"

#include "frontier/frontier_map.h"
#include "frontier/frontier_script_context.h"

#include "filesystem.h"
#include "heap.h"
#include "message_format.h"
#include "msgdata.h"
#include "pm_string.h"

// Script command table of the frontier scripts and its size (frontier script commands' data).
extern const FrontierScrCmdFunc gFrontierCommandTable[];
extern const u32 ov80_0223B9CC;

u32 FrontierMap_GetParam(u32 mapId, u32 field);
FrontierMap *Frontier_GetMap(void *frontierWork);

FrontierSystem *FrontierSystem_Create(void *frontierWork, enum HeapID heapId, u32 mapId) {
    FrontierSystem *system = Heap_Alloc(heapId, sizeof(FrontierSystem));
    MI_CpuFill8(system, 0, sizeof(FrontierSystem));
    system->frontierWork = frontierWork;
    system->heapId = heapId;
    system->mapId = mapId;
    FrontierSystem_LoadMapScripts(&system->scripts, &system->msgData, mapId, heapId);
    system->msgFormat = MessageFormat_New_Custom(8, 64, heapId);
    system->msgBuffer = String_New(1024, heapId);
    system->msgTemplate = String_New(1024, heapId);
    FrontierSystem_InitDummy(system, mapId, 0, 0);
    return system;
}

// Runs every task one step; finished tasks are freed. Returns TRUE once no task is left.
BOOL FrontierSystem_Main(FrontierSystem *system) {
    s32 i;
    if (system->taskCount == 0) {
        return TRUE;
    }
    if (system->unk_38 == 0) {
        for (i = 0; i < 8; i++) {
            FrontierScriptContext *task = system->tasks[i];
            if (task != NULL && !FrontierScriptContext_Run(task)) {
                FrontierSystem_FreeTask(system, task);
                system->tasks[i] = NULL;
                system->taskCount--;
            }
        }
        FrontierMap_RunObjectMovements(system->frontierWork);
    }
    return system->taskCount == 0;
}

void FrontierSystem_Delete(FrontierSystem *system) {
    if (system->taskCount != 0) {
        GF_AssertFail();
    }
    MessageFormat_Delete(system->msgFormat);
    String_Delete(system->msgBuffer);
    String_Delete(system->msgTemplate);
    Heap_Free(system->scripts);
    DestroyMsgData(system->msgData);
    Heap_Free(system);
}

// Starts script scriptIndex of map mapId (0xFFFF: the current map) in a new task.
void FrontierSystem_AddTask(FrontierSystem *system, u16 mapId, u32 scriptIndex) {
    FrontierScriptContext *task = Heap_Alloc(system->heapId, sizeof(FrontierScriptContext));
    MI_CpuFill8(task, 0, sizeof(FrontierScriptContext));
    FrontierScriptContext_Init(task, gFrontierCommandTable, ov80_0223B9CC);
    task->frontierSystem = system;
    if (mapId == 0xFFFF || system->mapId == mapId) {
        task->scripts = system->scripts;
        task->msgLoader = system->msgData;
    } else {
        FrontierSystem_LoadMapScripts(&task->scripts, &task->msgLoader, mapId, system->heapId);
    }
    FrontierScriptContext_Start(task, task->scripts);
    FrontierScriptContext_GoToScript(task, scriptIndex);
    FrontierSystem_InsertTask(system, task);
}

BOOL FrontierSystem_InsertTask(FrontierSystem *system, FrontierScriptContext *task) {
    s32 i;
    for (i = 0; i < 8; i++) {
        if (system->tasks[i] == NULL) {
            system->tasks[i] = task;
            system->taskCount++;
            return TRUE;
        }
    }
    GF_AssertFail();
    return FALSE;
}

// Frees a task and the script data / messages it loaded for another map.
void FrontierSystem_FreeTask(FrontierSystem *system, FrontierScriptContext *task) {
    if (task->msgLoader != system->msgData) {
        DestroyMsgData(task->msgLoader);
    }
    if (task->scripts != system->scripts) {
        Heap_Free(task->scripts);
    }
    Heap_Free(task);
}

void FrontierSystem_InitDummy(FrontierSystem *system, u16 mapId, u32 a2, u32 a3) {
}

void FrontierSystem_LoadMapScripts(u8 **scripts, MsgData **msgData, u32 mapId, enum HeapID heapId) {
    u32 member = FrontierMap_GetParam(mapId, 1);
    u32 bank = FrontierMap_GetParam(mapId, 2);
    *scripts = AllocAndReadWholeNarcMemberByIdPair(NARC_a_1_8_2, member, heapId);
    *msgData = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, bank, heapId);
}

// Switches the system's messages (and the tasks using them) to the message bank of map mapId.
void FrontierSystem_ChangeMsgBank(FrontierSystem *system, u16 mapId, enum HeapID heapId) {
    s32 i;
    u32 oldBank = FrontierMap_GetParam(system->mapId, 2);
    u32 newBank = FrontierMap_GetParam(mapId, 2);
    if (oldBank != newBank) {
        MsgData *msgData = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, newBank, heapId);
        for (i = 0; i < 8; i++) {
            if (system->tasks[i] != NULL && system->tasks[i]->msgLoader == system->msgData) {
                system->tasks[i]->msgLoader = msgData;
            }
        }
        DestroyMsgData(system->msgData);
        system->msgData = msgData;
    }
}

u16 *FrontierSystem_SaveVars(FrontierSystem *system, enum HeapID heapId) {
    u16 *saved = Heap_Alloc(heapId, sizeof(system->vars));
    u16 *src = system->vars;
    u16 *dest = saved;
    u32 count = 8;
    do {
        *dest++ = *src++;
    } while (--count);
    return saved;
}

void FrontierSystem_RestoreVars(FrontierSystem *system, u16 *saved) {
    u16 *src = saved;
    u16 *dest = system->vars;
    u32 count = 8;
    do {
        *dest++ = *src++;
    } while (--count);
    Heap_Free(saved);
}

// The script data starts with a table of relative offsets, one word per script.
void FrontierScriptContext_GoToScript(FrontierScriptContext *ctx, u32 scriptIndex) {
    ctx->scriptPtr += scriptIndex * 4;
    ctx->scriptPtr += FrontierScriptContext_ReadWord(ctx);
}

u16 *FrontierSystem_GetVarPtr(FrontierSystem *system, u32 index) {
    return &system->vars[index];
}

FrontierMap *FrontierSystem_GetFrontierMap(FrontierSystem *system) {
    return Frontier_GetMap(system->frontierWork);
}
