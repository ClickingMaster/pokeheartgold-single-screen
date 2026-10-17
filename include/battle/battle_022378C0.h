#ifndef POKEHEARTGOLD_BATTLE_022378C0_H
#define POKEHEARTGOLD_BATTLE_022378C0_H

#include "battle.h"
#include "battle_setup.h"
#include "overlay_manager.h"

BOOL Battle_Run(OverlayManager *man, int *state);
void BattleSystem_FreeInputForSubMenu(BattleSystem *battleSystem);
void BattleSystem_FreeGraphics(BattleSystem *battleSystem);
void BattleSystem_RestoreInputAfterSubMenu(BattleSystem *battleSystem);
void BattleSystem_FreeMainBgLayers(BattleSystem *battleSystem);
void BattleSystem_ReinitMainBgLayers(BattleSystem *battleSystem);

// Battle overlay manager steps (link set-up, party preview, VS screen, battle, end) and their helpers
void Battle_LinkInit(OverlayManager *man, BattleSetup *setup);
BOOL Battle_LinkMain(OverlayManager *man);
void Battle_LinkExit(OverlayManager *man);
BOOL Battle_PartyPreviewInit(OverlayManager *man);
BOOL Battle_PartyPreviewMain(OverlayManager *man);
BOOL Battle_VsScreenInit(OverlayManager *man);
BOOL Battle_VsScreenMain(OverlayManager *man);
void Battle_BattleInit(OverlayManager *man);
BOOL Battle_BattleMain(OverlayManager *man);
void Battle_BattleExit(OverlayManager *man);
BOOL Battle_EndInit(OverlayManager *man);
BOOL Battle_EndMain(OverlayManager *man);
void BgConfig_CleanupBattleMenuBackgrounds(BgConfig *bgConfig);
void BattleSystem_SetupTextFlags(BattleSystem *battleSystem);

// Battle set-up helpers
void BgConfig_CleanupBattleBackgrounds(BgConfig *config);
void BattleSystem_FreeGraphicsDummy(BattleSystem *battleSystem);
void BattleSystem_SetTrainerAIOverlayLoaded(BattleSystem *battleSystem, int loaded);
void Battle_InitBackgrounds(BattleSystem *battleSystem, BgConfig *bgConfig);
void BattleSystem_InitTerrainsAndBalls(BattleSystem *battleSystem);

#endif
