#ifndef POKEHEARTGOLD_VS_RECORDER_APP_INTERNAL_H
#define POKEHEARTGOLD_VS_RECORDER_APP_INTERNAL_H
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

typedef struct FiveValues {
    u32 values[5];
} FiveValues;

extern const OverlayManagerTemplate gOverlayTemplate_VsRecorderMenu;
extern const OverlayManagerTemplate gOverlayTemplate_VsRecorderMenuFromTerminal;
extern const OverlayManagerTemplate gOverlayTemplate_Battle;
extern const OverlayManagerTemplate *gVsRecorderAppTemplates[];
extern const FiveValues _021028A0;
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

#endif
