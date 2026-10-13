#include "application/vs_recorder/vs_recorder_screens.h"

#include "bag_cursor.h"
#include "battle_record.h"
#include "battle_recording.h"
#include "field_bgm.h"
#include "game_stats.h"
#include "global_terminal_app_internal.h"
#include "heap.h"
#include "poke_overlay.h"
#include "sound_system.h"
#include "vs_recorder_app_internal.h"

FS_EXTERN_OVERLAY(OVY_39);
FS_EXTERN_OVERLAY(OVY_40);

VsRecorderWork *VsRecorderApp_GetRecorderWork(OverlayManager *manager) {
    return ((VsRecorderApp *)OverlayManager_GetArgs(manager))->recorder;
}

void VsRecorderApp_SetResult(void *result, u32 first, u32 second) {
    VsRecorderAppResult *pair = result;
    pair->first = first;
    pair->second = second;
}

void VsRecorderApp_Create(OverlayManager *manager, u32 mode) {
    VsRecorderApp *work;
    VsRecorderWork *recorder;
    FieldSystem *fieldSystem;
    const VsRecorderMenuEntry *menu;
    int depth;

    Heap_Create(HEAP_ID_3, HEAP_ID_126, 0x10000);
    work = OverlayManager_CreateAndGetData(manager, sizeof(VsRecorderApp), HEAP_ID_126);
    MI_CpuFill8(work, 0, sizeof(VsRecorderApp));
    fieldSystem = OverlayManager_GetArgs(manager);
    work->fieldSystem = fieldSystem;
    work->saveData = fieldSystem->saveData;
    work->recorder = Heap_Alloc(HEAP_ID_126, sizeof(VsRecorderWork));
    MI_CpuFill8(work->recorder, 0, sizeof(VsRecorderWork));
    work->recorder->result = &work->result;
    work->recorder->saveData = work->saveData;
    work->recorder->mode = mode;
    recorder = work->recorder;
    menu = VsRecorderApp_GetStartMenu(recorder, recorder->mode);
    depth = recorder->menuDepth;
    recorder->menuStack[depth] = menu;
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
        if (VsRecorderApp_RunMenu(work, HEAP_ID_126)) {
            if (work->result == 1) {
                *state = 1;
                work->state = 0;
            } else {
                return TRUE;
            }
        }
        break;
    case 1:
        if (VsRecorderApp_RunPlayback(work, HEAP_ID_126)) {
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
    Heap_Free(work->recorder);
    OverlayManager_FreeData(manager);
    GF_SndHandleSetPlayerVolume(1, 0x7F);
    GF_SndHandleSetPlayerVolume(7, 0x7F);
    Heap_Destroy(HEAP_ID_126);
    return TRUE;
}

BOOL VsRecorderApp_RunMenu(VsRecorderApp *work, enum HeapID heapId) {
    if (work->state == 0) {
        if (work->recorder->mode == 0) {
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

BOOL VsRecorderApp_RunPlayback(VsRecorderApp *work, enum HeapID heapId) {
    BOOL result;
    u16 bgm;
    FSOverlayID overlayId;

    switch (work->state) {
    case 0:
        if (work->recorder->mode != 0) {
            UnloadOverlayByID(FS_OVERLAY_ID(OVY_39));
            UnloadOVY38();
        }
        work->state++;
        break;
    case 1:
        work->setup = BattleSetup_New(heapId, 0);
        if (!BattleRecord_Exists()) {
            BattleRecord_Load(work->saveData, heapId, &result, work->setup, work->recorder->selection);
        } else {
            BattleRecord_RestoreSetup(work->setup, work->saveData);
            result = TRUE;
        }
        work->setup->bagCursor = BagCursor_New(heapId);
        work->setup->gameStats = Save_GameStats_Get(work->saveData);
        overlayId = FS_OVERLAY_ID(OVY_40);
        if (HandleLoadOverlay(overlayId, OVY_LOAD_ASYNC) == TRUE) {
            VsRecorder_SanitizeBattleSetupNames(BattleRecord_Get(), work->setup, heapId);
            UnloadOverlayByID(overlayId);
        }
        work->recorder->battleResult = 1;
        if (result != TRUE) {
            Heap_Free(work->setup->bagCursor);
            BattleSetup_Delete(work->setup);
            work->state = 0;
            return TRUE;
        }
        work->state++;
        break;
    case 2:
        GF_SndHandleSetPlayerVolume(1, 0x7F);
        GF_SndHandleSetPlayerVolume(7, 0x7F);
        Sound_SetChatotRecordingDisabled(TRUE);
        if (VsRecorderApp_IsFrontierBrainClass(work->setup->trainer[BATTLER_ENEMY].data.trainerClass) == TRUE) {
            Sound_SetSceneAndPlayBGM(5, 0x47B, 1);
        } else {
            Sound_SetSceneAndPlayBGM(5, 0x45D, 1);
        }
        work->manager = OverlayManager_New(&gOverlayTemplate_Battle, work->setup, heapId);
        work->state++;
        break;
    default:
        if (OverlayManager_Run(work->manager)) {
            work->recorder->battleResult = work->setup->unk1C4;
            if (work->recorder->mode != 0 && work->recorder->battleResult == 0) {
                *(u8 *)work->recorder->videoFlagPtr = 1;
            }
            Heap_Free(work->setup->bagCursor);
            BattleSetup_Delete(work->setup);
            OverlayManager_Delete(work->manager);
            Sound_SetChatotRecordingDisabled(FALSE);
            Sound_SetScene(0);
            bgm = FieldBGM_GetEffective(work->fieldSystem, work->fieldSystem->location->mapId);
            Sound_SetFieldBGM(FieldBGM_GetForMapHeader(work->fieldSystem, work->fieldSystem->location->mapId));
            FieldBGM_Play(NULL, bgm);
            work->state = 0;
            if (work->recorder->mode != 0) {
                LoadOVY38();
                HandleLoadOverlay(FS_OVERLAY_ID(OVY_39), OVY_LOAD_ASYNC);
            }
            return TRUE;
        }
        break;
    }
    return FALSE;
}
