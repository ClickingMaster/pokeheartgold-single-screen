#include "global.h"

#include "battle/battle_setup.h"

#include "bag_cursor.h"
#include "field_bgm.h"
#include "field_system.h"
#include "game_stats.h"
#include "heap.h"
#include "global_terminal_app_internal.h"
#include "overlay_manager.h"
#include "poke_overlay.h"
#include "sound_system.h"
#include "sys_flags.h"
#include "battle_recording.h"

typedef struct VsRecorderApp {
    u32 state;
    u32 unk4;
    OverlayManager *manager;
    void *saveData;
    void *setup;
    u8 *large;
    u32 result;
    u32 unk1C;
    u32 unk20;
    u32 unk24;
    void *args;
} VsRecorderApp;

typedef struct VsRecorderAppResult {
    u32 first;
    u32 second;
} VsRecorderAppResult;

extern const OverlayManagerTemplate gOverlayTemplate_VsRecorderMenu;
extern const OverlayManagerTemplate gOverlayTemplate_VsRecorderMenuFromTerminal;
extern const OverlayManagerTemplate gOverlayTemplate_Battle;
extern const OverlayManagerTemplate *gVsRecorderAppTemplates[];
extern u8 SDK_OVERLAY_OVY_39_ID;
extern u8 SDK_OVERLAY_OVY_40_ID;

#define OVERLAY_39_ID ((FSOverlayID) & SDK_OVERLAY_OVY_39_ID)
#define OVERLAY_40_ID ((FSOverlayID) & SDK_OVERLAY_OVY_40_ID)

extern void BattleRecord_Delete(void);
extern void *BattleRecord_Get(void);
extern void BattleRecord_Load(void *saveData, enum HeapID heapId, BOOL *result, BattleSetup *setup, u32 value);
extern void BattleRecord_RestoreSetup(BattleSetup *setup, void *saveData);
extern void VsRecorder_SanitizeBattleSetupNames(void *data, BattleSetup *setup, enum HeapID heapId);
extern void sub_02005B68(BOOL enabled);
extern void *sub_02087FF8(void *large, u32 value);

void *VsRecorderApp_GetRecorderWork(OverlayManager *manager);
void VsRecorderApp_SetResult(VsRecorderAppResult *pair, u32 first, u32 second);
void VsRecorderApp_Create(OverlayManager *manager, u32 mode);
BOOL VsRecorderApp_InitMode0(OverlayManager *manager, int *state);
BOOL VsRecorderApp_InitMode1(OverlayManager *manager, int *state);
BOOL VsRecorderApp_InitMode2(OverlayManager *manager, int *state);
BOOL VsRecorderApp_InitMode3(OverlayManager *manager, int *state);
BOOL VsRecorderApp_InitMode4(OverlayManager *manager, int *state);
BOOL VsRecorderApp_InitMode5(OverlayManager *manager, int *state);
BOOL VsRecorderApp_InitMode6(OverlayManager *manager, int *state);
BOOL VsRecorderApp_Main(OverlayManager *manager, int *state);
BOOL VsRecorderApp_Exit(OverlayManager *manager, int *state);
BOOL VsRecorderApp_RunMenu(VsRecorderApp *work, enum HeapID heapId);
BOOL VsRecorderApp_RunPlayback(VsRecorderApp *work, enum HeapID heapId);
const OverlayManagerTemplate *VsRecorderApp_GetTemplate(u8 index);
BOOL sub_02087E1C(u8 *data);
BOOL sub_02087E34(u32 value);

BOOL VsRecorderApp_RunPlayback(VsRecorderApp *work, enum HeapID heapId) {
    BOOL result;
    u16 bgm;
    FSOverlayID overlayId;

    switch (work->state) {
    case 0:
        if (*(u32 *)work->large != 0) {
            UnloadOverlayByID(OVERLAY_39_ID);
            UnloadOVY38();
        }
        work->state++;
        break;
    case 1:
        work->setup = BattleSetup_New(heapId, 0);
        if (!BattleRecord_Exists()) {
            BattleRecord_Load(work->saveData, heapId, &result, work->setup, *(u32 *)(work->large + 0x86C));
        } else {
            BattleRecord_RestoreSetup(work->setup, work->saveData);
            result = TRUE;
        }
        *(BagCursor **)((u8 *)work->setup + 0x10C) = BagCursor_New(heapId);
        *(GameStats **)((u8 *)work->setup + 0x144) = Save_GameStats_Get(work->saveData);
        overlayId = OVERLAY_40_ID;
        if (HandleLoadOverlay(overlayId, OVY_LOAD_ASYNC) == TRUE) {
            VsRecorder_SanitizeBattleSetupNames(BattleRecord_Get(), work->setup, heapId);
            UnloadOverlayByID(overlayId);
        }
        *(u32 *)(work->large + 0x874) = 1;
        if (result != TRUE) {
            Heap_Free(*(void **)((u8 *)work->setup + 0x10C));
            BattleSetup_Delete(work->setup);
            work->state = 0;
            return TRUE;
        }
        work->state++;
        break;
    case 2:
        GF_SndHandleSetPlayerVolume(1, 0x7F);
        GF_SndHandleSetPlayerVolume(7, 0x7F);
        sub_02005B68(TRUE);
        if (sub_02087E34(*((u8 *)work->setup + 0x5D)) == TRUE) {
            Sound_SetSceneAndPlayBGM(5, 0x47B, 1);
        } else {
            Sound_SetSceneAndPlayBGM(5, 0x45D, 1);
        }
        work->manager = OverlayManager_New(&gOverlayTemplate_Battle, work->setup, heapId);
        work->state++;
        break;
    default:
        if (OverlayManager_Run(work->manager)) {
            *(u32 *)(work->large + 0x874) = *(u32 *)((u8 *)work->setup + 0x1C4);
            if (*(u32 *)work->large != 0 && *(u32 *)(work->large + 0x874) == 0) {
                **(u8 **)(work->large + 0x878) = 1;
            }
            Heap_Free(*(void **)((u8 *)work->setup + 0x10C));
            BattleSetup_Delete(work->setup);
            OverlayManager_Delete(work->manager);
            sub_02005B68(FALSE);
            Sound_SetScene(0);
            bgm = FieldBGM_GetEffective(work->args, ((FieldSystem *)work->args)->location->mapId);
            Sound_SetFieldBGM(FieldBGM_GetForMapHeader(work->args, ((FieldSystem *)work->args)->location->mapId));
            sub_02055198(NULL, bgm);
            work->state = 0;
            if (*(u32 *)work->large != 0) {
                LoadOVY38();
                HandleLoadOverlay(OVERLAY_39_ID, OVY_LOAD_ASYNC);
            }
            return TRUE;
        }
        break;
    }
    return FALSE;
}
