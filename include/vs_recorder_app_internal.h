#ifndef POKEHEARTGOLD_VS_RECORDER_APP_INTERNAL_H
#define POKEHEARTGOLD_VS_RECORDER_APP_INTERNAL_H

#include "global.h"

#include "application/vs_recorder/vs_recorder_internal.h"
#include "battle/battle_setup.h"

#include "field_system.h"
#include "overlay_manager.h"

// The VS Recorder application of the main ARM9 part: from the VS Recorder item (mode 0) or the Global Terminal
// (modes 1-6) it runs the recorder screens of overlay 40 (VsRecorderWork) and, when a video is chosen, plays the
// recorded battle back, then returns to the screens.

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
extern const VsRecorderMenuEntry _02102DC0[];
extern const VsRecorderMenuEntry _0210357C[];

// The root menus (the child fields of the 7 root entries, 0x24 bytes apart in the screen data): _021037B8 is
// the first child field, 0x64 bytes into _02103754
typedef struct VsRecorderRootMenu {
    const VsRecorderMenuEntry *menu;
    u8 nextEntry[0x20];
} VsRecorderRootMenu;

extern const u8 _02103754[];
extern const VsRecorderRootMenu _021037B8[];

void BattleRecord_RestoreSetup(BattleSetup *setup, SaveData *saveData);
void *BattleRecord_Get(void);
void sub_02005B68(BOOL enabled);

VsRecorderWork *VsRecorderApp_GetRecorderWork(OverlayManager *manager);
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
BOOL VsRecorderApp_HasVisitedBattleFrontier(void *work);
BOOL VsRecorderApp_IsFrontierBrainClass(u32 trainerClass);
int VsRecorderMenu_InitCommon(OverlayManager *manager, int *state, BOOL fromTerminal);
int VsRecorderMenu_Init(OverlayManager *manager, int *state);
int VsRecorderMenu_InitFromTerminal(OverlayManager *manager, int *state);
int VsRecorderMenu_Main(OverlayManager *manager, int *state);
int VsRecorderMenu_Exit(OverlayManager *manager, int *state);
const VsRecorderMenuEntry *VsRecorderApp_GetRootMenu(int mode);
const VsRecorderMenuEntry *VsRecorderApp_GetStartMenu(VsRecorderWork *recorder, int mode);

#endif // POKEHEARTGOLD_VS_RECORDER_APP_INTERNAL_H
