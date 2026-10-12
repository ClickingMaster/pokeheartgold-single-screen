#include "vs_recorder_app_internal.h"

void *VsRecorderApp_GetRecorderWork(OverlayManager *manager) {
    return ((void **)OverlayManager_GetArgs(manager))[5];
}

void VsRecorderApp_SetResult(VsRecorderAppResult *pair, u32 first, u32 second) {
    pair->first = first;
    pair->second = second;
}

void VsRecorderApp_Create(OverlayManager *manager, u32 mode) {
    VsRecorderApp *work;
    u8 *large;
    void *args;
    void *value;
    u32 index;

    Heap_Create((enum HeapID)3, (enum HeapID)0x7E, 0x10000);
    work = OverlayManager_CreateAndGetData(manager, sizeof(VsRecorderApp), (enum HeapID)0x7E);
    MI_CpuFill8(work, 0, sizeof(VsRecorderApp));
    args = OverlayManager_GetArgs(manager);
    work->args = args;
    work->saveData = ((void **)args)[3];
    work->large = Heap_Alloc((enum HeapID)0x7E, 0x4170);
    MI_CpuFill8(work->large, 0, 0x4170);
    *(void **)(work->large + 0x868) = &work->result;
    *(void **)(work->large + 0x830) = work->saveData;
    *(u32 *)work->large = mode;
    large = work->large;
    value = sub_02087FF8(large, *(u32 *)large);
    index = *(u32 *)(large + 0x6D8);
    *(void **)(large + 0x81C + index * 4) = value;
}

BOOL VsRecorderApp_InitMode0(OverlayManager *manager, int *state) {
    VsRecorderApp_Create(manager, 0);
    return TRUE;
}

BOOL VsRecorderApp_InitMode1(OverlayManager *manager, int *state) {
    VsRecorderApp_Create(manager, 1);
    return TRUE;
}

BOOL VsRecorderApp_InitMode2(OverlayManager *manager, int *state) {
    VsRecorderApp_Create(manager, 2);
    return TRUE;
}

BOOL VsRecorderApp_InitMode3(OverlayManager *manager, int *state) {
    VsRecorderApp_Create(manager, 3);
    return TRUE;
}

BOOL VsRecorderApp_InitMode4(OverlayManager *manager, int *state) {
    VsRecorderApp_Create(manager, 4);
    return TRUE;
}

BOOL VsRecorderApp_InitMode5(OverlayManager *manager, int *state) {
    VsRecorderApp_Create(manager, 5);
    return TRUE;
}

BOOL VsRecorderApp_InitMode6(OverlayManager *manager, int *state) {
    VsRecorderApp_Create(manager, 6);
    return TRUE;
}

BOOL VsRecorderApp_Main(OverlayManager *manager, int *state) {
    VsRecorderApp *work = OverlayManager_GetData(manager);
    switch (*state) {
    case 0:
        if (VsRecorderApp_RunMenu(work, (enum HeapID)0x7E)) {
            if (work->result == 1) {
                *state = 1;
                work->state = 0;
            } else {
                return TRUE;
            }
        }
        break;
    case 1:
        if (VsRecorderApp_RunPlayback(work, (enum HeapID)0x7E)) {
            *state = 0;
            work->state = 0;
        }
        break;
    }
    return FALSE;
}

BOOL VsRecorderApp_Exit(OverlayManager *manager, int *state) {
    VsRecorderApp *work = OverlayManager_GetData(manager);
    if (BattleRecord_Exists() == TRUE) {
        BattleRecord_Delete();
    }
    Heap_Free(work->large);
    OverlayManager_FreeData(manager);
    GF_SndHandleSetPlayerVolume(1, 0x7F);
    GF_SndHandleSetPlayerVolume(7, 0x7F);
    Heap_Destroy((enum HeapID)0x7E);
    return TRUE;
}

BOOL VsRecorderApp_RunMenu(VsRecorderApp *work, enum HeapID heapId) {
    if (work->state == 0) {
        if (*(u32 *)work->large == 0) {
            work->manager = OverlayManager_New(&gOverlayTemplate_VsRecorderMenu, work, heapId);
        } else {
            work->manager = OverlayManager_New(&gOverlayTemplate_VsRecorderMenuFromTerminal, work, heapId);
        }
        work->state++;
    } else if (OverlayManager_Run(work->manager)) {
        OverlayManager_Delete(work->manager);
        return TRUE;
    }
    return FALSE;
}
