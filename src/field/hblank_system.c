#include "field/hblank_system.h"

#include "global.h"

#include "assert.h"
#include "heap.h"

// Defined in main.c; no header declares it yet.
BOOL Main_SetHBlankIntrCB(void (*cb)(void *), void *arg);

typedef struct HBlankTask HBlankTask;
typedef void (*HBlankTaskCallback)(HBlankTask *, void *);

struct HBlankTask {
    BOOL active;
    void *arg;
    HBlankTaskCallback callback;
};

struct HBlankSystem {
    BOOL running;
    HBlankTask tasks[2];
};

HBlankTask *HBlankSystem_StartTask(HBlankSystem *system, HBlankTaskCallback callback, void *arg);
void HBlankTask_Delete(HBlankTask *task);
void HBlankSystem_Init(HBlankSystem *system);
void HBlankTask_Reset(HBlankTask *task);
void HBlankSystem_Callback(void *arg);
void HBlankTask_Dummy(HBlankTask *task, void *arg);
HBlankTask *HBlankSystem_GetFreeTask(HBlankSystem *system);

HBlankSystem *HBlankSystem_New(enum HeapID heapID) {
    HBlankSystem *system = Heap_Alloc(heapID, sizeof(HBlankSystem));
    HBlankSystem_Init(system);
    return system;
}

void HBlankSystem_Delete(HBlankSystem *system) {
    if (system != NULL) {
        if (system->running == TRUE) {
            HBlankSystem_Stop(system);
        }
        HBlankSystem_Init(system);
        Heap_Free(system);
    }
}

void HBlankSystem_Start(HBlankSystem *system) {
    if (Main_SetHBlankIntrCB(HBlankSystem_Callback, system) != TRUE) {
        GF_AssertFail();
    }
    system->running = TRUE;
}

void HBlankSystem_Stop(HBlankSystem *system) {
    if (Main_SetHBlankIntrCB(NULL, NULL) != TRUE) {
        GF_AssertFail();
    }
    system->running = FALSE;
}

HBlankTask *HBlankSystem_StartTask(HBlankSystem *system, HBlankTaskCallback callback, void *arg) {
    HBlankTask *task = HBlankSystem_GetFreeTask(system);
    if (task == NULL) {
        GF_AssertFail();
    }
    if (task != NULL) {
        task->arg = arg;
        task->callback = callback;
        task->active = TRUE;
    }
    return task;
}

void HBlankTask_Delete(HBlankTask *task) {
    HBlankTask_Reset(task);
}

void HBlankSystem_Init(HBlankSystem *system) {
    memset(system, 0, sizeof(HBlankSystem));
    system->running = FALSE;
    {
        s32 i;
        for (i = 0; i < 2; i++) {
            HBlankTask_Reset(&system->tasks[i]);
        }
    }
}

void HBlankTask_Reset(HBlankTask *task) {
    task->active = FALSE;
    task->callback = HBlankTask_Dummy;
    task->arg = NULL;
}

void HBlankSystem_Callback(void *arg) {
    HBlankSystem *system = arg;
    s32 i;
    for (i = 0; i < 2; i++) {
        system->tasks[i].callback(&system->tasks[i], system->tasks[i].arg);
    }
}

void HBlankTask_Dummy(HBlankTask *task, void *arg) {
}

HBlankTask *HBlankSystem_GetFreeTask(HBlankSystem *system) {
    s32 i;
    for (i = 0; i < 2; i++) {
        if (system->tasks[i].active == FALSE) {
            return &system->tasks[i];
        }
    }
    return NULL;
}
