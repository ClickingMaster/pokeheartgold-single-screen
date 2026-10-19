#ifndef POKEHEARTGOLD_VS_RECORDER_APP_INTERNAL_H
#define POKEHEARTGOLD_VS_RECORDER_APP_INTERNAL_H

#include "global.h"

#include "application/vs_recorder/vs_recorder_internal.h"
#include "battle/battle_setup.h"

#include "field_system.h"
#include "overlay_manager.h"

// The VS Recorder application: from the VS Recorder item (mode 0) or the Global Terminal (modes 1-6) it runs
// the recorder screens (VsRecorderWork) and, when a video is chosen, plays the recorded battle back.

typedef struct VsRecorderApp {
    u32 state;
    u32 unk4;
    OverlayManager *manager;
    SaveData *saveData;
    BattleSetup *setup;
    VsRecorderWork *recorder;
    u32 result;
    u32 unk1C;
    u32 unk20;
    u32 unk24;
    FieldSystem *fieldSystem;
} VsRecorderApp;

typedef struct VsRecorderAppResult {
    u32 first;
    u32 second;
} VsRecorderAppResult;

typedef struct FrontierBrainClasses {
    u32 trainerClass[5];
} FrontierBrainClasses;

extern const OverlayManagerTemplate gOverlayTemplate_Battle;
extern const OverlayManagerTemplate gOverlayTemplate_VsRecorderMenu;
extern const OverlayManagerTemplate gOverlayTemplate_VsRecorderMenuFromTerminal;
extern const VsRecorderMenuEntry sVsRecorderMode3MenuNoFrontier[];
extern const VsRecorderMenuEntry sVsRecorderMode0MenuNoFrontier[];

// One entry per mode; each entry's child is the first menu of that mode.
extern const VsRecorderMenuEntry sVsRecorderRootMenu[7];
extern const u8 _02103754[];

void BattleRecord_RestoreSetup(BattleSetup *setup, SaveData *saveData);
void *BattleRecord_Get(void);
void Sound_SetChatotRecordingDisabled(BOOL enabled);

VsRecorderWork *VsRecorderApp_GetRecorderWork(OverlayManager *manager);
BOOL VsRecorderApp_InitMode0(OverlayManager *manager, int *state);
BOOL VsRecorderApp_InitMode1(OverlayManager *manager, int *state);
BOOL VsRecorderApp_InitMode2(OverlayManager *manager, int *state);
BOOL VsRecorderApp_InitMode3(OverlayManager *manager, int *state);
BOOL VsRecorderApp_InitMode4(OverlayManager *manager, int *state);
BOOL VsRecorderApp_InitMode5(OverlayManager *manager, int *state);
BOOL VsRecorderApp_InitMode6(OverlayManager *manager, int *state);
BOOL VsRecorderApp_Main(OverlayManager *manager, int *state);
BOOL VsRecorderApp_Exit(OverlayManager *manager, int *state);
BOOL VsRecorderApp_HasVisitedBattleFrontier(void *work);
BOOL VsRecorderApp_IsFrontierBrainClass(u32 trainerClass);
const VsRecorderMenuEntry *VsRecorderApp_GetStartMenu(VsRecorderWork *recorder, int mode);

#endif // POKEHEARTGOLD_VS_RECORDER_APP_INTERNAL_H
